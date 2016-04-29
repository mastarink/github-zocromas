/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_output_defs.h"
#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi.h"                                                 /* duf_pdi_init; duf_pdi_shut; duf_pdi_close ✗ */
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */
#include "duf_pdi_structs.h"

#include "duf_pathinfo_ref.h"
#include "duf_sccbh_row.h"                                           /* duf_sccbh_row_get_*; sccbh_rows_eval ✗ */
#include "duf_sccb_row.h"                                            /* datarow_* ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sccbh_pstmt.h"

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */

/* ###################################################################### */
#include "duf_print.h"
/* ###################################################################### */

void
_convert_fmt( char *format, size_t fbsz, const char *fmt, const char *tail )
{
  if ( fbsz > 0 )
  {
    *format++ = '%';
    fbsz--;
  }
  MAST_TRACE( temp, 50, ">%ld>> '%s' (%c:%d:%d) :: '%s'", fbsz, fmt, *fmt, ( ( *fmt == '+' ) || ( *fmt == '-' ) ) ? 1 : 0, *fmt - '-', tail );
  if ( fbsz > 0 && ( *fmt == '+' || *fmt == '-' ) )
  {
    MAST_TRACE( temp, 50, ">%ld>> %s :: %s", fbsz, fmt, tail );
    *format++ = *fmt++;
    fbsz--;
  }
  while ( fbsz > 0 && *fmt >= '0' && *fmt <= '9' )
  {
    MAST_TRACE( temp, 50, ">%ld>> %s :: %s", fbsz, fmt, tail );
    *format++ = *fmt++;
    fbsz--;
  }
  strncpy( format, tail, fbsz );
}

typedef enum
{
  DUF_SFMT_CHR_SUFFIX = /*             */ '~',                       /* suffix */
  DUF_SFMT_CHR_UNDERLINE = /*          */ '_',                       /*  */
  DUF_SFMT_CHR_SHA1SUM = /*            */ '.',                       /* sha1sum */
  DUF_SFMT_CHR_MD5SUM = /*             */ '@',                       /* md5sum */
  DUF_SFMT_CHR_DATAID = /*             */ '&',                       /*  */
  DUF_SFMT_CHR_SEQ_LEAF = /*           */ '#',                       /* seqq.leaf */
  DUF_SFMT_CHR_CAMERAS = /*            */ 'a',                       /* camera */
  DUF_SFMT_CHR_CAMERA = /*             */ 'A',                       /* camera */
  DUF_SFMT_CHR_COLOR = /*              */ 'c',                       /*  */
  DUF_SFMT_CHR_MIME = /*               */ 'e',                       /* mime */
  DUF_SFMT_CHR_MIMEID = /*             */ 'E',                       /* mimeid */
  DUF_SFMT_CHR_FILENAME = /*           */ 'f',                       /* filename */
  DUF_SFMT_CHR_GROUP = /*              */ 'g',                       /* group */
  DUF_SFMT_CHR_DEPTH = /*              */ 'h',                       /* depth */
  DUF_SFMT_CHR_SHA1ID = /*             */ 'H',                       /* sha1id */
  DUF_SFMT_CHR_DIRID = /*              */ 'I',                       /* dirid */
  DUF_SFMT_CHR_MODE = /*               */ 'm',                       /* mimeid */
  DUF_SFMT_CHR_MD5ID = /*              */ 'M',                       /* md5id */
  DUF_SFMT_CHR_NLINK = /*              */ 'n',                       /* nlink */
  DUF_SFMT_CHR_NAMEID = /*             */ 'N',                       /* nameid */
  DUF_SFMT_CHR_INODE = /*              */ 'O',                       /* inode */
  DUF_SFMT_CHR_MOVE_TO_POSITION = /*   */ 'p',                       /* move to position */
  DUF_SFMT_CHR_PREFIX = /*             */ 'P',                       /* prefix */
  DUF_SFMT_CHR_SEQ_NODE = /*           */ 'q',                       /* seqq.node */
  DUF_SFMT_CHR_SEQ_ROW = /*            */ 'w',                       /* seqq.row */
  DUF_SFMT_CHR_SEQ = /*                */ 'Q',                       /* seqq.gen */
  DUF_SFMT_CHR_REALPATH = /*           */ 'r',                       /* realpath */
  DUF_SFMT_CHR_RELATIVE_PATH = /*      */ 'R',                       /* relative realpath (relative to 'top level') */
  DUF_SFMT_CHR_SPACE = /*              */ 's',                       /* << space >> */
  DUF_SFMT_CHR_NSAME = /*              */ 'S',                       /* nsame_md5 */
  DUF_SFMT_CHR_MTIME1 = /*             */ 't',                       /* tm,tc,ta: mtime */
  DUF_SFMT_CHR_MTIME2 = /*             */ 'T',                       /*  */
  DUF_SFMT_CHR_USER = /*               */ 'u',                       /* user */
  DUF_SFMT_CHR_EXIFID = /*             */ 'X',                       /* exifid */
  DUF_SFMT_CHR_FILESIZEH = /*          */ 'z',                       /* filesize */
  DUF_SFMT_CHR_FILESIZE = /*           */ 'Z',                       /* filesize */
} duf_sformat_char_t;

