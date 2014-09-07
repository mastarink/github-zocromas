#include <stdarg.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_levinfo.h"

#include "duf_config_ref.h"

#include "duf_uni_scan.h"
#include "duf_uni_scan2.h"

/* #include "duf_dir_scan1.h" */
#include "duf_dir_scan2.h"

/* #include "duf_item_scan1.h" */
#include "duf_item_scan2.h"

/* #include "duf_path2db.h" */
#include "duf_db2path.h"


/* ###################################################################### */
#include "duf_service.h"
/* ###################################################################### */

int
duf_check_field( const char *name, int have )
{
  if ( !have )
  {
    DUF_ERROR( "No such field: %s", name );
    /* assert(have); */
    return DUF_ERROR_NO_FIELD;
  }
  return 0;
}

typedef struct
{
  const char *name;
  duf_anyhook_t fun;
} duf_fundesc_t;

#define DUF_FUN(name) { #name, (duf_anyhook_t) name}
const char *
duf_dbg_funname( duf_anyhook_t p )
{
  static char buf[512];
  int found = 0;


  static duf_fundesc_t table[] = {
    /* DUF_FUN( duf_str_cb2_uni_scan_dir ), */
    /* DUF_FUN( duf_str_cb1_leaf_scan ), */
#ifdef DUF_COMPILE_EXPIRED
    DUF_FUN( duf_sel_cb_name_parid ),
#endif
    /* DUF_FUN( duf_sel_cb_items ), */
    /* DUF_FUN( duf_sel_cb_node ), */
    /* DUF_FUN( duf_sel_cb_leaf ), */
    /* DUF_FUN( duf_sel_cb_levinfo ), */
  };
  for ( int i = 0; i < sizeof( table ) / sizeof( table[9] ); i++ )
  {
    if ( p == table[i].fun )
    {
      snprintf( buf, sizeof( buf ), "%p:%s()", ( void * ) ( unsigned long long ) p, table[i].name );
      found = 1;
    }
  }
  if ( !found )
  {
    snprintf( buf, sizeof( buf ), "[%p]", ( void * ) ( unsigned long long ) p );
  }

  return buf;
}

