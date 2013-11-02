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


/* ###################################################################### */
#include "duf_update.h"
/* ###################################################################### */




unsigned long long
duf_add_path( const char *path, const char *group )
{
  unsigned long long pathid = 0;
  char *rpath;

  fprintf( stderr, "Add path %s group: %s\n", path, group );

  rpath = mas_malloc( PATH_MAX );
  if ( rpath )
  {
    ( void ) realpath( path, rpath );
    fprintf( stderr, "path:%s; rpath:%s;\n\n", path, rpath );
    if (  /* strlen( rpath ) > 1 && */ !( rpath && *rpath == '/' && rpath[1] == 0 ) )
      pathid = duf_update_realpath_self_up( rpath, group, DUF_TRUE );
    mas_free( rpath );
  }
  return pathid;
}

