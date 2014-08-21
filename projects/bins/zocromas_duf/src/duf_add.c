#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"

#include "duf_types.h"
#include "duf_config_ref.h"
#include "duf_utils.h"

#include "duf_dbg.h"
#include "duf_path.h"

#include "duf_levinfo.h"
#include "duf_pdi.h"


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

    DUF_TRACE( explain, 0, "adding path uni: %s", path );

    real_path = duf_realpath( path );

    duf_depthinfo_t di = {.depth = -1,
      .seq = 0,
      .levinfo = NULL,
      .u = duf_config->u,
      /* .opendir = sccb ? sccb->opendir : 0, */
      .opendir = 1,
      /* .name = real_path, */
    };

    r = duf_pdi_init( &di, real_path );


    DUF_TRACE( action, 0, "real_path:%s", real_path );
    DUF_TRACE( explain, 0, "converted to real_path: %s", real_path );
    if ( !( real_path && *real_path == '/' && real_path[1] == 0 ) )
      r = duf_real_path2db( &di, real_path, 1 /* add */  );
  DUF_TRACE( path, 0, " ***********  %llu", duf_levinfo( &di )->dirid );
    DUF_TRACE( explain, 0, "added path uni: %s", real_path );

    {
      const char *real_path_b = NULL;

      if ( !real_path_b )
        real_path_b = duf_levinfo_path( &di );

      DUF_TRACE( path, 0, "added path uni: [%s] :: [%s]  %llu", real_path, real_path_b, duf_levinfo( &di )->dirid);
    }
    duf_pdi_close( &di );


    mas_free( real_path );
  }
  DEBUG_ENDR( r );
  return r;
}
