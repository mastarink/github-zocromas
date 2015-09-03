#include <assert.h>

#include <mastar/wrap/mas_std_def.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_pdi_ref.h"
/* ###################################################################### */



int
duf_pdi_max_filter( const duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
  if ( pdi->pu->max_seq && pdi->seq >= pdi->pu->max_seq )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_SEQ_REACHED );
  else if ( pdi->pu->maxitems.files && pdi->items.files >= pdi->pu->maxitems.files )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pu->maxitems.dirs && pdi->items.dirs >= pdi->pu->maxitems.dirs )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );
  else if ( pdi->pu->maxitems.total && pdi->items.total >= pdi->pu->maxitems.total )
    DUF_MAKE_ERROR( r, DUF_ERROR_MAX_REACHED );


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

int
duf_pdi_deltadepth( const duf_depthinfo_t * pdi, int d )
{
  return pdi ? d - pdi->pathinfo.topdepth : 0;
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

  DEBUG_ENDRN( rd );
}

int
duf_pdi_depth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->pathinfo.depth : 0;
}

/* pdi->pathinfo.depth - pdi->pathinfo.topdepth */
int
duf_pdi_reldepth( const duf_depthinfo_t * pdi )
{
  return pdi ? duf_pdi_deltadepth( pdi, pdi->pathinfo.depth ) : 0;
}

void
duf_pdi_set_topdepth( duf_depthinfo_t * pdi )
{
  if ( pdi )
    pdi->pathinfo.topdepth = pdi->pathinfo.depth;
}

int
duf_pdi_topdepth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->pathinfo.topdepth : 0;
}

int
duf_pdi_maxdepth( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->maxdepth : 0;
}
/* pdi->pathinfo.topdepth + pdi->pathinfo.depth - pdi->pathinfo.topdepth === pdi->pathinfo.depth */
int
duf_pdi_is_good_depth_d( const duf_depthinfo_t * pdi, int delta, int d )
{
  int rd = 0;

  if ( duf_pdi_recursive( pdi ) )
    rd = d - duf_pdi_maxdepth( pdi ) < delta; /* d - maxdepth < delta */
  else
    rd = duf_pdi_deltadepth( pdi, d ) <= delta; /* d - topdepth <= delta */
  /* rd= duf_pdi_topdepth( pdi ) + duf_pdi_reldepth( pdi ) < duf_pdi_maxdepth( pdi ); */
  return rd;
}

int
duf_pdi_is_good_depth( const duf_depthinfo_t * pdi, int delta )
{
  return duf_pdi_is_good_depth_d( pdi, delta, pdi->pathinfo.depth );
}

void
duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes )
{
  if ( pdi )
    pdi->changes += changes;
}
