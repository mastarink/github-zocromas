#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_trace.h"

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
  DEBUG_STARTR( r );

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
  DEBUG_ENDR( r );
}
