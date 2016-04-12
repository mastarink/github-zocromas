/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pathinfo_credel.h"                                     /* duf_pi_shut; duf_pi_copy; duf_pi_levinfo_create; duf_pi_levinfo_delete etc. ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */

/* ###################################################################### */
#include "duf_levinfo_dbinit.h"
/* ###################################################################### */

/* 20151029.114756 */
SRN( PDI, void, levinfo_dbinit_level_d, duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type, int d )
{
  assert( pdi );
  assert(  /* duf_pdi_linear( pdi ) || */ pstmt_arg == duf_pdi_each_stmt( pdi, 1 ) );
  CRX( pi_dbinit_level_d, CRX( pdi_pathinfo_p, pdi ) /* &pdi->pathinfo */ , pstmt_arg, node_type, d );
  ERN( PDI, void, levinfo_dbinit_level_d, duf_depthinfo_t * pdi, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type, int d );
}
