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

/* #include <mastar/tools/mas_arg_tools.h> */

#include "duf_trace_defs.h"

#include "duf_types.h"
#include "duf_utils.h"

#include "duf_levinfo.h"

#include "duf_config_ref.h"

#include "duf_uni_scan.h"

/* #include "duf_sccb.h" */
#include "duf_dir_scan1.h"
#include "duf_dir_scan2.h"

#include "duf_item_scan1.h"
#include "duf_item_scan2.h"

#include "duf_path.h"

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
    DUF_FUN( duf_str_cb1_uni_scan_dir ),
    /* DUF_FUN( duf_str_cb1_leaf_scan ), */
#ifdef DUF_COMPILE_EXPIRED
    DUF_FUN( duf_sel_cb_name_parid ),
#endif
    /* DUF_FUN( duf_sel_cb_items ), */
    DUF_FUN( duf_sel_cb_node ),
    DUF_FUN( duf_sel_cb_leaf ),
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
duf_print_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_t * format, duf_pdi_cb_t prefix_cb, duf_pdi_cb_t suffix_cb )
{
  int ok = 0;

  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.seq && ( !format || format->seq ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{seq}" ) );
    DUF_PRINTF( 0, ".%6llu: ", pdi->seq );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.dirid && ( !format || format->dirid ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{dirid}" ) );
    DUF_PRINTF( 0, ".[%-8llu] ", pdi->levinfo[pdi->depth].dirid );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.prefix && ( !format || format->prefix ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{prefix}" ) );
    if ( prefix_cb )
      ( prefix_cb ) ( pdi );
    else
      DUF_PRINTF( 0, ".= __________________________________________________ = " );
    ok++;
  }
  else
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{noprefix}" ) );
    /* DUF_PRINTF( 0, ".      " ); */
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.suffix && ( !format || format->suffix ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{suffix}" ) );
    if ( suffix_cb )
      ( suffix_cb ) ( pdi );
    else
      DUF_PRINTF( 0, ".=%%%%%%%%%%%%%%%%%%%%%%%%= " );
    ok++;
  }
  else
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{nosuffix}" ) );
    /* DUF_PRINTF( 0, ".      " ); */
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.inode && ( !format || format->inode ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{inode}" ) );
    DUF_PRINTF( 0, ".i%-9llu ", ( unsigned long long ) pfi->st.st_ino );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.mode && ( !format || format->mode ) )
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
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{mode}" ) );
    DUF_PRINTF( 0, ".(%s) ", modebuf );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.uid && ( !format || format->uid ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{uid}" ) );
    DUF_PRINTF( 0, ".u%-9llu ", ( unsigned long long ) pfi->st.st_uid );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.gid && ( !format || format->gid ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{gid}" ) );
    DUF_PRINTF( 0, ".g%-9llu ", ( unsigned long long ) pfi->st.st_gid );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.filesize && ( !format || format->filesize ) )
  {
    unsigned long long sz = pfi->st.st_size;

    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{filesize}" ) );
    if ( duf_config->cli.format.human || ( format && format->human ) )
    {
      if ( sz < 1024 )
        DUF_PRINTF( 0, ".%4llu  ", sz );
      else if ( sz < 1024 * 1024 )
        DUF_PRINTF( 0, ".%4lluk ", sz / 1024 );
      else if ( sz < 1024 * 1024 * 1024 )
        DUF_PRINTF( 0, ".%4lluM ", sz / 1024 / 1024 );
      else
        DUF_PRINTF( 0, ".%4lluG ", sz / 1024 / 1024 / 1024 );
    }
    else
      DUF_PRINTF( 0, ".%10llu ", sz );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.nlink && ( !format || format->nlink ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{nlink}" ) );
    if ( pfi->st.st_nlink <= 1 )
      DUF_PRINTF( 0, ".%3s ", "" );
    else
      DUF_PRINTF( 0, ".~%-2llu ", ( unsigned long long ) pfi->st.st_nlink );

    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.nsame && ( !format || format->nsame ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{nsame}" ) );
    if ( pfi->nsame > 1 )
    {
      DUF_PRINTF( 0, ".=%-4llu ", ( unsigned long long ) pfi->nsame );
    }
    else
      DUF_PRINTF( 0, ".%5s ", "" );
    ok++;
    if ( duf_config->cli.format.md5id && ( !format || format->md5id ) )
    {
      if ( pfi->nsame > 1 )
      {
        DUF_DEBUG( 2, DUF_PRINTF( 0, ".{md5id}" ) );
        DUF_PRINTF( 0, ".##%-6llu ", pfi->md5id );
        ok++;
      }
      else
      {
        DUF_DEBUG( 2, DUF_PRINTF( 0, ".{nomd5id}" ) );
        DUF_PRINTF( 0, ".%8s ", "" );
        ok++;
      }
    }
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.dataid && ( !format || format->dataid ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{dataid}" ) );
    if ( pfi->dataid )
      DUF_PRINTF( 0, ".$%-6llu ", pfi->dataid );
    else
      DUF_PRINTF( 0, ".%7s ", "" );
    ok++;
  }

  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );
  if ( duf_config->cli.format.mtime && ( !format || format->mtime ) )
  {
    time_t mtimet;
    struct tm mtimetm, *pmtimetm;
    char mtimes[128];

    mtimet = ( time_t ) pfi->st.st_mtim.tv_sec;
    pmtimetm = localtime_r( &mtimet, &mtimetm );
    strftime( mtimes, sizeof( mtimes ), "%b %d %Y %H:%M:%S", pmtimetm );
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{mtime}" ) );
    DUF_PRINTF( 0, ".m%-s ", mtimes );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.truepath && ( !format || format->truepath ) )
  {
    int r = 0;
    char *true_path = NULL;

    true_path = duf_pathid_to_path2( pfi->truedirid, pdi, &r );
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{true_path}" ) );
    DUF_PRINTF( 0, ".%s", true_path );
    if ( true_path )
      mas_free( true_path );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.realpath && ( !format || format->realpath ) )
  {
    const char *real_path = NULL;

    real_path = duf_levinfo_path( pdi );
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{real_path}" ) );
    DUF_PRINTF( 0, ".%s", real_path );
    ok++;
  }

  /* DUF_PRINTF( 0, "[[ %d : %d  ]]", duf_config->cli.format.filename, format->filename ); */
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.filename && ( !format || format->filename ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{filename}" ) );
    if ( format->short_filename )
      DUF_PRINTF( 0, ".%s ", pfi->name );
    else
      DUF_PRINTF( 0, ".%-30s ", pfi->name );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );

  if ( duf_config->cli.format.md5 && ( !format || format->md5 ) )
  {
    DUF_DEBUG( 2, DUF_PRINTF( 0, ".{md5}" ) );
    DUF_PRINTF( 0, ".%016llx%016llx", pfi->md5sum1, pfi->md5sum2 );
    ok++;
  }
  DUF_DEBUG( 3, DUF_PRINTF( 0, ".▣" ) );


  assert( ok );
  return ok;
}
