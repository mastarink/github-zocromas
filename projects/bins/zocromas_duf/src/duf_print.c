#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>



#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"

#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"



/* ###################################################################### */
#include "duf_print.h"
/* ###################################################################### */

void
duf_convert_fmt( char *format, size_t fbsz, const char *fmt, const char *tail )
{
  if ( fbsz > 0 )
  {
    *format++ = '%';
    fbsz--;
  }
  DUF_TRACE( temp, 5, ">%ld>> '%s' (%c:%d:%d) :: '%s'", fbsz, fmt, *fmt, ( ( *fmt == '+' ) || ( *fmt == '-' ) ) ? 1 : 0, *fmt - '-', tail );
  if ( fbsz > 0 && ( *fmt == '+' || *fmt == '-' ) )
  {
    DUF_TRACE( temp, 5, ">%ld>> %s :: %s", fbsz, fmt, tail );
    *format++ = *fmt++;
    fbsz--;
  }
  while ( fbsz > 0 && *fmt >= '0' && *fmt <= '9' )
  {
    DUF_TRACE( temp, 5, ">%ld>> %s :: %s", fbsz, fmt, tail );
    *format++ = *fmt++;
    fbsz--;
  }
  strncpy( format, tail, fbsz );
}

typedef enum
{
  DUF_SFMT_CHR_SUFFIX = /*             */ '~', /* suffix */
  DUF_SFMT_CHR_UNDERLINE = /*          */ '_', /*  */
  DUF_SFMT_CHR_SHA1SUM = /*            */ '.', /* sha1sum */
  DUF_SFMT_CHR_MD5SUM = /*             */ '@', /* md5sum */
  DUF_SFMT_CHR_DATAID = /*             */ '&', /*  */
  DUF_SFMT_CHR_SEQ_LEAF = /*           */ '#', /* seq_leaf */
  DUF_SFMT_CHR_CAMERAS = /*            */ 'a', /* camera */
  DUF_SFMT_CHR_CAMERA = /*             */ 'A', /* camera */
  DUF_SFMT_CHR_COLOR = /*              */ 'c', /*  */
  DUF_SFMT_CHR_NDIRS = /*              */ 'D', /* ndirs */
  DUF_SFMT_CHR_MIME = /*               */ 'e', /* mime */
  DUF_SFMT_CHR_MIMEID = /*             */ 'E', /* mimeid */
  DUF_SFMT_CHR_FILENAME = /*           */ 'f', /* filename */
  DUF_SFMT_CHR_NFILES = /*             */ 'F', /* nfiles */
  DUF_SFMT_CHR_GROUP = /*              */ 'g', /* group */
  DUF_SFMT_CHR_DEPTH = /*              */ 'h', /* depth */
  DUF_SFMT_CHR_SHA1ID = /*             */ 'H', /* sha1id */
  DUF_SFMT_CHR_DIRID = /*              */ 'I', /* dirid */
  DUF_SFMT_CHR_MODE = /*               */ 'm', /* mimeid */
  DUF_SFMT_CHR_MD5ID = /*              */ 'M', /* md5id */
  DUF_SFMT_CHR_NLINK = /*              */ 'n', /* nlink */
  DUF_SFMT_CHR_NAMEID = /*             */ 'N', /* nameid */
  DUF_SFMT_CHR_INODE = /*              */ 'O', /* inode */
  DUF_SFMT_CHR_MOVE_TO_POSITION = /*   */ 'p', /* move to position */
  DUF_SFMT_CHR_PREFIX = /*             */ 'P', /* prefix */
  DUF_SFMT_CHR_SEQ_NODE = /*           */ 'q', /* seq_node */
  DUF_SFMT_CHR_SEQ = /*                */ 'Q', /* seq */
  DUF_SFMT_CHR_REALPATH = /*           */ 'r', /* realpath */
  DUF_SFMT_CHR_RELATIVE_PATH = /*      */ 'R', /* relative realpath (relative to 'top level') */
  DUF_SFMT_CHR_SPACE = /*              */ 's', /* << space >> */
  DUF_SFMT_CHR_NSAME = /*              */ 'S', /* nsame_md5 */
  DUF_SFMT_CHR_MTIME1 = /*             */ 't', /* tm,tc,ta: mtime */
  DUF_SFMT_CHR_MTIME2 = /*             */ 'T', /*  */
  DUF_SFMT_CHR_USER = /*               */ 'u', /* user */
  DUF_SFMT_CHR_EXIFID = /*             */ 'X', /* exifid */
  DUF_SFMT_CHR_FILESIZEH = /*          */ 'z', /* filesize */
  DUF_SFMT_CHR_FILESIZE = /*           */ 'Z', /* filesize */
} duf_sformat_char_t;

