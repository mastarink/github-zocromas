#include <string.h>

#include <mastar/tools/mas_utils_path.h> /* mas_pathdepth */

#include "duf_maintenance.h"

#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"
#include "duf_levinfo_credel.h"

#include "duf_li_credel.h"
#include "duf_li.h"


#include "duf_dh.h"

#include "duf_pathinfo_credel.h"
#include "duf_pathinfo_ref.h"

#include "duf_pathinfo_ref_def.h"
/* ###################################################################### */
#include "duf_pathinfo.h"
/* ###################################################################### */

int
duf_pi_levinfo_calc_depth( duf_pathinfo_t * pi )
{
  int d;

  assert( pi );
  d = pi->levinfo ? duf_li_calc_depth( pi->levinfo ) : 0;
  return d;
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
