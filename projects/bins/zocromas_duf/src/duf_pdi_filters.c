/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"

#include <mastar/trace/mas_trace.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_ufilter_ref.h"

/* ###################################################################### */
#include "duf_pdi_filters.h"
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

/* 20151114.124337 */
/* needless?!? TODO */
int
duf_pdi_max_filter( const duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );

  assert( pdi );
#if 0
  if ( pdi->pup->max_seq && pdi->seq >= pdi->pup->max_seq )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_SEQ_REACHED );
  else if ( pdi->pup->maxitems.files && pdi->items.files >= pdi->pup->maxitems.files )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pup->maxitems.dirs && pdi->items.dirs >= pdi->pup->maxitems.dirs )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pup->maxitems.total && pdi->items.total >= pdi->pup->maxitems.total )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
#else
  DOR( r, duf_ufilter_max_filter( pdi->pup, pdi->seq, &pdi->items ) );

#endif

/* rv = ( ( !pdi->pup->max_seq || pdi->seq <= pdi->pup->max_seq )                                  */
/*        && ( !pdi->pup->maxitems.files || ( pdi->items.files ) < pdi->pup->maxitems.files )    */
/*        && ( !pdi->pup->maxitems.dirs || ( pdi->items.dirs ) < pdi->pup->maxitems.dirs )       */
/*        && ( !pdi->pup->maxitems.total || ( pdi->items.total ) < pdi->pup->maxitems.total ) ); */
  DUF_ENDR( r );
}
