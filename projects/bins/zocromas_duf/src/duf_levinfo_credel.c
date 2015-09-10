#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_levinfo_updown.h"

#include "duf_pathinfo_ref.h"
/* ###################################################################### */
#include "duf_levinfo_credel.h"
/* ###################################################################### */


/* create level-control array, open 0 level */
int
duf_levinfo_create( duf_depthinfo_t * pdi, int pathdepth, int recursive, int opendir )
{
  DEBUG_STARTR( r );

  assert( pdi );
#if 0
  if ( !pdi->pathinfo.levinfo )
  {
    int max_rel_depth = 0;

    max_rel_depth = pdi && pdi->pu ? pdi->pu->max_rel_depth : 20;
    assert( pdi->pathinfo.depth == -1 );
    /* DUF_TRACE( temp, 0, "@@@@@@@ %u", max_rel_depth ); */
    if ( max_rel_depth /* FIXME */  )
    {
      pdi->maxdepth = max_rel_depth + ( pathdepth ? pathdepth : 20 );
      pdi->recursive = recursive ? 1 : 0;
      pdi->opendir = opendir ? 1 : 0;

      duf_pi_levinfo_create( &pdi->pathinfo, pdi->maxdepth + 3 );
      assert( pdi->pathinfo.levinfo );
      assert( pdi->pathinfo.depth == -1 );
    }
    else
    {
      T( "max_rel_depth:%d", max_rel_depth );
      DUF_MAKE_ERROR( r, DUF_ERROR_LEVINFO_SIZE );
    }
    /* DOR( r, duf_levinfo_open_dh( pdi, path ) ); */
  }
  else
  {
  }
#else
  duf_pi_levinfo_create( &pdi->pathinfo, pdi->maxdepth + 3 );
  assert( pdi->pathinfo.levinfo );
  assert( pdi->pathinfo.depth == -1 );
#endif
  DEBUG_ENDR( r );
}

/* delete level-control array, close 0 level */
int
duf_levinfo_delete( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  assert( pdi );
  /* assert( pdi->pathinfo.levinfo ); */

  if ( pdi->pathinfo.levinfo )
  {
    while ( DUF_NOERROR( r ) && pdi->pathinfo.depth >= 0 )
    {
      duf_levinfo_goup( pdi );
    }
    assert( pdi->pathinfo.depth == -1 );

    mas_free( pdi->pathinfo.levinfo );
    pdi->pathinfo.levinfo = NULL;
  }
  DEBUG_ENDR( r );
}
