#undef MAS_TRACING
#include "duf_maintenance.h"

#include "duf_context.h"

/* ###################################################################### */
#include "duf_pdi_context.h"
/* ###################################################################### */




void
duf_pdi_set_context( duf_depthinfo_t * pdi, void *ctx )
{
  assert( pdi );
  duf_set_context( &pdi->context, ctx );
}

void
duf_pdi_set_context_destructor( duf_depthinfo_t * pdi, duf_void_voidp_func_t destr )
{
  assert( pdi );
  duf_set_context_destructor( &pdi->context, destr );
}

void *
duf_pdi_context( const duf_depthinfo_t * pdi )
{
  assert( pdi );
  return duf_context( &pdi->context );
}
