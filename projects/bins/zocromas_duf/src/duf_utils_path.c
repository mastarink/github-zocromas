#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fnmatch.h>
/* #include <unistd.h> */
#include <sys/time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_errors.h"

#include "duf_sql_const.h"

/* ###################################################################### */
#include "duf_utils_path.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */

#define FN_FMT "27s"
/* #define FN_FMT "23s" */


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
