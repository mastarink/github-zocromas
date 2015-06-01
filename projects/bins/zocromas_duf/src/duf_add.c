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
duf_add_path_uni( const char *path, const char *node_selector2 )
{
  DEBUG_STARTR( r );
  char *real_path = NULL;

  DUF_TRACE( explain, 0, "adding path uni: %s", path );

  real_path = duf_realpath( path, &r );

  duf_depthinfo_t di = {.depth = -1,
    .seq = 0,
    .levinfo = NULL,
    .pu = NULL,
    /* .opendir = sccb ? sccb->opendir : 0, */
    .opendir = 1,
    /* .name = real_path, */
  };

  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( &di, real_path, 1 /* caninsert ~ "root" */ , node_selector2,
                                        1 /* recursive */ , 0 /* opendir */  ) );
  DUF_TRACE( path, 1, "@@@#%-5llu    added path:[%40s]", duf_levinfo_dirid( &di ), real_path );

  /* xchanges = di.changes; --- needless!? */
  duf_pdi_close( &di );
  mas_free( real_path );
  DEBUG_ENDR( r );
}
