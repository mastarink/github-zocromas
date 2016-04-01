/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>

#include "duf_pathinfo_credel.h"                                     /* duf_pi_shut; duf_pi_copy; duf_pi_levinfo_create; duf_pi_levinfo_delete etc. ✗ */

#include "duf_pdi_pi_ref.h"

/* ###################################################################### */
#include "duf_levinfo_dbinit.h"
/* ###################################################################### */

/* 20151029.114756 */
void
duf_levinfo_dbinit_level_d( duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type, int d )
{
  assert( pdi );
  duf_pi_dbinit_level_d( duf_pdi_pathinfo_p( pdi ) /* &pdi->pathinfo */ , pstmt_arg, node_type, d );
}
