/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_ufilter_structs.h"
#include "duf_ufilter_ref.h"

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
/* ###################################################################### */

/* 20151114.124333 */
const duf_ufilter_t *
duf_pdi_pu( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->pup : NULL;
}

/* 20151114.124335 */
const duf_yfilter_t *
duf_pdi_py( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->pyp : NULL;
}

SRX( PDI, const char *, pack_field, NULL, pdi_pack_field, const duf_depthinfo_t * pdi )
{
  pack_field = duf_pdi_pu( pdi )->pack_field;
  ERX( PDI, const char *, pack_field, NULL, pdi_pack_field, const duf_depthinfo_t * pdi );
}

/* 20151114.124337 */
/* needless?!? TODO */
SR( PDI, pdi_max_filter, const duf_depthinfo_t * pdi )
{
  assert( pdi );
#if 0
  if ( pdi->pup->max_seq && pdi->seqq.gen >= pdi->pup->max_seq )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_SEQ_REACHED );
  else if ( pdi->pup->maxitems.files && pdi->items.files >= pdi->pup->maxitems.files )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pup->maxitems.dirs && pdi->items.dirs >= pdi->pup->maxitems.dirs )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pup->maxitems.total && pdi->items.total >= pdi->pup->maxitems.total )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
#else
  CR( ufilter_max_filter, pdi->pup, pdi->seqq.gen, &pdi->items );

#endif

/* rv = ( ( !pdi->pup->max_seq || pdi->seqq.gen <= pdi->pup->max_seq )                           */
/*        && ( !pdi->pup->maxitems.files || ( pdi->items.files ) < pdi->pup->maxitems.files )    */
/*        && ( !pdi->pup->maxitems.dirs || ( pdi->items.dirs ) < pdi->pup->maxitems.dirs )       */
/*        && ( !pdi->pup->maxitems.total || ( pdi->items.total ) < pdi->pup->maxitems.total ) ); */
  ER( PDI, pdi_max_filter, const duf_depthinfo_t * pdi );
}
