#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"
#include "duf_utils.h"

#include "duf_dbg.h"
#include "duf_path.h"


/* ###################################################################### */
#include "duf_add.h"
/* ###################################################################### */


int
duf_add_path_uni( const char *path )
{
  int r = 0;

  DEBUG_START(  );
  {
    char *real_path = NULL;

    real_path = duf_realpath( path );
    DUF_TRACE( action, 0, "real_path:%s", real_path );
    if ( !( real_path && *real_path == '/' && real_path[1] == 0 ) )
      r = duf_real_path_to_pathid2( ( duf_depthinfo_t * ) NULL, real_path, 1 /* add */ , 0 /* need_id */  );
    mas_free( real_path );
  }
  DEBUG_ENDR( r );
  return r;
}
