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
  return pdi ? pdi->pup : NULL;
}

const duf_yfilter_t *
duf_pdi_py( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->pyp : NULL;
}

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

int
duf_pdi_seq( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->seq : 0;
}

int
duf_pdi_recursive( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->recursive ? 1 : 0 ) : 0;
}

int
duf_pdi_allow_dirs( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->allow_dirs ? 1 : 0 ) : 0;
}

int
duf_pdi_linear( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->linear ? 1 : 0 ) : 0;
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

int
duf_pdi_is_good_depth( const duf_depthinfo_t * pdi, int delta )
{
  int rd = 0;

  rd = duf_pdi_is_good_depth_d( pdi, delta, duf_pdi_depth( pdi ) );
  return rd;
}

void
duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes )
{
  if ( pdi )
    pdi->changes += changes;
}
