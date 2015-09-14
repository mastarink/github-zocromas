#include <assert.h>

#include <mastar/wrap/mas_std_def.h>

#include "duf_maintenance.h"

#include "duf_ufilter_ref.h"
#include "duf_pathinfo_ref.h"
/* ###################################################################### */
#include "duf_pdi_ref.h"
/* ###################################################################### */


duf_depthinfo_t *
duf_pdi_root( duf_depthinfo_t * pdi )
{
  return pdi->root_pdi ? pdi->root_pdi : pdi;
}

const duf_ufilter_t *
duf_pdi_pu( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->pu : NULL;
}

/* needless?!? TODO */
int
duf_pdi_max_filter( const duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
#if 0
  if ( pdi->pu->max_seq && pdi->seq >= pdi->pu->max_seq )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_SEQ_REACHED );
  else if ( pdi->pu->maxitems.files && pdi->items.files >= pdi->pu->maxitems.files )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pu->maxitems.dirs && pdi->items.dirs >= pdi->pu->maxitems.dirs )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pu->maxitems.total && pdi->items.total >= pdi->pu->maxitems.total )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
#else
  DOR( r, duf_ufilter_max_filter( pdi->pu, pdi->seq, &pdi->items ) );

#endif

  /* rv = ( ( !pdi->pu->max_seq || pdi->seq <= pdi->pu->max_seq )                                  */
  /*        && ( !pdi->pu->maxitems.files || ( pdi->items.files ) < pdi->pu->maxitems.files )    */
  /*        && ( !pdi->pu->maxitems.dirs || ( pdi->items.dirs ) < pdi->pu->maxitems.dirs )       */
  /*        && ( !pdi->pu->maxitems.total || ( pdi->items.total ) < pdi->pu->maxitems.total ) ); */
  DEBUG_ENDR( r );
}

int
duf_pdi_seq( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->seq : 0;
}

static int
duf_pdi_deltadepth_d( const duf_depthinfo_t * pdi, int d )
{
#if 0
  return pdi ? d - pdi->pathinfo.topdepth : 0;
#else
  return pdi ? duf_pi_deltadepth_d( &pdi->pathinfo, d ) : 0;
#endif
}

int
duf_pdi_recursive( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->recursive ? 1 : 0 ) : 0;
}

int
duf_pdi_opendir( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->opendir ? 1 : 0 ) : 0;
}

int
duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od )
{
  DEBUG_STARTR( rd );

  assert( pdi );
  rd = pdi->opendir;
  pdi->opendir = od;
  DUF_TRACE( fs, 3, "set opendir:%d", od );

  DEBUG_ENDR( rd );
}

int
duf_pdi_depth( const duf_depthinfo_t * pdi )
{
#if 0
  return pdi ? pdi->pathinfo.depth : 0;
#else
  return pdi ? duf_pi_depth( &pdi->pathinfo ) : 0;
#endif
}

/* pdi->pathinfo.depth - pdi->pathinfo.topdepth */
int
duf_pdi_reldepth( const duf_depthinfo_t * pdi )
{
#if 0
  return pdi ? duf_pdi_deltadepth_d( pdi, pdi->pathinfo.depth ) : 0;
#else
  return pdi ? duf_pi_deltadepth( &pdi->pathinfo ) : 0;
#endif
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

/* pdi->pathinfo.topdepth + pdi->pathinfo.depth - pdi->pathinfo.topdepth === pdi->pathinfo.depth */
int
duf_pdi_is_good_depth_d( const duf_depthinfo_t * pdi, int delta, int d )
{
  int rd = 0;

  if ( duf_pdi_recursive( pdi ) )
  {
    rd = d - duf_pdi_maxdepth( pdi ) < delta; /* d - pathinfo.maxdepth < delta */
    DUF_TRACE( temp, 5, "(%d>0) d:%d - pathinfo.maxdepth:%d < delta:%d", ( rd ), d, duf_pdi_maxdepth( pdi ), delta );
  }
  else
  {
    rd = duf_pdi_deltadepth_d( pdi, d ) <= delta; /* d - topdepth <= delta */
    DUF_TRACE( temp, 5, "(%d>0) duf_pdi_topdepth(pdi):%d; duf_pdi_deltadepth_d( pdi, %d ):%d <= delta:%d;", ( rd ), d,
#if 0
               pdi->pathinfo.topdepth, duf_pdi_deltadepth_d( pdi, d ), delta );
#else
               duf_pdi_topdepth( pdi ), duf_pdi_deltadepth_d( pdi, d ), delta );
#endif
  }



  /* rd= duf_pdi_topdepth( pdi ) + duf_pdi_reldepth( pdi ) < duf_pdi_maxdepth( pdi ); */
  return rd;
}

int
duf_pdi_is_good_depth( const duf_depthinfo_t * pdi, int delta )
{
  int rd = 0;

#if 0
  rd = duf_pdi_is_good_depth_d( pdi, delta, pdi->pathinfo.depth );
#else
  rd = duf_pdi_is_good_depth_d( pdi, delta, duf_pdi_depth( pdi ) );
#endif
  return rd;
}

void
duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes )
{
  if ( pdi )
    pdi->changes += changes;
}