static
SRX( OTHER, size_t, slen, 0, sformat_id, int fcolor, const char **pfmt, char **ppbuffer, size_t position, size_t bfsz, const duf_depthinfo_t * pdi,
     int use_row, const duf_sccb_data_row_t * row, const duf_sccb_data_row_t * prevrow MAS_UNUSED, duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi,
     duf_sccb_print_cb_t prefix_scb, duf_sccb_print_cb_t suffix_scb MAS_UNUSED, size_t * pwidth )
{
/* size_t slen = 0; */
  size_t swidth = 0;
  duf_sformat_char_t c;
  char *pe = NULL;
  long v = 0;
  size_t fbsz = 512;
  const char *fmt = *pfmt;
  char *pbuffer = *ppbuffer;
  char format[fbsz];
  const char *fmt0;

  fmt0 = fmt;
  v = strtol( fmt, &pe, 10 );
  fmt = pe;
  c = *fmt++;
  switch ( c )
  {
  case DUF_SFMT_CHR_DEPTH:                                          /* depth */
#if 1
    _convert_fmt( format, fbsz, fmt0, "u" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldu", v );
    else
      snprintf( format, fbsz, "%%u" );
#endif
  /* assert( duf_pdi_reldepth( pdi ) == duf_pi_deltadepth( duf_sccbh_row_pathinfo( sccbh ) ) ); */
  /* QT( "@%d  ?  %d  ?  %d", duf_pi_deltadepth( duf_sccbh_row_pathinfo( sccbh ) ), duf_pi_deltadepth( duf_pdi_pathinfo( pdi ) ),                */
  /*     duf_pdi_reldepth( pdi ) );                                                                                                              */
  /* QT( "@@%d  ?  %d  ?  %d", duf_pi_depth( duf_sccbh_row_pathinfo( sccbh ) ), duf_pi_depth( duf_pdi_pathinfo( pdi ) ), duf_pdi_depth( pdi ) ); */
  /* snprintf( pbuffer, bfsz, format, row? duf_pi_deltadepth( duf_sccbh_row_pathinfo( sccbh ) ) : duf_pdi_reldepth( pdi ) ); */
    snprintf( pbuffer, bfsz, format, row
              && use_row ? duf_pi_depth( CRX( datarow_get_pathinfo, row ) /* duf_sccbh_row_pathinfo( sccbh ) */  ) : duf_pdi_depth( pdi ) );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_SEQ:                                            /* seq */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif
    assert( !row || !pfi || pfi->seqq.gen == CRP( datarow_get_number, row, "seq_gen" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "seq_gen" ) : pfi->seqq.gen );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_SEQ_NODE:                                       /* seqq.node */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif
    assert( !row || !pfi || pfi->seqq.node == CRP( datarow_get_number, row, "seq_node" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "seq_node" ) : pfi->seqq.node );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_SEQ_ROW:                                        /* seqq.row */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif
    assert( !row || !pfi || pfi->seqq.row == CRP( datarow_get_number, row, "seq_row" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "seq_row" ) : pfi->seqq.row );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_SEQ_LEAF:                                       /* seqq.leaf */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif
    assert( !row || !pfi || pfi->seqq.leaf == CRP( datarow_get_number, row, "seq_leaf" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "seq_leaf" ) : pfi->seqq.leaf );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_MD5ID:                                          /* md5id */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif
    assert( !row || !pfi || pfi->md5id == CRP( datarow_get_number, row, "md5id" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "md5id" ) : pfi->md5id );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_SHA1ID:                                         /* sha1id */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif
    assert( !row || !pfi || pfi->sha1id == CRP( datarow_get_number, row, "sha1id" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "sha1id" ) : pfi->sha1id );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_NSAME:                                          /* nsame */
    {
      char c2 = 0;
      unsigned long long ns = 0;

#if 1
      _convert_fmt( format, fbsz, fmt0, "llu" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%ldllu", v );
      else
        snprintf( format, fbsz, "%%llu" );
#endif
      c2 = *fmt;
      switch ( c2 )
      {
      case 'm':
        fmt++;
        assert( !row || !pfi || pfi->nsame_md5 == CRP( datarow_get_number, row, "nsame_md5" ) );
        ns = row && use_row ? CRP( datarow_get_number, row, "nsame_md5" ) : ( unsigned long long ) pfi->nsame_md5;
        break;
      case 'x':
        fmt++;
        assert( !row || !pfi || pfi->nsame_exif == CRP( datarow_get_number, row, "nsame_exif" ) );
        ns = row && use_row ? CRP( datarow_get_number, row, "nsame_exif" ) : ( unsigned long long ) pfi->nsame_exif;
        break;
      case 's':
        fmt++;
        assert( !row || !pfi || pfi->nsame_sha1 == CRP( datarow_get_number, row, "nsame_sha1" ) );
        ns = row && use_row ? CRP( datarow_get_number, row, "nsame_sha1" ) : ( unsigned long long ) pfi->nsame_sha1;
        break;
      default:
        assert( !row || !pfi || pfi->nsame == CRP( datarow_get_number, row, "nsame" ) );
        ns = row && use_row ? CRP( datarow_get_number, row, "nsame" ) : ( unsigned long long ) pfi->nsame;
        break;
      }
      snprintf( pbuffer, bfsz, format, ns );
    }
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_UNDERLINE:                                      /* underline */
    memset( pbuffer, '_', v > 0 ? v : ( v < 0 ? -v : 1 ) );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_SPACE:                                          /* space */
  /* pbuffer += strlen( pbuffer ); */
    memset( pbuffer, ' ', v > 0 ? v : ( v < 0 ? -v : 1 ) );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_MOVE_TO_POSITION:
    {
      long vp = 0;

      vp = v - position;
    /* memset( pbuffer, ' ', vp > 0 ? vp : ( vp < 0 ? -vp : 1 ) ); */
      memset( pbuffer, ' ', vp > 0 ? vp : ( vp < 0 ? 1 : 1 ) );
    }
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_PREFIX:                                         /* prefix */
    if ( prefix_scb )
    {
      ( prefix_scb ) ( pbuffer, bfsz, pdi, use_row, row, sccbh, &swidth );
    }
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_DIRID:                                          /* dirid */
    {
      unsigned long long dirid;

#if 1
      _convert_fmt( format, fbsz, fmt0, "llu" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%ldllu", v );
      else
        snprintf( format, fbsz, "%%llu" );
#endif
    /* QT( "@%llu - %llu", duf_levinfo_nodedirid( pdi ), duf_pi_nodedirid( duf_sccbh_row_pathinfo( sccbh ) ) ); */
    /* assert( duf_levinfo_nodedirid( pdi ) == duf_pi_nodedirid( duf_sccbh_row_pathinfo( sccbh ) ) ); */

      dirid = row && use_row ? CRP( datarow_get_number, row, "dirid" ) : ( unsigned long long ) pfi->dirid;

#if 0
      snprintf( pbuffer, bfsz, format, row && use_row ? duf_pi_nodedirid( duf_sccbh_row_pathinfo( sccbh ) ) : duf_levinfo_nodedirid( pdi ) );
#else
      snprintf( pbuffer, bfsz, format, dirid );
#endif
    } swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_DATAID:                                         /* dataid */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    assert( !row || !pfi || pfi->dataid == CRP( datarow_get_number, row, "dataid" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "dataid" ) : pfi->dataid );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_COLOR:                                          /* color */
    {
      static char *hls[] = { "", "1;33;41", "1;7;32;44", "1;7;108;33", "1;7;108;32", "1;33;44", "1;37;46", "1;7;33;41", "7;101;35", "30;47" };

#if 0
      if ( v > 0 && v < sizeof( hls ) )
        snprintf( pbuffer, bfsz, "\x1b[%sm", hls[v] );
      else
        snprintf( pbuffer, bfsz, "\x1b[%sm", hls[0] );
#else
      mas_sncolor_s( fcolor, pbuffer, bfsz, "\x1b[%sm", ( v > 0 && ( size_t ) v < sizeof( hls ) ) ? hls[v] : hls[0] );
#endif
    }
  /* Not here : swidth += strlen( pbuffer ); */
    break;
  case DUF_SFMT_CHR_SUFFIX:                                         /* suffix */
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_INODE:                                          /* inode */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    assert( !row || !pfi || pfi->st.st_ino == CRP( datarow_get_number, row, "inode" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "inode" ) : ( unsigned long long ) pfi->st.st_ino );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_MODE:                                           /* mode */
    {
      char modebuf[] = "----------";
      char *pmode = modebuf;
      unsigned long long filemode;

      assert( !row || !pfi || pfi->st.st_mode == CRP( datarow_get_number, row, "filemode" ) );
      filemode = row && use_row ? CRP( datarow_get_number, row, "filemode" ) : pfi->st.st_mode;
      pmode++;
      if ( S_IRUSR & filemode )
        *pmode = 'r';
      pmode++;
      if ( S_IWUSR & filemode )
        *pmode = 'w';
      pmode++;
      if ( S_IXUSR & filemode )
        *pmode = 'x';
      pmode++;
      if ( S_IRGRP & filemode )
        *pmode = 'r';
      pmode++;
      if ( S_IWGRP & filemode )
        *pmode = 'w';
      pmode++;
      if ( S_IXGRP & filemode )
        *pmode = 'x';
      pmode++;
      if ( S_IROTH & filemode )
        *pmode = 'r';
      pmode++;
      if ( S_IWOTH & filemode )
        *pmode = 'w';
      pmode++;
      if ( S_IXOTH & filemode )
        *pmode = 'x';
      snprintf( pbuffer, bfsz, "%s", modebuf );
    }
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_NLINK:                                          /* nlink */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    assert( !row || !pfi || pfi->st.st_nlink == CRP( datarow_get_number, row, "nlink" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "nlink" ) : ( unsigned long long ) pfi->st.st_nlink );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_USER:                                           /* user */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    assert( !row || !pfi || pfi->st.st_uid == CRP( datarow_get_number, row, "uid" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "uid" ) : ( unsigned long long ) pfi->st.st_uid );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_GROUP:                                          /* group */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    assert( !row || !pfi || pfi->st.st_gid == CRP( datarow_get_number, row, "gid" ) );
    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "gid" ) : ( unsigned long long ) pfi->st.st_gid );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_FILESIZE:                                       /* filesize */
    {
      unsigned long long sz = row && use_row ? CRP( datarow_get_number, row, "filesize" ) : ( unsigned long long ) pfi->st.st_size;

      assert( !row || !pfi || ( size_t ) pfi->st.st_size == ( size_t ) CRP( datarow_get_number, row, "filesize" ) );
#if 1
      _convert_fmt( format, fbsz, fmt0, "llu" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%ldllu.", v );
      else
        snprintf( format, fbsz, "%%llu." );
#endif

      snprintf( pbuffer, bfsz, format, sz );
    }
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_FILESIZEH:                                      /* filesize */
    {
      unsigned long long sz = row && use_row ? CRP( datarow_get_number, row, "filesize" ) : ( unsigned long long ) pfi->st.st_size;

      assert( !row || !pfi || ( size_t ) pfi->st.st_size == ( size_t ) CRP( datarow_get_number, row, "filesize" ) );
      if ( sz < 1024 )
      {
#if 1
        _convert_fmt( format, fbsz, fmt0, "llu." );
#else
        if ( v )
          snprintf( format, fbsz, "%%%ldllu.", v );
        else
          snprintf( format, fbsz, "%%llu." );
#endif

        snprintf( pbuffer, bfsz, format, sz );
      }
      else if ( sz < 1024 * 1024 )
      {
#if 1
        _convert_fmt( format, fbsz, fmt0, "lluk" );
#else
        if ( v )
          snprintf( format, fbsz, "%%%ldlluk", v );
        else
          snprintf( format, fbsz, "%%lluk" );
#endif

        snprintf( pbuffer, bfsz, format, sz / 1024 );
      }
      else if ( sz < 1024 * 1024 * 1024 )
      {
#if 1
        _convert_fmt( format, fbsz, fmt0, "lluM" );
#else
        if ( v )
          snprintf( format, fbsz, "%%%ldlluM", v );
        else
          snprintf( format, fbsz, "%%lluM" );
#endif

        snprintf( pbuffer, bfsz, format, sz / 1024 / 1024 );
      }
      else
      {
#if 1
        _convert_fmt( format, fbsz, fmt0, "lluG" );
#else
        if ( v )
          snprintf( format, fbsz, "%%%ldlluG", v );
        else
          snprintf( format, fbsz, "%%lluG" );
#endif

        snprintf( pbuffer, bfsz, format, sz / 1024 / 1024 / 1024 );
      }
    }
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_MTIME1:                                         /* time */
  case DUF_SFMT_CHR_MTIME2:                                         /* time */
    {
      time_t timet;
      struct tm time_tm, *ptime_tm = NULL;
      char stime[128] = "-";
      char c2 = 0;

      timet = ( time_t ) 0;
      c2 = *fmt++;
      switch ( c2 )
      {
      case 'm':
        timet = ( time_t ) ( row && use_row ? CRP( datarow_get_number, row, "mtime" ) : ( unsigned long long ) pfi->st.st_mtim.tv_sec );
        break;
      case 'a':
        timet = ( time_t ) ( row && use_row ? CRP( datarow_get_number, row, "atime" ) : ( unsigned long long ) pfi->st.st_atim.tv_sec );
        break;
      case 'c':
        timet = ( time_t ) ( row && use_row ? CRP( datarow_get_number, row, "ctime" ) : ( unsigned long long ) pfi->st.st_atim.tv_sec );
        break;
      case 'x':
      /* if ( pfi->exifid ) */
        timet = ( time_t ) ( row && use_row ? CRP( datarow_get_number, row, "exifdt" ) : ( unsigned long long ) pfi->exifdt );
        break;
      default:
        timet = ( time_t ) ( row && use_row ? CRP( datarow_get_number, row, "mtime" ) : ( unsigned long long ) pfi->st.st_mtim.tv_sec );
        fmt--;
      }
      if ( timet )
        ptime_tm = localtime_r( &timet, &time_tm );
      switch ( ( char ) c )
      {
      case DUF_SFMT_CHR_MTIME1:
        if ( ptime_tm )
          strftime( stime, sizeof( stime ), "%b %d %Y %H:%M:%S", ptime_tm );
        else
          strcpy( stime, "-------------------" );
        break;
      case DUF_SFMT_CHR_MTIME2:
        {
          char *subfmt = NULL;

          if ( *fmt == '{' )
          {
            const char *efmt;

            fmt++;
            efmt = strchr( fmt, '}' );
            if ( efmt )
            {
              subfmt = mas_strndup( fmt, efmt - fmt );
            /* MAST_TRACE( temp, 0, "=== '%s'", subfmt ); */
              fmt = efmt;
              fmt++;
            }
          }
          if ( !subfmt )
            subfmt = mas_strdup( "%Y.%m.%d %H:%M:%S" );
          if ( subfmt && ptime_tm )
            strftime( stime, sizeof( stime ), subfmt, ptime_tm );
          else
            strcpy( stime, "-------------------" );
          mas_free( subfmt );
        }
        break;
      }
#if 1
      _convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif

      snprintf( pbuffer, bfsz, format, stime );
    }
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_REALPATH:                                       /* realpath */
    {
      unsigned long long dirid MAS_UNUSED;
      unsigned long long pdirid MAS_UNUSED;

      dirid = row && use_row ? CRP( datarow_get_number, row, "dirid" ) : ( unsigned long long ) pfi->dirid;
      pdirid = prevrow && use_row ? CRP( datarow_get_number, prevrow, "dirid" ) : 0;
      {
        const char *real_path = NULL;

#if 1
        _convert_fmt( format, fbsz, fmt0, "s" );
#else
        if ( v )
          snprintf( format, fbsz, "%%%lds", v );
        else
          snprintf( format, fbsz, "%%s" );
#endif

      /* XXX XXX XXX Wrong!! XXX XXX XXX */
        real_path = ( !dirid || !pdirid || dirid != pdirid ) ?
                ( row && use_row ? duf_pi_path( CRX( datarow_get_pathinfo, row ) ) : duf_levinfo_path( pdi ) ) : "";
        snprintf( pbuffer, bfsz, format, real_path );
      }
    }
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_RELATIVE_PATH:                                  /* relative realpath */
    {
      unsigned long long dirid MAS_UNUSED;
      unsigned long long pdirid MAS_UNUSED;

      dirid = row && use_row ? CRP( datarow_get_number, row, "dirid" ) : ( unsigned long long ) pfi->dirid;
      pdirid = prevrow && use_row ? CRP( datarow_get_number, prevrow, "dirid" ) : 0;
      {
        const char *rel_real_path = NULL;

#if 1
        _convert_fmt( format, fbsz, fmt0, "s" );
#else
        if ( v )
          snprintf( format, fbsz, "%%%lds", v );
        else
          snprintf( format, fbsz, "%%s" );
#endif

        rel_real_path = ( !dirid || !pdirid || dirid != pdirid ) ?
                ( row && use_row ? duf_pi_relpath( CRX( datarow_get_pathinfo, row ) ) : duf_levinfo_relpath( pdi ) ) : "";
        snprintf( pbuffer, bfsz, format, rel_real_path );
      }
    }
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_FILENAME:                                       /* filename */
    {
#if 1
      _convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif

      snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_string, row, "fname" ) : pfi->name );
    }
    swidth += strlen( pbuffer );
    break;
#if 0
  case 'f':                                                         /* itemname */
    {
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      snprintf( pbuffer, bfsz, format, row && use_row ? duf_pi_itemname( duf_sccbh_row_pathinfo( sccbh ) ) : duf_levinfo_itemname( pdi ) );
    }
    swidth += strlen( pbuffer );
    break;
#endif
  case DUF_SFMT_CHR_MD5SUM:                                         /* md5sum */
    {
      unsigned long long md5sum1, md5sum2;

      md5sum1 = row && use_row ? CRP( datarow_get_number, row, "md5sum1" ) : pfi->md5sum1;
      md5sum2 = row && use_row ? CRP( datarow_get_number, row, "md5sum2" ) : pfi->md5sum2;
      if ( md5sum1 || md5sum2 )
        snprintf( pbuffer, bfsz, "%016llx%016llx", md5sum1, md5sum2 );
      else
        snprintf( pbuffer, bfsz, "%-32s", "-" );
      swidth += strlen( pbuffer );
    }
    break;
  case DUF_SFMT_CHR_SHA1SUM:                                        /* sha1sum */
    {
      unsigned long long sha1sum1, sha1sum2, sha1sum3;

      sha1sum1 = row && use_row ? CRP( datarow_get_number, row, "sha1sum1" ) : pfi->sha1sum1;
      sha1sum2 = row && use_row ? CRP( datarow_get_number, row, "sha1sum2" ) : pfi->sha1sum2;
      sha1sum3 = row && use_row ? CRP( datarow_get_number, row, "sha1sum3" ) : pfi->sha1sum3;
      if ( sha1sum1 || sha1sum2 || sha1sum3 )
        snprintf( pbuffer, bfsz, "%08llx:%016llx:%016llx", sha1sum1, sha1sum2, sha1sum3 );
      else
        snprintf( pbuffer, bfsz, "%-40s", "-" );
      swidth += strlen( pbuffer );
    }
    break;
  case DUF_SFMT_CHR_EXIFID:                                         /* exifid */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif
    {
      unsigned long long exifid;

      exifid = row && use_row ? CRP( datarow_get_number, row, "exifid" ) : pfi->exifid;
      if ( exifid )
        snprintf( pbuffer, bfsz, format, exifid );
      else
        *pbuffer = '?';
      swidth += strlen( pbuffer );
    }
    break;
  case DUF_SFMT_CHR_CAMERAS:                                        /* camera */
  case DUF_SFMT_CHR_CAMERA:                                         /* camera */
    {
      const char *camera;

      camera = row && use_row ? CRP( datarow_get_string, row, "camera" ) : pfi->camera;
      if ( camera && *camera )
      {
#if 1
        _convert_fmt( format, fbsz, fmt0, "s" );
#else
        if ( v )
          snprintf( format, fbsz, "%%%lds", v );
        else
          snprintf( format, fbsz, "%%s" );
#endif

        snprintf( pbuffer, bfsz, format, camera );
        if ( *pbuffer && c == DUF_SFMT_CHR_CAMERAS )
        {
          char *p;

          p = strchr( pbuffer, ' ' );
          while ( p && *p && p < pbuffer + bfsz )
            *p++ = 0;
        }
      }
      else
        *pbuffer = '?';
      swidth += strlen( pbuffer );
    }
    break;
  case DUF_SFMT_CHR_MIMEID:                                         /* mimeid */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "mimeid" ) : pfi->mimeid );
    swidth += strlen( pbuffer );
    break;
  case DUF_SFMT_CHR_MIME:                                           /* mime */
