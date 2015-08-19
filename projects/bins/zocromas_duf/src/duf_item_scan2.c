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
#include "duf_levinfo_updown.h"

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

#include "duf_leaf_scan2.h"

#include "duf_sel_cb_leaf.h"
#include "duf_sel_cb_node.h"

/* ###################################################################### */
#include "duf_item_scan2.h"
/* ###################################################################### */

static char *
duf_selector2sql( const duf_sql_set_t * sql_set )
{
  char *sql = NULL;

  if ( sql_set->fieldset && sql_set->selector2 )
  {
    if ( 0 == strncmp( sql_set->selector2, "SELECT", 6 ) )
    {
      char *sql3;

      sql3 = duf_sql_mprintf( sql_set->selector2, sql_set->fieldset );
      sql = mas_strdup( sql3 );
      mas_free( sql3 );
    }
    else
    {
      sql = mas_strdup( "SELECT " );
      sql = mas_strcat_x( sql, sql_set->fieldset );
      sql = mas_strcat_x( sql, " " );
      sql = mas_strcat_x( sql, sql_set->selector2 );
    }
  }
  else
    DUF_SHOW_ERROR( "Bad arg" );
  return sql;
}

char *
duf_selector_total2sql( const duf_sql_set_t * sql_set )
{
  char *sql = NULL;

  assert( sql_set );
  assert( sql_set->selector_total2 );
  if ( sql_set->selector_total2 )
  {
    if ( 0 == strncmp( sql_set->selector_total2, "SELECT", 6 ) )
    {
      char *sql3;

      sql3 = duf_sql_mprintf( sql_set->selector_total2, sql_set->fieldset );
      sql = mas_strdup( sql3 );
      mas_free( sql3 );
    }
    else
    {
      sql = mas_strdup( "SELECT " );
      sql = mas_strcat_x( sql, "COUNT(*) AS nf" );
      sql = mas_strcat_x( sql, " " );
      sql = mas_strcat_x( sql, sql_set->selector_total2 );
    }
  }
  else
  {
    DUF_SHOW_ERROR( "Bad arg" );
  }
  return sql;
}


/*
 * do sel_cb2 for 1 record at statment pstmt_selector
 *
 * sel_cb2 can be:
 *   duf_sel_cb2_node
 *   duf_sel_cb2_leaf
 * */
static int
duf_scan_db_items2_row( duf_sqlite_stmt_t * pstmt_selector, /* duf_sel_cb2_match_t match_cb2, *//* duf_sel_cb2_t sel_cb2, */ duf_str_cb2_t str_cb2,
                        duf_sccb_handle_t * sccbh, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );
#if 0
  assert( sel_cb2 == duf_sel_cb2_leaf || sel_cb2 == duf_sel_cb2_node );
  assert( ( sel_cb2 == duf_sel_cb2_leaf && node_type == DUF_NODE_LEAF ) || ( sel_cb2 == duf_sel_cb2_node && node_type == DUF_NODE_NODE ) );
#endif
  /* if ( !match_cb2 || ( match_cb2 ) ( pstmt_selector ) ) */
  {
#if 0
    DOR( r, ( sel_cb2 ) ( pstmt_selector, str_cb2, sccbh ) );
#else
    assert( ( node_type == DUF_NODE_NODE ) || ( node_type == DUF_NODE_LEAF ) );
    if ( node_type == DUF_NODE_NODE )
      DOR( r, duf_sel_cb2_node( pstmt_selector, str_cb2, sccbh ) );
    else if ( node_type == DUF_NODE_LEAF )
      DOR( r, duf_sel_cb2_leaf( pstmt_selector, str_cb2, sccbh ) );
#endif
  }
  /* else     */
  /*   r = 0; */
  DEBUG_ENDR( r );
}

/*
 * call sel_cb2 for pstmt by 
 *           csql (arg 1)
 *       and dirID
 *      from pdi (arg 5)
 * 
 * str_cb2 (sub-item scanner):
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 *
 * match_cb2,sel_cb2 from sccb:
 * DUF_NODE_LEAF => duf_match_leaf2, duf_sel_cb2_leaf
 * DUF_NODE_NODE =>                  duf_sel_cb2_node
 * */