static size_t
duf_sformat_id( int is_atty, const char **pfmt, char **ppbuffer, size_t position, size_t bfsz, duf_depthinfo_t * pdi, duf_fileinfo_t * pfi,
                duf_pdi_scb_t prefix_scb, duf_pdi_scb_t suffix_scb DUF_UNUSED )
{
  size_t slen = 0;
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
  case DUF_SFMT_CHR_DEPTH:     /* depth */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "u" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldu", v );
    else
      snprintf( format, fbsz, "%%u" );
#endif

    snprintf( pbuffer, bfsz, format, duf_pdi_reldepth( pdi ) );
    break;
  case DUF_SFMT_CHR_SEQ:       /* seq */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pdi->seq );
    break;
  case DUF_SFMT_CHR_SEQ_NODE:  /* seq_node */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pdi->seq_node );
    break;
  case DUF_SFMT_CHR_SEQ_LEAF:  /* seq_leaf */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pdi->seq_leaf );
    break;
  case DUF_SFMT_CHR_MD5ID:     /* md5id */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif
    snprintf( pbuffer, bfsz, format, pfi->md5id );
    break;
  case DUF_SFMT_CHR_SHA1ID:    /* sha1id */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif
    snprintf( pbuffer, bfsz, format, pfi->sha1id );
    break;
  case DUF_SFMT_CHR_NSAME:     /* nsame */
    {
      char c2 = 0;
      unsigned long long ns = 0;

#if 1
      duf_convert_fmt( format, fbsz, fmt0, "llu" );
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
        ns = ( unsigned long long ) pfi->nsame_md5;
        break;
      case 'x':
        fmt++;
        ns = ( unsigned long long ) pfi->nsame_exif;
        break;
      case 's':
        fmt++;
        ns = ( unsigned long long ) pfi->nsame_sha1;
        break;
      default:
        ns = ( unsigned long long ) pfi->nsame;
        break;
      }
      snprintf( pbuffer, bfsz, format, ns );
    }
    break;
  case DUF_SFMT_CHR_UNDERLINE: /* underline */
    memset( pbuffer, '_', v > 0 ? v : ( v < 0 ? -v : 1 ) );
    break;
  case DUF_SFMT_CHR_SPACE:     /* space */
    /* pbuffer += strlen( pbuffer ); */
    memset( pbuffer, ' ', v > 0 ? v : ( v < 0 ? -v : 1 ) );
    break;
  case DUF_SFMT_CHR_MOVE_TO_POSITION:
    {
      long vp = 0;

      vp = v - position;
      /* memset( pbuffer, ' ', vp > 0 ? vp : ( vp < 0 ? -vp : 1 ) ); */
      memset( pbuffer, ' ', vp > 0 ? vp : ( vp < 0 ? 1 : 1 ) );
    }
    break;
  case DUF_SFMT_CHR_PREFIX:    /* prefix */
    if ( prefix_scb )
    {
      ( prefix_scb ) ( pbuffer, bfsz, pdi );
    }
    break;
  case DUF_SFMT_CHR_DIRID:     /* dirid */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, duf_levinfo_nodedirid( pdi ) );
    break;
  case DUF_SFMT_CHR_NFILES:    /* nfiles */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pdi->pathinfo.levinfo[pdi->pathinfo.depth].items.files );
    break;
  case DUF_SFMT_CHR_NDIRS:     /* ndirs */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pdi->pathinfo.levinfo[pdi->pathinfo.depth].items.dirs );
    break;
  case DUF_SFMT_CHR_DATAID:    /* dataid */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pfi->dataid );
    break;
  case DUF_SFMT_CHR_COLOR:     /* color */
    {
      static char *hls[] = { "", "1;33;41", "1;7;32;44", "1;7;108;33", "1;7;108;32", "1;33;44", "1;37;46", "1;7;33;41", "7;101;35", "30;47" };

#if 0
      if ( v > 0 && v < sizeof( hls ) )
        snprintf( pbuffer, bfsz, "\x1b[%sm", hls[v] );
      else
        snprintf( pbuffer, bfsz, "\x1b[%sm", hls[0] );
#else
      duf_sncolor_s( is_atty, pbuffer, bfsz, "\x1b[%sm", ( v > 0 && (size_t)v < sizeof( hls ) ) ? hls[v] : hls[0] );
#endif
    }
    break;
  case DUF_SFMT_CHR_SUFFIX:    /* suffix */
    break;
  case DUF_SFMT_CHR_INODE:     /* inode */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_ino );
    break;
  case DUF_SFMT_CHR_MODE:      /* mode */
    {
      char modebuf[] = "----------";
      char *pmode = modebuf;

      pmode++;
      if ( S_IRUSR & pfi->st.st_mode )
        *pmode = 'r';
      pmode++;
      if ( S_IWUSR & pfi->st.st_mode )
        *pmode = 'w';
      pmode++;
      if ( S_IXUSR & pfi->st.st_mode )
        *pmode = 'x';
      pmode++;
      if ( S_IRGRP & pfi->st.st_mode )
        *pmode = 'r';
      pmode++;
      if ( S_IWGRP & pfi->st.st_mode )
        *pmode = 'w';
      pmode++;
      if ( S_IXGRP & pfi->st.st_mode )
        *pmode = 'x';
      pmode++;
      if ( S_IROTH & pfi->st.st_mode )
        *pmode = 'r';
      pmode++;
      if ( S_IWOTH & pfi->st.st_mode )
        *pmode = 'w';
      pmode++;
      if ( S_IXOTH & pfi->st.st_mode )
        *pmode = 'x';
      snprintf( pbuffer, bfsz, "%s", modebuf );
    }
    break;
  case DUF_SFMT_CHR_NLINK:     /* nlink */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_nlink );
    break;
  case DUF_SFMT_CHR_USER:      /* user */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_uid );
    break;
  case DUF_SFMT_CHR_GROUP:     /* group */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_gid );
    break;
  case DUF_SFMT_CHR_FILESIZE:  /* filesize */
    {
      unsigned long long sz = pfi->st.st_size;

#if 1
      duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%ldllu.", v );
      else
        snprintf( format, fbsz, "%%llu." );
#endif


      snprintf( pbuffer, bfsz, format, sz );
    }
    break;
  case DUF_SFMT_CHR_FILESIZEH: /* filesize */
    {
      unsigned long long sz = pfi->st.st_size;

      if ( sz < 1024 )
      {
#if 1
        duf_convert_fmt( format, fbsz, fmt0, "llu." );
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
        duf_convert_fmt( format, fbsz, fmt0, "lluk" );
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
        duf_convert_fmt( format, fbsz, fmt0, "lluM" );
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
        duf_convert_fmt( format, fbsz, fmt0, "lluG" );
#else
        if ( v )
          snprintf( format, fbsz, "%%%ldlluG", v );
        else
          snprintf( format, fbsz, "%%lluG" );
#endif

        snprintf( pbuffer, bfsz, format, sz / 1024 / 1024 / 1024 );
      }
    }
    break;
  case DUF_SFMT_CHR_MTIME1:    /* time */
  case DUF_SFMT_CHR_MTIME2:    /* time */
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
        timet = ( time_t ) pfi->st.st_mtim.tv_sec;
        break;
      case 'a':
        timet = ( time_t ) pfi->st.st_atim.tv_sec;
        break;
      case 'c':
        timet = ( time_t ) pfi->st.st_atim.tv_sec;
        break;
      case 'x':
        if ( pfi->exifid )
          timet = ( time_t ) pfi->exifdt;
        break;
      default:
        timet = ( time_t ) pfi->st.st_mtim.tv_sec;
        fmt--;
      }
      if ( timet )
        ptime_tm = localtime_r( &timet, &time_tm );
      switch ( ( char ) c )
      {
      case DUF_SFMT_CHR_MTIME1:
        if ( ptime_tm )
          strftime( stime, sizeof( stime ), "%b %d %Y %H:%M:%S", ptime_tm );
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
              /* DUF_TRACE( temp, 0, "=== '%s'", subfmt ); */
              fmt = efmt;
              fmt++;
            }
          }
          if ( !subfmt )
            subfmt = mas_strdup( "%Y/%m/%d/%H.%M.%S" );
          if ( subfmt && ptime_tm )
            strftime( stime, sizeof( stime ), subfmt, ptime_tm );
          mas_free( subfmt );
        }
        break;
      }
