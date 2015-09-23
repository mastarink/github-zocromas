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
  int r = DUF_ERROR_PATH;
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
    r = 0;
  }
  return r < 0 ? r : depth;
}

char *
duf_realpath( const char *path, int *pr )
{
  int r = DUF_ERROR_PATH;
  char *real_path = NULL;
  char *rp = realpath( path, NULL );

  DUF_TRACE( temp, 2, ">> path:%s; rp:%s", path, rp );

  if ( rp )
  {
    real_path = mas_strdup( rp );
    free( rp );
    r = 0;
  }
  if ( pr )
    *pr = r;
  return real_path;
}

char *
duf_concat_path( char *path, const char *subpath )
{
  if ( path && subpath && *path && *subpath )
  {
    path = duf_normalize_path( path );
    path = mas_strcat_x( path, subpath );
  }
  return path;
}

char *
duf_normalize_path( char *path )
{
  if ( path && *path )
  {
    if ( path[strlen( path ) - 1] != '/' )
      path = mas_strcat_x( path, "/" );
  }
  return path;
}
