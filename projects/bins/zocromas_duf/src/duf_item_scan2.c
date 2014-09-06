#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_utils.h"

#include "duf_levinfo.h"
#include "duf_pdi.h"
#include "duf_sccb.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql.h"

/* #include "duf_sql1.h" */
#include "duf_sql2.h"


#include "duf_item_match2.h"

/* ###################################################################### */
#include "duf_item_scan2.h"
/* ###################################################################### */

static char *
duf_selector2sql( duf_sql_set_t * sql_set )
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
    DUF_ERROR( "Bad arg" );
  return sql;
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
/* will be static! */

static int
duf_sel_cb2_leaf( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb )
{
  int r = 0;


  DEBUG_START(  );
  assert( pdi );

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", duf_pdi_depth( pdi ) );
  DUF_TRACE( explain, 4, "@ sel cb2 leaf" );

  /* if ( duf_filename_match(  &duf_config->u.glob, filename ) && duf_filesize_match( duf_config, filesize ) ) */
  {
    DUF_SFIELD2( filename );

    DUF_DO_TEST_R( r, duf_levinfo_godown( pdi, 0 /* dir_id */ , filename, 0, 0, 1 /* is_leaf */  ) );

    if ( r >= 0 )
    {
/*
 * 4. call function str_cb2
 * */
      pdi->seq++;
      pdi->seq_leaf++;
      if ( DUF_ACT_FLAG( progress ) )
        duf_percent( pdi->seq_leaf, pdi->total_files, duf_uni_scan_action_title( sccb ) );
      DUF_TRACE( seq, 0, "seq:%llu; seq_leaf:%llu", pdi->seq, pdi->seq_leaf );

      /* called both for leaves (files) and nodes (dirs) */
      DUF_DO_TEST_R( r, duf_levinfo_openat_dh( pdi ) );

      if ( r >= 0 )
      {
        DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 5. leaf str cb2; r:%d; dfd:%d ; opendir:%d", r,
                      duf_levinfo_dfd( pdi ), pdi->opendir );
        if ( str_cb2 && !duf_levinfo_item_deleted( pdi ) )
          DUF_DO_TEST_R( r, ( str_cb2 ) ( pstmt, pdi, sccb ) );
      }
      DUF_DO_TEST_R( r, duf_levinfo_goup( pdi ) );
    }
    if ( r == DUF_ERROR_MAX_DEPTH )
      r = 0;
    /* if ( r >= 0 )                                                        */
    /*   r = duf_pdi_max_filter( pdi ); (* check if any of max's reached *) */
    DUF_DO_TEST_R( r, duf_pdi_max_filter( pdi ) ); /* check if any of max's reached */
  }
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r;
}

/* duf_sel_cb_leaves:
 * this is callback of type: duf_sel_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 *
 * known str_cb for duf_sel_cb_node:
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd;  duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd ; str_cb_udata_unused
 *   duf_str_cb_uni_scan_dir
 *   ...
 * */
/* will be static! */


static int
duf_sel_cb2_node( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb )
{
  int r = 0;

  DEBUG_START(  );
  assert( pdi );

  {
    DUF_UFIELD2( dirid );
    DUF_UFIELD2( ndirs );
    DUF_UFIELD2( nfiles );
    DUF_SFIELD2( dfname );
    /* Not here : assert( dirid == duf_levinfo_dirid( pdi ) );
     * */
    DUF_TRACE( scan, 10, "before duf_levinfo_godown() : dirID:%llu", dirid );
    DUF_TRACE( explain, 2, "@ sel cb2 node" );

    /*!! dirid need not be same as duf_levinfo_dirid( pdi ) before duf_levinfo_godown */
    r = duf_levinfo_godown( pdi, dirid, dfname, ndirs, nfiles, 0 /*is_leaf */  );
    assert( dirid == duf_levinfo_dirid( pdi ) ); /* was set by duf_levinfo_godown */
  }
  assert( pdi->depth >= 0 );

  DUF_TEST_RQ( r, r == DUF_ERROR_MAX_DEPTH );

  if ( r >= 0 )                 /* levinfo_down OK */
  {
/*
 * 4. call function str_cb2
 * */
    pdi->seq++;
    pdi->seq_node++;
    DUF_TRACE( seq, 0, "seq:%llu; seq_node:%llu", pdi->seq, pdi->seq_node );

    /* called both for leaves (files) and nodes (dirs) */
    if ( r >= 0 && str_cb2 )
    {
      DUF_DO_TEST_R( r, duf_levinfo_openat_dh( pdi ) );

      /* if ( r >= 0 && !duf_levinfo_item_deleted( pdi ) ) */
      if ( r >= 0 )
      {
        DUF_TRACE( explain, 2, "=> str cb2" );
        DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 5. node str cb2" );
        DUF_DO_TEST_R( r, ( str_cb2 ) ( pstmt, pdi, sccb ) );
      }
    }
    else
      DUF_ERROR( "str_cb2 not set" );
    DUF_DO_TEST_R( r, duf_levinfo_goup( pdi ) );
  }

  /* r = ( r == DUF_ERROR_MAX_DEPTH ) ? 0 : r; */
  if ( r == DUF_ERROR_MAX_DEPTH ) /* reset error if it was `MAX_DEPTH` */
    r = 0;

  /* if ( r >= 0 )                                                        */
  /*   r = duf_pdi_max_filter( pdi ); (* check if any of max's reached *) */
  /* DUF_TEST_R( r );                                                     */
  DUF_DO_TEST_R( r, duf_pdi_max_filter( pdi ) ); /* check if any of max's reached */

  DEBUG_END(  );
  return r;
}

