#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"
#include "duf_utils.h"

/* #include "duf_sql.h" */
/* #include "duf_path.h" */
/* #include "duf_insert.h" */

#include "duf_update_realpath.h"
/* #include "duf_update_path.h" */

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_add.h"
/* ###################################################################### */




unsigned long long
duf_add_path( const char *path, const char *group )
{
  unsigned long long pathid = 0;
  char *real_path;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  real_path = mas_malloc( PATH_MAX );
  if ( real_path )
  {
    ( void ) realpath( path, real_path );
    DUF_TRACE( action, 0, "real_path:%s", real_path );
    if (  /* strlen( real_path ) > 1 && */ !( real_path && *real_path == '/' && real_path[1] == 0 ) )
      pathid = duf_update_realpath_self_up( real_path, group, DUF_TRUE );
    DUF_TRACE( action, 0, "Added path %s as %llu;  group: %s", path, pathid, group );
    mas_free( real_path );
  }
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return pathid;
}
