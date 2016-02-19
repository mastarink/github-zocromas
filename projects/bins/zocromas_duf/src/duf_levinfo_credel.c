#include <string.h>

#include "duf_maintenance.h"


#include "duf_levinfo_updown.h"
#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"

#include "duf_pathinfo.h"
#include "duf_pathinfo_credel.h"

/* ###################################################################### */
#include "duf_levinfo_credel.h"
/* ###################################################################### */

int
duf_levinfo_set_li( duf_depthinfo_t * pdi, duf_levinfo_t * pli, size_t maxdepth )
{
  DUF_STARTR( r );
 
  DOR( r, duf_pi_levinfo_set( &pdi->pathinfo, pli, maxdepth ) );

  DUF_ENDR( r );
}

/* create level-control array, open 0 level */
int
duf_levinfo_create( duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );

  assert( pdi );

  assert( duf_levinfo_dirid( pdi ) == 0 );
  duf_pi_levinfo_create( &pdi->pathinfo, pdi->pathinfo.maxdepth );

  assert( pdi->pathinfo.levinfo );
  assert( pdi->pathinfo.depth == -1 );

  DUF_ENDR( r );
}

/* delete level-control array, close 0 level */
int
duf_levinfo_delete( duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );

  assert( pdi );
  /* assert( pdi->pathinfo.levinfo ); */

  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );

  if ( pdi->pathinfo.levinfo )
  {
    while ( DUF_NOERROR( r ) && pdi->pathinfo.depth >= 0 )
    {
      assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );
      duf_levinfo_goup( pdi );
    }
    assert( pdi->pathinfo.depth == -1 );

    mas_free( pdi->pathinfo.levinfo );
    pdi->pathinfo.levinfo = NULL;
  }
  DUF_ENDR( r );
}
