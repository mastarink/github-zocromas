/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"

#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth etc. ♣ */
#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_levinfo_ref_def.h"
/* #include "duf_levinfo_context.h" */
#include "duf_levinfo_credel.h"

#include "duf_li_credel.h"
#include "duf_li.h"

#include "duf_dh.h"

#include "duf_pathinfo_credel.h"
#include "duf_pathinfo_ref.h"
#include "duf_sccb_scanstage.h"

#include "duf_pathinfo_ref_def.h"
/* ###################################################################### */
#include "duf_pathinfo.h"
/* ###################################################################### */

int
duf_pi_levinfo_set( duf_pathinfo_t * pi, duf_levinfo_t * pli, size_t maxdepth )
{
  DUF_STARTR( r );

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

  DUF_ENDR( r );
}

int
duf_pi_set_max_rel_depth( duf_pathinfo_t * pi, const char *real_path, int max_rd )
{
  DUF_STARTR( r );

  pi->depth = -1;

  if ( real_path )
  {
    DOR( r, mas_pathdepth( real_path ) );
    if ( DUF_NOERROR( r ) )
      pi->topdepth = r;
  }

  assert( pi->depth == -1 );
/* MAST_TRACE( temp, 0, "@@@@@@@ %u", max_rd ); */
  pi->maxdepth = max_rd ? max_rd : 20 + ( pi->topdepth ? pi->topdepth : 20 ); /* FIXME ??? */

  DUF_ENDR( r );
}

void
duf_pi_clear_d( duf_pathinfo_t * pi, int d )
{
  assert( pi );
  duf_li_clear( &pi->levinfo[d] );
}
/* *INDENT-OFF*  */
DUF_PATHINFO_VF( void, clear )
DUF_PATHINFO_VF_UP( void, clear )
/* *INDENT-ON*  */

void
duf_pi_clear_all( duf_pathinfo_t * pi )
{
  assert( pi );
  duf_li_clear_alln( pi->levinfo, pi->maxdepth );
}
