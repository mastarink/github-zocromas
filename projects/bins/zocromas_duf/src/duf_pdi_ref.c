#include "duf_maintenance.h"

#include "duf_ufilter_ref.h"
/* #include "duf_pathinfo_ref.h" */

#include "duf_pdi_pi_ref.h"
/* ###################################################################### */
#include "duf_pdi_ref.h"
/* ###################################################################### */


duf_depthinfo_t *
duf_pdi_root( duf_depthinfo_t * pdi )
{
  return pdi->root_pdi ? pdi->root_pdi : pdi;
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

void
duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes )
{
  if ( pdi )
    pdi->changes += changes;
}
