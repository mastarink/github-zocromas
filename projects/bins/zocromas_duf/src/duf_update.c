#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

/* #include "duf_sql.h" */
/* #include "duf_path.h" */
/* #include "duf_insert.h" */

#include "duf_update_realpath.h"
/* #include "duf_update_pathid.h" */
/* #include "duf_update_path.h" */
/* #include "duf_update_pathentries.h" */

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_update.h"
/* ###################################################################### */




unsigned long long
duf_add_path( const char *path, const char *group )
{
  unsigned long long pathid = 0;
  char *real_path;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  fprintf( stderr, "Add path %s group: %s\n", path, group );

  real_path = mas_malloc( PATH_MAX );
  if ( real_path )
  {
    ( void ) realpath( path, real_path );
    fprintf( stderr, "path:%s; real_path:%s;\n\n", path, real_path );
    if (  /* strlen( real_path ) > 1 && */ !( real_path && *real_path == '/' && real_path[1] == 0 ) )
      pathid = duf_update_realpath_self_up( real_path, group, DUF_TRUE );
    mas_free( real_path );
  }
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return pathid;
}
