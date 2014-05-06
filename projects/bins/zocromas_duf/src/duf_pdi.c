#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_dbg.h"

#include "duf_levinfo.h"

/* ###################################################################### */
#include "duf_pdi.h"
/* ###################################################################### */

void
duf_clear_context( duf_context_t * pcontext )
{
  if ( pcontext )
  {
    if ( pcontext->destructor )
      ( pcontext->destructor ) ( pcontext->ptr );
    else
      mas_free( pcontext->ptr );
  }

  pcontext->ptr = NULL;
}

void *
duf_context( duf_context_t * pcontext )
{
  return pcontext ? pcontext->ptr : NULL;
}

void
duf_set_context( duf_context_t * pcontext, void *ptr )
{
  duf_clear_context( pcontext );
  pcontext->ptr = ptr;
}

void
duf_set_context_destructor( duf_context_t * pcontext, duf_void_voidp_t destr )
{
  pcontext->destructor = destr;
}

int
duf_pdi_init( duf_depthinfo_t * pdi, const char *path )
{
  int r = 0;

  r = duf_levinfo_create( pdi, path );

  return r;
}

void
duf_pdi_set_context( duf_depthinfo_t * pdi, void *ctx )
{
  duf_set_context( &pdi->context, ctx );
}

void
duf_pdi_set_context_destructor( duf_depthinfo_t * pdi, duf_void_voidp_t destr )
{
  duf_set_context_destructor( &pdi->context, destr );
}

void *
duf_pdi_context( duf_depthinfo_t * pdi )
{
  return duf_context( &pdi->context );
}

void
duf_pdi_close( duf_depthinfo_t * pdi )
{
  duf_clear_context( &pdi->context );
  duf_levinfo_delete( pdi );
}

int
duf_pdi_max_filter( const duf_depthinfo_t * pdi )
{
  int rv = 0;

  rv = ( ( !pdi->u.maxseq || pdi->seq <= pdi->u.maxseq )
         && ( !pdi->u.maxitems.files || ( pdi->items.files ) < pdi->u.maxitems.files )
         && ( !pdi->u.maxitems.dirs || ( pdi->items.dirs ) < pdi->u.maxitems.dirs )
         && ( !pdi->u.maxitems.total || ( pdi->items.total ) < pdi->u.maxitems.total ) );
  /* DUF_PRINTF( 0, "%llu ? %llu : %d", pdi->seq, pdi->u.maxseq, rv ); */
  return rv;
}

int
duf_pdi_deltadepth( const duf_depthinfo_t * pdi, int d )
{
  return pdi ? d - pdi->topdepth : 0;
}

int
duf_pdi_depth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->depth : 0;
}

int
duf_pdi_reldepth( const duf_depthinfo_t * pdi )
{
  return pdi ? duf_pdi_deltadepth( pdi, pdi->depth ) : 0;
}

int
duf_pdi_topdepth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->topdepth : 0;
}

void
duf_pdi_set_topdepth( duf_depthinfo_t * pdi )
{
  if ( pdi )
    pdi->topdepth = pdi->depth;
}
