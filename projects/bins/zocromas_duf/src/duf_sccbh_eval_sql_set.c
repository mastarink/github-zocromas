/* 20150819.134107 */
#include <string.h>
#include <unistd.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_pdi_ref.h"

#include "duf_levinfo_ref.h"

#include "duf_sql_stmt_defs.h"
#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"
#include "duf_ufilter_bind.h"
#include "evsql_selector.h"

#include "duf_sccb.h"
#include "duf_sccbh_shortcuts.h"

#include "duf_sel_cb_leaf.h"
#include "duf_sel_cb_node.h"

/* ###################################################################### */
#include "duf_sccbh_eval_sql_set.h"
/* ###################################################################### */

/* 20151013.125934 */
static int
duf_eval_sccbh_sql_row_str_cb( duf_scanstage_t scanstage, duf_node_type_t node_type, duf_stmnt_t * pstmt_selector, duf_str_cb2_t str_cb2,
                               duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  assert( ( node_type == DUF_NODE_NODE ) || ( node_type == DUF_NODE_LEAF ) );
#if 1
  {
    duf_sel_cb2_t cb = NULL;

    if ( node_type == DUF_NODE_NODE )
      cb = duf_sel_cb2_node;    /* str_cb2 is duf_eval_sccbh_all */
    else if ( node_type == DUF_NODE_LEAF )
      cb = duf_sel_cb2_leaf;    /* str_cb2 is duf_eval_sccbh_db_leaf_str_cb or duf_eval_sccbh_db_leaf_fd_str_cb */

    DUF_TRACE( sccbh, 2, "@@@cb(%d) str_cb2(%d) :%llu (%s) %s", cb ? 1 : 0, str_cb2 ? 1 : 0, duf_levinfo_dirid( PDI ),
               duf_uni_scan_action_title( SCCB ), SCCB->name );

    DUF_TRACE( sccbh, 2, "@has cb(%d) n/t:%d (%s) %s", cb ? 1 : 0, node_type, duf_uni_scan_action_title( SCCB ), SCCB->name );


    if ( cb )
      DOR( r, ( cb ) ( scanstage, pstmt_selector, str_cb2, sccbh ) );
    else
      DUF_MAKE_ERROR( r, DUF_ERROR_ARGUMENT );
  }
#elif 0
  switch ( node_type )
  {
  case DUF_NODE_NODE:
    DOR( r, duf_sel_cb2_node( pstmt_selector, str_cb2, sccbh ) ); /* str_cb2 == duf_eval_sccbh_all */
    break;
  case DUF_NODE_LEAF:
    DOR( r, duf_sel_cb2_leaf( pstmt_selector, str_cb2, sccbh ) );
    break;
  default:
    DUF_MAKE_ERROR( r, DUF_ERROR_ARGUMENT );
  }
#elif 0
  if ( node_type == DUF_NODE_NODE )
    DOR( r, duf_sel_cb2_node( pstmt_selector, str_cb2, sccbh ) ); /* str_cb2 == duf_eval_sccbh_all */
  else if ( node_type == DUF_NODE_LEAF )
    DOR( r, duf_sel_cb2_leaf( pstmt_selector, str_cb2, sccbh ) );
  else
    DUF_MAKE_ERROR( r, DUF_ERROR_ARGUMENT );
#else
  {
    duf_sel_cb2_t cb = NULL;

    cb = ( ( node_type == DUF_NODE_NODE ) ? duf_sel_cb2_node : ( node_type == DUF_NODE_LEAF ? duf_sel_cb2_leaf : NULL ) );
    if ( cb )
      ( cb ) ( pstmt_selector, str_cb2, sccbh );
  }
#endif

  /* ==> Moved to duf_sccb_eval_dirs.c: duf_eval_sccbh_all() <==
   * DUF_CLEAR_ERROR( r, DUF_ERROR_TOO_DEEP ); (* reset error if it was `MAX_DEPTH` *)
   * */
  DOR( r, duf_pdi_max_filter( PDI ) ); /* check if any of max's reached */
  DEBUG_ENDR( r );
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
  DEBUG_STARTR( r );

/* TODO Can't ‘DUF_SQL_START_STMT’ due to recursion : same id : &main_sql_selector_index (static in this case is bad!) TODO */
#if 1
  DUF_SQL_START_STMT_NOPDI( sql_selector, r, pstmt_selector );
#else
  DUF_SQL_START_STMT_LOCAL( PDI, sql_selector, r, pstmt_selector );
#endif

/* TODO : sccbh->pstmt_selector = pstmt_selector OR via pdi */
  DUF_TRACE( select, 1, "S:%s", sql_selector );
/* XXX With parent ! XXX */
  DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid( PDI ), r, pstmt_selector );

  /* DUF_SQL_BIND_LL_NZ_OPT( topDirID, duf_levinfo_dirid_d( PDI, duf_pdi_topdepth( PDI ) ), r, pstmt_selector ); */
  /* duf_yfilter_t yf={.topdirid= duf_levinfo_dirid_d( PDI, duf_pdi_topdepth( PDI ) )}; */
  DOR( r, duf_bind_ufilter_uni( pstmt_selector, PU, PY, NULL ) );

  /* cal one of duf_sel_cb2_(leaf|node) by node_type
   * i.e. DOR( r, (( node_type == DUF_NODE_NODE ) ? duf_sel_cb2_node : ( node_type == DUF_NODE_LEAF ? duf_sel_cb2_leaf : NULL ) ) ( pstmt_selector, str_cb2, sccbh ) )
   * */

  DUF_TRACE( sccbh, 2, "@@@@@scan rows dirid:%llu (%s) %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), SCCB->name );

  DUF_SQL_EACH_ROW( r, pstmt_selector, DOR( r, duf_eval_sccbh_sql_row_str_cb( scanstage, node_type, pstmt_selector, str_cb2, sccbh ) ) );
#if 1
  DUF_SQL_END_STMT_NOPDI( r, pstmt_selector );
#else
  DUF_SQL_END_STMT_LOCAL( PDI, r, pstmt_selector );
#endif
  DEBUG_ENDR( r );
}

/* 20151013.125952 */
int
duf_eval_sccbh_sql_set_str_cb( duf_scanstage_t scanstage, duf_node_type_t node_type, const duf_sql_set_t * sql_set, duf_str_cb2_t str_cb2,
                               duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  char *sql_selector = NULL;

  /* TODO sql_set is needless here, accessible via duf_get_sql_set( SCCB, node_type ) */
  assert( duf_sccb_get_sql_set( SCCB, node_type ) == sql_set );

#ifdef MAS_TRACING
  const char *set_type_title = node_type == DUF_NODE_LEAF ? "leaf" : ( node_type == DUF_NODE_LEAF ? "node" : "UNDEF" );
#endif

  if ( DUF_NOERROR( r ) )
    sql_selector = duf_selector2sql( sql_set, PDI->pdi_name, &r );


  DUF_TRACE( scan, 14, "sql:%s", sql_selector );
  DUF_TRACE( scan, 10, "[%s] (slctr2) #%llu: \"%s\"", set_type_title, duf_levinfo_dirid( PDI ), duf_levinfo_itemshowname( PDI ) );
/*
 * str_cb2 (sub-item scanner ): (~~content-scanner!?)
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_eval_sccbh_db_leaf_str_cb    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
  DOR( r, duf_eval_sccbh_sql_str_cb( scanstage, node_type, sql_selector, str_cb2, sccbh ) );
  mas_free( sql_selector );
  sql_selector = NULL;
  DEBUG_ENDR( r );
}
