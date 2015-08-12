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
/* ###################################################################### */
#include "duf_item_scan2.h"
/* ###################################################################### */

char *
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

static int
duf_pstmt_levinfo_godown_openat_dh( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  if ( r >= 0 )
  {
    DUF_UFIELD2( dirid );
    DUF_UFIELD2( ndirs );
    DUF_UFIELD2( nfiles );
    /* Not here : assert( dirid == duf_levinfo_dirid( pdi ) );
     * */
    DUF_TRACE( scan, 10, "before duf_levinfo_godown() : dirID:%llu", dirid );
    DUF_TRACE( explain, 2, "@ sel cb2 node" );

    /*!! dirid need not be same as duf_levinfo_dirid( pdi ) before duf_levinfo_godown */
#if 0
    DOR( r, duf_levinfo_godown_openat_dh( pdi, dirid, DUF_GET_SFIELD2( dfname ), ndirs, nfiles, 0 /*is_leaf */  ) );
#else
    DOR( r, duf_levinfo_godown_dbopenat_dh( pdi, 0, DUF_GET_SFIELD2( dfname ), ndirs, nfiles, 0 /* is_leaf */ , pstmt ) );
#endif
    {
      unsigned long long did = duf_levinfo_dirid( pdi );

      assert( r < 0 || dirid == did ); /* was set by duf_levinfo_godown */
    }
  }
  DEBUG_ENDR( r );
}

/* duf_sel_cb_leaves:
 * this is callback of type: duf_sel_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 *
 * known str_cb for duf_sel_cb_leaf:
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd;  duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd ; str_cb_udata_unused
 *   duf_str_cb_uni_scan_dir (in theory ?!)
 *   ...
 * */
static int
duf_sel_cb2_leaf( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  assert( PDI );

  /* data from db at pstmt */

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", duf_pdi_depth( PDI ) );
  DUF_TRACE( explain, 4, "@ sel cb2 leaf" );


#if 0
  DOR( r, duf_levinfo_godown_openat_dh( PDI, 0 /* dir_id */ , DUF_GET_SFIELD2( filename ), 0, 0, 1 /* is_leaf */  ) );
#else
  DOR( r, duf_levinfo_godown_dbopenat_dh( PDI, 0, DUF_GET_SFIELD2( filename ), 0 /* ndirs */ , 0 /* nfiles */ , 1 /* is_leaf */ , pstmt ) );
#endif


  if ( r >= 0 )
  {
    unsigned long m;

    m = TOTITEMS;
/*
 * 4. call function str_cb2
 * */
    PDI->seq++;
    PDI->seq_leaf++;
    DUF_TRACE( scan_reg, 0, "* %llu / %llu %s", PDI->seq_leaf, TOTITEMS, SCCB->title );
    if ( !SCCB->no_progress && !SCCB->count_nodes && DUF_ACT_FLAG( progress ) && TOTITEMS > 0 && m > 0 )
    {
      /* assert( PDI->seq_leaf <= m ); */
      duf_percent( PDI->seq_leaf, TOTITEMS, duf_uni_scan_action_title( SCCB ) );
    }
    DUF_TRACE( seq, 0, "seq:%llu; seq_leaf:%llu", PDI->seq, PDI->seq_leaf );

    /* called both for leaves (files) and nodes (dirs) */

    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 5. leaf str cb2; r:%d; dfd:%d ; opendir:%d", r,
                  duf_levinfo_dfd( PDI ), PDI->opendir );

    if ( str_cb2 /* && !duf_levinfo_item_deleted( PDI ) */  )
    {
      if ( duf_levinfo_item_deleted( PDI ) )
      {
        DUF_TRACE( fs, 1, "@@@ >>> [%s] ", duf_levinfo_itemshowname( PDI ) );
      }
      DOR( r, ( str_cb2 ) ( pstmt, sccbh ) );
      if ( r == DUF_ERROR_OPENAT_ENOENT )
        r = 0;
    }

    DOR( r, duf_levinfo_goup( PDI ) );
  }
  /* else if ( r == DUF_ERROR_OPENAT_ENOENT )                                                                        */
  /* {                                                                                                               */
  /*   DUF_TRACE( temp, 0, "@!!! %d : [%s] ==> [%s]", r, duf_levinfo_itemname( PDI ), DUF_GET_SFIELD2( filename ) ); */
  /*   sleep( 1 );                                                                                                   */
  /*   r = 0;                                                                                                        */
  /* }                                                                                                               */
  DUF_CLEAR_ERROR( r, DUF_ERROR_TOO_DEEP );
  DOR( r, duf_pdi_max_filter( PDI ) ); /* check if any of max's reached */

  DEBUG_ENDR( r );
}

