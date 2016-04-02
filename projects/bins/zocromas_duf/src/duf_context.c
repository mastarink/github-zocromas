/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/trace/mas_trace.h>

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_levinfo_structs.h"

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_context.h"
/* ###################################################################### */

void
duf_clear_context( duf_pdi_context_t * pcontext )
{
  if ( pcontext )
  {
    if ( pcontext->destructor )
      ( pcontext->destructor ) ( pcontext->ptr );
    else
      mas_free( pcontext->ptr );
    pcontext->ptr = NULL;
    pcontext->destructor = NULL;
  }
}

void *
duf_context( const duf_pdi_context_t * pcontext )
{
  return pcontext ? pcontext->ptr : NULL;
}

void
duf_set_context( duf_pdi_context_t * pcontext, void *ptr )
{
  duf_clear_context( pcontext );
  assert( pcontext );
  pcontext->ptr = ptr;
}

void
duf_set_context_destructor( duf_pdi_context_t * pcontext, duf_void_voidp_func_t destr )
{
  assert( pcontext );
  pcontext->destructor = destr;
}
