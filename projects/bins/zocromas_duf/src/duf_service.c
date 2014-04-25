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

  int duf_directory_scan_sample_uni( unsigned long long pathid, duf_dirhandle_t * pdh, duf_depthinfo_t * pdi, duf_record_t * precord );

  static duf_fundesc_t table[] = {
    DUF_FUN( duf_uni_scan_dir ),
    DUF_FUN( duf_sel_cb_name_parid ),
    /* DUF_FUN( duf_sel_cb_items ), */
    DUF_FUN( duf_sel_cb_node ),
    DUF_FUN( duf_sel_cb_leaf ),
    DUF_FUN( duf_sel_cb_dirid ),
    DUF_FUN( duf_directory_scan_sample_uni ),
    DUF_FUN( duf_scan_file ),
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
duf_print_file_info( FILE * f, duf_depthinfo_t * pdi, duf_fileinfo_t * pfi, duf_format_t * format )
{
  if ( duf_config->cli.format.seq && ( !format || format->seq ) )
    printf( "%8llu ", pdi->seq );
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
    printf( "%12llu ", ( unsigned long long ) pfi->st.st_size );
  if ( duf_config->cli.format.nsame && ( !format || format->nsame ) )
    printf( "=%-2llu ", ( unsigned long long ) pfi->nsame );

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
    printf( " :: %-30s ", pfi->name );
  if ( duf_config->cli.format.md5 && ( !format || format->md5 ) )
    printf( "\t:  %016llx%016llx", pfi->md5sum1, pfi->md5sum2 );
  return 0;
}

int
duf_statat_dh( duf_dirhandle_t * pdh, const duf_dirhandle_t * pdhu, const char *name )
{
  int r = DUF_ERROR_PTR;

  if ( duf_config->cli.noopenat )
    return 0;
  if ( pdh && pdhu && name && pdhu->dfd )
  {
    r = fstatat( pdhu->dfd, name, &pdh->st, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );
    if ( r > 0 )
    {
      pdh->rs = r;
    }
    else if ( r == -1 )
    {
      char serr[1024] = "";
      char *s;

      s = strerror_r( errno, serr, sizeof( serr ) );
      DUF_ERROR( "(%d) errno:%d statat_dh :%s; name:'%s' ; at-dfd:%d", r, errno, s ? s : serr, name, pdhu ? pdhu->dfd : 555 );
      r = DUF_ERROR_OPENAT;
    }
  }
  else
  {
    DUF_ERROR( "parameter error pdh:%d; pdhu:%d; name:%d; pdhu->dfd:%d", pdh ? 1 : 0, pdhu ? 1 : 0, name ? 1 : 0, pdhu
               && pdhu->dfd ? 1 : 0 );
    r = DUF_ERROR_OPENAT;
  }
  return r;
}

int
duf_openat_dh( duf_dirhandle_t * pdh, const duf_dirhandle_t * pdhu, const char *name )
{
  int r = DUF_ERROR_PTR;

  if ( duf_config->cli.noopenat )
    return 0;
  assert( pdh );
  assert( pdhu );
  assert( pdhu->dfd );
  assert( name );
  if ( pdh && pdhu && name && pdhu->dfd )
  {
    r = openat( pdhu->dfd, name, O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );
    if ( r > 0 )
    {
      pdh->dfd = r;
      r = 0;
      pdh->rs = fstatat( pdhu->dfd, name, &pdh->st, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );

      duf_config->nopen++;
      DUF_TRACE( fs, 0, "openated %s (%u - %u = %u) h%u", name, duf_config->nopen, duf_config->nclose,
                 duf_config->nopen - duf_config->nclose, pdh->dfd );
    }
    else if ( r == -1 )
    {
      char serr[1024] = "";
      char *s;

      s = strerror_r( errno, serr, sizeof( serr ) );
      DUF_ERROR( "(%d) errno:%d openat_dh :%s; name:'%s' ; at-dfd:%d", r, errno, s ? s : serr, name, pdhu ? pdhu->dfd : 555 );
      r = DUF_ERROR_OPENAT;
    }
  }
  else
  {
    DUF_ERROR( "parameter error pdh:%d; pdhu:%d; name:%d; pdhu->dfd:%d", pdh ? 1 : 0, pdhu ? 1 : 0, name ? 1 : 0, pdhu
               && pdhu->dfd ? 1 : 0 );
    r = DUF_ERROR_OPENAT;
  }
  return r;
}

int
duf_open_dh( duf_dirhandle_t * pdh, const char *path )
{
  int r = DUF_ERROR_PTR;

  if ( duf_config->cli.noopenat )
    return 0;
  if ( pdh && path )
  {
    r = open( path, O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );
    if ( r > 0 )
    {
      pdh->dfd = r;
      r = 0;
      pdh->rs = stat( path, &pdh->st );

      duf_config->nopen++;
      DUF_TRACE( fs, 0, "opened %s (%u - %u = %u)  h%u", path, duf_config->nopen, duf_config->nclose,
                 duf_config->nopen - duf_config->nclose, pdh->dfd );
      assert( pdh->dfd );
    }
    else
    {
      char serr[512] = "";
      char *s;

      s = strerror_r( errno, serr, sizeof( serr ) );
      DUF_ERROR( "(%d) errno:%d open_dh :%s; name:'%s'", r, errno, s ? s : serr, path );
      r = DUF_ERROR_OPEN;
    }
  }
  else if ( path )
  {
    r = 0;
    assert( pdh->dfd );
  }
  else
  {
    DUF_ERROR( "parameter error pdh:%d; path:%d;", pdh ? 1 : 0, path ? 1 : 0 );
  }
  return r;
}

int
duf_close_dh( duf_dirhandle_t * pdh )
{
  int r = DUF_ERROR_PTR;

  if ( duf_config->cli.noopenat )
    return 0;
  assert( pdh );
  if ( pdh )
  {
    r = DUF_ERROR_NOT_OPEN;
    assert( pdh->dfd );
    if ( pdh->dfd )
    {
      r = close( pdh->dfd );
      if ( r )
      {
        {
          /* for debug only!!! */
          /* assert( pdh->dfd < 1000 ); */
        }

        DUF_ERROR( "close dfd:%d", pdh->dfd );
        r = DUF_ERROR_CLOSE;
      }
      DUF_TRACE( fs, 0, "closed (%u - %u = %u)  h%u", duf_config->nopen, duf_config->nclose, duf_config->nopen - duf_config->nclose,
                 pdh->dfd );
      duf_config->nclose++;
    }
    else
    {
      DUF_ERROR( "parameter error pdhu->dfd:%d", pdh && pdh->dfd ? 1 : 0 );
    }

    pdh->dfd = 0;
  }
  else
    r = 0;
  return r;
}

int
duf_check_dh( const char *msg )
{
  int r = 0;

  if ( duf_config->cli.noopenat )
    return 0;
  DUF_TRACE( fs, 2, "%s (%u - %u = %u)", msg, duf_config->nopen, duf_config->nclose, duf_config->nopen - duf_config->nclose );
  return r;
}
