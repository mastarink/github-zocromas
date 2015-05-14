#include <time.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

/* #include "duf_levinfo.h" */
#include "duf_levinfo_ref.h"

#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_db2path.h"


/* ###################################################################### */
#include "duf_print.h"
/* ###################################################################### */

static int
duf_print_file_info_depth( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* h */
  if ( duf_config->cli.format.v.flag.depth && ( !format || format->v.flag.depth ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{depth}" ) );
    DUF_PRINTF( 6, ".  +%-2u ", duf_pdi_reldepth( pdi ) );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_seq( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* Q */
  if ( duf_config->cli.format.v.flag.seq && ( !format || format->v.flag.seq ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{seq}" ) );
    DUF_PRINTF( 2, ".#%-3llu ", pdi->seq );
    ok++;
  }
  else
  {
    /* DUF_PRINTF( 2, ". %-3s ", "" ); */
  }

  return ok;
}

static int
duf_print_file_info_seq_node( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* q */
  if ( duf_config->cli.format.v.flag.seq_node && ( !format || format->v.flag.seq_node ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{seq_node}" ) );
    DUF_PRINTF( 2, ".#%-3llu ", pdi->seq_node );
    ok++;
  }
  else
  {
    /* DUF_PRINTF( 2, ". %-3s ", "" ); */
  }

  return ok;
}

static int
duf_print_file_info_seq_leaf( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* # */
  if ( duf_config->cli.format.v.flag.seq_leaf && ( !format || format->v.flag.seq_leaf ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{seq_leaf}" ) );
    DUF_PRINTF( 2, ".#%-3llu ", pdi->seq_leaf );
    ok++;
  }
  else
  {
    DUF_PRINTF( 2, ". %-3s ", "" );
  }

  return ok;
}

static int
duf_print_file_info_md5id( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* M */
  if ( duf_config->cli.format.v.flag.md5id && ( !format || format->v.flag.md5id ) )
  {
    if ( 1 || pfi->nsame > 1 )
    {
      DUF_DEBUG( 12, DUF_PRINTF( 0, ".{md5id}" ) );
      if ( pfi->md5id )
        DUF_PRINTF( 0, ". _%-6llu", pfi->md5id );
      else
        DUF_PRINTF( 0, ".  %-6s", "" );
      ok++;
    }
    else
    {
      DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nomd5id}" ) );
      DUF_PRINTF( 0, ".  %-6s", "" );
      ok++;
    }
  }

  return ok;
}

static int
duf_print_file_info_nsame( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %S */
  if ( duf_config->cli.format.nsame && ( !format || format->nsame ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nsame}" ) );
    if ( pfi->nsame > 1 )
    {
      DUF_PRINTF( 0, ". =%-3llu", ( unsigned long long ) pfi->nsame );
    }
    else
      DUF_PRINTF( 0, ".  %-3s", "." );
    ok++;
  }

  return ok;
}

int
duf_print_file_info_prefix( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format, duf_pdi_cb_t prefix_cb, duf_pdi_cb_t suffix_cb )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %P */
  if ( duf_config->cli.format.v.flag.prefix && ( !format || format->v.flag.prefix ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{prefix}" ) );
    if ( prefix_cb )
      ( prefix_cb ) ( pdi );
    /* else                                                                           */
    /*   DUF_PRINTF( 0, ".= __________________________________________________ = " ); */
    ok++;
  }
  else
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{noprefix}" ) );
    /* DUF_PRINTF( 0, ".      " ); */
  }
  return ok;
}

static int
duf_print_file_info_dirid( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %I */
  if ( duf_config->cli.format.v.flag.dirid && ( !format || format->v.flag.dirid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{dirid}" ) );
    DUF_PRINTF( 4, ".d%-6llu ", duf_levinfo_nodedirid( pdi ) );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.format.v.flag.dirid_space && ( !format || format->v.flag.dirid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{dirid_space}" ) );
    DUF_PRINTF( 4, ".%3s", "" );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_nfiles( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %F */
  if ( duf_config->cli.format.v.flag.nfiles && ( !format || format->v.flag.nfiles ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nfiles}" ) );
    DUF_PRINTF( 0, ".(%6llu) ", pdi->levinfo[pdi->depth].items.files );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.nfiles_space && ( !format || format->v.flag.nfiles_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nfiles_space}" ) );
    DUF_PRINTF( 0, ". %6s  ", "" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_ndirs( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %D */
  if ( duf_config->cli.format.v.flag.ndirs && ( !format || format->v.flag.ndirs ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{ndirs}" ) );
    DUF_PRINTF( 0, ".{%6llu} ", pdi->levinfo[pdi->depth].items.dirs );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.ndirs_space && ( !format || format->v.flag.ndirs_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{ndirs_space}" ) );
    DUF_PRINTF( 0, ". %6s  ", "" );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_dataid( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %i */
  if ( duf_config->cli.format.v.flag.dataid && ( !format || format->v.flag.dataid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{dataid}" ) );
    if ( pfi->dataid )
      DUF_PRINTF( 2, ".~%-6llu ", pfi->dataid );
    else
      DUF_PRINTF( 2, ". %6s ", "" );
    ok++;
  }
  return ok;
}

int
duf_print_file_info_suffix( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format, duf_pdi_cb_t prefix_cb, duf_pdi_cb_t suffix_cb )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %~ */
  if ( duf_config->cli.format.v.flag.suffix && ( !format || format->v.flag.suffix ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{suffix}" ) );
    if ( suffix_cb )
      ( suffix_cb ) ( pdi );
    else
      DUF_PRINTF( 0, ".=%%%%%%%%%%%%%%%%%%%%%%%%= " );
    ok++;
  }
  else
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nosuffix}" ) );
    /* DUF_PRINTF( 0, ".      " ); */
  }

  return ok;
}

static int
duf_print_file_info_inode( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %O */
  if ( duf_config->cli.format.v.flag.inode && ( !format || format->v.flag.inode ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{inode}" ) );
    DUF_PRINTF( 9, ".i%-9llu ", ( unsigned long long ) pfi->st.st_ino );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_mode( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %m */
  if ( duf_config->cli.format.v.flag.mode && ( !format || format->v.flag.mode ) )
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
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mode}" ) );
    DUF_PRINTF( 7, ". %s", modebuf );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_nlink( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %n */
  if ( duf_config->cli.format.v.flag.nlink && ( !format || format->v.flag.nlink ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nlink}" ) );
    if ( pfi->st.st_nlink <= 1 )
      DUF_PRINTF( 0, ".   %2s", "" );
    else
      DUF_PRINTF( 0, ". ln%-2llu", ( unsigned long long ) pfi->st.st_nlink );

    ok++;
  }
  return ok;
}

static int
duf_print_file_info_user( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %u */
  if ( duf_config->cli.format.v.flag.user && ( !format || format->v.flag.user ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{user}" ) );
    DUF_PRINTF( 2, ". u%-9llu", ( unsigned long long ) pfi->st.st_uid );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_group( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %g */
  if ( duf_config->cli.format.v.flag.group && ( !format || format->v.flag.group ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{group}" ) );
    DUF_PRINTF( 3, ". g%-9llu", ( unsigned long long ) pfi->st.st_gid );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_filesize( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %z */
  if ( duf_config->cli.format.v.flag.filesize && ( !format || format->v.flag.filesize ) )
  {
    unsigned long long sz = pfi->st.st_size;

    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{filesize}" ) );
    if ( duf_config->cli.format.v.flag.human || ( format && format->v.flag.human ) )
    {
      if ( sz < 1024 )
        DUF_PRINTF( 0, ". %9llu.", sz );
      else if ( sz < 1024 * 1024 )
        DUF_PRINTF( 0, ". %9lluk", sz / 1024 );
      else if ( sz < 1024 * 1024 * 1024 )
        DUF_PRINTF( 0, ". %9lluM", sz / 1024 / 1024 );
      else
        DUF_PRINTF( 0, ". %9lluG", sz / 1024 / 1024 / 1024 );
    }
    else
      DUF_PRINTF( 2, ". %9llu.", sz );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_mtime( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %t */
  if ( duf_config->cli.format.v.flag.mtime && ( !format || format->v.flag.mtime ) )
  {
    time_t mtimet;
    struct tm mtimetm, *pmtimetm;
    char mtimes[128];

    mtimet = ( time_t ) pfi->st.st_mtim.tv_sec;
    pmtimetm = localtime_r( &mtimet, &mtimetm );
    strftime( mtimes, sizeof( mtimes ), "%b %d %Y %H:%M:%S", pmtimetm );
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mtime}" ) );
    /* DUF_PRINTF( 0, ". %-s (%lu) ", mtimes, mtimet ); */
    DUF_PRINTF( 2, ". %-s", mtimes );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_realpath( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %r */
  if ( duf_config->cli.format.v.flag.realpath && ( !format || format->v.flag.realpath ) )
  {
    const char *real_path = NULL;

    real_path = duf_levinfo_path( pdi );
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{real_path}" ) );
    DUF_PRINTF( 0, ".%s", real_path );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_filename( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  /* DUF_PRINTF( 0, "[[ %d : %d  ]]", duf_config->cli.format.v.flag.filename, format->v.flag.filename ); */
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %f */
  if ( duf_config->cli.format.v.flag.filename && ( !format || format->v.flag.filename ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{filename}" ) );
    if ( format->v.flag.short_filename )
      DUF_PRINTF( 0, ".%-12s", pfi->name );
    else
      DUF_PRINTF( 0, ".%-30s", pfi->name );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_md5( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %@ */
  if ( duf_config->cli.format.v.flag.md5 && ( !format || format->v.flag.md5 ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{md5}" ) );
    if ( pfi->md5sum1 || pfi->md5sum2 )
      DUF_PRINTF( 8, ". %016llx%016llx", pfi->md5sum1, pfi->md5sum2 );
    else
      DUF_PRINTF( 8, ". %-32s", "-" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_exifid( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %X */
  if ( duf_config->cli.format.v.flag.exifid && ( !format || format->v.flag.exifid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{exifid}" ) );
    if ( pfi->exifid )
      DUF_PRINTF( 5, ". X%-6llu", pfi->exifid );
    else
      DUF_PRINTF( 5, ".  %-6s", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.format.v.flag.exifid_space && ( !format || format->v.flag.exifid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{exifid_space}" ) );
    DUF_PRINTF( 5, ".  %s", "" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_exif_date_time( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %T */
  if ( duf_config->cli.format.v.flag.exifdt && ( !format || format->v.flag.exifdt ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{exifdt}" ) );
    {
      time_t xtimet;
      struct tm xtimetm, *pxtimetm;
      char xtimes[128];

      xtimet = ( time_t ) pfi->exifdt;
      pxtimetm = localtime_r( &xtimet, &xtimetm );
      strftime( xtimes, sizeof( xtimes ), "%b %d %Y %H:%M:%S", pxtimetm );
      DUF_DEBUG( 12, DUF_PRINTF( 0, ".{xtime}" ) );
      /* DUF_PRINTF( 0, ". %-s (%lu) ", xtimes, xtimet ); */
      DUF_PRINTF( 5, ". {%-s}", xtimes );
      ok++;
    }
  }

  return ok;
}

static int
duf_print_file_info_mimeid( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %e */
  if ( duf_config->cli.format.v.flag.mimeid && ( !format || format->v.flag.mimeid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mimeid}" ) );
    if ( pfi->mimeid )
      DUF_PRINTF( 5, ". M%-6llu", pfi->mimeid );
    else
      DUF_PRINTF( 5, ".  %-6s", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.format.v.flag.mimeid_space && ( !format || format->v.flag.mimeid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mimeid_space}" ) );
    DUF_PRINTF( 5, ".  %3s", "" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_mime( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %E */
  if ( ( duf_config->cli.format.v.flag.mime && ( !format || format->v.flag.mime ) ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mime}" ) );
    if ( pfi->mime )
      DUF_PRINTF( 3, ". <%s>", pfi->mime );
    else
      DUF_PRINTF( 3, ". <%s>", "" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_nameid( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %N */
  if ( duf_config->cli.format.v.flag.nameid && ( !format || format->v.flag.nameid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nameid}" ) );
    if ( pfi->nameid )
      DUF_PRINTF( 9, ". N%-6llu", pfi->nameid );
    else
      DUF_PRINTF( 9, ".  %-6s", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.format.v.flag.nameid_space && ( !format || format->v.flag.nameid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nameid_space}" ) );
    DUF_PRINTF( 5, ".  %1s", "" );
    ok++;
  }

  return ok;
}

static int __attribute__ ( ( unused ) ) duf_print_file_info_template( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format )
{
  int ok = 0;

  return ok;
}




int
duf_print_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format, duf_pdi_cb_t prefix_cb, duf_pdi_cb_t suffix_cb )
{
  int ok = 0;

  ok += duf_print_file_info_depth( pdi, pfi, format );
  ok += duf_print_file_info_seq( pdi, pfi, format );
  ok += duf_print_file_info_seq_node( pdi, pfi, format );
  ok += duf_print_file_info_seq_leaf( pdi, pfi, format );
  ok += duf_print_file_info_md5id( pdi, pfi, format );
  ok += duf_print_file_info_nsame( pdi, pfi, format );
  ok += duf_print_file_info_prefix( pdi, pfi, format, prefix_cb, suffix_cb );
  ok += duf_print_file_info_dirid( pdi, pfi, format );
  ok += duf_print_file_info_nfiles( pdi, pfi, format );
  ok += duf_print_file_info_ndirs( pdi, pfi, format );
  ok += duf_print_file_info_dataid( pdi, pfi, format );
  ok += duf_print_file_info_suffix( pdi, pfi, format, prefix_cb, suffix_cb );
  ok += duf_print_file_info_inode( pdi, pfi, format );
  ok += duf_print_file_info_mode( pdi, pfi, format );
  ok += duf_print_file_info_nlink( pdi, pfi, format );
  ok += duf_print_file_info_user( pdi, pfi, format );
  ok += duf_print_file_info_group( pdi, pfi, format );
  ok += duf_print_file_info_filesize( pdi, pfi, format );
  ok += duf_print_file_info_mtime( pdi, pfi, format );
  ok += duf_print_file_info_realpath( pdi, pfi, format );
  ok += duf_print_file_info_filename( pdi, pfi, format );
  ok += duf_print_file_info_md5( pdi, pfi, format );
  ok += duf_print_file_info_exifid( pdi, pfi, format );
  ok += duf_print_file_info_exif_date_time( pdi, pfi, format );
  ok += duf_print_file_info_mimeid( pdi, pfi, format );
  ok += duf_print_file_info_mime( pdi, pfi, format );
  ok += duf_print_file_info_nameid( pdi, pfi, format );


  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( !ok )
    DUF_PRINTF( 0, " %llx :: %llx", format->v.bit, duf_config->cli.format.v.bit );
  assert( ok );
  return ok;
}