#include <string.h>

#include "duf_maintenance.h"

#include "duf_config.h"

#include "duf_levinfo.h"
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
duf_pdi_copy( duf_depthinfo_t * pdidst, duf_depthinfo_t * pdisrc, int no_li )
{
  char *pdname;

  pdname = pdidst->pdi_name;
#if 1
  memcpy( pdidst, pdisrc, sizeof( duf_depthinfo_t ) );
#else
  *pdidst = *pdisrc;
#endif
  pdidst->pdi_name = pdname;
  pdidst->next = NULL;
  if ( !pdidst->pdi_name )
  {
    pdidst->created_name = pdisrc->created_name;
    if ( pdisrc->created_name )
      pdidst->pdi_name = mas_strdup( pdisrc->pdi_name );
    else
      pdidst->pdi_name = pdisrc->pdi_name;
  }
  /* pdidst->db_attached_selected = mas_strdup( pdisrc->db_attached_selected ); */
  assert( !pdidst->next );
  pdidst->attached_copy = 1;
  pdidst->num_idstatements = 0;
  pdidst->idstatements = NULL;
  if ( pdisrc->pyp_created )
  {
    assert( pdisrc->pyp );
    pdidst->pyp_created = 1;
    pdidst->pyp = mas_malloc( sizeof( *pdidst->pyp ) );
    memcpy( pdidst->pyp, pdisrc->pyp, sizeof( *pdidst->pyp ) );
  }
  /* assert( pdisrc->num_idstatements == 0 ); */
  /* assert( !pdisrc->idstatements ); */

  duf_pi_copy( &pdidst->pathinfo, &pdisrc->pathinfo, no_li );
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
duf_pdi_clone( duf_depthinfo_t * pdisrc, int no_li )
{
  duf_depthinfo_t *pdi = NULL;

  pdi = duf_pdi_create( NULL /* pdisrc->pdi_name */  );
  duf_pdi_copy( pdi, pdisrc, no_li );
  pdi->root_pdi = duf_pdi_root( pdisrc );
  DUF_TRACE( pdi, 0, "@@@@@@cloned pdi %p <= %p (%p:%p:%p:%p)", pdi, pdisrc, pdi->root_pdi, pdisrc, duf_pdi_root( pdisrc ), pdisrc->root_pdi );
  return pdi;
}


int
duf_pdi_delete( duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );

  assert( pdi && pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

  /* assert( pdi->pathinfo.levinfo[pdi->pathinfo.depth].itemname ); */
  DOR( r, duf_pdi_close( pdi ) );
  if ( pdi->created_name )
    mas_free( pdi->pdi_name );
  pdi->pdi_name = NULL;
  mas_free( pdi );
  DUF_ENDR( r );
}

int
duf_pdi_kill( duf_depthinfo_t ** ppdi )
{
  DUF_STARTR( r );
  if ( ppdi && *ppdi )
    DOR( r, duf_pdi_delete( *ppdi ) );
  *ppdi = NULL;
  DUF_ENDR( r );
}
