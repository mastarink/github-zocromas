#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_utils.h"
#include "duf_path.h"
#include "duf_dirent.h"
#include "duf_insert.h"

#include "duf_update_realpath.h"
#include "duf_update_path.h"
#include "duf_update_pathentries.h"

#include "duf_update_pathid.h"

unsigned long long
duf_update_pathid_down( unsigned long long parentid, int recursive, int dofiles, unsigned long long pathid )
{
  int r;
  struct stat st_dir;
  char *real_path;

  real_path = duf_pathid_to_path( pathid );
  r = stat( real_path, &st_dir );
  if ( !r )
  {
    unsigned long items = 0;

    items = duf_update_realpath_entries( real_path, &st_dir, pathid, recursive, dofiles );
    /* fprintf( stderr, "Down %s  items:%lu\n", real_path, items ); */
    duf_sql( "UPDATE duf_paths SET items='%u', last_updated=datetime()  WHERE id='%lu'", items, pathid );
    duf_pathid_group( "updated", pathid );
  }
  else
    fprintf( stderr, "Error down %s\n", real_path );

  mas_free( real_path );
  return pathid;
}
