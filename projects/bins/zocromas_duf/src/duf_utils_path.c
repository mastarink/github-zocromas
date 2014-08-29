#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_error_types.h"

/* ###################################################################### */
#include "duf_utils_path.h"
/* ###################################################################### */

int
duf_pathdepth( const char *path, int *pr )
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
  if ( pr )
    *pr = r;
  return depth;
}

char *
duf_realpath( const char *path, int *pr )
{
  int r = DUF_ERROR_PATH;
  char *real_path = NULL;
  char *rp = realpath( path, NULL );

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
