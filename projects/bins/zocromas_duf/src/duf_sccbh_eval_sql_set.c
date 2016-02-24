#undef MAS_TRACING
#include <string.h>
#include <unistd.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config.h"

#include "duf_pdi_filters.h"
#include "duf_pdi_pi_ref.h"
#include "duf_pdi_ref.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_credel.h"

#include "duf_sql_stmt_defs.h"
#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"
#include "duf_ufilter_bind.h"

/* #include "evsql_selector.h" */
#include "evsql_selector_new.h"

#include "duf_sccb.h"
#include "duf_sccbh_shortcuts.h"
#include "duf_sccb_handle.h"
#include "duf_sccb_scanstage.h"

#include "duf_sel_cb_leaf.h"
#include "duf_sel_cb_node.h"



#include "duf_pdi_credel.h"
#include "duf_levinfo_credel.h"
#include "duf_li_credel.h"


#include "duf_sql_defs.h"
#include "duf_sql_field.h"

/* ###################################################################### */
#include "duf_sccbh_eval_sql_set.h"
/* ###################################################################### */

/* 20151014.093121 */
static int
duf_eval_sccbh_sql_row_str_cb( duf_scanstage_t scanstage, duf_node_type_t node_type, duf_stmnt_t * pstmt_selector, duf_str_cb2_t str_cb2,
                               duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );
  assert( ( node_type == DUF_NODE_NODE ) || ( node_type == DUF_NODE_LEAF ) );
  DUF_TRACE( sql, 3, "EACH %llu ... %s", duf_levinfo_dirid( PDI ), sqlite3_sql( pstmt_selector ) );
  DUF_TRACE( sccbh, 0, "EACH %llu; %s(%d) @ %s @ %s @ %s", duf_levinfo_dirid( PDI ), duf_nodetype_name( node_type ), node_type,
             duf_levinfo_path( PDI ), DUF_GET_STMT_SFIELD2( pstmt_selector, dfname ), duf_levinfo_itemtruename( PDI ) );

  PDI->seq_row++;
  duf_sel_cb2_t cbs[] = {
    [DUF_NODE_NODE] = duf_sel_cb2_node, /* str_cb2 is duf_eval_sccbh_all */
    [DUF_NODE_LEAF] = duf_sel_cb2_leaf, /* str_cb2 is duf_eval_sccbh_db_leaf_str_cb or duf_eval_sccbh_db_leaf_fd_str_cb */
    /* [DUF_NODE_MAX] = NULL, */
  };
  sccbh->current_node_type = node_type;
  DUF_TRACE( sccbh, 2, "@@@str_cb2(%d) :%llu n/t:%s (%s) %s", str_cb2 ? 1 : 0, duf_levinfo_dirid( PDI ), duf_nodetype_name( node_type ),
             duf_uni_scan_action_title( SCCB ), SCCB->name );
  IF_DORF( r, cbs[node_type], scanstage, pstmt_selector, str_cb2, sccbh );


  DOR( r, duf_pdi_max_filter( PDI ) ); /* check if any of max's reached */
  DUF_ENDR( r );
}

/*
 * 20151013.125945 
 * 1. bind ufilter
 * 2. duf_eval_sccbh_sql_row_str_cb for each row from db by ufilter
 * */
static int
duf_eval_sccbh_sql_str_cb( duf_scanstage_t scanstage, duf_node_type_t node_type, const char *sql_selector, duf_str_cb2_t str_cb2,
                           duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );
/* TODO Can't ‘DUF_SQL_START_STMT’ due to recursion : same id : &main_sql_selector_index (static in this case is bad!) TODO */
#if 1
  DUF_SQL_START_STMT_NOPDI( sql_selector, r, pstmt_selector );
#else
  DUF_SQL_START_STMT_LOCAL( PDI, sql_selector, r, pstmt_selector );
#endif

/* TODO : sccbh->pstmt_selector = pstmt_selector OR via pdi */
  DUF_TRACE( select, 1, "S:%s", sql_selector );
