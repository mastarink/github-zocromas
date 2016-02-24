/* #undef MAS_TRACING */
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"

#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"



/* ###################################################################### */
#include "duf_printb.h"
/* ###################################################################### */

static int
duf_print_file_info_depth( const duf_depthinfo_t * pdi, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %h : depth */
  if ( duf_config->opt.bformat.v.flag.depth && ( !bformat || bformat->v.flag.depth ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{depth}" ) );
    DUF_PRINTF( 6, ".  +%-2u ", duf_pdi_reldepth( pdi ) );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_seq(const  duf_depthinfo_t * pdi, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %Q : seq */
  if ( duf_config->opt.bformat.v.flag.seq && ( !bformat || bformat->v.flag.seq ) )
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
duf_print_file_info_seq_node(const  duf_depthinfo_t * pdi, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %q : seq_node */
  if ( duf_config->opt.bformat.v.flag.seq_node && ( !bformat || bformat->v.flag.seq_node ) )
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
duf_print_file_info_seq_leaf(const  duf_depthinfo_t * pdi, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %# : seq_leaf */
  if ( duf_config->opt.bformat.v.flag.seq_leaf && ( !bformat || bformat->v.flag.seq_leaf ) )
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
duf_print_file_info_md5id( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %M : md5id */
  if ( duf_config->opt.bformat.v.flag.md5id && ( !bformat || bformat->v.flag.md5id ) )
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
duf_print_file_info_nsame( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %S : nsame */
  if ( duf_config->opt.bformat.nsame && ( !bformat || bformat->nsame ) )
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
duf_print_file_info_prefix( const duf_depthinfo_t * pdi, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat, duf_pdi_cb_t prefix_cb,
                            duf_pdi_cb_t suffix_cb DUF_UNUSED )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %P : prefix */
  if ( duf_config->opt.bformat.v.flag.prefix && ( !bformat || bformat->v.flag.prefix ) )
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
duf_print_file_info_dirid( const duf_depthinfo_t * pdi, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %I : dirid */
  if ( duf_config->opt.bformat.v.flag.dirid && ( !bformat || bformat->v.flag.dirid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{dirid}" ) );
    DUF_PRINTF( 4, ".d%-6llu ", duf_levinfo_nodedirid( pdi ) );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->opt.bformat.v.flag.dirid_space && ( !bformat || bformat->v.flag.dirid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{dirid_space}" ) );
    DUF_PRINTF( 4, ".%3s", "" );
    ok++;
  }
  return ok;
}

#ifndef MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
static int
duf_print_file_info_nfiles( const duf_depthinfo_t * pdi, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %F : nfiles */
  if ( duf_config->opt.bformat.v.flag.nfiles && ( !bformat || bformat->v.flag.nfiles ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nfiles}" ) );
    DUF_PRINTF( 0, ".(%6llu) ", pdi->pathinfo.levinfo[pdi->pathinfo.depth].items.files );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->opt.bformat.v.flag.nfiles_space && ( !bformat || bformat->v.flag.nfiles_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nfiles_space}" ) );
    DUF_PRINTF( 0, ". %6s  ", "" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_ndirs( const duf_depthinfo_t * pdi, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %D : ndirs */
  if ( duf_config->opt.bformat.v.flag.ndirs && ( !bformat || bformat->v.flag.ndirs ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{ndirs}" ) );
    DUF_PRINTF( 0, ".{%6llu} ", pdi->pathinfo.levinfo[pdi->pathinfo.depth].items.dirs );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->opt.bformat.v.flag.ndirs_space && ( !bformat || bformat->v.flag.ndirs_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{ndirs_space}" ) );
    DUF_PRINTF( 0, ". %6s  ", "" );
    ok++;
  }
  return ok;
}
#endif

static int
duf_print_file_info_dataid( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %A : dataid */
  if ( duf_config->opt.bformat.v.flag.dataid && ( !bformat || bformat->v.flag.dataid ) )
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
duf_print_file_info_suffix( const duf_depthinfo_t * pdi, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat, duf_pdi_cb_t prefix_cb DUF_UNUSED,
                            duf_pdi_cb_t suffix_cb )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %~ : suffix */
  if ( duf_config->opt.bformat.v.flag.suffix && ( !bformat || bformat->v.flag.suffix ) )
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
duf_print_file_info_inode( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %O : inode */
  if ( duf_config->opt.bformat.v.flag.inode && ( !bformat || bformat->v.flag.inode ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{inode}" ) );
    DUF_PRINTF( 9, ".i%-9llu ", ( unsigned long long ) pfi->st.st_ino );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_mode( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %m : mode */
  if ( duf_config->opt.bformat.v.flag.mode && ( !bformat || bformat->v.flag.mode ) )
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
duf_print_file_info_nlink( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %n : nlink */
  if ( duf_config->opt.bformat.v.flag.nlink && ( !bformat || bformat->v.flag.nlink ) )
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
duf_print_file_info_user( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %u : user */
  if ( duf_config->opt.bformat.v.flag.user && ( !bformat || bformat->v.flag.user ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{user}" ) );
    DUF_PRINTF( 2, ". u%-9llu", ( unsigned long long ) pfi->st.st_uid );
    ok++;
  }
  return ok;
}

static int
duf_print_file_info_group( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %g : group */
  if ( duf_config->opt.bformat.v.flag.group && ( !bformat || bformat->v.flag.group ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{group}" ) );
    DUF_PRINTF( 3, ". g%-9llu", ( unsigned long long ) pfi->st.st_gid );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_filesize( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %z : filesize */
  if ( duf_config->opt.bformat.v.flag.filesize && ( !bformat || bformat->v.flag.filesize ) )
  {
    unsigned long long sz = pfi->st.st_size;

    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{filesize}" ) );
    if ( duf_config->opt.bformat.v.flag.human || ( bformat && bformat->v.flag.human ) )
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
duf_print_file_info_mtime( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %t : mtime */
  if ( duf_config->opt.bformat.v.flag.mtime && ( !bformat || bformat->v.flag.mtime ) )
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
duf_print_file_info_realpath( const duf_depthinfo_t * pdi, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %r : realpath */
  if ( duf_config->opt.bformat.v.flag.realpath && ( !bformat || bformat->v.flag.realpath ) )
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
duf_print_file_info_filename( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  /* DUF_PRINTF( 0, "[[ %d : %d  ]]", duf_config->opt.bformat.v.flag.filename, bformat->v.flag.filename ); */
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %f : filename */
  if ( duf_config->opt.bformat.v.flag.filename && ( !bformat || bformat->v.flag.filename ) )
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
duf_print_file_info_md5( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %@ : md5sum */
  if ( duf_config->opt.bformat.v.flag.md5 && ( !bformat || bformat->v.flag.md5 ) )
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
duf_print_file_info_exifid( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %X : exifid */
  if ( duf_config->opt.bformat.v.flag.exifid && ( !bformat || bformat->v.flag.exifid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{exifid}" ) );
    if ( pfi->exifid )
      DUF_PRINTF( 5, ". X%-6llu", pfi->exifid );
    else
      DUF_PRINTF( 5, ".  %-6s", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->opt.bformat.v.flag.exifid_space && ( !bformat || bformat->v.flag.exifid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{exifid_space}" ) );
    DUF_PRINTF( 5, ".  %s", "" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_exif_date_time( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %T : exif datatime */
  if ( duf_config->opt.bformat.v.flag.exifdt && ( !bformat || bformat->v.flag.exifdt ) )
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
duf_print_file_info_mimeid( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  /* %E : mimeid */
  if ( duf_config->opt.bformat.v.flag.mimeid && ( !bformat || bformat->v.flag.mimeid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mimeid}" ) );
    if ( pfi->mimeid )
      DUF_PRINTF( 5, ". M%-6llu", pfi->mimeid );
    else
      DUF_PRINTF( 5, ".  %-6s", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->opt.bformat.v.flag.mimeid_space && ( !bformat || bformat->v.flag.mimeid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mimeid_space}" ) );
    DUF_PRINTF( 5, ".  %3s", "" );
    ok++;
  }

  return ok;
}

static int
duf_print_file_info_mime( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  /* %e : mime */
  if ( ( duf_config->opt.bformat.v.flag.mime && ( !bformat || bformat->v.flag.mime ) ) )
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
duf_print_file_info_nameid( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  /* %N : nameid */
  if ( duf_config->opt.bformat.v.flag.nameid && ( !bformat || bformat->v.flag.nameid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nameid}" ) );
    if ( pfi->nameid )
      DUF_PRINTF( 9, ". N%-6llu", pfi->nameid );
    else
      DUF_PRINTF( 9, ".  %-6s", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->opt.bformat.v.flag.nameid_space && ( !bformat || bformat->v.flag.nameid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nameid_space}" ) );
    DUF_PRINTF( 5, ".  %1s", "" );
    ok++;
  }

  return ok;
}

static int DUF_UNUSED duf_print_file_info_template( const duf_depthinfo_t * pdi DUF_UNUSED, duf_fileinfo_t * pfi DUF_UNUSED, duf_bformat_combo_t * bformat DUF_UNUSED )
{
  int ok = 0;

  return ok;
}

int
duf_print_bformat_file_info( const duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_bformat_combo_t * bformat, duf_pdi_cb_t prefix_cb,
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
#ifndef MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
  ok += duf_print_file_info_nfiles( pdi, pfi, bformat );
  ok += duf_print_file_info_ndirs( pdi, pfi, bformat );
#endif
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
    DUF_PRINTF( 0, " %llx :: %llx", bformat->v.bit, duf_config->opt.bformat.v.bit );
  assert( ok );
  return ok;
}
