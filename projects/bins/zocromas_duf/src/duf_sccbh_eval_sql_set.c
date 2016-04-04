/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_structs.h"

#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_ufilter_bind.h"                                        /* duf_bind_ufilter_uni ✗ */

#include "duf_evsql_selector_new.h"                                  /* duf_selector2sql_new; duf_selector2sql_2new; duf_expand_sql; ✗ */

#include "duf_sccb.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */
#include "duf_sccb_structs.h"

#include "duf_sccbh_eval_all.h"                                      /* duf_sccbh_eval_all ✗ */
#include "duf_sccbh_eval_leaf.h"                                     /* duf_sccbh_eval_db_leaf_str_cb; duf_sccbh_eval_db_leaf_fd_str_cb; ✗ */
#include "duf_sccbh_eval.h"

#include "duf_sel_cb_leaf.h"
#include "duf_sel_cb_node.h"

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

/* #include "duf_sql_defs.h"                                            (* DUF_SQL_IDFIELD etc. ✗ *) */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_QUFIELD2 etc. ✗ */

#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */

#include "duf_nodetype.h"                                            /* duf_nodetype_name ✗ */

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_sccbh_eval_sql_set.h"
/* ###################################################################### */

/* 20151014.093121 */
static
SR( SCCBH, eval_sccbh_sql_row_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_stmnt_t * pstmt_arg, duf_str_cb2_t str_cb2,
    duf_scanstage_t scanstage )
{
  assert( ( node_type == DUF_NODE_NODE ) || ( node_type == DUF_NODE_LEAF ) );
  assert( H_PDI->pathinfo.levinfo[H_PDI->pathinfo.maxdepth + 1].d == 0 );

  MAST_TRACE( sql, 3, "EACH %llu ... %s", CRX( levinfo_dirid, H_PDI ), sqlite3_sql( pstmt_arg ) );
  MAST_TRACE( sccbh, 0, "EACH %llu; %s(%d) @ %s @ %s @ %s", CRX( levinfo_dirid, H_PDI ), CRX( nodetype_name, node_type ), node_type,
              CRX( levinfo_path, H_PDI ), DUF_GET_STMT_SFIELD2( pstmt_arg, dfname ), CRX( levinfo_itemtruename, H_PDI ) );

  H_PDI->seq_row++;
  sccbh->assert__current_node_type = node_type;
  MAST_TRACE( sccbh, 2, "@@@str_cb2(%d) :%llu n/t:%s (%s) %s", str_cb2 ? 1 : 0, CRX( levinfo_dirid, H_PDI ), CRX( nodetype_name, node_type ),
              CRX( uni_scan_action_title, H_SCCB ), H_SCCB->name );
#if 0
  duf_sel_cb2_t cbs[] = {
    [DUF_NODE_NODE] = duf_sel_cb2_node,                              /* str_cb2 is duf_sccbh_eval_all */
    [DUF_NODE_LEAF] = duf_sel_cb2_leaf,                              /* str_cb2 is duf_sccbh_eval_db_leaf_str_cb or duf_sccbh_eval_db_leaf_fd_str_cb */
  /* [DUF_NODE_MAX] = NULL, */
  };

/* IF_DORF( r, cbs[node_type], sccbh, pstmt_arg, str_cb2, scanstage ); */
  IF_CRV( cbs[node_type], sccbh, pstmt_arg, str_cb2, scanstage );
#else
  switch ( node_type )
  {
  case DUF_NODE_LEAF:
    CR( sel_cb2_leaf, sccbh, pstmt_arg, str_cb2, scanstage );
    break;
  case DUF_NODE_NODE:
    CR( sel_cb2_node, sccbh, pstmt_arg, str_cb2, scanstage );
    break;
  /* case DUF_NODE_NONE:    */
  /* case DUF_NODE_FS:      */
  /* case DUF_NODE_FS_FILE: */
  /* case DUF_NODE_FS_DIR:  */
  default:
    break;
  }
#endif
  CR( pdi_max_filter, H_PDI );                                       /* check if any of max's reached */
  ER( SCCBH, eval_sccbh_sql_row_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_stmnt_t * pstmt_arg, duf_str_cb2_t str_cb2,
      duf_scanstage_t scanstage );
}

/*
 * 20151013.125945 
 * 1. bind ufilter
 * 2. duf_eval_sccbh_sql_row_str_cb for each row from db by ufilter
 * */
