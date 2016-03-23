/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/trace/mas_trace.h>

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* #include "duf_debug_defs.h"                                          (* DUF_WRAPSTATIC; DUF_WRAPPED ...  ♠ *) */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo.h"                                             /* duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc. ✗ */
#include "duf_pdi_ref.h"
#include "duf_pathinfo_credel.h"                                     /* duf_pi_shut; duf_pi_copy; duf_pi_levinfo_create; duf_pi_levinfo_delete etc. ✗ */

#include "duf_pdi.h"                                                 /* duf_pdi_init; duf_pdi_shut; duf_pdi_close ✗ */

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_pdi_credel.h"                                          /* duf_pdi_create; duf_pdi_kill ✗ */
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
  MAST_TRACE( pdi, 0, "@@@@@@cloned pdi %p <= %p (%p:%p:%p:%p)", pdi, pdisrc, pdi->root_pdi, pdisrc, duf_pdi_root( pdisrc ), pdisrc->root_pdi );
  return pdi;
}

SR( PDI, pdi_delete, duf_depthinfo_t * pdi )
{
/* DUF_STARTR( r ); */

  assert( pdi && pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

/* assert( pdi->pathinfo.levinfo[pdi->pathinfo.depth].itemname ); */
  CR( pdi_close, pdi );
  if ( pdi->created_name )
    mas_free( pdi->pdi_name );
  pdi->pdi_name = NULL;
  mas_free( pdi );
/* DUF_ENDR( r ); */
  ER( PDI, pdi_delete, duf_depthinfo_t * pdi );
}

SR( PDI, pdi_kill, duf_depthinfo_t ** ppdi )
{
/* DUF_STARTR( r ); */
  if ( ppdi && *ppdi )
    CR( pdi_delete, *ppdi );
  *ppdi = NULL;
/* DUF_ENDR( r ); */
  ER( PDI, pdi_kill, duf_depthinfo_t ** ppdi );
}
