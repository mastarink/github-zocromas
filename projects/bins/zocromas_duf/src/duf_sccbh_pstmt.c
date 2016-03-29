/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_updown.h"

#include "duf_sccbh_shortcuts.h"
#include "duf_sccb_structs.h"

#include "duf_pdi_structs.h"
#include "duf_pathinfo_structs.h"

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

/* ###################################################################### */
#include "duf_sccbh_pstmt.h"
/* ###################################################################### */

/* 20150831.202009 */
SR( SCCBH, sccbh_pstmt_godown_dbopenat_dh, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_node_type_t node_type )
{
/* DUF_STARTR( r ); */
  assert( H_PDI->pathinfo.levinfo[H_PDI->pathinfo.maxdepth + 1].d == 0 );

/* Not here : assert( DUF_GET_UFIELD2( dirid) == duf_levinfo_dirid( pdi ) ); */
  MAST_TRACE( scan, 10, "before duf_levinfo_godown() : dirID:%llu", DUF_GET_UFIELD2( dirid ) );
  MAST_TRACE( explain, 20, "@ sel cb2 node" );

  CR( levinfo_godown_dbopenat_dh, H_PDI, node_type, pstmt );

  assert( QISERR || DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( H_PDI ) ); /* was set by duf_levinfo_godown */
/* QT( "@@%s", sqlite3_sql( pstmt ) ); */
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_pstmt_godown_dbopenat_dh, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_node_type_t node_type );
}
