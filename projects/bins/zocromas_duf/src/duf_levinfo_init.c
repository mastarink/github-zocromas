/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>

#include "duf_pathinfo_credel.h"                                     /* duf_pi_shut; duf_pi_copy; duf_pi_levinfo_create; duf_pi_levinfo_delete etc. ✗ */

#include "duf_levinfo.h"                                             /* duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc. ✗ */

#include "duf_pdi_structs.h"

/* ###################################################################### */
#include "duf_levinfo_init.h"                                        /* duf_levinfo_init_level; duf_levinfo_init_level_d; etc. ✗ */
/* ###################################################################### */

/* 20150831.000000 */
void
duf_levinfo_init_level_d( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type, int d )
{
  assert( pdi );
  assert( d >= 0 );
  assert( pdi->pathinfo.levinfo );
  duf_pi_init_level_d( &pdi->pathinfo, itemname, dirid, node_type, d );
}

/* 20150901.173353 */
/* resets levinfo  (currenl level) */
void
duf_levinfo_init_level( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type )
{
  duf_levinfo_init_level_d( pdi, itemname, dirid, node_type, pdi->pathinfo.depth );

  assert( pdi->pathinfo.depth == duf_levinfo_calc_depth( pdi ) );
}