#if 1
      duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif

      snprintf( pbuffer, bfsz, format, stime );
    }
    break;
  case DUF_SFMT_CHR_REALPATH:  /* realpath */
    {
      const char *real_path = NULL;

#if 1
      duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif

      real_path = duf_levinfo_path( pdi );
      snprintf( pbuffer, bfsz, format, real_path );
    }
    break;
  case DUF_SFMT_CHR_RELATIVE_PATH: /* relative realpath */
    {
      const char *rel_real_path = NULL;

#if 1
      duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif


      rel_real_path = duf_levinfo_relpath( pdi );
      snprintf( pbuffer, bfsz, format, rel_real_path );
    }
    break;
  case DUF_SFMT_CHR_FILENAME:  /* filename */
    {
#if 1
      duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif


      snprintf( pbuffer, bfsz, format, pfi->name );
    }
    pbuffer += strlen( pbuffer );
    break;
#if 0
  case 'f':                    /* itemname */
    {
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      snprintf( pbuffer, bfsz, format, duf_levinfo_itemname( pdi ) );
    }
    pbuffer += strlen( pbuffer );
    break;
#endif
  case DUF_SFMT_CHR_MD5SUM:    /* md5sum */
    if ( pfi->md5sum1 || pfi->md5sum2 )
      snprintf( pbuffer, bfsz, "%016llx%016llx", pfi->md5sum1, pfi->md5sum2 );
    else
      snprintf( pbuffer, bfsz, "%-32s", "-" );
    break;
  case DUF_SFMT_CHR_SHA1SUM:   /* sha1sum */
    if ( pfi->sha1sum1 || pfi->sha1sum2 || pfi->sha1sum3 )
      snprintf( pbuffer, bfsz, "%08llx:%016llx:%016llx", pfi->sha1sum1, pfi->sha1sum2, pfi->sha1sum3 );
    else
      snprintf( pbuffer, bfsz, "%-40s", "-" );
    break;
  case DUF_SFMT_CHR_EXIFID:    /* exifid */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    if ( pfi->exifid )
      snprintf( pbuffer, bfsz, format, pfi->exifid );
    else
      *pbuffer = '?';
    break;
  case DUF_SFMT_CHR_CAMERAS:   /* camera */
  case DUF_SFMT_CHR_CAMERA:    /* camera */
    if ( pfi->camera && *pfi->camera )
    {
#if 1
      duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
#endif

      snprintf( pbuffer, bfsz, format, pfi->camera );
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
    break;
  case DUF_SFMT_CHR_MIMEID:    /* mimeid */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pfi->mimeid );
    break;
  case DUF_SFMT_CHR_MIME:      /* mime */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "s" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%lds", v );
    else
      snprintf( format, fbsz, "%%s" );
