#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_error_types.h"

/* ###################################################################### */
#include "duf_utils_path.h"
/* ###################################################################### */

int
duf_pathdepth( const char *path )
{
  int r = 0;
  int depth = 0;
  const char *bd;

  bd = path;
  if ( path && *path )
  {
    while ( bd && *bd && *bd == '/' )
      bd++;
    depth = 0;
    while ( bd && *bd )
    {
      const char *ed;

      ed = bd;
      while ( ed && *ed && *ed != '/' )
        ed++;
      while ( ed && *ed && *ed == '/' )
        ed++;

      bd = ed;
      depth++;
    }
  }
  else
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
  }
  return r < 0 ? r : depth;
}

char *
duf_realpath( const char *path, int *pr )
{
  int r = 0;
  char *real_path = NULL;
  char *rp = realpath( path, NULL );

  DUF_TRACE( path, 2, ">> path:%s; rp:%s", path, rp );

  if ( rp )
  {
    real_path = mas_strdup( rp );
    free( rp );
  }
  else
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_PATH );
  }

  if ( pr )
    *pr = r;
  return real_path;
}

/* 20151007.135909 */
/* allocates! */
char *
duf_concat_path( const char *path, const char *subpath )
{
  char *npath = NULL;

  if ( path && subpath && *path && *subpath )
  {
    npath = duf_normalize_path( path );
    npath = mas_strcat_x( npath, subpath );
  }
  return npath;
}

/* 20151007.135934 */
/* allocates! */
char *
duf_normalize_path( const char *path )
{
  char *npath = NULL;

  if ( path && *path )
  {
#if 0
    size_t len;

    len = strlen( path );
    if ( path[len - 1] != '/' )
    {
      npath = mas_strdup( path );
      npath = mas_strcat_x( npath, "/" );
    }
    else
    {
      while ( path[len - 1] == '/' )
        len--;
      npath = mas_strndup( path, len + 1 );
    }
#else
    const char *ppath;
    const char *sl;

    ppath = path;
    /* npath = mas_strdup( "" ); */
    while ( ( sl = strchr( ppath, '/' ) ) )
    {
      npath = mas_strncat_x( npath, ppath, sl - ppath );
      npath = mas_strcat_x( npath, "/" );
      ppath = sl + 1;
      while ( *ppath == '/' )
        ppath++;
    }
    if ( ppath && *ppath )
    {
      npath = mas_strcat_x( npath, ppath );
      npath = mas_strcat_x( npath, "/" );
    }
#endif
  }
  return npath;
}
