#include <stdarg.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

/* #include <mastar/tools/mas_arg_tools.h> */

#include "duf_types.h"
#include "duf_utils.h"

#include "duf_config.h"
#include "duf_uni_scan.h"
#include "duf_file_scan.h"
#include "duf_path.h"

/* ###################################################################### */
#include "duf_service.h"
/* ###################################################################### */


int
duf_check_field( const char *name, int have )
{
  if ( !have )
  {
    char *p = NULL;

    DUF_ERROR( ">>>>>>>>>>>>>>>>>>>> No such field: %s", name );
    /* force segfault */
    DUF_ERROR( ">>>>>>>>>>>>>>>>>>>>  %c", *p );
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

  int duf_directory_scan_sample_uni( unsigned long long pathid, duf_dirhandle_t * pdh, duf_depthinfo_t * pdi, duf_record_t * precord );

  static duf_fundesc_t table[] = {
    DUF_FUN( duf_uni_scan_dir ),
    DUF_FUN( duf_sel_cb_name_parid ),
    DUF_FUN( duf_sel_cb_items ),
    DUF_FUN( duf_sel_cb_dirid ),
    DUF_FUN( duf_directory_scan_sample_uni ),
  };
  for ( int i = 0; i < sizeof( table ) / sizeof( table[9] ); i++ )
  {
    if ( p == table[i].fun )
    {
      snprintf( buf, sizeof( buf ), "%s()", table[i].name );
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
duf_print_file_info( FILE * f, duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_t * format )
{
  if ( duf_config->cli.format.seq && ( !format || format->seq ) )
    printf( "%6llu ", pdi->seq );
  if ( duf_config->cli.format.dirid && ( !format || format->dirid ) )
    printf( "[%8llu] ", pdi->levinfo[pdi->depth].dirid );
  if ( duf_config->cli.format.inode && ( !format || format->inode ) )
    printf( "%9llu ", ( unsigned long long ) pfi->st.st_ino );
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
    printf( "%s", modebuf );
  }
  if ( duf_config->cli.format.nlink && ( !format || format->nlink ) )
    printf( "%3llu ", ( unsigned long long ) pfi->st.st_nlink );
  if ( duf_config->cli.format.uid && ( !format || format->uid ) )
    printf( "%9llu ", ( unsigned long long ) pfi->st.st_uid );
  if ( duf_config->cli.format.gid && ( !format || format->gid ) )
    printf( "%9llu ", ( unsigned long long ) pfi->st.st_gid );
  if ( duf_config->cli.format.filesize && ( !format || format->filesize ) )
    printf( "%9llu ", ( unsigned long long ) pfi->st.st_size );
  if ( duf_config->cli.format.mtime && ( !format || format->mtime ) )
  {
    time_t mtimet;
    struct tm mtimetm, *pmtimetm;
    char mtimes[128];

    mtimet = ( time_t ) pfi->st.st_mtim.tv_sec;
    pmtimetm = localtime_r( &mtimet, &mtimetm );
    strftime( mtimes, sizeof( mtimes ), "%b %d %Y %H:%M:%S", pmtimetm );
    printf( "%s ", mtimes );
  }
  if ( duf_config->cli.format.filename && ( !format || format->filename ) )
    printf( "%-30s ", pfi->name );
  if ( duf_config->cli.format.md5 && ( !format || format->md5 ) )
    printf( "\t:  %016llx%016llx", pfi->md5sum1, pfi->md5sum2 );
  return 0;
}
