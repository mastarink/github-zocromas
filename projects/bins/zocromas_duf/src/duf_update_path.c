#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <dirent.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

/* #include "duf_sql.h" */
/* #include "duf_path.h" */
/* #include "duf_insert.h" */
#include "duf_update_realpath.h"
/* #include "duf_update_pathid.h" */


/* ###################################################################### */
#include "duf_update_path.h"
/* ###################################################################### */


unsigned long long
duf_update_path_down( const char *path, unsigned long long parentid, int recursive, int dofiles )
{
  unsigned long long pathid = 0;
  char *real_path;

  real_path = mas_malloc( PATH_MAX );
  if ( real_path )
  {
    ( void ) realpath( path, real_path );
    mas_free( real_path );
    pathid = duf_update_realpath_down( real_path, parentid, recursive, dofiles );
  }
  return pathid;
}
