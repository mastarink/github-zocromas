/* 20150819.134107 */
#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"     /* for DUF_ACT_FLAG( progress ) !ONLY! */
#include "duf_option_defs.h"    /* for DUF_ACT_FLAG( progress ) !ONLY! */

#include "duf_utils.h"          /* duf_percent */

#include "duf_levinfo_ref.h"
/* #include "duf_levinfo_updown.h" */

#include "duf_pdi.h"
#include "duf_sccb_def.h"
#include "duf_sccb.h"


#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql.h"

#include "duf_sql2.h"
#include "duf_ufilter_bind.h"

#include "duf_sccbh_shortcuts.h"
#include "duf_sccb_eval_dirs.h" /* for assert  */

#include "duf_selector.h"
#include "duf_leaf_scan2.h"

#include "duf_sel_cb_leaf.h"
#include "duf_sel_cb_node.h"

/* ###################################################################### */
#include "duf_item_scan2.h"
/* ###################################################################### */

/* 20150819.132024 */
static int
duf_scan_db_row_with_str_cb( duf_sqlite_stmt_t * pstmt_selector, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh, duf_node_type_t node_type )
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
    if ( cb )
      DOR( r, ( cb ) ( pstmt_selector, str_cb2, sccbh ) );
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

/*20150819.132038
 * 1. bind ufilter
 * 2. duf_scan_db_row_with_str_cb for each row from db by ufilter
 * */
static int
duf_scan_db_items_with_str_cb_sql( const char *csql_selector, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );

  DUF_SQL_START_STMT_NOPDI( csql_selector, r, pstmt_selector );
  DUF_TRACE( select, 0, "S:%s", csql_selector );

  DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid( PDI ), r, pstmt_selector );
  duf_bind_ufilter_uni( pstmt_selector );

  /* cal one of duf_sel_cb2_(leaf|node) by node_type
   * i.e. DOR( r, (( node_type == DUF_NODE_NODE ) ? duf_sel_cb2_node : ( node_type == DUF_NODE_LEAF ? duf_sel_cb2_leaf : NULL ) ) ( pstmt_selector, str_cb2, sccbh ) )
   * */
#if 1
  DUF_SQL_EACH_ROW( r, pstmt_selector, DOR( r, duf_scan_db_row_with_str_cb( pstmt_selector, str_cb2, sccbh, node_type ) ) );
#else
  DUF_SQL_EACH_ROW( r, pstmt_selector, DOR( r, duf_scan_db_row_with_str_cb( pstmt_selector, str_cb2, sccbh, node_type ) ) );
#endif
  DUF_SQL_END_STMT_NOPDI( r, pstmt_selector );
  DEBUG_ENDR( r );
}

static int
duf_scan_db_items_with_str_cb_sql_set( const duf_sql_set_t * sql_set, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );
  char *sql_selector = NULL;

#ifdef MAS_TRACING
  const char *set_type_title = node_type == DUF_NODE_LEAF ? "leaf" : ( node_type == DUF_NODE_LEAF ? "node" : "UNDEF" );
#endif

  if ( r >= 0 )
    sql_selector = duf_selector2sql( sql_set );

  DUF_TRACE( scan, 14, "sql:%s", sql_selector );
  DUF_TRACE( scan, 10, "[%s] (slctr2) #%llu: \"%s\"", set_type_title, duf_levinfo_dirid( PDI ), duf_levinfo_itemshowname( PDI ) );
/*
 * str_cb2 (sub-item scanner ): (~~content-scanner!?)
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_eval_sccbh_db_leaf_str_cb    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
  DOR( r, duf_scan_db_items_with_str_cb_sql( sql_selector, str_cb2, sccbh, node_type ) );
  if ( sql_selector )
    mas_free( sql_selector );
  sql_selector = NULL;
  DEBUG_ENDR( r );
}

/* 20150819.133354 */
int
duf_eval_sccbh_db_items_str_cb( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  const duf_sql_set_t *sql_set = NULL;

  assert( str_cb2 == DUF_WRAPPED( duf_eval_sccbh_all ) || ( str_cb2 == duf_eval_sccbh_db_leaf_fd_str_cb )
          || ( str_cb2 == duf_eval_sccbh_db_leaf_str_cb ) );
#ifdef MAS_TRACING
  const char *set_type_title = node_type == DUF_NODE_LEAF ? "leaf" : ( node_type == DUF_NODE_LEAF ? "node" : "UNDEF" );
#endif
  DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 4. set '%s' str_cb2%c", set_type_title, str_cb2 ? '+' : '-' );
  DUF_TRACE( scan, 2, "cb2 %s", set_type_title );
  DUF_TRACE( scan, 2, "ql%llu / qn%llu / q%llu %s", PDI->seq_leaf, PDI->seq_node, PDI->seq, SCCB->title );
  sql_set = duf_get_sql_set( SCCB, node_type, &r );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( sql_set->selector2 && sql_set->fieldset )
  {
    DOR( r, duf_scan_db_items_with_str_cb_sql_set( sql_set, str_cb2, sccbh, node_type ) );
    if ( DUF_NOERROR( r ) )
    {
      if ( r > 0 )
        DUF_TRACE( explain, 0, "%u records processed of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                   r, set_type_title, duf_uni_scan_action_title( SCCB ), duf_levinfo_dirid( PDI ) );
      else
        DUF_TRACE( explain, 1, "no records found of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                   set_type_title, duf_uni_scan_action_title( SCCB ), duf_levinfo_dirid( PDI ) );
    }
  }
  else
  {
    DUF_TRACE( explain, 0, "= ? ============ NO selector2 ≪%s≫", set_type_title );
    DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
  }
  DUF_TRACE( scan, 13, "(%d) end scan items str_cb2%c", r, str_cb2 ? '+' : '-' );

  DEBUG_ENDR( r );
}