int
duf_count_db_items2( duf_sel_cb2_match_t match_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_sql_set_t * sql_set )
{
  unsigned long long cnt = 0;
  int r = 0;

  DEBUG_START(  );

  /* match_cb2 = duf_match_leaf2; ... */

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( sql_set->selector2 )
  {
    /* duf_sqlite_stmt_t *pstmt = NULL; */
    char *sql = NULL;

    if ( !sql_set->fieldset )
      r = DUF_ERROR_SQL_NO_FIELDSET;
    if ( r >= 0 )
      sql = duf_selector2sql( sql_set );
    if ( r >= 0 )
    {
      const char *csql;

      csql = sql;
      /* if ( r >= 0 )                          */
      /*   r = duf_sql_prepare( csql, &pstmt ); */
      DUF_SQL_START_STMT_NOPDI( csql, r, pstmt );

      DUF_TEST_R( r );
      DUF_TRACE( select, 0, "S:%s %llu/%llu/%u/%u", csql, duf_config->u.size.min, duf_config->u.size.max, duf_config->u.same.min,
                 duf_config->u.same.max );

      DUF_TRACE( scan, 12, "sql:%s diridpdi:%llu", csql, duf_levinfo_dirid( pdi ) );
      DUF_TEST_R( r );
      {
        DUF_SQL_BIND_LL( dirID, duf_levinfo_dirid( pdi ), r, pstmt );
        DUF_TEST_R( r );

/*                                                  *INDENT-OFF*  */
        DUF_SQL_EACH_ROW( r, pstmt, 
/*                                                  *INDENT-ON*  */
        if ( !match_cb2 || ( match_cb2 ) ( pstmt ) )
          cnt++;
        r = 0;
/*                                                  *INDENT-OFF*  */
	);
/*                                                  *INDENT-ON*  */

        DUF_SQL_END_STMT_NOPDI( r, pstmt );
      }
    }
    if ( sql )
      mas_free( sql );
    sql = NULL;
    if ( r >= 0 )
      duf_levinfo_set_items_files( pdi, cnt );
  }
  /* else                 */
  /*   r = DUF_ERROR_PTR; */

  /* DUF_ERROR( "r:%d; sel_cb2:%s", r, DUF_FUNN( sel_cb2 ) ); */
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r;
}

/*
 * call sel_cb2 for pstmt by 
 *           csql (arg 1)
 *       and dirID
 *      from pdi (arg 5)
 * 
 * str_cb2 (sub-item scanner):
 *       duf_str_cb2_uni_scan_dir
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 *
 * match_cb2,sel_cb2 from sccb:
 * DUF_NODE_LEAF => duf_match_leaf2, duf_sel_cb2_leaf
 * DUF_NODE_NODE =>                  duf_sel_cb2_node
 * */
