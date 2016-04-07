/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_output_util.h"

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */

#include "duf_sccb_handle.h"
#include "duf_sccb_structs.h"

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_QUFIELD2 etc. ✗ */

#include "duf_sccbh_eval_all.h"                                      /* duf_sccbh_eval_all ✗ */
#include "duf_sccbh_eval_sql_set.h"
/* #include "duf_sccbh_eval_fs.h" */
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_eval_leaf.h"                                     /* duf_sccbh_eval_db_leaf_str_cb; duf_sccbh_eval_db_leaf_fd_str_cb; ✗ */
#include "duf_sccbh_scanner.h"

#include "duf_sccbh_eval.h"
/* ###################################################################### */
#include "duf_sccbh_eval_node.h"
/* ###################################################################### */

/* 20151027.144606 */
SR( SCCBH, sccbh_eval_db_node, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage )
{
  MAST_TRACE( scan, 4, "? scan node [%s] by %5llu", CRX( scanstage_name, scanstage ), CRX( levinfo_dirid, H_PDI ) );
/* if ( DUF_ACTG_FLAG( allow_dirs ) ) */
  {
    duf_scanner_fun_t scanner = NULL;

#if 0
    H_PDI->items.total++;
    H_PDI->items.dirs++;
#endif

    MAST_TRACE( scan, 4, "? (dirs+) scan node [%s] by %5llu", CRX( scanstage_name, scanstage ), CRX( levinfo_dirid, H_PDI ) );
    scanner = CRX( sccb_scanstage_scanner, H_SCCB, scanstage, 0 /* deleted */ , DUF_NODE_NODE );
    if ( scanner )
    {
      MAST_TRACE( scan, 4, "scan node %s by %5llu", CRX( scanstage_name, scanstage ), CRX( levinfo_dirid, H_PDI ) );
      CR( sccbh_call_scanner, sccbh, pstmt_arg, scanstage, scanner, DUF_NODE_NODE );
    }
    else
    {
      MAST_TRACE( scan, 4, "NOT scan node %s by %5llu - sccb->node_scan_%s empty for %s", CRX( scanstage_name, scanstage ),
                  CRX( levinfo_dirid, H_PDI ), CRX( scanstage_name, scanstage ), CRX( uni_scan_action_title, H_SCCB ) );
    }
  }
  ER( SCCBH, sccbh_eval_db_node, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage );
}

SR( SCCBH, sccbh_eval_db_node_scanner_set, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage )
{
  int n = 0;

  for ( const duf_scanner_set_t * scanner_set = H_SCCB->scanners; scanner_set && scanner_set->fun; scanner_set++ )
  {
    if ( ( scanner_set->flags & DUF_SCANNER_SET_FLAG_DB )
         && !( scanner_set->flags & DUF_SCANNER_SET_FLAG_DISABLED )
         && ( ( scanner_set->scanstage & scanstage ) || scanner_set->scanstage == DUF_SCANSTAGE_NONE )
         && ( ( scanner_set->type & DUF_NODE_NODE ) || scanner_set->type == DUF_NODE_NONE ) )
    {
      CR( sccbh_call_scanner, sccbh, pstmt_arg, scanstage, scanner_set->fun, DUF_NODE_NODE );
    }
    n++;
  }

  ER( SCCBH, sccbh_eval_db_node_scanner_set, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage );
}

SR( SCCBH, sccbh_eval_db_node_new, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage )
{
  CR( sccbh_eval_db_node, sccbh, pstmt_arg, scanstage );
  CR( sccbh_eval_db_node_scanner_set, sccbh, pstmt_arg, scanstage );
  ER( SCCBH, sccbh_eval_db_node_new, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage );
}

/* 20150820.085615 */
/* TODO duf_sccb_handle_t * sccbh must be first in all such funcs 20160210.120016 */
SR( SCCBH, sccbh_eval_db_subnodes, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg MAS_UNUSED, duf_scanstage_t scanstage )
{

  assert( sccbh );
  assert( H_SCCB );

  MAST_TRACE( sql, 0, "@@@EACH SUB %llu ... %s", CRX( levinfo_dirid, H_PDI ), sqlite3_sql( pstmt_arg ) );
  MAST_TRACE( scan, 4, "scan dirent by %5llu:%s; %s", CRX( levinfo_dirid, H_PDI ), CRX( uni_scan_action_title, H_SCCB ), CRX( levinfo_path, H_PDI ) );

  CR( sccbh_eval_db_items_str_cb, sccbh, DUF_NODE_NODE, F2ND( sccbh_eval_all ), scanstage );
  ER( SCCBH, sccbh_eval_db_subnodes, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg MAS_UNUSED, duf_scanstage_t scanstage );
}