/* XXX With parent ! XXX */
  DUF_SQL_BIND_LL_NZ_OPT( parentdirID, duf_levinfo_dirid( PDI ), r, pstmt_selector );

  /* DUF_SQL_BIND_LL_NZ_OPT( topDirID, duf_levinfo_dirid_d( PDI, duf_pdi_topdepth( PDI ) ), r, pstmt_selector ); */
  /* duf_yfilter_t yf={.topdirid= duf_levinfo_dirid_d( PDI, duf_pdi_topdepth( PDI ) )}; */
  DOR( r, duf_bind_ufilter_uni( pstmt_selector, PU, PY, NULL, NULL /* ptr */ ) );

  /* cal one of duf_sel_cb2_(leaf|node) by node_type
   * i.e. DOR( r, (( node_type == DUF_NODE_NODE ) ? duf_sel_cb2_node : ( node_type == DUF_NODE_LEAF ? duf_sel_cb2_leaf : NULL ) ) ( pstmt_selector, str_cb2, sccbh ) )
   * */

  DUF_TRACE( sccbh, 2, "@@@@@scan rows dirid:%llu (%s) %d:%llu", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), TOTCOUNTED, TOTITEMS );
  DUF_TRACE( sql, 0, "EACH ... id=%llu (%llu:%llu:%llu) of %llu -- %s", duf_levinfo_dirid( PDI ), PDI->seq, PDI->seq_node, PDI->seq_leaf, TOTITEMS,
             sqlite3_sql( pstmt_selector ) );
  /* assert( !TOTCOUNTED || TOTITEMS ); */
  if ( !TOTCOUNTED || TOTITEMS )
  {
    PDI->seq_row = 0;
    /* PDI->total_bytes = 0; */
    /* T( "@pdi->total_bytes:%llu", PDI->total_bytes ); */

    DUF_SQL_EACH_ROW( r, pstmt_selector, DOR( r, duf_eval_sccbh_sql_row_str_cb( scanstage, node_type, pstmt_selector, str_cb2, sccbh ) ) );

    /* mas_force_count_ereport( 1 ); */
    DUF_TEST_R( r );
  }
  else
  {
    T( "SKIP: TOTCOUNTED:%d; TOTITEMS:%llu [%s] for %s", TOTCOUNTED, TOTITEMS, duf_scanstage_name( scanstage ), duf_uni_scan_action_title( SCCB ) );
    /* assert( 0 );                                                                                               */
  }
#if 1
  DUF_SQL_END_STMT_NOPDI( r, pstmt_selector );
#else
  DUF_SQL_END_STMT_LOCAL( PDI, r, pstmt_selector );
#endif
  DUF_ENDR( r );
}

/* 20160212.130857 */
int
duf_eval_sccbh_sql_set_str_cb( duf_scanstage_t scanstage, duf_node_type_t node_type, duf_sql_set_pair_t sql_set_pair, duf_str_cb2_t str_cb2,
                               duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );
  char *sql_selector = NULL;


  if ( DUF_NOERROR( r ) )
    sql_selector = duf_selector2sql_2new( sql_set_pair.active, sql_set_pair.second, PDI->pdi_name, 0, &r );

  DUF_TRACE( scan, 14, "sql:%s", sql_selector );
  {
#ifdef MAS_TRACING
    const char *set_type_title = duf_nodetype_name( node_type );
#endif
    DUF_TRACE( scan, 10, "[%s] (slctr2) #%llu: \"%s\"", set_type_title, duf_levinfo_dirid( PDI ), duf_levinfo_itemshowname( PDI ) );
  }
/*
 * str_cb2 (sub-item scanner ): (~~content-scanner!?)
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_eval_sccbh_db_leaf_str_cb    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
  DOR( r, duf_eval_sccbh_sql_str_cb( scanstage, node_type, sql_selector, str_cb2, sccbh ) );
  mas_free( sql_selector );
  sql_selector = NULL;
  DUF_ENDR( r );
}
