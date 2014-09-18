/* File 20140903.131105 */
#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_utils_path.h"

/* #include "duf_levinfo.h" */
#include "duf_levinfo_ref.h"
#include "duf_pdi.h"


/* ###################################################################### */
#include "duf_add.h"
/* ###################################################################### */


int
duf_add_path_uni( const char *path )
{
  DEBUG_STARTR( r );
  char *real_path = NULL;

  DUF_TRACE( explain, 0, "adding path uni: %s", path );

  real_path = duf_realpath( path, &r );

  duf_depthinfo_t di = {.depth = -1,
    .seq = 0,
    .levinfo = NULL,
    .pu = duf_config->pu,
    /* .opendir = sccb ? sccb->opendir : 0, */
    .opendir = 1,
    /* .name = real_path, */
  };

  DOR( r, duf_pdi_init_wrap( &di, real_path, 1, 1 /* recursive */  ) );
  DUF_TRACE( path, 0, "added path uni: [%s] :: [%s]  %llu", real_path, duf_levinfo_path( &di ), duf_levinfo_dirid( &di ) );
  duf_pdi_close( &di );
  mas_free( real_path );
  DEBUG_ENDR( r );
}
