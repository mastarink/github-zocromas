/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>

#include "duf_pathinfo.h"                                            /* duf_pi_clear*; duf_pi_levinfo_set; duf_pi_set_max_rel_depth; etc. ✗ */
#include "duf_pathinfo_depth.h"                                      /* duf_pi_calc_depth; duf_pi_is_good_depth; duf_pi_check_depth; duf_pi_godown; ✗ */

#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */

/* #include "duf_pdi_structs.h" */
/* ###################################################################### */
#include "duf_levinfo.h"                                             /* duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc. ✗ */
/* ###################################################################### */

int
duf_levinfo_closed( const duf_depthinfo_t * pdi )
{
  return duf_pi_closed_all( duf_pdi_pathinfo( pdi ) );
}

void
duf_levinfo_clear_level_d( duf_depthinfo_t * pdi, int d )
{
  assert( pdi );
  if ( d >= 0 )
    duf_pi_clear_d( duf_pdi_pathinfo_p( pdi ) /* &pdi->pathinfo */ , d );
}

int
duf_levinfo_calc_depth( duf_depthinfo_t * pdi )
{
  return duf_pi_calc_depth( duf_pdi_pathinfo_p( pdi ) /* &pdi->pathinfo */  );
}