#if 1
    _convert_fmt( format, fbsz, fmt0, "s" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%lds", v );
    else
      snprintf( format, fbsz, "%%s" );
#endif
    {
      const char *mime;

      mime = row && use_row ? CRP( datarow_get_string, row, "mime" ) : pfi->mime;
      snprintf( pbuffer, bfsz, format, mime ? mime : "?" );
      swidth += strlen( pbuffer );
    }
    break;
  case DUF_SFMT_CHR_NAMEID:                                         /* nameid */
#if 1
    _convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, row && use_row ? CRP( datarow_get_number, row, "nameid" ) : pfi->nameid );
    swidth += strlen( pbuffer );
    break;
  default:
    QT( "@fmt %c", c );
    pbuffer += strlen( pbuffer );
    *pbuffer++ = '%';
    if ( v )
      snprintf( pbuffer, bfsz, "%ld", v );
    pbuffer += strlen( pbuffer );
    *pbuffer++ = c;
    *pbuffer = 0;
    swidth += strlen( pbuffer );
    break;
  }
  slen = strlen( pbuffer );
  pbuffer += slen;
  *pfmt = fmt;
  *ppbuffer = pbuffer;
  if ( pwidth )
    *pwidth += swidth;
/* return slen; */
  ERX( OTHER, size_t, slen, 0, sformat_id, int fcolor, const char **pfmt, char **ppbuffer, size_t position, size_t bfsz, const duf_depthinfo_t * pdi,
       int use_row, const duf_sccb_data_row_t * row, const duf_sccb_data_row_t * prevrow, duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi,
       duf_sccb_print_cb_t prefix_scb, duf_sccb_print_cb_t suffix_scb MAS_UNUSED, size_t * pwidth );
}

