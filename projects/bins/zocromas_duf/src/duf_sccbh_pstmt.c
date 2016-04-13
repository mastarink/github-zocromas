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
#include "duf_levinfo_structs.h"

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccb_structs.h"

#include "duf_pdi_structs.h"
#include "duf_pdi_ref.h"
#include "duf_pathinfo_structs.h"

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
/* #include "duf_sccb_row.h"                                            (* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ *) */

#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_QUFIELD2 etc. ✗ */

/* ###################################################################### */
#include "duf_sccbh_pstmt.h"
/* ###################################################################### */

/* 20150831.202009 */
SR( SCCBH, sccbh_pstmt_godown_dbopenat_dh, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type )
{
  assert( H_PDI->pathinfo.levinfo[H_PDI->pathinfo.maxdepth + 1].d == 0 );
/* {                                                     */
/*   unsigned long long di1 = DUF_GET_QUFIELD3(pstmt_arg, dirid ); */
/*   unsigned long long di2 = DUF_GET_RUFIELD2( dirid ); */
/*                                                       */
/*   QT( "@%lld -- %lld", di1, di2 );                    */
/*   assert( 0 );                                        */
/* }                                                     */
/* Not here : assert( DUF_GET_QUFIELD3(pstmt_arg, dirid) == CRX( levinfo_dirid, pdi ) ); */
  MAST_TRACE( scan, 10, "before godown : dirID:%llu", DUF_GET_QUFIELD3( pstmt_arg, dirid ) );
  MAST_TRACE( explain, 20, "@ sel cb2 node" );

  assert( /* duf_pdi_linear( H_PDI ) || */ pstmt_arg == duf_pdi_each_stmt( H_PDI, 0 ) );
  CR( levinfo_godown_dbopenat_dh, H_PDI, node_type, pstmt_arg );
  assert( QISERR_N( TOO_DEEP ) || /* duf_pdi_linear( H_PDI ) || */ pstmt_arg == duf_pdi_each_stmt( H_PDI, 1 ) );

  assert( QISERR || DUF_GET_QUFIELD3( pstmt_arg, dirid ) == CRX( levinfo_dirid, H_PDI ) ); /* was set by duf_levinfo_godown */
/* QT( "@@%s", sqlite3_sql( pstmt_arg ) ); */
  ER( SCCBH, sccbh_pstmt_godown_dbopenat_dh, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_node_type_t node_type );
}
