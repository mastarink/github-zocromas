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

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ✗ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ✗ *) */
/* #include "duf_dodefs.h"                                              (* DOR ✗ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* #include "duf_debug_defs.h"                                          (* DUF_WRAPSTATIC; DUF_WRAPPED ...  ✗ *) */

#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_ref.h"                                          (* needed for DUF_PRINTF *) */
#include "duf_config_output_util.h"

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "duf_sccb_scanstage.h"

#include "duf_sccb_handle.h"
#include "duf_sccbh_eval_leaf.h"

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_sccbh_eval_all.h"
#include "duf_sccbh_eval_sql_set.h"
/* #include "duf_sccbh_eval_fs.h" */
#include "duf_sccbh_shortcuts.h"

#include "duf_sccbh_eval.h"
/* ###################################################################### */
#include "duf_sccbh_eval_node.h"
/* ###################################################################### */

/* 20151027.144606 */
SR( SCCBH, sccbh_eval_db_node, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */

  MAST_TRACE( scan, 4, "? scan node [%s] by %5llu", duf_scanstage_name( scanstage ), duf_levinfo_dirid( H_PDI ) );
/* if ( DUF_ACTG_FLAG( allow_dirs ) ) */
  {
    duf_scanner_t scanner = NULL;

#if 0
    H_PDI->items.total++;
    H_PDI->items.dirs++;
#endif
    /* QT( "@@%d. %llu", scanstage, H_PDI->items.total ); */

    MAST_TRACE( scan, 4, "? (dirs+) scan node [%s] by %5llu", duf_scanstage_name( scanstage ), duf_levinfo_dirid( H_PDI ) );
#if 1
    if ( ( scanner = duf_scanstage_scanner( H_SCCB, scanstage, 1 /* deleted */ , DUF_NODE_NODE ) ) && duf_levinfo_if_deleted( H_PDI ) )
    {
      MAST_TRACE( scan, 4, "scan node %s_deleted by %5llu", duf_scanstage_name( scanstage ), duf_levinfo_dirid( H_PDI ) );
# if 0
      sccbh->current_scanner = scanner;
      sccbh->current_node_type = DUF_NODE_NODE;
      CRV( ( scanner ), pstmt, H_PDI );
      assert( sccbh->current_node_type == DUF_NODE_NODE );
      if ( sccbh->atom_cb )                                          /* atom is fs-direntry(dir or reg) or item(node or leaf) */
        sccbh->atom_cb( sccbh, pstmt, scanstage, scanner, DUF_NODE_NODE, QERRIND );
# else
      CR( sccbh_call_scanner, sccbh, pstmt,  scanstage, scanner, DUF_NODE_NODE );
# endif
    }
    else if ( ( scanner = duf_scanstage_scanner( H_SCCB, scanstage, 0 /* deleted */ , DUF_NODE_NODE ) ) )
    {
      MAST_TRACE( scan, 4, "scan node %s by %5llu", duf_scanstage_name( scanstage ), duf_levinfo_dirid( H_PDI ) );
# if 0
      sccbh->current_scanner = scanner;
      sccbh->current_node_type = DUF_NODE_NODE;
      CRV( ( scanner ), pstmt, H_PDI );
      assert( sccbh->current_node_type == DUF_NODE_NODE );
      if ( sccbh->atom_cb )                                          /* atom is fs-direntry(dir or reg) or item(node or leaf) */
        sccbh->atom_cb( sccbh, pstmt, scanstage, scanner, DUF_NODE_NODE, QERRIND );
# else
      CR( sccbh_call_scanner, sccbh, pstmt,  scanstage, scanner, DUF_NODE_NODE );
# endif
    }
#else
    if ( ( scanner = duf_scanstage_scanner( H_SCCB, scanstage, duf_levinfo_if_deleted( H_PDI ), DUF_NODE_NODE ) ) )
    {
      sccbh->current_scanner = scanner;
      MAST_TRACE( scan, 4, "scan node %s_deleted by %5llu", duf_scanstage_name( scanstage ), duf_levinfo_dirid( H_PDI ) );
      DOR( r, ( scanner ) ( pstmt, H_PDI ) );
    }
#endif
    else
    {
      MAST_TRACE( scan, 4, "NOT scan node %s by %5llu - sccb->node_scan_%s empty for %s", duf_scanstage_name( scanstage ),
                  duf_levinfo_dirid( H_PDI ), duf_scanstage_name( scanstage ), duf_uni_scan_action_title( H_SCCB ) );
    }
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_db_node, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage );
}

/* 20150820.085615 */
/* TODO duf_sccb_handle_t * sccbh must be first in all such funcs 20160210.120016 */
SR( SCCBH, sccbh_eval_db_subnodes, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt MAS_UNUSED, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */

  assert( sccbh );
  assert( H_SCCB );

#ifdef DUF_NO_RNUMS
  MAST_TRACE( sql, 0, "@@@EACH SUB %llu ... %s", duf_levinfo_dirid( H_PDI ), sqlite3_sql( pstmt ) );
#else
  MAST_TRACE( sql, 0, "@@@EACH SUB %llu {%llu}... %s", duf_levinfo_dirid( H_PDI ), pstmt ? DUF_GET_UFIELD2( rnfiles ) : 0, sqlite3_sql( pstmt ) );
#endif
  MAST_TRACE( scan, 4, "scan dirent by %5llu:%s; %s", duf_levinfo_dirid( H_PDI ), duf_uni_scan_action_title( H_SCCB ), duf_levinfo_path( H_PDI ) );

  CR( sccbh_eval_db_items_str_cb, sccbh, DUF_NODE_NODE, duf_sccbh_eval_all, scanstage );
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_db_subnodes, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt MAS_UNUSED, duf_scanstage_t scanstage );
}