SRX( OTHER, char *, buffer, NULL, sformat_file_info, const duf_depthinfo_t * pdi, int use_row, const duf_sccb_data_row_t * row,
     const duf_sccb_data_row_t * prevrow, duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi, int fcolor, const char *format,
     duf_sccb_print_cb_t prefix_scb, duf_sccb_print_cb_t suffix_scb, size_t max_width MAS_UNUSED, size_t * pslen, size_t * pwidth, int *pover )
{
/* char *buffer = NULL; */
  char *pbuffer = NULL;
  size_t slen = 0;
  size_t swidth = 0;
  const char *fmt = format;
  size_t bfsz = 1024 * 16;
  int over = 0;
  int inactive = 0;

  pbuffer = buffer = mas_malloc( bfsz + 8 );
  memset( buffer, 0, bfsz );
  while ( fmt && *fmt && buffer + bfsz - pbuffer > 0 /* && !( over = ( max_width > 0 && swidth > max_width ) ) */  )
  {
    if ( *fmt == '`' )
    {
      fmt++;
      inactive = !inactive;
      continue;
    }
    else if ( inactive )
    {
      fmt++;
      continue;
    }
    else if ( *fmt == '%' )
    {
      fmt++;
      slen += CRX( sformat_id, fcolor, &fmt, &pbuffer, pbuffer - buffer, buffer + bfsz - pbuffer /* remaining bytes cnt */ , pdi, use_row, row,
                   prevrow, sccbh, pfi, prefix_scb, suffix_scb, &swidth );
    }
    else if ( *fmt == '\\' )
    {
      fmt++;
      if ( *fmt == 'n' )
        *pbuffer++ = '\n';
      else if ( *fmt == ':' )
        *pbuffer++ = ':';
      else if ( *fmt == 'r' )
        *pbuffer++ = '\r';
      else if ( *fmt == 't' )
        *pbuffer++ = '\t';                                           /* FIXME swidth is wrong for \t */
      else if ( *fmt == 'x' )
      {
        char c;
        char *pe = NULL;

        c = strtol( fmt + 1, &pe, 16 );
        *pbuffer++ = c;
        fmt = pe;
        fmt--;
      }
      else
      {
        *pbuffer++ = '\\';
        *pbuffer++ = *fmt;
        swidth++;
      }
      swidth++;
      fmt++;
    }
    else
    {
      *pbuffer++ = *fmt++;
      swidth++;
    }
  }
  if ( pover )
    *pover = over;
  if ( pslen )
    *pslen = slen;
  if ( pwidth )
    *pwidth = swidth;
/* buffer[slen] = 0; */
/* return buffer; */
  ERX( OTHER, char *, buffer, NULL, sformat_file_info, const duf_depthinfo_t * pdi, int use_row, const duf_sccb_data_row_t * row,
       const duf_sccb_data_row_t * prevrow, duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi, int fcolor, const char *format,
       duf_sccb_print_cb_t prefix_scb, duf_sccb_print_cb_t suffix_scb, size_t max_width MAS_UNUSED, size_t * pslen, size_t * pwidth, int *pover );
}

