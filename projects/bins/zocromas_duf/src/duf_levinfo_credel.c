#include <string.h>
#include <assert.h>
#include <limits.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_levinfo_updown.h"
/* ###################################################################### */
#include "duf_levinfo_credel.h"
/* ###################################################################### */




/* create level-control array, open 0 level */
int
duf_levinfo_create( duf_depthinfo_t * pdi, int pathdepth, int recursive )
{
  DEBUG_STARTR( r );

  assert( pdi );

  if ( !pdi->levinfo )
  {
    size_t lsz;
    int max_rel_depth = 0;

    max_rel_depth = pdi && pdi->pu ? pdi->pu->max_rel_depth : 20;
    assert( pdi->depth == -1 );
    if ( max_rel_depth /* FIXME */  )
    {
      pdi->maxdepth = max_rel_depth + pathdepth;
      pdi->recursive = recursive ? 1 : 0;
      lsz = sizeof( pdi->levinfo[0] ) * ( pdi->maxdepth + 3 );
      /* DUF_DIE( 0, "@@@@@@@ %lu : %u : %lu : %lu", lsz,pdi->maxdepth, sizeof( pdi->levinfo[0] ), sizeof( duf_levinfo_t ) ); */
      pdi->levinfo = mas_malloc( lsz );
      assert( pdi->levinfo );
      memset( pdi->levinfo, 0, lsz );
      assert( pdi->depth == -1 );
    }
    else
    {
      T( "max_rel_depth:%d", max_rel_depth );
      DOR( r, DUF_ERROR_LEVINFO_SIZE );
    }
    /* r = duf_levinfo_open_dh( pdi, path ); */
  }
  else
  {
  }
  DEBUG_ENDR( r );
}

/* delete level-control array, close 0 level */
int
duf_levinfo_delete( duf_depthinfo_t * pdi )
{
  int r = 0;

  assert( pdi );
  assert( pdi->levinfo );

  if ( pdi->levinfo )
  {
    while ( r >= 0 && pdi->depth >= 0 )
    {
      duf_levinfo_goup( pdi );
    }
    assert( pdi->depth == -1 );

    mas_free( pdi->levinfo );
    pdi->levinfo = NULL;
  }
  DUF_TEST_R( r );
  return r;
}