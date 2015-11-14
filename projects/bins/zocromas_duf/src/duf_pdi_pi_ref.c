#include "duf_maintenance.h"

/* #include "duf_ufilter_ref.h" */
#include "duf_pathinfo_ref.h"

/* ###################################################################### */
#include "duf_pdi_pi_ref.h"
/* ###################################################################### */

int
duf_pdi_depth( const duf_depthinfo_t * pdi )
{
#if 0
  return pdi ? pdi->pathinfo.depth : 0;
#else
  return pdi ? duf_pi_depth( &pdi->pathinfo ) : 0;
#endif
}

int
duf_pdi_reldepth_d( const duf_depthinfo_t * pdi, int d )
{
  return pdi ? duf_pi_deltadepth_d( &pdi->pathinfo, d ) : 0;
}

/* pdi->pathinfo.depth - pdi->pathinfo.topdepth */
int
duf_pdi_reldepth( const duf_depthinfo_t * pdi )
{
  return pdi ? duf_pi_deltadepth( &pdi->pathinfo ) : 0;
}

int
duf_pdi_topdepth( const duf_depthinfo_t * pdi )
{
#if 0
  return pdi ? pdi->pathinfo.topdepth : 0;
#else
  return pdi ? duf_pi_topdepth( &pdi->pathinfo ) : 0;
#endif
}

void
duf_pdi_set_topdepth( duf_depthinfo_t * pdi )
{
#if 0
  if ( pdi )
    pdi->pathinfo.topdepth = pdi->pathinfo.depth;
#else
  duf_pi_set_topdepth( &pdi->pathinfo );
#endif
}

int
duf_pdi_maxdepth( const duf_depthinfo_t * pdi )
{
#if 0
  return pdi ? pdi->pathinfo.maxdepth : 0;
#else
  return pdi ? duf_pi_maxdepth( &pdi->pathinfo ) : 0;
#endif
}

duf_levinfo_t *
duf_pdi_levinfo( const duf_depthinfo_t * pdi )
{
  return pdi ? duf_pi_levinfo( &pdi->pathinfo ) : NULL;
}