SRX( OTHER, size_t, slen, 0, print_sformat_file_info, const duf_depthinfo_t * pdi, int use_row, const duf_sccb_data_row_t * row,
     const duf_sccb_data_row_t * prevrow, duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi, const char *format,
     duf_sccb_print_cb_t prefix_scb, duf_sccb_print_cb_t suffix_scb, size_t max_width, int force_color, int nocolor, size_t * pswidth, int *pover )
{
/* size_t slen = 0; */
  size_t swidth = 0;
  char *buffer;
  FILE *out;

#if 0
/* out = duf_config && duf_config->opt.output.out ? duf_config->opt.output.out : stdout; */
#else
  out = mas_output_file(  );
#endif

  buffer = CRX( sformat_file_info, pdi, use_row, row, prevrow, sccbh, pfi, ( force_color || ( !nocolor && isatty( fileno( out ) ) ) ), format,
                prefix_scb, suffix_scb, max_width, &slen, &swidth, pover );

  DUF_WRITES( 0, buffer );
/* if ( duf_levinfo_count_gfiles( pdi ) )                                */
/*   DUF_PRINTF( 0, ".@ gfiles:%llu", duf_levinfo_count_gfiles( pdi ) ); */

  mas_free( buffer );
  if ( pswidth )
    *pswidth = swidth;
/* return slen; */
  ERX( OTHER, size_t, slen, 0, print_sformat_file_info, const duf_depthinfo_t * pdi, int use_row, const duf_sccb_data_row_t * row,
       const duf_sccb_data_row_t * prevrow, duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi, const char *format, duf_sccb_print_cb_t prefix_scb,
       duf_sccb_print_cb_t suffix_scb, size_t max_width, int force_color, int nocolor, size_t * pswidth, int *pover );
}
