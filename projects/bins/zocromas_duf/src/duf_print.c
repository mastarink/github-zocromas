#include <string.h>
#include <stdlib.h>
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
duf_print_file_info_depth( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %h : depth */
  if ( duf_config->cli.bformat.v.flag.depth && ( !bformat || bformat->v.flag.depth ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{depth}" ) );
    DUF_PRINTF( 6, ".  +%-2u ", duf_pdi_reldepth( pdi ) );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_seq( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %Q : seq */
  if ( duf_config->cli.bformat.v.flag.seq && ( !bformat || bformat->v.flag.seq ) )
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
duf_print_file_info_seq_node( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %q : seq_node */
  if ( duf_config->cli.bformat.v.flag.seq_node && ( !bformat || bformat->v.flag.seq_node ) )
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
duf_print_file_info_seq_leaf( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %# : seq_leaf */
  if ( duf_config->cli.bformat.v.flag.seq_leaf && ( !bformat || bformat->v.flag.seq_leaf ) )
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
duf_print_file_info_md5id( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %M : md5id */
  if ( duf_config->cli.bformat.v.flag.md5id && ( !bformat || bformat->v.flag.md5id ) )
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
duf_print_file_info_nsame( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %S : nsame */
  if ( duf_config->cli.bformat.nsame && ( !bformat || bformat->nsame ) )
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
duf_print_file_info_prefix( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat, duf_pdi_cb_t prefix_cb,
                            duf_pdi_cb_t suffix_cb )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %P : prefix */
  if ( duf_config->cli.bformat.v.flag.prefix && ( !bformat || bformat->v.flag.prefix ) )
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
duf_print_file_info_dirid( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %I : dirid */
  if ( duf_config->cli.bformat.v.flag.dirid && ( !bformat || bformat->v.flag.dirid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{dirid}" ) );
    DUF_PRINTF( 4, ".d%-6llu ", duf_levinfo_nodedirid( pdi ) );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.bformat.v.flag.dirid_space && ( !bformat || bformat->v.flag.dirid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{dirid_space}" ) );
    DUF_PRINTF( 4, ".%3s", "" );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_nfiles( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %F : nfiles */
  if ( duf_config->cli.bformat.v.flag.nfiles && ( !bformat || bformat->v.flag.nfiles ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nfiles}" ) );
    DUF_PRINTF( 0, ".(%6llu) ", pdi->levinfo[pdi->depth].items.files );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.bformat.v.flag.nfiles_space && ( !bformat || bformat->v.flag.nfiles_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nfiles_space}" ) );
    DUF_PRINTF( 0, ". %6s  ", "" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_ndirs( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %D : ndirs */
  if ( duf_config->cli.bformat.v.flag.ndirs && ( !bformat || bformat->v.flag.ndirs ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{ndirs}" ) );
    DUF_PRINTF( 0, ".{%6llu} ", pdi->levinfo[pdi->depth].items.dirs );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.bformat.v.flag.ndirs_space && ( !bformat || bformat->v.flag.ndirs_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{ndirs_space}" ) );
    DUF_PRINTF( 0, ". %6s  ", "" );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_dataid( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %A : dataid */
  if ( duf_config->cli.bformat.v.flag.dataid && ( !bformat || bformat->v.flag.dataid ) )
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
duf_print_file_info_suffix( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat, duf_pdi_cb_t prefix_cb,
                            duf_pdi_cb_t suffix_cb )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %~ : suffix */
  if ( duf_config->cli.bformat.v.flag.suffix && ( !bformat || bformat->v.flag.suffix ) )
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
duf_print_file_info_inode( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %O : inode */
  if ( duf_config->cli.bformat.v.flag.inode && ( !bformat || bformat->v.flag.inode ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{inode}" ) );
    DUF_PRINTF( 9, ".i%-9llu ", ( unsigned long long ) pfi->st.st_ino );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_mode( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %m : mode */
  if ( duf_config->cli.bformat.v.flag.mode && ( !bformat || bformat->v.flag.mode ) )
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
duf_print_file_info_nlink( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %n : nlink */
  if ( duf_config->cli.bformat.v.flag.nlink && ( !bformat || bformat->v.flag.nlink ) )
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
duf_print_file_info_user( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %u : user */
  if ( duf_config->cli.bformat.v.flag.user && ( !bformat || bformat->v.flag.user ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{user}" ) );
    DUF_PRINTF( 2, ". u%-9llu", ( unsigned long long ) pfi->st.st_uid );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_group( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %g : group */
  if ( duf_config->cli.bformat.v.flag.group && ( !bformat || bformat->v.flag.group ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{group}" ) );
    DUF_PRINTF( 3, ". g%-9llu", ( unsigned long long ) pfi->st.st_gid );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_filesize( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %z : filesize */
  if ( duf_config->cli.bformat.v.flag.filesize && ( !bformat || bformat->v.flag.filesize ) )
  {
    unsigned long long sz = pfi->st.st_size;

    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{filesize}" ) );
    if ( duf_config->cli.bformat.v.flag.human || ( bformat && bformat->v.flag.human ) )
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
duf_print_file_info_mtime( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %t : mtime */
  if ( duf_config->cli.bformat.v.flag.mtime && ( !bformat || bformat->v.flag.mtime ) )
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
duf_print_file_info_realpath( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %r : realpath */
  if ( duf_config->cli.bformat.v.flag.realpath && ( !bformat || bformat->v.flag.realpath ) )
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
duf_print_file_info_filename( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  /* DUF_PRINTF( 0, "[[ %d : %d  ]]", duf_config->cli.bformat.v.flag.filename, bformat->v.flag.filename ); */
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %f : filename */
  if ( duf_config->cli.bformat.v.flag.filename && ( !bformat || bformat->v.flag.filename ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{filename}" ) );
    if ( bformat->v.flag.short_filename )
      DUF_PRINTF( 0, ".%-12s", pfi->name );
    else
      DUF_PRINTF( 0, ".%-30s", pfi->name );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_md5( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %@ : md5sum */
  if ( duf_config->cli.bformat.v.flag.md5 && ( !bformat || bformat->v.flag.md5 ) )
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
duf_print_file_info_exifid( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %X : exifid */
  if ( duf_config->cli.bformat.v.flag.exifid && ( !bformat || bformat->v.flag.exifid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{exifid}" ) );
    if ( pfi->exifid )
      DUF_PRINTF( 5, ". X%-6llu", pfi->exifid );
    else
      DUF_PRINTF( 5, ".  %-6s", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.bformat.v.flag.exifid_space && ( !bformat || bformat->v.flag.exifid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{exifid_space}" ) );
    DUF_PRINTF( 5, ".  %s", "" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_exif_date_time( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %T : exif datatime */
  if ( duf_config->cli.bformat.v.flag.exifdt && ( !bformat || bformat->v.flag.exifdt ) )
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
duf_print_file_info_mimeid( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %E : mimeid */
  if ( duf_config->cli.bformat.v.flag.mimeid && ( !bformat || bformat->v.flag.mimeid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mimeid}" ) );
    if ( pfi->mimeid )
      DUF_PRINTF( 5, ". M%-6llu", pfi->mimeid );
    else
      DUF_PRINTF( 5, ".  %-6s", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.bformat.v.flag.mimeid_space && ( !bformat || bformat->v.flag.mimeid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mimeid_space}" ) );
    DUF_PRINTF( 5, ".  %3s", "" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_mime( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %e : mime */
  if ( ( duf_config->cli.bformat.v.flag.mime && ( !bformat || bformat->v.flag.mime ) ) )
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
duf_print_file_info_nameid( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %N : nameid */
  if ( duf_config->cli.bformat.v.flag.nameid && ( !bformat || bformat->v.flag.nameid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nameid}" ) );
    if ( pfi->nameid )
      DUF_PRINTF( 9, ". N%-6llu", pfi->nameid );
    else
      DUF_PRINTF( 9, ".  %-6s", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.bformat.v.flag.nameid_space && ( !bformat || bformat->v.flag.nameid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nameid_space}" ) );
    DUF_PRINTF( 5, ".  %1s", "" );
    ok++;
  }

  return ok;
}

static int __attribute__ ( ( unused ) ) duf_print_file_info_template( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  return ok;
}

/***
   %h : depth
   %Q : seq
   %q : seq_node
   %# : seq_leaf
   %M : md5id
   %S : nsame
   %P : prefix
   %I : dirid
   %F : nfiles
   %D : ndirs
   %A : dataid
   %~ : suffix
   %O : inode
   %m : mode
   %n : nlink
   %u : user
   %g : group
   %z : filesize
   %t : mtime
   %r : realpath
   %f : filename
   %@ : md5sum
   %X : exifid
   %T : exif datatime
   %E : mimeid
   %e : mime
   %N : nameid
   %s : << space >>

***/
static int
duf_sformat_id( const char **pfmt, char **ppbuffer, size_t bfsz, duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_pdi_scb_t prefix_scb,
                duf_pdi_scb_t suffix_scb )
{
  int ok = 0;
  char c;
  char *pe = NULL;
  long v __attribute__ ( ( unused ) ) = 0;
  size_t fbsz = 512;
  const char *fmt = *pfmt;
  char *pbuffer = *ppbuffer;
  char format[fbsz];

  v = strtol( fmt, &pe, 10 );
  fmt = pe;
  c = *fmt++;
  switch ( c )
  {
  case 'h':                    /* depth */
    if ( v )
      snprintf( format, fbsz, "%%%ldu", v );
    else
      snprintf( format, fbsz, "%%u" );
    snprintf( pbuffer, bfsz, format, duf_pdi_reldepth( pdi ) );
    ok++;
    break;
  case 'Q':                    /* seq */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pdi->seq );
    ok++;
    break;
  case 'q':                    /* seq_node */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pdi->seq_node );
    ok++;
    break;
  case '#':                    /* seq_leaf */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pdi->seq_leaf );
    ok++;
    break;
  case 'M':                    /* md5id */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pfi->md5id );
    ok++;
    break;
  case 'S':                    /* nsame */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->nsame );
    ok++;
    break;
  case 's':                    /* prefix */
    {
      if ( v )
      {
        size_t l;

        v = v < 0 ? -v : v;
        l = ( v < bfsz ) ? v : bfsz;
        memset( pbuffer, ' ', l );
      }
      ok++;
    }
    break;
  case 'P':                    /* prefix */
    if ( prefix_scb )
    {
      char sprefix[512];

      ( prefix_scb ) ( sprefix, sizeof( sprefix ), pdi );
/*
      if ( v )
      {
        size_t l;

        l = ( v < bfsz ) ? v : bfsz;
        memset( pbuffer, ' ', l );
        bfsz -= l;
        pbuffer += l;
      }
*/
      strncpy( pbuffer, sprefix, bfsz );
      ok++;
    }
    break;
  case 'I':                    /* dirid */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, duf_levinfo_nodedirid( pdi ) );
    ok++;
    break;
  case 'L':                    /* nfiles */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pdi->levinfo[pdi->depth].items.files );
    ok++;
    break;
  case 'D':                    /* ndirs */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pdi->levinfo[pdi->depth].items.dirs );
    ok++;
    break;
  case 'A':                    /* dataid */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pfi->dataid );
    ok++;
    break;
  case '~':                    /* suffix */
    break;
  case 'O':                    /* inode */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_ino );
    ok++;
    break;
  case 'm':                    /* mode */
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
      ok++;
    }
    break;
  case 'n':                    /* nlink */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_nlink );
    ok++;
    break;
  case 'u':                    /* user */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_uid );
    ok++;
    break;
  case 'g':                    /* group */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, ( unsigned long long ) pfi->st.st_gid );
    ok++;
    break;
  case 'z':                    /* filesize */
    {
      unsigned long long sz = pfi->st.st_size;

      if ( 0 /* not human */  )
      {
      }
      else
      {
        if ( sz < 1024 )
        {
          if ( v )
            snprintf( format, fbsz, "%%%ldllu.", v );
          else
            snprintf( format, fbsz, "%%llu." );
          snprintf( pbuffer, bfsz, format, sz );
          ok++;
        }
        else if ( sz < 1024 * 1024 )
        {
          if ( v )
            snprintf( format, fbsz, "%%%ldlluk", v );
          else
            snprintf( format, fbsz, "%%lluk" );
          snprintf( pbuffer, bfsz, format, sz / 1024 );
          ok++;
        }
        else if ( sz < 1024 * 1024 * 1024 )
        {
          if ( v )
            snprintf( format, fbsz, "%%%ldlluM", v );
          else
            snprintf( format, fbsz, "%%lluM" );
          snprintf( pbuffer, bfsz, format, sz / 1024 / 1024 );
          ok++;
        }
        else
        {
          if ( v )
            snprintf( format, fbsz, "%%%ldlluG", v );
          else
            snprintf( format, fbsz, "%%lluG" );
          snprintf( pbuffer, bfsz, format, sz / 1024 / 1024 / 1024 );
          ok++;
        }
      }
    }
    break;
  case 't':                    /* mtime */
    {
      time_t mtimet;
      struct tm mtimetm, *pmtimetm;
      char mtimes[128];

      mtimet = ( time_t ) pfi->st.st_mtim.tv_sec;
      pmtimetm = localtime_r( &mtimet, &mtimetm );
      strftime( mtimes, sizeof( mtimes ), "%b %d %Y %H:%M:%S", pmtimetm );
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );
      snprintf( pbuffer, bfsz, format, mtimes );
      ok++;
    }
    break;
  case 'r':                    /* realpath */
    {
      const char *real_path = NULL;

      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      real_path = duf_levinfo_path( pdi );
      snprintf( pbuffer, bfsz, format, real_path );
      ok++;
    }
    break;
  case 'f':                    /* filename */
    {
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      snprintf( pbuffer, bfsz, format, pfi->name );
      ok++;
    }
    pbuffer += strlen( pbuffer );
    break;
#if 0
  case 'F':                    /* itemname */
    {
      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      snprintf( pbuffer, bfsz, format, duf_levinfo_itemname( pdi ) );
      ok++;
    }
    pbuffer += strlen( pbuffer );
    break;
#endif
  case '@':                    /* md5sum */
    if ( pfi->md5sum1 || pfi->md5sum2 )
      snprintf( pbuffer, bfsz, "%016llx%016llx", pfi->md5sum1, pfi->md5sum2 );
    else
      snprintf( pbuffer, bfsz, "%-32s", "-" );
    ok++;
    break;
  case 'X':                    /* exifid */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pfi->exifid );
    ok++;
    break;
  case 'T':                    /* exif datatime */
    {
      time_t xtimet;
      struct tm xtimetm, *pxtimetm;
      char xtimes[128];

      if ( v )
        snprintf( format, fbsz, "%%%lds", v );
      else
        snprintf( format, fbsz, "%%s" );

      xtimet = ( time_t ) pfi->exifdt;
      pxtimetm = localtime_r( &xtimet, &xtimetm );
      strftime( xtimes, sizeof( xtimes ), "%b %d %Y %H:%M:%S", pxtimetm );
      snprintf( pbuffer, bfsz, format, xtimes );
      ok++;
    }
    break;
  case 'E':                    /* mimeid */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pfi->mimeid );
    ok++;
    break;
  case 'e':                    /* mime */
    if ( v )
      snprintf( format, fbsz, "%%%lds", v );
    else
      snprintf( format, fbsz, "%%s" );
    snprintf( pbuffer, bfsz, format, pfi->mime );
    ok++;
    break;
  case 'N':                    /* nameid */
    if ( v )
      snprintf( format, fbsz, "%%%ldllu", v );
    else
      snprintf( format, fbsz, "%%llu" );
    snprintf( pbuffer, bfsz, format, pfi->nameid );
    ok++;
    break;
  case '_':                    /* space */
    pbuffer += strlen( pbuffer );
    memset( pbuffer, ' ', v > 0 ? v : ( v < 0 ? -v : 1 ) );
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
  pbuffer += strlen( pbuffer );
  *pfmt = fmt;
  *ppbuffer = pbuffer;
  return ok;
}

int
duf_print_sformat_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, const char *format, duf_pdi_scb_t prefix_scb, duf_pdi_scb_t suffix_scb )
{
  int ok = 0;
  const char *fmt = format;
  char *buffer;
  char *pbuffer;
  size_t bfsz = 1024 * 16;

  pbuffer = buffer = mas_malloc( bfsz + 8 );
  memset( buffer, 0, bfsz );
  while ( fmt && *fmt && buffer + bfsz - pbuffer > 0 )
  {
    if ( *fmt == '%' )
    {
      fmt++;
      ok = duf_sformat_id( &fmt, &pbuffer, buffer + bfsz - pbuffer, pdi, pfi, prefix_scb, suffix_scb );
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
  DUF_WRITES( 0, buffer );
  mas_free( buffer );
  return ok;
}

int
duf_print_bformat_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat, duf_pdi_cb_t prefix_cb,
                             duf_pdi_cb_t suffix_cb )
{
  int ok = 0;

  ok += duf_print_file_info_depth( pdi, pfi, bformat );
  ok += duf_print_file_info_seq( pdi, pfi, bformat );
  ok += duf_print_file_info_seq_node( pdi, pfi, bformat );
  ok += duf_print_file_info_seq_leaf( pdi, pfi, bformat );
  ok += duf_print_file_info_md5id( pdi, pfi, bformat );
  ok += duf_print_file_info_nsame( pdi, pfi, bformat );
  ok += duf_print_file_info_prefix( pdi, pfi, bformat, prefix_cb, suffix_cb );
  ok += duf_print_file_info_dirid( pdi, pfi, bformat );
  ok += duf_print_file_info_nfiles( pdi, pfi, bformat );
  ok += duf_print_file_info_ndirs( pdi, pfi, bformat );
  ok += duf_print_file_info_dataid( pdi, pfi, bformat );
  ok += duf_print_file_info_suffix( pdi, pfi, bformat, prefix_cb, suffix_cb );
  ok += duf_print_file_info_inode( pdi, pfi, bformat );
  ok += duf_print_file_info_mode( pdi, pfi, bformat );
  ok += duf_print_file_info_nlink( pdi, pfi, bformat );
  ok += duf_print_file_info_user( pdi, pfi, bformat );
  ok += duf_print_file_info_group( pdi, pfi, bformat );
  ok += duf_print_file_info_filesize( pdi, pfi, bformat );
  ok += duf_print_file_info_mtime( pdi, pfi, bformat );
  ok += duf_print_file_info_realpath( pdi, pfi, bformat );
  ok += duf_print_file_info_filename( pdi, pfi, bformat );
  ok += duf_print_file_info_md5( pdi, pfi, bformat );
  ok += duf_print_file_info_exifid( pdi, pfi, bformat );
  ok += duf_print_file_info_exif_date_time( pdi, pfi, bformat );
  ok += duf_print_file_info_mimeid( pdi, pfi, bformat );
  ok += duf_print_file_info_mime( pdi, pfi, bformat );
  ok += duf_print_file_info_nameid( pdi, pfi, bformat );


  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( !ok )
    DUF_PRINTF( 0, " %llx :: %llx", bformat->v.bit, duf_config->cli.bformat.v.bit );
  assert( ok );
  return ok;
}