static int
duf_scan_db_items2_sql( const char *csql_selector, /* duf_sel_cb2_match_t match_cb2, *//* duf_sel_cb2_t sel_cb2, */ duf_str_cb2_t str_cb2,
                        duf_sccb_handle_t * sccbh, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );

  DUF_SQL_START_STMT_NOPDI( csql_selector, r, pstmt_selector );
  DUF_TRACE( select, 0, "S:%s", csql_selector );

  DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid( PDI ), r, pstmt_selector );
  duf_bind_ufilter_uni( pstmt_selector );

#if 1
  DUF_SQL_EACH_ROW( r, pstmt_selector, DOR( r, duf_scan_db_items2_row( pstmt_selector, str_cb2, sccbh, node_type ) ) );
#else
  DUF_SQL_EACH_ROW( r, pstmt_selector, r = duf_scan_db_items2_row( pstmt_selector, str_cb2, sccbh, node_type ) );
#endif
  DUF_SQL_END_STMT_NOPDI( r, pstmt_selector );
  DEBUG_ENDR( r );
}

int
duf_scan_db_items2( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  const duf_sql_set_t *sql_set = NULL;

  assert( str_cb2 == DUF_WRAPPED( duf_sccbh_eval_pdi_dirs ) || ( str_cb2 == duf_str_cb2_leaf_scan_fd ) || ( str_cb2 == duf_str_cb2_leaf_scan ) );
#ifdef MAS_TRACING
  const char *set_type_title = node_type == DUF_NODE_LEAF ? "leaf" : ( node_type == DUF_NODE_LEAF ? "node" : "UNDEF" );
#endif
  DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 4. set '%s' str_cb2%c", set_type_title, str_cb2 ? '+' : '-' );
  if ( node_type == DUF_NODE_LEAF )
  {
    DUF_TRACE( scan, 2, "cb2 %s", set_type_title );
    DUF_TRACE( scan_reg, 2, "ql%llu / q%llu %s", PDI->seq_leaf, PDI->seq, SCCB->title );
    sql_set = duf_get_sql_set( SCCB, node_type, &r ); /* or simply "sql_set = duf_get_leaf_sql_set( SCCB, &r )" */
  }
  else if ( node_type == DUF_NODE_NODE )
  {
    DUF_TRACE( scan, 2, "cb2 %s", set_type_title );
    DUF_TRACE( scan_dir, 2, "qn%llu / q%llu %s", PDI->seq_node, PDI->seq, SCCB->title );
    sql_set = duf_get_sql_set( SCCB, node_type, &r ); /* or simply "sql_set = duf_get_node_sql_set( SCCB, &r )" */
  }
  else
    DUF_MAKE_ERROR( r, DUF_ERROR_UNKNOWN_NODE );
  DUF_TEST_R( r );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( sql_set->selector2 && sql_set->fieldset )
  {
    {
      char *sql_selector = NULL;

      if ( r >= 0 )
        sql_selector = duf_selector2sql( sql_set );

      DUF_TRACE( scan, 14, "sql:%s", sql_selector );
      DUF_TRACE( scan, 10, "[%s] (slctr2) #%llu: \"%s\"", set_type_title, duf_levinfo_dirid( PDI ), duf_levinfo_itemshowname( PDI ) );
/*
 * str_cb2 (sub-item scanner ): (~~content-scanner!?)
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
      DOR( r, duf_scan_db_items2_sql( sql_selector, str_cb2, sccbh, node_type ) );
      if ( sql_selector )
        mas_free( sql_selector );
      sql_selector = NULL;
    }
    if ( r > 0 )
    {
      DUF_TRACE( explain, 0, "%u records processed of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                 r, set_type_title, duf_uni_scan_action_title( SCCB ), duf_levinfo_dirid( PDI ) );
    }
    else if ( r == 0 )
    {
      /* DUF_TRACE( explain, 0, "= ? ============ ≪%s≫", csql ); */
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

  /* DUF_SHOW_ERROR( "r:%d; sel_cb2:%s", r, DUF_FUNN( sel_cb2 ) ); */
  DEBUG_ENDR( r );
}
