#include "duf_maintenance.h"

#include "duf_pathinfo_ref.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"
/* ###################################################################### */
#include "duf_pdi_depth.h"
/* ###################################################################### */

int
duf_pi_is_good_depth_d( const duf_pathinfo_t * pi, int delta, int frecursive, int flinear, int d )
{
  int rd = 0;

  /* if ( duf_pi_recursive( pi ) ) */
  {
    rd = d - duf_pi_maxdepth( pi ) < delta; /* d - pathinfo.maxdepth < delta */
    DUF_TRACE( temp, 5, "(%d>0) d:%d - pathinfo.maxdepth:%d < delta:%d", ( rd ), d, duf_pi_maxdepth( pi ), delta );
  }
  DUF_TRACE( temp, 2, "@@rd:%d max:%d; rel(%d):%d", rd, duf_pi_maxdepth( pi ), d, duf_pi_deltadepth_d( pi, d ) );
  if ( flinear )
    rd = 1;
  else if ( rd && !frecursive /* && duf_pi_allow_dirs( pi ) */  )
  {
    rd = duf_pi_deltadepth_d( pi, d ) <= delta; /* d - topdepth <= delta */
    DUF_TRACE( temp, 3, "(%d>0) duf_pi_topdepth(pi):%d; duf_pi_reldepth_d( pdi, %d ):%d ? delta:%d;", ( rd ), d,
               duf_pi_topdepth( pi ), duf_pi_deltadepth_d( pi, d ), delta );
  }

  /* rd= duf_pdi_topdepth( pdi ) + duf_pdi_reldepth( pdi ) < duf_pdi_maxdepth( pdi ); */
  return rd;
}

int
duf_pi_is_good_depth( const duf_pathinfo_t * pi, int delta, int frecursive, int flinear )
{
  int rd = 0;

  rd = duf_pi_is_good_depth_d( pi, delta, duf_pi_depth( pi ), frecursive, flinear );
  return rd;
}

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
    rd = d - duf_pdi_maxdepth( pdi ) < delta; /* d - pathinfo.maxdepth < delta */
    DUF_TRACE( temp, 5, "(%d>0) d:%d - pathinfo.maxdepth:%d < delta:%d", ( rd ), d, duf_pdi_maxdepth( pdi ), delta );
  }
  DUF_TRACE( temp, 2, "@@[R:%d] rd:%d max:%d; rel(%d):%d", rd, duf_pdi_recursive( pdi ), duf_pdi_maxdepth( pdi ), d, duf_pdi_reldepth_d( pdi, d ) );
  if ( duf_pdi_linear( pdi ) )
    rd = 1;
  else if ( rd && !duf_pdi_recursive( pdi ) /* && duf_pdi_allow_dirs( pdi ) */  )
  {
    rd = duf_pdi_reldepth_d( pdi, d ) <= delta; /* d - topdepth <= delta */
    DUF_TRACE( temp, 3, "(%d>0) duf_pdi_topdepth(pdi):%d; duf_pdi_reldepth_d( pdi, %d ):%d ? delta:%d;", ( rd ), d,
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