static int
duf_sel_cb2_node( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  assert( PDI );

  DOR( r, duf_pstmt_levinfo_godown_openat_dh( pstmt, PDI ) );
  assert( PDI->depth >= 0 );

  DUF_TEST_R( r );

  if ( r >= 0 )                 /* levinfo_down OK */
  {
    long long m;

    PDI->seq++;
    PDI->seq_node++;
    if ( SCCB->count_nodes && !SCCB->no_progress && TOTITEMS > 0 && DUF_ACT_FLAG( progress ) )
    {
      m = TOTITEMS + duf_pdi_reldepth( PDI ) - duf_pdi_depth( PDI ) - 1;
      DUF_SCCB( DUF_TRACE, action, 0, "total_items: %llu; m: %llu rd:%d; d:%d", TOTITEMS, m, duf_pdi_reldepth( PDI ), duf_pdi_depth( PDI ) );
/*
 * 4. call function str_cb2
 * */
      DUF_TRACE( scan_dir, 0, "* qn%llu/q%llu T%llu %s", PDI->seq_node, PDI->seq, TOTITEMS, SCCB->title );
      if ( m > 0 )
      {
        assert( PDI->seq_node <= m );
        duf_percent( PDI->seq_node, m, duf_uni_scan_action_title( SCCB ) );
      }
    }


    DUF_TRACE( seq, 0, "seq:%llu; seq_node:%llu", PDI->seq, PDI->seq_node );

    /* called both for leaves (files) and nodes (dirs) */
    if ( str_cb2 )
    {
      /* if ( r >= 0 && !duf_levinfo_item_deleted( PDI ) ) */
      DUF_TRACE( explain, 2, "=> str cb2" );
      DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 5. node str cb2" );
      /* DUF_E_NO( DUF_ERROR_OPENAT_ENOENT ); */
      /* DUF_TRACE( temp, 0, "@+[%s]", duf_levinfo_itemname( PDI ) ); */
      DOR( r, ( str_cb2 ) ( pstmt, sccbh ) );
      /* DUF_TRACE( temp, 0, "@@-[%s]", duf_levinfo_itemname( PDI ) ); */
      /* assert( r != DUF_ERROR_OPENAT_ENOENT ); */
      /* DUF_E_YES( DUF_ERROR_OPENAT_ENOENT ); */
      /* if ( r == DUF_ERROR_OPENAT_ENOENT ) */
      /* r = 0; */
      /* DUF_TRACE( temp, 0, "r:%d DUF_ERROR_OPENAT_ENOENT:%d", r, DUF_ERROR_OPENAT_ENOENT ); */
    }
    else
      DUF_SHOW_ERROR( "str_cb2 not set" );
    DOR( r, duf_levinfo_goup( PDI ) );
  }

  DUF_CLEAR_ERROR( r, DUF_ERROR_TOO_DEEP ); /* reset error if it was `MAX_DEPTH` */

  DOR( r, duf_pdi_max_filter( PDI ) ); /* check if any of max's reached */

  DEBUG_ENDR( r );
}



/*
 * do sel_cb2 for 1 record at statment pstmt_selector
 *
 * sel_cb2 can be:
 *   duf_sel_cb2_node
 *   duf_sel_cb2_leaf
 * */
