#include <string.h>

#include <mastar/tools/mas_utils_path.h> /* mas_pathdepth */

#include "duf_maintenance.h"

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"
#include "duf_levinfo_credel.h"

#include "duf_li_credel.h"
#include "duf_li.h"


#include "duf_dh.h"


#include "duf_pathinfo_ref_def.h"
/* ###################################################################### */
#include "duf_pathinfo_credel.h"
/* ###################################################################### */

int
duf_pi_levinfo_calc_depth( duf_pathinfo_t * pi )
{
  int d;
  assert( pi );
  d = pi->levinfo ? duf_li_calc_depth( pi->levinfo ) : 0;
  return d;
}

void
duf_pi_levinfo_delete( duf_pathinfo_t * pi )
{
  if ( pi->levinfo )
  {
    duf_li_clear_n( pi->levinfo, pi->maxdepth );
    mas_free( pi->levinfo );
    pi->levinfo = NULL;
  }
}

int
duf_pi_levinfo_set( duf_pathinfo_t * pi, duf_levinfo_t * pli, size_t maxdepth )
{
  DEBUG_STARTR( r );

  assert( pi );

  if ( maxdepth )
  {
    duf_pi_levinfo_delete( pi );
    assert( !pi->levinfo );
    pi->maxdepth = maxdepth;
    pi->levinfo = pli;
    pi->depth = duf_li_calc_depth( pli );
    assert( pi->levinfo );
  }

  DEBUG_ENDR( r );
}

int
duf_pi_levinfo_create( duf_pathinfo_t * pi, size_t maxdepth )
{
  DEBUG_STARTR( r );
  duf_levinfo_t *pli = NULL;

  pli = duf_li_create( maxdepth );
  DOR( r, duf_pi_levinfo_set( pi, pli, maxdepth ) );

  DEBUG_ENDR( r );
}

void
duf_pi_copy( duf_pathinfo_t * pidst, const duf_pathinfo_t * pisrc, int no_li )
{
  assert( pidst );
  assert( pisrc );
  memcpy( pidst, pisrc, sizeof( duf_pathinfo_t ) );
  pidst->levinfo = ( no_li ) ? NULL : duf_li_clone( pisrc->levinfo, pisrc->maxdepth );
}

int
duf_pi_shut( duf_pathinfo_t * pi )
{
  DEBUG_STARTR( r );

  pi->depth = pi->topdepth = 0;
  pi->maxdepth = 0;
  assert( !pi->maxdepth );
  assert( !pi->topdepth );
  assert( !pi->depth );
  assert( !pi->levinfo );

  DEBUG_ENDR( r );
}

int
duf_pi_set_max_rel_depth( duf_pathinfo_t * pi, const char *real_path, int max_rd )
{
  DEBUG_STARTR( r );

  pi->depth = -1;

  if ( real_path )
  {
    DOR( r, mas_pathdepth( real_path ) );
    if ( DUF_NOERROR( r ) )
      pi->topdepth = r;
  }

  assert( pi->depth == -1 );
  /* DUF_TRACE( temp, 0, "@@@@@@@ %u", max_rd ); */
  pi->maxdepth = max_rd ? max_rd : 20 + ( pi->topdepth ? pi->topdepth : 20 ); /* FIXME ??? */

  DEBUG_ENDR( r );
}
