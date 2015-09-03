#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/wrap/mas_memory.h> */

#include "duf_maintenance.h"

#include "duf_context.h"

/* ###################################################################### */
#include "duf_levinfo_context.h"
/* ###################################################################### */


#if 0
void
duf_levinfo_clear_context( duf_levinfo_t * pli )
{
  assert( pli );
  duf_clear_context( &pli->context );
}
#endif

void
duf_levinfo_set_context_up_destructor( duf_depthinfo_t * pdi, duf_void_voidp_t destr )
{
  assert( pdi );
  {
    int d;

    d = pdi->pathinfo.depth - 1;
    if ( d >= 0 )
      duf_set_context_destructor( &pdi->pathinfo.levinfo[d].context, destr );
  }
}
