#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_config.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_pdi.h"
/* ###################################################################### */


int
duf_pdi_filter( duf_depthinfo_t * pdi )
{
  return ( ( !pdi->u.maxseq || pdi->seq <= pdi->u.maxseq )
           && ( !pdi->u.maxitems.files || ( pdi->items.files ) < pdi->u.maxitems.files )
           && ( !pdi->u.maxitems.dirs || ( pdi->items.dirs ) < pdi->u.maxitems.dirs )
           && ( !pdi->u.maxitems.total || ( pdi->items.total ) < pdi->u.maxitems.total ) );
}