static int
duf_scan_db_items2_row( duf_sqlite_stmt_t * pstmt_selector, duf_sel_cb2_match_t match_cb2, duf_sel_cb2_t sel_cb2, duf_str_cb2_t str_cb2,
                        duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  if ( !match_cb2 || ( match_cb2 ) ( pstmt_selector ) )
  {
    DOR( r, ( sel_cb2 ) ( pstmt_selector, str_cb2, sccbh ) );
  }
  else
    r = 0;
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
duf_scan_db_items2_sql( const char *csql_selector, duf_sel_cb2_match_t match_cb2, duf_sel_cb2_t sel_cb2, duf_str_cb2_t str_cb2,
                        duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  DUF_SQL_START_STMT_NOPDI( csql_selector, r, pstmt_selector );
  DUF_TRACE( select, 0, "S:%s", csql_selector );

  DUF_SQL_BIND_LL( parentdirID, duf_levinfo_dirid( PDI ), r, pstmt_selector );
  duf_bind_ufilter_uni( pstmt_selector );

  DUF_SQL_EACH_ROW( r, pstmt_selector, r = duf_scan_db_items2_row( pstmt_selector, match_cb2, sel_cb2, str_cb2, sccbh ) );
  DUF_SQL_END_STMT_NOPDI( r, pstmt_selector );
  DEBUG_ENDR( r );
}

/* 
 * call str_cb + pdi as str_cb_udata for each record by sql with corresponding args
 *
 * 1. choose by node_type:
 *      sql_set
 *      sel_cb2
 *      match_cb2
 * 2. call duf_scan_db_items2_sql
 *
 *
 * known str_cb for duf_scan_db_items2 (to pass to duf_sql_vselect and then to duf_sel_cb_leaf OR duf_sel_cb_node ):
 *   duf_str_cb2_leaf_scan;   duf_str_cb2_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb2_scan_file_fd;  duf_str_cb2_scan_file_fd is just a wrapper for sccb->leaf_scan_fd ; str_cb_udata_unused
 *   duf_scan_dirs_by_pdi_maxdepth
 *   ...
 *
 * really:
 *   DUF_NODE_NODE => sccb -> node.selector2, sccb -> node.fieldset
 *   DUF_NODE_LEAF => sccb -> leaf.selector2, sccb -> leaf.fieldset
 * */
int
duf_scan_db_items2( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  duf_sel_cb2_t sel_cb2 = NULL;
  duf_sel_cb2_match_t match_cb2 = NULL;
  const duf_sql_set_t *sql_set = NULL;

#ifdef MAS_TRACING
  const char *set_type_title = node_type == DUF_NODE_LEAF ? "leaf" : ( node_type == DUF_NODE_LEAF ? "node" : "UNDEF" );
#endif
/* duf_sel_cb_(node|leaf):
 * this is callback of type: duf_sel_cb_t (first range): 
 *
 * called with precord
 * str_cb2 + str_cb_udata to be called for precord with correspondig args
 * */
  DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 4. set '%s' sel_cb2%c; str_cb2%c",
                set_type_title, sel_cb2 ? '+' : '-', str_cb2 ? '+' : '-' );
  if ( node_type == DUF_NODE_LEAF )
  {
    sel_cb2 = duf_sel_cb2_leaf;
    match_cb2 = NULL /* duf_match_leaf2 */ ;
    DUF_TRACE( explain, 2, "set sel_cb2 <= cb2 %s", set_type_title );
    DUF_TRACE( scan, 2, "set sel_cb2 <= cb2 %s", set_type_title );
    DUF_TRACE( scan_reg, 2, "ql%llu / q%llu %s", PDI->seq_leaf, PDI->seq, SCCB->title );
#if 0
    sql_set = &SCCB->leaf;
#elif 0
    sql_set = duf_get_leaf_sql_set( SCCB );
#else
    sql_set = duf_get_sql_set( SCCB, node_type, &r );
#endif

  }
  else if ( node_type == DUF_NODE_NODE )
  {
    sel_cb2 = duf_sel_cb2_node;
    match_cb2 = NULL /* duf_match_node2 ?? */ ;
    DUF_TRACE( explain, 2, "set sel_cb2 <= cb2 %s", set_type_title );
    DUF_TRACE( scan, 2, "set sel_cb2 <= cb2 %s", set_type_title );
    DUF_TRACE( scan_dir, 2, "qn%llu / q%llu %s", PDI->seq_node, PDI->seq, SCCB->title );
#if 0
    sql_set = &SCCB->node;
#elif 0
    sql_set = duf_get_node_sql_set( SCCB );
#else
    sql_set = duf_get_sql_set( SCCB, node_type, &r );
#endif
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
 * match_cb2,sel_cb2 from sccb:
 * DUF_NODE_LEAF => duf_match_leaf2, duf_sel_cb2_leaf
 * DUF_NODE_NODE =>                  duf_sel_cb2_node
 * 
 * str_cb2 (sub-item scanner ): (~~content-scanner!?)
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
      DOR( r, duf_scan_db_items2_sql( sql_selector, match_cb2, sel_cb2, str_cb2, sccbh ) );
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