int
duf_print_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_combo_t * format, duf_pdi_cb_t prefix_cb, duf_pdi_cb_t suffix_cb )
{
  int ok = 0;

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.seq && ( !format || format->v.flag.seq ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{seq}" ) );
    DUF_PRINTF( 2, ".#%-6llu ", pdi->seq );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.format.v.flag.seq_node && ( !format || format->v.flag.seq_node ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{seq_node}" ) );
    DUF_PRINTF( 2, ".#%-6llu ", pdi->seq_node );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.seq_leaf && ( !format || format->v.flag.seq_leaf ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{seq_leaf}" ) );
    DUF_PRINTF( 2, ".#%-6llu ", pdi->seq_leaf );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  if ( duf_config->cli.format.v.flag.dirid && ( !format || format->v.flag.dirid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{dirid}" ) );
    DUF_PRINTF( 4, ".[%6llu  ] ", duf_levinfo_nodedirid( pdi ) );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.format.v.flag.dirid_space && ( !format || format->v.flag.dirid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{dirid_space}" ) );
    DUF_PRINTF( 2, ". %6s    ", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

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
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

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
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

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
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

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
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.inode && ( !format || format->v.flag.inode ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{inode}" ) );
    DUF_PRINTF( 1, ".%-9llu ", ( unsigned long long ) pfi->st.st_ino );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

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
    DUF_PRINTF( 0, ".%s ", modebuf );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.nlink && ( !format || format->v.flag.nlink ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nlink}" ) );
    /* if ( pfi->st.st_nlink <= 1 )    */
    /*   DUF_PRINTF( 0, ".%3s ", "" ); */
    /* else                            */
    DUF_PRINTF( 0, ".%-2llu ", ( unsigned long long ) pfi->st.st_nlink );

    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.user && ( !format || format->v.flag.user ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{user}" ) );
    DUF_PRINTF( 0, ".u%-9llu ", ( unsigned long long ) pfi->st.st_uid );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.group && ( !format || format->v.flag.group ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{group}" ) );
    DUF_PRINTF( 0, ".g%-9llu ", ( unsigned long long ) pfi->st.st_gid );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.filesize && ( !format || format->v.flag.filesize ) )
  {
    unsigned long long sz = pfi->st.st_size;

    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{filesize}" ) );
    if ( duf_config->cli.format.v.flag.human || ( format && format->v.flag.human ) )
    {
      if ( sz < 1024 )
        DUF_PRINTF( 0, ".%9llu  ", sz );
      else if ( sz < 1024 * 1024 )
        DUF_PRINTF( 0, ".%9lluk ", sz / 1024 );
      else if ( sz < 1024 * 1024 * 1024 )
        DUF_PRINTF( 0, ".%9lluM ", sz / 1024 / 1024 );
      else
        DUF_PRINTF( 0, ".%9lluG ", sz / 1024 / 1024 / 1024 );
    }
    else
      DUF_PRINTF( 0, ".%10llu ", sz );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.dataid && ( !format || format->v.flag.dataid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{dataid}" ) );
    if ( pfi->dataid )
      DUF_PRINTF( 2, ".D%-6llu ", pfi->dataid );
    else
      DUF_PRINTF( 2, ". %6s ", "" );
    ok++;
  }

  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
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
    DUF_PRINTF( 0, ". %-s ", mtimes );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.nsame && ( !format || format->nsame ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nsame}" ) );
    if ( pfi->nsame > 1 )
    {
      DUF_PRINTF( 0, ".=%-4llu ", ( unsigned long long ) pfi->nsame );
    }
    else
      DUF_PRINTF( 0, ". %-4s ", "." );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.realpath && ( !format || format->v.flag.realpath ) )
  {
    const char *real_path = NULL;

    real_path = duf_levinfo_path( pdi );
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{real_path}" ) );
    DUF_PRINTF( 0, ".%s", real_path );
    ok++;
  }

  /* DUF_PRINTF( 0, "[[ %d : %d  ]]", duf_config->cli.format.v.flag.filename, format->v.flag.filename ); */
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.filename && ( !format || format->v.flag.filename ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{filename}" ) );
    if ( format->v.flag.short_filename )
      DUF_PRINTF( 0, ".%-12s ", pfi->name );
    else
      DUF_PRINTF( 0, ".%-30s ", pfi->name );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.md5id && ( !format || format->v.flag.md5id ) )
  {
    if ( 1 || pfi->nsame > 1 )
    {
      DUF_DEBUG( 12, DUF_PRINTF( 0, ".{md5id}" ) );
      if ( pfi->md5id )
        DUF_PRINTF( 0, ".#%-6llu ", pfi->md5id );
      else
        DUF_PRINTF( 0, ". %-6s ", "" );
      ok++;
    }
    else
    {
      DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nomd5id}" ) );
      DUF_PRINTF( 0, ". %-6s ", "" );
      ok++;
    }
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.md5 && ( !format || format->v.flag.md5 ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{md5}" ) );
    if ( pfi->md5sum1 || pfi->md5sum2 )
      DUF_PRINTF( 5, ".%016llx%016llx ", pfi->md5sum1, pfi->md5sum2 );
    else
      DUF_PRINTF( 5, ".%-32s ", "-" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.exifid && ( !format || format->v.flag.exifid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{exifid}" ) );
    if ( pfi->exifid )
      DUF_PRINTF( 5, ".X%llu ", pfi->exifid );
    else
      DUF_PRINTF( 5, ". %s ", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.format.v.flag.exifid_space && ( !format || format->v.flag.exifid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{exifid_space}" ) );
    DUF_PRINTF( 5, ". %s ", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.v.flag.mimeid && ( !format || format->v.flag.mimeid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mimeid}" ) );
    if ( pfi->mimeid )
      DUF_PRINTF( 5, ".M%llu ", pfi->mimeid );
    else
      DUF_PRINTF( 5, ". %s ", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.format.v.flag.mimeid_space && ( !format || format->v.flag.mimeid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mimeid_space}" ) );
    DUF_PRINTF( 5, ". %6s ", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );

  if ( ( duf_config->cli.format.v.flag.mime && ( !format || format->v.flag.mime ) ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{mime}" ) );
    if ( pfi->mime )
      DUF_PRINTF( 0, ".<%s> ", pfi->mime );
    else
      DUF_PRINTF( 0, ".<%s> ", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );


  if ( duf_config->cli.format.v.flag.nameid && ( !format || format->v.flag.nameid ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nameid}" ) );
    if ( pfi->nameid )
      DUF_PRINTF( 5, ".N%llu ", pfi->nameid );
    else
      DUF_PRINTF( 5, ". %s ", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.format.v.flag.nameid_space && ( !format || format->v.flag.nameid_space ) )
  {
    DUF_DEBUG( 12, DUF_PRINTF( 0, ".{nameid_space}" ) );
    DUF_PRINTF( 5, ". %s ", "" );
    ok++;
  }
  DUF_DEBUG( 13, DUF_PRINTF( 0, ".▣" ) );
  if ( !ok )
    DUF_PRINTF( 0, "%llx :: %llx", format->v.bit, duf_config->cli.format.v.bit );
  assert( ok );
  return ok;
}
