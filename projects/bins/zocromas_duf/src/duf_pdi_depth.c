/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"
#include "duf_config_util.h"

#include "duf_pathinfo_ref.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"
/* ###################################################################### */
#include "duf_pdi_depth.h"
/* ###################################################################### */

/**! \brief
 20151114.124346
 */
/* pdi->pathinfo.topdepth + pdi->pathinfo.depth - pdi->pathinfo.topdepth === pdi->pathinfo.depth */
int
duf_pdi_is_good_depth_d( const duf_depthinfo_t * pdi, int delta, int d )
{
  int rd = 0;

/* if ( duf_pdi_recursive( pdi ) ) */
  {
    rd = d - duf_pdi_maxdepth( pdi ) < delta;                        /* d - pathinfo.maxdepth < delta */
    MAST_TRACE( temp, 60, "(%d>0) d:%d - pathinfo.maxdepth:%d < delta:%d", ( rd ), d, duf_pdi_maxdepth( pdi ), delta );
  }
  MAST_TRACE( temp, 30, "@@[R:%d] rd:%d max:%d; rel(%d):%d", rd, duf_pdi_recursive( pdi ), duf_pdi_maxdepth( pdi ), d, duf_pdi_reldepth_d( pdi, d ) );
  if ( duf_pdi_linear( pdi ) )
    rd = 1;
  else if ( rd && !duf_pdi_recursive( pdi ) /* && duf_pdi_allow_dirs( pdi ) */  )
  {
    rd = duf_pdi_reldepth_d( pdi, d ) <= delta;                      /* d - topdepth <= delta */
    MAST_TRACE( temp, 30, "(%d>0) duf_pdi_topdepth(pdi):%d; duf_pdi_reldepth_d( pdi, %d ):%d ? delta:%d;", ( rd ), d,
               duf_pdi_topdepth( pdi ), duf_pdi_reldepth_d( pdi, d ), delta );
  }

/* rd= duf_pdi_topdepth( pdi ) + duf_pdi_reldepth( pdi ) < duf_pdi_maxdepth( pdi ); */
  return rd;
}

/* 20151114.124348 */
int
duf_pdi_is_good_depth( const duf_depthinfo_t * pdi, int delta )
{
  int rd = 0;

  rd = duf_pdi_is_good_depth_d( pdi, delta, duf_pdi_depth( pdi ) );
  return rd;
}