static int
duf_scan_db_items2_sql( const char *csql, duf_sel_cb2_match_t match_cb2, duf_sel_cb2_t sel_cb2, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi,
                        duf_scan_callbacks_t * sccb )
{
  int r = 0;

  /* if ( r >= 0 && sql )                  */
  /*   r = duf_sql_prepare( sql, &pstmt ); */
  DUF_SQL_START_STMT_NOPDI( csql, r, pstmt );
  DUF_TRACE( select, 0, "S:%s", csql );
  DUF_TEST_R( r );

  DUF_TEST_R( r );
  {
    DUF_SQL_BIND_LL( dirID, duf_levinfo_dirid( pdi ), r, pstmt );
    DUF_TEST_R( r );

    if ( r >= 0 )
    {
      int cnt = 0;

      /* r = duf_sql_reset( pstmt ); */
      DUF_TEST_R( r );
      /* if ( r >= 0 )                                            */
      /*   r = duf_sql_bind_string( pstmt, ":dirname", bd );      */

/*                                                            *INDENT-OFF*  */
         DUF_SQL_EACH_ROW( r, pstmt, 
/*                                                            *INDENT-ON*  */
      if ( !match_cb2 || ( match_cb2 ) ( pstmt ) )
      {
/*
 * match_cb2,sel_cb2 from sccb:
 * DUF_NODE_LEAF => duf_match_leaf2, duf_sel_cb2_leaf
 * DUF_NODE_NODE =>                  duf_sel_cb2_node
 * 
 * str_cb2 (sub-item scanner):
 *       duf_str_cb2_uni_scan_dir
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
        r = ( sel_cb2 ) ( pstmt, str_cb2, pdi, sccb );
        DUF_TEST_R( r );
        cnt++;
      }
      else
        r = 0;
/*                                                            *INDENT-OFF*  */
	    );
/*                                                            *INDENT-ON*  */

      DUF_TRACE( scan, 1, "had %d records by %s", cnt, csql );
      DUF_TEST_RR( r );
      if ( !r )
        r = cnt;
      DUF_SQL_END_STMT_NOPDI( r, pstmt );
    }
  }
  return r;
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
 *   duf_str_cb2_uni_scan_dir
 *   ...
 *
 * really:
 *   DUF_NODE_NODE => sccb->node.selector2, sccb->node.fieldset
 *   DUF_NODE_LEAF => sccb->leaf.selector2, sccb->leaf.fieldset
 * */
int
duf_scan_db_items2( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi,
                    duf_scan_callbacks_t * sccb /* , const char *selector2, const char *fieldset */  )
{
  int r = 0;
  duf_sel_cb2_t sel_cb2 = NULL;
  duf_sel_cb2_match_t match_cb2 = NULL;
  duf_sql_set_t *sql_set = NULL;

  DEBUG_START(  );
/* duf_sel_cb_(node|leaf):
 * this is callback of type: duf_sel_cb_t (first range): 
 *
 * called with precord
 * str_cb2 + str_cb_udata to be called for precord with correspondig args
 * */
  DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 4. set %s sel_cb2%c; str_cb2%c",
                node_type == DUF_NODE_LEAF ? "leaf" : "node", sel_cb2 ? '+' : '-', str_cb2 ? '+' : '-' );
  if ( node_type == DUF_NODE_LEAF )
  {
    sel_cb2 = duf_sel_cb2_leaf;
    match_cb2 = NULL /* duf_match_leaf2 */ ;
    DUF_TRACE( explain, 2, "set sel_cb2 <= cb2 leaf" );
    sql_set = &sccb->leaf;
  }
  else if ( node_type == DUF_NODE_NODE )
  {
    sel_cb2 = duf_sel_cb2_node;
    DUF_TRACE( explain, 2, "set sel_cb2 <= cb2 node" );
    sql_set = &sccb->node;
  }
  else
    r = DUF_ERROR_UNKNOWN_NODE;
  DUF_TEST_R( r );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( sql_set->selector2 && sql_set->fieldset )
  {
    {
      char *sql = NULL;

      if ( r >= 0 )
        sql = duf_selector2sql( sql_set );

      DUF_TRACE( scan, 14, "sql:%s", sql );
      DUF_TRACE( scan, 10, "[%s] (selector2) diridpid:%llu", node_type == DUF_NODE_LEAF ? "leaf" : "node", duf_levinfo_dirid( pdi ) );
/*
 * match_cb2,sel_cb2 from sccb:
 * DUF_NODE_LEAF => duf_match_leaf2, duf_sel_cb2_leaf
 * DUF_NODE_NODE =>                  duf_sel_cb2_node
 * 
 * str_cb2 (sub-item scanner):
 *       duf_str_cb2_uni_scan_dir
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
      if ( r >= 0 )
        DUF_DO_TEST_R( r, duf_scan_db_items2_sql( sql, match_cb2, sel_cb2, str_cb2, pdi, sccb ) );
      if ( sql )
        mas_free( sql );
      sql = NULL;
    }
    if ( r > 0 )
    {
      DUF_TRACE( explain, 0, "%u records processed of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                 r, node_type == DUF_NODE_LEAF ? "leaf" : "node", duf_uni_scan_action_title( sccb ), duf_levinfo_dirid( pdi ) );
    }
    else if ( r == 0 )
    {
      /* DUF_TRACE( explain, 0, "= ? ============ ≪%s≫", csql ); */
      DUF_TRACE( explain, 1, "no records found of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                 node_type == DUF_NODE_LEAF ? "leaf" : "node", duf_uni_scan_action_title( sccb ), duf_levinfo_dirid( pdi ) );
    }
  }
  else
  {
    DUF_TRACE( explain, 0, "= ? ============ NO selector2 ≪%s≫", node_type == DUF_NODE_LEAF ? "leaf" : "node" );
    r = DUF_ERROR_PTR;
  }
  DUF_TRACE( scan, 13, "(%d) end scan items str_cb2%c", r, str_cb2 ? '+' : '-' );

  /* DUF_ERROR( "r:%d; sel_cb2:%s", r, DUF_FUNN( sel_cb2 ) ); */
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r;
}
