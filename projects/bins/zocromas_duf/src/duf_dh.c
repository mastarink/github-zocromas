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
#include "duf_dh.h"
/* ###################################################################### */



int
duf_statat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name )
{
  int r = DUF_ERROR_PTR;

  if ( duf_config->cli.noopenat )
    return 0;
  if ( pdhandle && pdhandleup && name && pdhandleup->dfd )
  {
    r = fstatat( pdhandleup->dfd, name, &pdhandle->st, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );
    if ( r > 0 )
    {
      pdhandle->rs = r;
    }
    else if ( r == -1 )
    {
      char serr[1024] = "";
      char *s;

      s = strerror_r( errno, serr, sizeof( serr ) );
      DUF_ERROR( "(%d) errno:%d statat_dh :%s; name:'%s' ; at-dfd:%d", r, errno, s ? s : serr, name, pdhandleup ? pdhandleup->dfd : 555 );
      r = DUF_ERROR_STATAT;
    }
  }
  else
  {
    DUF_ERROR( "parameter error pdhandle:%d; pdhandleup:%d; name:%d; pdhandleup->dfd:%d", pdhandle ? 1 : 0, pdhandleup ? 1 : 0,
               name ? 1 : 0, pdhandleup && pdhandleup->dfd ? 1 : 0 );
    r = DUF_ERROR_STATAT;
  }
  return r;
}

int
duf_openat_dh( duf_dirhandle_t * pdhandle, const duf_dirhandle_t * pdhandleup, const char *name, int asfile )
{
  int r = DUF_ERROR_PTR;
  int updfd = 0;

  if ( duf_config->cli.noopenat )
    return 0;
  assert( pdhandle );
  assert( name );
  assert( *name );
  updfd = pdhandleup ? pdhandleup->dfd : 0;
  if ( pdhandle && name && updfd )
  {
    if ( asfile )
      r = openat( updfd, name, O_NOFOLLOW | O_RDONLY );
    else
      r = openat( updfd, *name ? name : "/", O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );
    if ( r > 0 )
    {
      pdhandle->dfd = r;
      r = 0;
      pdhandle->rs = fstatat( updfd, name, &pdhandle->st, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT );

      duf_config->nopen++;
      DUF_TRACE( fs, 0, "openated %s (%u - %u = %u) h%u", name, duf_config->nopen, duf_config->nclose,
                 duf_config->nopen - duf_config->nclose, pdhandle->dfd );
    }
    else if ( r < 0 && errno == ENOENT )
    {
      r = DUF_ERROR_OPENAT_ENOENT;
    }
    else if ( r < 0 )
    {
      char serr[1024] = "";
      char *s;

      s = strerror_r( errno, serr, sizeof( serr ) );
      DUF_ERROR( "(%d) errno:%d openat_dh :%s; name:'%s' ; at-dfd:%d", r, errno, s ? s : serr, name, updfd );
      r = DUF_ERROR_OPENAT;
    }
  }
  else
  {
    DUF_ERROR( "parameter error pdhandle:%d; name:%s; updfd:%d", pdhandle ? 1 : 0, name, updfd );
    r = DUF_ERROR_OPENAT;
  }
  return r;
}

int
duf_open_dh( duf_dirhandle_t * pdhandle, const char *path )
{
  int r = DUF_ERROR_PTR;

  if ( duf_config->cli.noopenat )
    return 0;
  if ( pdhandle && path )
  {
    r = open( path, O_DIRECTORY | O_NOFOLLOW | O_PATH | O_RDONLY );
    if ( r > 0 )
    {
      pdhandle->dfd = r;
      r = 0;
      pdhandle->rs = stat( path, &pdhandle->st );

      duf_config->nopen++;
      DUF_TRACE( fs, 0, "opened %s (%u - %u = %u)  h%u", path, duf_config->nopen, duf_config->nclose,
                 duf_config->nopen - duf_config->nclose, pdhandle->dfd );
      assert( pdhandle->dfd );
    }
    else if ( errno == ENOENT )
    {
      r = DUF_ERROR_OPEN_ENOENT;
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
    assert( pdhandle->dfd );
  }
  else
  {
    DUF_ERROR( "parameter error pdhandle:%d; path:%d;", pdhandle ? 1 : 0, path ? 1 : 0 );
  }
  return r;
}

/* returns handle >0 */
int
duf_opened_dh( duf_dirhandle_t * pdhandle )
{
  int r = DUF_ERROR_PTR;

  if ( duf_config->cli.noopenat )
    return 0;
  if ( pdhandle )
    r = pdhandle->dfd;
  return r;
}

int
duf_close_dh( duf_dirhandle_t * pdhandle )
{
  int r = DUF_ERROR_PTR;

  if ( duf_config->cli.noopenat )
    return 0;
  assert( pdhandle );
  if ( pdhandle )
  {
    r = DUF_ERROR_NOT_OPEN;
    assert( pdhandle->dfd );
    if ( pdhandle->dfd )
    {
      r = close( pdhandle->dfd );
      if ( r )
      {
        {
          /* for debug only!!! */
          /* assert( pdhandle->dfd < 1000 ); */
        }

        DUF_ERROR( "close dfd:%d", pdhandle->dfd );
        r = DUF_ERROR_CLOSE;
      }
      DUF_TRACE( fs, 0, "closed (%u - %u = %u)  h%u", duf_config->nopen, duf_config->nclose, duf_config->nopen - duf_config->nclose,
                 pdhandle->dfd );
      duf_config->nclose++;
    }
    else
    {
      DUF_ERROR( "parameter error pdhandleup->dfd:%d", pdhandle && pdhandle->dfd ? 1 : 0 );
    }

    pdhandle->dfd = 0;
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