#endif

    snprintf( pbuffer, bfsz, format, pfi->mime ? pfi->mime : "?" );
    break;
  case DUF_SFMT_CHR_NAMEID:    /* nameid */
#if 1
    duf_convert_fmt( format, fbsz, fmt0, "llu" );
#else
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
#endif

    snprintf( pbuffer, bfsz, format, pfi->nameid );
    break;
  default:
    pbuffer += strlen( pbuffer );
    *pbuffer++ = '%';
    if ( v )
      snprintf( pbuffer, bfsz, "%ld", v );
    pbuffer += strlen( pbuffer );
    *pbuffer++ = c;
    *pbuffer = 0;
    break;
  }
  slen = strlen( pbuffer );
  pbuffer += slen;
  *pfmt = fmt;
  *ppbuffer = pbuffer;
  return slen;
}

char *
duf_sformat_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, int is_atty, const char *format, duf_pdi_scb_t prefix_scb,
                       duf_pdi_scb_t suffix_scb, size_t * pslen )
{
  size_t slen = 0;
  const char *fmt = format;
  char *buffer = NULL;
  char *pbuffer = NULL;
  size_t bfsz = 1024 * 16;

  pbuffer = buffer = mas_malloc( bfsz + 8 );
  memset( buffer, 0, bfsz );
  while ( fmt && *fmt && buffer + bfsz - pbuffer > 0 )
  {
    if ( *fmt == '%' )
    {
      fmt++;
      slen += duf_sformat_id( is_atty, &fmt, &pbuffer, pbuffer - buffer, buffer + bfsz - pbuffer /* remaining bytes cnt */ , pdi, pfi, prefix_scb,
                              suffix_scb );
    }
    else if ( *fmt == '\\' )
    {
      fmt++;
      if ( *fmt == 'n' )
        *pbuffer++ = '\n';
      else if ( *fmt == 'r' )
        *pbuffer++ = '\r';
      else if ( *fmt == 't' )
        *pbuffer++ = '\t';
      else
      {
        *pbuffer++ = '\\';
        *pbuffer++ = *fmt;
      }
      fmt++;
    }
    else
    {
      *pbuffer++ = *fmt++;
    }
  }
  if ( pslen )
    *pslen = slen;
  return buffer;
}

size_t
duf_print_sformat_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, const char *format, duf_pdi_scb_t prefix_scb, duf_pdi_scb_t suffix_scb,
                             size_t max_width DUF_UNUSED )
{
  size_t slen = 0;
  char *buffer;
  FILE *out;

  out = duf_config && duf_config->cli.output.out ? duf_config->cli.output.out : stdout;
  buffer = duf_sformat_file_info( pdi, pfi, isatty( fileno( out ) ), format, prefix_scb, suffix_scb, &slen );
#if 0
  {
    if ( max_width > 0 && buffer[slen - 1] == '\n' && max_width < slen )
    {
      buffer[max_width] = 0;
      buffer[max_width - 1] = '\n';
    }
    else
    {
    }
  }
#endif

  /* DUF_WRITES( 0, "[[[" ); */
  DUF_WRITES( 0, buffer );
  /* DUF_WRITES( 0, "]]]" ); */
  mas_free( buffer );
  return slen;
}
