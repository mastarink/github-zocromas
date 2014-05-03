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

#include "duf_types.h"
#include "duf_utils.h"

#include "duf_config.h"
/* #include "duf_sql_def.h" */
#include "duf_uni_scan.h"
#include "duf_dir_scan.h"
#include "duf_item_scan.h"
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
    DUF_FUN( duf_str_cb_uni_scan_dir ),
    DUF_FUN( duf_str_cb_leaf_scan ),
#ifdef DUF_COMPILE_EXPIRED
    DUF_FUN( duf_sel_cb_name_parid ),
#endif
    /* DUF_FUN( duf_sel_cb_items ), */
    DUF_FUN( duf_sel_cb_node ),
    DUF_FUN( duf_sel_cb_leaf ),
    DUF_FUN( duf_sel_cb_levinfo ),
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
duf_print_file_info( duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_t * format )
{
  if ( duf_config->cli.format.seq && ( !format || format->seq ) )
    DUF_PRINTF( 0, ".%-8llu ", pdi->seq );
  if ( duf_config->cli.format.dirid && ( !format || format->dirid ) )
    DUF_PRINTF( 0, ".[%8llu] ", pdi->levinfo[pdi->depth].dirid );
  if ( duf_config->cli.format.inode && ( !format || format->inode ) )
    DUF_PRINTF( 0, ".%9llu ", ( unsigned long long ) pfi->st.st_ino );
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
    DUF_PRINTF( 0, ".%s", modebuf );
  }
  if ( duf_config->cli.format.nlink && ( !format || format->nlink ) )
    DUF_PRINTF( 0, ".%3llu ", ( unsigned long long ) pfi->st.st_nlink );
  if ( duf_config->cli.format.uid && ( !format || format->uid ) )
    DUF_PRINTF( 0, ".%9llu ", ( unsigned long long ) pfi->st.st_uid );
  if ( duf_config->cli.format.gid && ( !format || format->gid ) )
    DUF_PRINTF( 0, ".%9llu ", ( unsigned long long ) pfi->st.st_gid );
  if ( duf_config->cli.format.filesize && ( !format || format->filesize ) )
    DUF_PRINTF( 0, ".%12llu ", ( unsigned long long ) pfi->st.st_size );
  if ( duf_config->cli.format.nsame && ( !format || format->nsame ) )
    DUF_PRINTF( 0, ".=%-2llu ", ( unsigned long long ) pfi->nsame );

  if ( duf_config->cli.format.mtime && ( !format || format->mtime ) )
  {
    time_t mtimet;
    struct tm mtimetm, *pmtimetm;
    char mtimes[128];

    mtimet = ( time_t ) pfi->st.st_mtim.tv_sec;
    pmtimetm = localtime_r( &mtimet, &mtimetm );
    strftime( mtimes, sizeof( mtimes ), "%b %d %Y %H:%M:%S", pmtimetm );
    DUF_PRINTF( 0, ".%s ", mtimes );
  }
  if ( duf_config->cli.format.filename && ( !format || format->filename ) )
    DUF_PRINTF( 0, ". :: %-30s ", pfi->name );
  if ( duf_config->cli.format.md5 && ( !format || format->md5 ) )
    DUF_PRINTF( 0, ".\t:  %016llx%016llx", pfi->md5sum1, pfi->md5sum2 );
  return 0;
}
