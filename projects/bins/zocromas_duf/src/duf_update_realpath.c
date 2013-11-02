#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

/* #include "duf_sql.h" */
#include "duf_path.h"
#include "duf_insert.h"

/* #include "duf_update_path.h" */
#include "duf_update_pathid.h"


/* ###################################################################### */
#include "duf_update_realpath.h"
/* ###################################################################### */


unsigned long long
duf_update_realpath_down( const char *real_path, unsigned long long parentid, int recursive, int dofiles )
{
  unsigned long long pathid = 0;

  /* fprintf( stderr, "Update real_path UP %s\n", real_path ); */
  pathid = duf_update_realpath_self_up( real_path, NULL, DUF_TRUE /* up */  );
  pathid = duf_update_pathid_down( parentid, recursive, dofiles, pathid );
  return pathid;
}

unsigned long long
duf_update_realpath_self_up( const char *real_path, const char *group, int up )
{
  unsigned long long pathid = 0;
  char *dir_name;
  const char *base_name = NULL;
  char *rpath = mas_strdup( real_path );

  base_name = basename( rpath );
  dir_name = dirname( rpath );
  if ( !base_name || !*base_name )
  {
    base_name = real_path + 1;
    dir_name = NULL;
  }
  /* fprintf( stderr, ">>>>>>>>>>>>> Update real path %s : %s + %s\n", real_path, dir_name, base_name ); */
  if ( base_name && *base_name )
  {
    int r;
    struct stat st_dir;

    r = stat( real_path, &st_dir );
    if ( !r )
    {
      unsigned long long parentid = 0;

      /* fprintf( stderr, "Update %s + %s; group:%s\n", dir_name, base_name, group ); */
      if ( ( dir_name && *dir_name ) && up )
        parentid = duf_update_realpath_self_up( dir_name, NULL /* group */ , DUF_TRUE /* up */  );
      pathid = duf_insert_path( base_name, &st_dir, parentid );
      /* fprintf( stderr, "Updated %s + %s; group:%s ==> %lld\n", dir_name, base_name, group, pathid ); */

      if ( group )
        duf_pathid_group( group, pathid );

      /* fprintf( stderr, "inserted pathid %llu\n", pathid ); */
    }
  }
  /* fprintf( stderr, "Update real path %s done\n", rpath ); */
  mas_free( rpath );
  return pathid;
}