static
SR( SCCBH, eval_sccbh_sql_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, const char *sql_selector, duf_str_cb2_t str_cb2,
    duf_scanstage_t scanstage )
{
  /* int have_rows=0; */
/* TODO Can't ‘DUF_SQL_START_STMT’ due to recursion : same id : &main_sql_selector_index (static in this case is bad!) TODO */
#if 1
  DUF_SQL_SE_START_STMT_NOPDI( sql_selector, pstmt_local );
#else
  DUF_SQL_SE_START_STMT_LOCAL( H_PDI, sql_selector, pstmt_local );
#endif

  MAST_TRACE( select, 1, "S:%s", sql_selector );
/* XXX With parent ! XXX */
  DUF_SQL_SE_BIND_LL_NZ_OPT( parentdirID, CRX( levinfo_dirid, H_PDI ), pstmt_local );

/* DUF_SQL_SE_BIND_LL_NZ_OPT( topDirID, CRX(levinfo_dirid_d, H_PDI, CRX(pdi_topdepth, H_PDI ) ), pstmt_local ); */
/* duf_yfilter_t yf={.topdirid= CRX(levinfo_dirid_d, H_PDI, CRX(pdi_topdepth, H_PDI ) )}; */
  CR( bind_ufilter_uni, pstmt_local, H_PU, H_PY, NULL, NULL /* ptr */  );

/* cal one of duf_sel_cb2_(leaf|node) by node_type
 * i.e. DOR( r, (( node_type == DUF_NODE_NODE ) ? duf_sel_cb2_node : ( node_type == DUF_NODE_LEAF ? duf_sel_cb2_leaf : NULL ) ) ( pstmt_local, str_cb2, sccbh ) )
 * */

  MAST_TRACE( sccbh, 2, "@@@@@scan rows dirid:%llu (%s) %d:%llu", CRX( levinfo_dirid, H_PDI ), CRX( uni_scan_action_title, H_SCCB ), H_TOTCOUNTED,
              H_TOTITEMS );
  MAST_TRACE( sql, 0, "EACH ... id=%llu (%llu:%llu:%llu) of %llu -- %s", CRX( levinfo_dirid, H_PDI ), H_PDI->seq, H_PDI->seq_node, H_PDI->seq_leaf,
              H_TOTITEMS, sqlite3_sql( pstmt_local ) );
/* assert( !H_TOTCOUNTED || H_TOTITEMS ); */
  if ( !H_TOTCOUNTED || H_TOTITEMS )
  {
    H_PDI->seq_row = 0;
  /* H_PDI->total_bytes = 0; */
  /* T( "@pdi->total_bytes:%llu", H_PDI->total_bytes ); */

    DUF_SQL_SE_EACH_ROW( pstmt_local, CR( eval_sccbh_sql_row_str_cb, sccbh, node_type, pstmt_local, str_cb2, scanstage ) );
    /* have_rows=1; */

    /* QT( "@%s", H_SCCB->name ); */
    assert( str_cb2 == duf_sccbh_eval_db_leaf_str_cb || str_cb2 == duf_sccbh_eval_db_leaf_fd_str_cb || str_cb2 == duf_sccbh_eval_all );
  /* mas_force_count_ereport( 1 ); */
  /* DUF_TEST_R( r ); */
  }
  else
  {
    QT( "SKIP: H_TOTCOUNTED:%d; H_TOTITEMS:%llu [%s] for %s", H_TOTCOUNTED, H_TOTITEMS, CRX( scanstage_name, scanstage ),
        CRX( uni_scan_action_title, H_SCCB ) );
  /* assert( 0 );                                                                                               */
  }
#if 1
  DUF_SQL_SE_END_STMT_NOPDI( pstmt_local );
#else
  DUF_SQL_SE_END_STMT_LOCAL( H_PDI, pstmt_local );
#endif
/* QT( "@@xxx : %s", QERRNAME ); */

  
  /* CRX( sccbh_row_next, sccbh, NULL );                     */
  /* if ( have_rows && node_type == DUF_NODE_LEAF )          */
  /*   CR( sccbh_call_leaf_pack_scanner, sccbh, scanstage ); */

  ER( SCCBH, eval_sccbh_sql_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, const char *sql_selector, duf_str_cb2_t str_cb2,
      duf_scanstage_t scanstage );
}

/* 20160212.130857 */
SR( SCCBH, eval_sccbh_sql_set_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_sql_set_pair_t sql_set_pair, duf_str_cb2_t str_cb2,
    duf_scanstage_t scanstage )
{
  char *sql_selector = NULL;

  if ( QNOERR )
    sql_selector = duf_selector2sql_2new( sql_set_pair.active, sql_set_pair.second, sql_set_pair.orderid, H_PDI->pdi_name, 0, QPERRIND );

  MAST_TRACE( scan, 14, "sql:%s", sql_selector );
  {
#ifdef MAS_TRACING
    const char *set_type_title = CRX( nodetype_name, node_type );
#endif
    MAST_TRACE( scan, 10, "[%s] (slctr2) #%llu: \"%s\"", set_type_title, CRX( levinfo_dirid, H_PDI ), CRX( levinfo_itemshowname, H_PDI ) );
  }

  assert( str_cb2 == duf_sccbh_eval_all || ( str_cb2 == duf_sccbh_eval_db_leaf_fd_str_cb ) || ( str_cb2 == duf_sccbh_eval_db_leaf_str_cb ) );
  CR( eval_sccbh_sql_str_cb, sccbh, node_type, sql_selector, str_cb2, scanstage );
  mas_free( sql_selector );
  sql_selector = NULL;
  ER( SCCBH, eval_sccbh_sql_set_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_sql_set_pair_t sql_set_pair, duf_str_cb2_t str_cb2,
      duf_scanstage_t scanstage );
}
