#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_context.h"
/* ###################################################################### */

void
duf_clear_context( duf_levinfo_context_t * pcontext )
{
  if ( pcontext )
  {
    if ( pcontext->destructor )
      ( pcontext->destructor ) ( pcontext->ptr );
    else
      mas_free( pcontext->ptr );
  }
  pcontext->ptr = NULL;
  /*? */
  pcontext->destructor = NULL;
}

void *
duf_context( const duf_levinfo_context_t * pcontext )
{
  return pcontext ? pcontext->ptr : NULL;
}

void
duf_set_context( duf_levinfo_context_t * pcontext, void *ptr )
{
  duf_clear_context( pcontext );
  assert( pcontext );
  pcontext->ptr = ptr;
}

void
duf_set_context_destructor( duf_levinfo_context_t * pcontext, duf_void_voidp_func_t destr )
{
  assert( pcontext );
  pcontext->destructor = destr;
}
