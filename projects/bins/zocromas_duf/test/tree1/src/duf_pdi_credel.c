#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

/* #include <mastar/tools/mas_arg_tools.h> */

#include "duf_maintenance.h"


#include "duf_pdi_ref.h"
#include "duf_pathinfo_credel.h"

#include "duf_pdi.h"

/* ###################################################################### */
#include "duf_pdi_credel.h"
/* ###################################################################### */


duf_depthinfo_t *
duf_pdi_create( const char *name )
{
  duf_depthinfo_t *pdi;

  pdi = mas_malloc( sizeof( duf_depthinfo_t ) );
  memset( pdi, 0, sizeof( duf_depthinfo_t ) );
  if ( name )
  {
    pdi->pdi_name = mas_strdup( name );
    pdi->created_name = 1;
  }
  return pdi;
}

void
duf_pdi_copy( duf_depthinfo_t * pdidst, duf_depthinfo_t * pdisrc )
{
  char *pdname;

  pdname = pdidst->pdi_name;
#if 1
  memcpy( pdidst, pdisrc, sizeof( duf_depthinfo_t ) );
#else
  *pdidst = *pdisrc;
#endif
  pdidst->pdi_name = pdname;
  if ( !pdidst->pdi_name )
  {
    pdidst->created_name = pdisrc->created_name;
    if ( pdisrc->created_name )
      pdidst->pdi_name = mas_strdup( pdisrc->pdi_name );
    else
      pdidst->pdi_name = pdisrc->pdi_name;
  }
  pdidst->db_attached_selected = mas_strdup( pdisrc->db_attached_selected );
  pdidst->attached_copy = 1;
  pdidst->num_idstatements = 0;
  pdidst->idstatements = NULL;
  /* assert( pdisrc->num_idstatements == 0 ); */
  /* assert( !pdisrc->idstatements ); */

  duf_pi_copy( &pdidst->pathinfo, &pdisrc->pathinfo );
#if 0
  duf_items_copy( pdidst->items, pdisrc->items );
#else
  /* it's OK: no allocations at duf_items_t */
#endif

  pdidst->context.ptr = NULL;
#if 0
  duf_levinfo_context_copy( pdidst->context, pdisrc->context );
#else
  assert( !pdisrc->context.ptr );
#endif

#if 0
  duf_modcnts_copy( pdidst->cnts, pdisrc->cnts );
#else
  /* it's OK: no allocations at duf_modcnts_t */
#endif
}

duf_depthinfo_t *
duf_pdi_clone( duf_depthinfo_t * pdisrc )
{
  duf_depthinfo_t *pdi = NULL;

  pdi = duf_pdi_create( NULL /* pdisrc->pdi_name */  );
  duf_pdi_copy( pdi, pdisrc );
  pdi->root_pdi = duf_pdi_root( pdisrc );
  DUF_TRACE( pdi, 0, "@@@@@@cloned pdi %p <= %p (%p:%p:%p:%p)", pdi, pdisrc, pdi->root_pdi, pdisrc, duf_pdi_root( pdisrc ), pdisrc->root_pdi );
  return pdi;
}


int
duf_pdi_delete( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DOR( r, duf_pdi_close( pdi ) );
  if ( pdi->created_name )
    mas_free( pdi->pdi_name );
  pdi->pdi_name = NULL;
  mas_free( pdi );
  DEBUG_ENDR( r );
}

int
duf_pdi_kill( duf_depthinfo_t ** ppdi )
{
  DEBUG_STARTR( r );
  if ( ppdi )
    DOR( r, duf_pdi_delete( *ppdi ) );
  *ppdi = NULL;
  DEBUG_ENDR( r );
}
