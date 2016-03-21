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

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ✗ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ✗ *) */
/* #include "duf_dodefs.h"                                              (* DOR ✗ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_pi_ref.h"
#include "duf_pdi_ref.h"

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_credel.h"                                      /* duf_levinfo_create; duf_levinfo_delete ✗ */

/* #include "duf_sql_stmt_defs.h"                                       (* DUF_SQL_BIND_S_OPT etc. ✗ *) */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */
#include "duf_ufilter_bind.h"

#include "duf_evsql_selector_new.h"

#include "duf_sccb.h"
#include "duf_sccbh_shortcuts.h"
#include "duf_sccb_handle.h"
#include "duf_sccb_scanstage.h"

#include "duf_sel_cb_leaf.h"
#include "duf_sel_cb_node.h"

#include "duf_pdi_credel.h"                                          /* duf_pdi_create; duf_pdi_kill ✗ */
#include "duf_levinfo_credel.h"                                      /* duf_levinfo_create; duf_levinfo_delete ✗ */
#include "duf_li_credel.h"

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_sql_positional.h"                                      /* duf_sql_column_long_long etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */

/* ###################################################################### */
#include "duf_sccbh_eval_sql_set.h"
/* ###################################################################### */

/* 20151014.093121 */
static
SR( SCCBH, eval_sccbh_sql_row_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2,
    duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */
  assert( ( node_type == DUF_NODE_NODE ) || ( node_type == DUF_NODE_LEAF ) );
  MAST_TRACE( sql, 3, "EACH %llu ... %s", duf_levinfo_dirid( H_PDI ), sqlite3_sql( pstmt ) );
  MAST_TRACE( sccbh, 0, "EACH %llu; %s(%d) @ %s @ %s @ %s", duf_levinfo_dirid( H_PDI ), duf_nodetype_name( node_type ), node_type,
              duf_levinfo_path( H_PDI ), DUF_GET_STMT_SFIELD2( pstmt, dfname ), duf_levinfo_itemtruename( H_PDI ) );

  H_PDI->seq_row++;
  duf_sel_cb2_t cbs[] = {
    [DUF_NODE_NODE] = duf_sel_cb2_node,                              /* str_cb2 is duf_sccbh_eval_all */
    [DUF_NODE_LEAF] = duf_sel_cb2_leaf,                              /* str_cb2 is duf_sccbh_eval_db_leaf_str_cb or duf_sccbh_eval_db_leaf_fd_str_cb */
  /* [DUF_NODE_MAX] = NULL, */
  };
  sccbh->assert__current_node_type = node_type;
  MAST_TRACE( sccbh, 2, "@@@str_cb2(%d) :%llu n/t:%s (%s) %s", str_cb2 ? 1 : 0, duf_levinfo_dirid( H_PDI ), duf_nodetype_name( node_type ),
              duf_uni_scan_action_title( H_SCCB ), H_SCCB->name );

#if 1

  struct duf_data_value_s
  {
    duf_sqltype_t typ;
    char *name;
    union
    {
      unsigned long long n;
    /* char *s; */
    } value;
    char *svalue;
  };
  typedef struct duf_data_value_s duf_data_value_t;

/* struct duf_data_row_s {}; typedef struct duf_data_row_s duf_data_row_t; */
  duf_data_value_t *row MAS_UNUSED;
  size_t ccnt;

  ccnt = duf_sql_column_count( pstmt );
  row = mas_malloc( ccnt * sizeof( duf_data_value_t ) );
  memset( row, 0, ccnt * sizeof( duf_data_value_t ) );
  for ( size_t i = 0; i < ccnt; i++ )                                /* sqlite3_column_count( pstmt ) */
  {
    row[i].typ = duf_sql_column_type( pstmt, i );
    row[i].name = mas_strdup( duf_sql_column_name( pstmt, i ) );
    row[i].svalue = mas_strdup( duf_sql_column_string( pstmt, i ) );
    switch ( row[i].typ )
    {
    case DUF_SQLTYPE_NONE:
      break;
    case DUF_SQLTYPE_INTEGER:
      row[i].value.n = duf_sql_column_long_long( pstmt, i );
      break;
    case DUF_SQLTYPE_FLOAT:
      break;
    case DUF_SQLTYPE_TEXT:
    /* row[i].value.n = duf_sql_column_long_long( pstmt, i ); */
      break;
    case DUF_SQLTYPE_BLOB:
      break;
    case DUF_SQLTYPE_NULL:
      break;
    }
    {
# if 0
      const char *s;
      const char *n;
      const char *st;
      duf_sqltype_t it;

      n = duf_sql_column_name( pstmt, i );                           /* sqlite3_column_name */
      s = duf_sql_column_string( pstmt, i );                         /* sqlite3_column_text( pstmt, i ) */
      st = duf_sql_column_decltype( pstmt, i );
      it = duf_sql_column_type( pstmt, i );
      if ( s )
        QT( "@%lu. %s/%d %s='%s'", i, st, it, n, s );
      else
        QT( "@%lu. %s/%d %s=NULL", i, st, it, n );
# endif
    }
    mas_free( row[i].svalue );
    mas_free( row[i].name );
  }
  mas_free( row );
#endif

/* IF_DORF( r, cbs[node_type], sccbh, pstmt, str_cb2, scanstage ); */
  IF_CRV( cbs[node_type], sccbh, pstmt, str_cb2, scanstage );

  CR( pdi_max_filter, H_PDI );                                       /* check if any of max's reached */
/* DUF_ENDR( r ); */
  ER( SCCBH, eval_sccbh_sql_row_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2,
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
/* DUF_STARTR( r ); */
/* TODO Can't ‘DUF_SQL_START_STMT’ due to recursion : same id : &main_sql_selector_index (static in this case is bad!) TODO */
#if 1
  DUF_SQL_SE_START_STMT_NOPDI( sql_selector, pstmt );
#else
  DUF_SQL_SE_START_STMT_LOCAL( H_PDI, sql_selector, pstmt );
#endif

/* TODO : sccbh->pstmt = pstmt OR via pdi */
  MAST_TRACE( select, 1, "S:%s", sql_selector );
/* XXX With parent ! XXX */
  DUF_SQL_SE_BIND_LL_NZ_OPT( parentdirID, duf_levinfo_dirid( H_PDI ), pstmt );

/* DUF_SQL_SE_BIND_LL_NZ_OPT( topDirID, duf_levinfo_dirid_d( H_PDI, duf_pdi_topdepth( H_PDI ) ), pstmt ); */
/* duf_yfilter_t yf={.topdirid= duf_levinfo_dirid_d( H_PDI, duf_pdi_topdepth( H_PDI ) )}; */
  CR( bind_ufilter_uni, pstmt, H_PU, H_PY, NULL, NULL /* ptr */  );

/* cal one of duf_sel_cb2_(leaf|node) by node_type
 * i.e. DOR( r, (( node_type == DUF_NODE_NODE ) ? duf_sel_cb2_node : ( node_type == DUF_NODE_LEAF ? duf_sel_cb2_leaf : NULL ) ) ( pstmt, str_cb2, sccbh ) )
 * */

  MAST_TRACE( sccbh, 2, "@@@@@scan rows dirid:%llu (%s) %d:%llu", duf_levinfo_dirid( H_PDI ), duf_uni_scan_action_title( H_SCCB ), H_TOTCOUNTED,
              H_TOTITEMS );
  MAST_TRACE( sql, 0, "EACH ... id=%llu (%llu:%llu:%llu) of %llu -- %s", duf_levinfo_dirid( H_PDI ), H_PDI->seq, H_PDI->seq_node, H_PDI->seq_leaf,
              H_TOTITEMS, sqlite3_sql( pstmt ) );
/* assert( !H_TOTCOUNTED || H_TOTITEMS ); */
  if ( !H_TOTCOUNTED || H_TOTITEMS )
  {
    H_PDI->seq_row = 0;
  /* H_PDI->total_bytes = 0; */
  /* T( "@pdi->total_bytes:%llu", H_PDI->total_bytes ); */

    DUF_SQL_SE_EACH_ROW( pstmt, CR( eval_sccbh_sql_row_str_cb, sccbh, node_type, pstmt, str_cb2, scanstage ) );

  /* mas_force_count_ereport( 1 ); */
  /* DUF_TEST_R( r ); */
  }
  else
  {
    QT( "SKIP: H_TOTCOUNTED:%d; H_TOTITEMS:%llu [%s] for %s", H_TOTCOUNTED, H_TOTITEMS, duf_scanstage_name( scanstage ),
        duf_uni_scan_action_title( H_SCCB ) );
  /* assert( 0 );                                                                                               */
  }
#if 1
  DUF_SQL_SE_END_STMT_NOPDI( pstmt );
#else
  DUF_SQL_SE_END_STMT_LOCAL( H_PDI, pstmt );
#endif
/* DUF_ENDR( r ); */
  ER( SCCBH, eval_sccbh_sql_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, const char *sql_selector, duf_str_cb2_t str_cb2,
      duf_scanstage_t scanstage );
}

/* 20160212.130857 */
SR( SCCBH, eval_sccbh_sql_set_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_sql_set_pair_t sql_set_pair, duf_str_cb2_t str_cb2,
    duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */
  char *sql_selector = NULL;

  if ( QNOERR )
    sql_selector = duf_selector2sql_2new( sql_set_pair.active, sql_set_pair.second, sql_set_pair.orderid, H_PDI->pdi_name, 0, QPERRIND );

  MAST_TRACE( scan, 14, "sql:%s", sql_selector );
  {
#ifdef MAS_TRACING
    const char *set_type_title = duf_nodetype_name( node_type );
#endif
    MAST_TRACE( scan, 10, "[%s] (slctr2) #%llu: \"%s\"", set_type_title, duf_levinfo_dirid( H_PDI ), duf_levinfo_itemshowname( H_PDI ) );
  }
/*
 * str_cb2 (sub-item scanner ): (~~content-scanner!?)
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_sccbh_eval_db_leaf_str_cb    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
  CR( eval_sccbh_sql_str_cb, sccbh, node_type, sql_selector, str_cb2, scanstage );
  mas_free( sql_selector );
  sql_selector = NULL;
/* DUF_ENDR( r ); */
  ER( SCCBH, eval_sccbh_sql_set_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_sql_set_pair_t sql_set_pair, duf_str_cb2_t str_cb2,
      duf_scanstage_t scanstage );
}
