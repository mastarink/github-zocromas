#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"


#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_options.h"

#include "duf_levinfo.h"
#include "duf_pdi.h"
#include "duf_sccb.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql.h"
/* #include "duf_sql1.h" */
#include "duf_sql2.h"

#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_item_scan2.h"
/* ###################################################################### */


int
duf_match_leaf2( duf_sqlite_stmt_t * pstmt )
{
  int r;

  DUF_SFIELD2( filename );
  DUF_UFIELD2( filesize );
  DUF_UFIELD2( nsame );
  DUF_UFIELD2( md5id );

  r = duf_filename_match( &duf_config->u.glob, filename ) && duf_lim_match( duf_config->u.size, filesize )
        && duf_lim_match( duf_config->u.same, nsame ) && duf_md5id_match( duf_config->u.md5id, md5id );
  if ( !r )
    DUF_TRACE( match, 0, "NOT MATCH %s (mode 2)", filename );
  return r;
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
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );

  DUF_TRACE( scan, 0, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", duf_pdi_depth( pdi ) );
  DUF_TRACE( explain, 0, "@ sel cb2 leaf" );

  /* if ( duf_filename_match(  &duf_config->u.glob, filename ) && duf_filesize_match( duf_config, filesize ) ) */
  {
    DUF_SFIELD2( filename );
    r = duf_levinfo_down( pdi, 0 /* dirid */ , filename, 0, 0, 1 /* is_leaf */  );
    DUF_TEST_R( r );
    DUF_OINVC( pdi-> );
    if ( r >= 0 )
    {
/*
 * 4. call function str_cb2
 * */
      pdi->seq++;
      pdi->seq_leaf++;
      {
        float width = 90;

        pdi->bar.percent = ( ( ( float ) pdi->seq_leaf ) / ( ( float ) pdi->total_files ) );
        pdi->bar.width = width * pdi->bar.percent;
        if ( DUF_ACT_FLAG( progress ) && pdi->bar.width != ( pdi->bar.prev_width - 1 ) )
        {
          if ( pdi->bar.width == 0 )
            fputs( "\n", stderr );
          fprintf( stderr, "\r [" );
          for ( int i = 0; i < pdi->bar.width; i++ )
            fputc( '=', stderr );
          for ( int i = pdi->bar.width; i < width; i++ )
            fputc( ' ', stderr );
          fprintf( stderr, "] %d%%; %llu of %llu {%llu}", ( int ) ( pdi->bar.percent * 100. ), pdi->seq_leaf, pdi->total_files,
                   pdi->cnts.dirent_content2 );
          pdi->bar.prev_width = pdi->bar.width + 1;
          if ( pdi->bar.width == width )
            fputs( "\n", stderr );
        }
      }
      DUF_TRACE( seq, 0, "seq:%llu; seq_leaf:%llu", pdi->seq, pdi->seq_leaf );

      DUF_TEST_R( r );
      /* called both for leaves (files) and nodes (dirs) */
      if ( str_cb2 )
      {
        r = duf_levinfo_openat_dh( pdi );

        /* if ( r >= 0 && !duf_levinfo_item_deleted( pdi ) ) */
        if ( r >= 0 )
        {
          r = ( str_cb2 ) ( pstmt, pdi, sccb );
        }
        DUF_TEST_R( r );
        /* DUF_ERROR( "r:%d; str_cb2:%s", r, DUF_FUNN( str_cb2 ) ); */
      }
      else
        DUF_TRACE( error, 0, "str_cb2 not set" );
      duf_levinfo_up( pdi );
      DUF_OINV_OPENED( pdi-> );
    }
    if ( r == DUF_ERROR_MAX_DEPTH )
      r = 0;
    DUF_TEST_R( r );
    DUF_OINV_OPENED( pdi-> );
    if ( !duf_pdi_max_filter( pdi ) )
      r = DUF_ERROR_MAX_REACHED;
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

  DUF_UFIELD2( dirid );
  DUF_UFIELD2( ndirs );
  DUF_UFIELD2( nfiles );
  DUF_SFIELD2( dfname );

  DEBUG_START(  );
  assert( pdi );
  DUF_OINV_OPENED( pdi-> );

  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );

  /* Not here : assert( dirid == duf_levinfo_dirid( pdi ) );
   * */
  DUF_TRACE( scan, 0, "before duf_levinfo_down() : dirid:%llu", dirid );
  DUF_TRACE( explain, 2, "@ sel cb2 node" );

  r = duf_levinfo_down( pdi, dirid, dfname, ndirs, nfiles, 0 /*is_leaf */  );
  assert( dirid == duf_levinfo_dirid( pdi ) );

  assert( pdi->depth >= 0 );
  if ( r != DUF_ERROR_MAX_DEPTH )
    DUF_TEST_R( r );
  /* DUF_ERROR( "r:%d;", r ); */
  if ( r >= 0 )
  {
    DUF_OINV_NOT_OPENED( pdi-> );
/*
 * 4. call function str_cb2
 * */
    pdi->seq++;
    pdi->seq_node++;
    DUF_TRACE( seq, 0, "seq:%llu; seq_node:%llu", pdi->seq, pdi->seq_node );

    DUF_OINVC( pdi-> );
    /* called both for leaves (files) and nodes (dirs) */
    if ( str_cb2 )
    {
      if ( r >= 0 )
      {
        DUF_OINV_NOT_OPENED( pdi-> );
        r = duf_levinfo_openat_dh( pdi );

        /* if ( r >= 0 && !duf_levinfo_item_deleted( pdi ) ) */
        if ( r >= 0 )
        {
          DUF_OINV_OPENED( pdi-> );
          DUF_TEST_R( r );
          DUF_TRACE( scan, 1, "  " DUF_DEPTH_PFMT ": str_cb2 node:     by %5llu", duf_pdi_depth( pdi ), dirid );

          if ( r >= 0 )
          {
            DUF_TRACE( scan, 0, "before ... as (str_cb2)" );
            DUF_TRACE( explain, 0, "=> str cb2" );
            r = ( str_cb2 ) ( pstmt, pdi, sccb );
          }
          DUF_TEST_R( r );
          /* DUF_ERROR( "F:%s", DUF_FUNN( str_cb2 ) ); */
          DUF_OINV_OPENED( pdi-> );
        }
        DUF_OINV( pdi-> );
        DUF_TEST_R( r );
      }
      DUF_TEST_R( r );
    }
    else
      DUF_ERROR( "str_cb2 not set" );
    DUF_TEST_R( r );
    duf_levinfo_up( pdi );
    DUF_OINV_OPENED( pdi-> );
  }
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  DUF_TEST_R( r );
  if ( r == DUF_ERROR_MAX_DEPTH )
    r = 0;
  DUF_OINV_OPENED( pdi-> );
  if ( !duf_pdi_max_filter( pdi ) )
    r = DUF_ERROR_MAX_REACHED;
  DUF_TEST_R( r );
  DEBUG_END(  );
  return r;
}

/* 
 * call str_cb + pdi as str_cb_udata for each record by sql with corresponding args
 *
 *
 * known str_cb for duf_scan_db_vitems2 (to pass to duf_sql_vselect and then to duf_sel_cb_leaf OR duf_sel_cb_node ):
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd;  duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd ; str_cb_udata_unused
 *   duf_str_cb_uni_scan_dir
 *   ...
 * */
static int
duf_scan_db_vitems2( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi,
                     duf_scan_callbacks_t * sccb, const char *selector2, const char *fieldset, va_list args )
{
  int r = 0;
  duf_sel_cb2_t sel_cb2 = NULL;
  duf_sel_cb2_match_t match_cb2 = NULL;

  DEBUG_START(  );
/* duf_sel_cb_(node|leaf):
 * this is callback of type: duf_sel_cb_t (first range): 
 *
 * called with precord
 * str_cb2 + str_cb_udata to be called for precord with correspondig args
 * */
  if ( node_type == DUF_NODE_LEAF )
  {
    sel_cb2 = duf_sel_cb2_leaf;
    match_cb2 = duf_match_leaf2;
    DUF_TRACE( explain, 0, "set sel_cb2 <= cb2 leaf" );
  }
  else if ( node_type == DUF_NODE_NODE )
  {
    sel_cb2 = duf_sel_cb2_node;
    DUF_TRACE( explain, 2, "set sel_cb2 <= cb2 node" );
  }
  else
    r = DUF_ERROR_UNKNOWN_NODE;
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  DUF_TRACE( scan, 0, "scan items [%s] sel_cb2%c; str_cb2%c", node_type == DUF_NODE_LEAF ? "leaf" : "node", sel_cb2 ? '+' : '-',
             str_cb2 ? '+' : '-' );
  DUF_TEST_R( r );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( selector2 )
  {
    /* duf_sqlite_stmt_t *pstmt = NULL; */
    unsigned long long dirid = 0;
    dirid = va_arg( args, unsigned long long );


    {
      char *sql = NULL;

      if ( fieldset )
      {
        if ( 0 == strncmp( selector2, "SELECT", 6 ) )
        {
          char *sql3;

          sql3 = duf_sql_mprintf( selector2, fieldset );
          sql = mas_strdup( sql3 );
          mas_free( sql3 );
        }
        else
        {
          sql = mas_strdup( "SELECT " );
          sql = mas_strcat_x( sql, fieldset );
          sql = mas_strcat_x( sql, " " );
          sql = mas_strcat_x( sql, selector2 );
        }
      }
      else
      {
        DUF_TRACE( explain, ( node_type == DUF_NODE_LEAF ? 0 : 2 ), "= ? ============ NO fieldset ≪%s≫",
                   node_type == DUF_NODE_LEAF ? "leaf" : "node" );
        r = DUF_ERROR_SQL_NO_FIELDSET;
      }
      {
        const char *csql;

        csql = sql;
        /* if ( r >= 0 && sql )                  */
        /*   r = duf_sql_prepare( sql, &pstmt ); */
        DUF_SQL_START_STMT_NOPDI( csql, r, pstmt );
        DUF_TRACE( select, 0, "S:%s", csql );
        DUF_TEST_R( r );

        DUF_TRACE( scan, 4, "sql:%s", csql );
        DUF_TRACE( scan, 0, "[%s] (selector2) dirid:%llu", node_type == DUF_NODE_LEAF ? "leaf" : "node", dirid );
        DUF_TEST_R( r );
        {
          /* if ( r >= 0 )                                           */
          /*   r = duf_sql_bind_long_long( pstmt, ":dirid", dirid ); */
          DUF_SQL_BIND_LL( dirid, dirid, r, pstmt );
          /* if ( r >= 0 )                                                                */
          /* {                                                                            */
          /*   r = duf_sql_bind_long_long_nz( pstmt, ":size.min", duf_config->u.size .min); */
          /*   if ( r == DUF_ERROR_BIND_NAME )                                            */
          /*     r = 0;                                                                   */
          /* }                                                                            */
          DUF_SQL_BIND_LL_NZ_OPT( size.min, duf_config->u.size.min, r, pstmt );
          /* if ( r >= 0 )                                                                */
          /* {                                                                            */
          /*   r = duf_sql_bind_long_long_nz( pstmt, ":size.max", duf_config->u.size .max); */
          /*   if ( r == DUF_ERROR_BIND_NAME )                                            */
          /*     r = 0;                                                                   */
          /* }                                                                            */
          DUF_SQL_BIND_LL_NZ_OPT( size.max, duf_config->u.size.max, r, pstmt );
          /* if ( r >= 0 )                                                                */
          /* {                                                                            */
          /*   r = duf_sql_bind_long_long_nz( pstmt, ":same.min", duf_config->u.same .min); */
          /*   if ( r == DUF_ERROR_BIND_NAME )                                            */
          /*     r = 0;                                                                   */
          /* }                                                                            */
          DUF_SQL_BIND_LL_NZ_OPT( same.min, duf_config->u.same.min, r, pstmt );
          /* if ( r >= 0 )                                                                */
          /* {                                                                            */
          /*   r = duf_sql_bind_long_long_nz( pstmt, ":same.max", duf_config->u.same .max); */
          /*   if ( r == DUF_ERROR_BIND_NAME )                                            */
          /*     r = 0;                                                                   */
          /* }                                                                            */
          DUF_SQL_BIND_LL_NZ_OPT( same.max, duf_config->u.same.max, r, pstmt );
          DUF_TEST_R( r );

          if ( r >= 0 )
          {
            int cnt = 0;

            /* r = duf_sql_reset( pstmt ); */
            DUF_TEST_R( r );
            /* if ( r >= 0 )                                            */
            /*   r = duf_sql_bind_string( pstmt, ":dirname", bd );      */
            while ( r >= 0 && r != DUF_SQL_DONE )
            {
              /* r = duf_sql_step( pstmt ); */
              DUF_SQL_STEP( r, pstmt );
              DUF_TRACE( scan, 0, "[%s] (selector2) dirid:%llu", node_type == DUF_NODE_LEAF ? "leaf" : "node", dirid );
              DUF_TEST_RR( r );
              DUF_TRACE( scan, 2, "#%d sql_step : r:%d; %s", cnt, r,
                         r == DUF_SQL_ROW ? "_ROW" : ( r == DUF_SQL_DONE ? "_DONE" : "_____" ) );

              if ( r == DUF_SQL_ROW )
              {
                if ( !match_cb2 || ( match_cb2 ) ( pstmt ) )
                {
                  DUF_TRACE( scan, 0, "before duf_sel_cb2_%s(...) as (sel_cb2) : (selector2) dirid:%llu",
                             node_type == DUF_NODE_LEAF ? "leaf" : "node", dirid );
                  /* sel_cb can be duf_sel_cb2_(node|leaf) */
                  DUF_TRACE( explain, ( node_type == DUF_NODE_LEAF ? 0 : 2 ), "=> sel_cb2 ≪%s≫",
                             node_type == DUF_NODE_LEAF ? "leaf" : "node" );
                  r = ( sel_cb2 ) ( pstmt, str_cb2, pdi, sccb );
                  DUF_TEST_R( r );
                  cnt++;
                }
                else
                  r = 0;
              }
            }
            if ( cnt )
            {
              DUF_TRACE( explain, 0, "%u records processed of type ≪%s≫ ; action ≪%s≫; dirid:%llu",
                         cnt, node_type == DUF_NODE_LEAF ? "leaf" : "node", duf_uni_scan_action_title( sccb ), dirid );
            }
            else
            {
              /* DUF_TRACE( explain, 0, "= ? ============ ≪%s≫", csql ); */
              DUF_TRACE( explain, 0, "no records found of type ≪%s≫ ; action ≪%s≫; dirid:%llu",
                         node_type == DUF_NODE_LEAF ? "leaf" : "node", duf_uni_scan_action_title( sccb ), dirid );
            }
            DUF_TRACE( scan, 0, "had %d records by %s", cnt, csql );
            DUF_TEST_RR( r );
            DUF_SQL_END_STMT_NOPDI( r, pstmt );
            /* {                                                                     */
            /*   int rf = duf_sql_finalize( pstmt );                                 */
            /*                                                                       */
            /*   pstmt = NULL;                                                       */
            /*   DUF_TEST_R( rf );                                                   */
            /*   (* DUF_TRACE( action, 0, "FINALIZE %s;", rf < 0 ? "FAIL" : "" ); *) */
            /*                                                                       */
            /*   if ( r >= 0 || r == DUF_SQL_DONE )                                  */
            /*     r = rf;                                                           */
            /*   DUF_TEST_R( r );                                                    */
            /* }                                                                     */
            /* if ( r == DUF_SQL_DONE )                                              */
            /*   r = 0;                                                              */
            /* DUF_TEST_R( r );                                                      */
          }
        }
      }
      if ( sql )
        mas_free( sql );
      sql = NULL;
    }
  }
  else
  {
    DUF_TRACE( explain, 0, "= ? ============ NO selector2 ≪%s≫", node_type == DUF_NODE_LEAF ? "leaf" : "node" );
    r = DUF_ERROR_PTR;
  }
  DUF_TRACE( scan, 3, "(%d) end scan items str_cb2%c", r, str_cb2 ? '+' : '-' );
  DUF_OINV( pdi-> );

  /* DUF_ERROR( "r:%d; sel_cb2:%s", r, DUF_FUNN( sel_cb2 ) ); */
  DUF_OINV_OPENED( pdi-> );
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r;
}

unsigned long long
duf_count_db_vitems2( duf_sel_cb2_match_t match_cb2, duf_depthinfo_t * pdi,
                      duf_scan_callbacks_t * sccb, const char *selector2, const char *fieldset, unsigned long long dirid, int *pr )
{
  unsigned long long cnt = 0;
  int r = 0;

  DEBUG_START(  );

  /* match_cb2 = duf_match_leaf2; */
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  DUF_TEST_R( r );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( selector2 )
  {
    /* duf_sqlite_stmt_t *pstmt = NULL; */
    char *sql = NULL;

    if ( fieldset )
    {
      if ( 0 == strncmp( selector2, "SELECT", 6 ) )
      {
        char *sql3;

        sql3 = duf_sql_mprintf( selector2, fieldset );
        sql = mas_strdup( sql3 );
        mas_free( sql3 );
      }
      else
      {
        sql = mas_strdup( "SELECT " );
        sql = mas_strcat_x( sql, fieldset );
        sql = mas_strcat_x( sql, " " );
        sql = mas_strcat_x( sql, selector2 );
      }
    }
    else
      r = DUF_ERROR_SQL_NO_FIELDSET;

    if ( r >= 0 )
    {
      const char *csql;

      csql = sql;
      /* if ( r >= 0 )                          */
      /*   r = duf_sql_prepare( csql, &pstmt ); */
      DUF_SQL_START_STMT_NOPDI( csql, r, pstmt );

      DUF_TEST_R( r );
      DUF_TRACE( select, 0, "S:%s", csql );

      DUF_TRACE( scan, 2, "sql:%s dirid:%llu", csql, dirid );
      DUF_TEST_R( r );
      {
        /* if ( r >= 0 )                                           */
        /*   r = duf_sql_bind_long_long( pstmt, ":dirid", dirid ); */
        DUF_SQL_BIND_LL( dirid, dirid, r, pstmt );
        /* if ( r >= 0 )                                                                */
        /* {                                                                            */
        /*   r = duf_sql_bind_long_long_nz( pstmt, ":size.min", duf_config->u.size .min); */
        /*   if ( r == DUF_ERROR_BIND_NAME )                                            */
        /*     r = 0;                                                                   */
        /* }                                                                            */
        DUF_SQL_BIND_LL_NZ_OPT( size.min, duf_config->u.size.min, r, pstmt );
        /* if ( r >= 0 )                                                                */
        /* {                                                                            */
        /*   r = duf_sql_bind_long_long_nz( pstmt, ":size.max", duf_config->u.size .max); */
        /*   if ( r == DUF_ERROR_BIND_NAME )                                            */
        /*     r = 0;                                                                   */
        /* }                                                                            */
        DUF_SQL_BIND_LL_NZ_OPT( size.max, duf_config->u.size.max, r, pstmt );
        /* if ( r >= 0 )                                                                */
        /* {                                                                            */
        /*   r = duf_sql_bind_long_long_nz( pstmt, ":same.min", duf_config->u.same .min); */
        /*   if ( r == DUF_ERROR_BIND_NAME )                                            */
        /*     r = 0;                                                                   */
        /* }                                                                            */
        DUF_SQL_BIND_LL_NZ_OPT( same.min, duf_config->u.same.min, r, pstmt );
        /* if ( r >= 0 )                                                                */
        /* {                                                                            */
        /*   r = duf_sql_bind_long_long_nz( pstmt, ":same.max", duf_config->u.same .max); */
        /*   if ( r == DUF_ERROR_BIND_NAME )                                            */
        /*     r = 0;                                                                   */
        /* }                                                                            */
        DUF_SQL_BIND_LL_NZ_OPT( same.max, duf_config->u.same.max, r, pstmt );

        DUF_TEST_R( r );

        while ( r >= 0 && r != DUF_SQL_DONE )
        {
          r = duf_sql_step( pstmt );
          DUF_TEST_RR( r );
          if ( r == DUF_SQL_ROW )
          {
            if ( !match_cb2 || ( match_cb2 ) ( pstmt ) )
              cnt++;
            r = 0;
          }
        }
        DUF_SQL_END_STMT_NOPDI( r, pstmt );
        /* if ( r == DUF_SQL_DONE ) */
        /*   r = 0;                 */
        /* DUF_TEST_R( r );         */
      }
    }
    if ( sql )
      mas_free( sql );
    sql = NULL;

  }
  else
    r = DUF_ERROR_PTR;
  DUF_OINV( pdi-> );

  /* DUF_ERROR( "r:%d; sel_cb2:%s", r, DUF_FUNN( sel_cb2 ) ); */
  DUF_OINV_OPENED( pdi-> );
  DUF_TEST_R( r );
  if ( pr )
    *pr = r;
  DEBUG_ENDULL( r );
  return cnt;
}

/* duf_scan_db_items:
 * call str_cb + pdi as str_cb_udata for each record by sql with corresponding args
 *
 * known str_cb for duf_scan_db_items:
 *   duf_str_cb2_leaf_scan;   duf_str_cb2_leaf_scan is just a wrapper for sccb->leaf_scan2 ; str_cb_udata_unused
 *   duf_str_cb2_scan_file_fd;  duf_str_cb2_scan_file_fd is just a wrapper for sccb->leaf_scan_fd2 ;
 *   duf_str_cb2_uni_scan_dir
 *   ...
 * */

/* called from  duf_scan_files_by_di2 */
int
duf_scan_db_items2( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                    const char *selector2, const char *fieldset, ... )
{
  int r = 0;
  va_list args;

  DEBUG_START(  );
  {
    va_start( args, fieldset );
    if ( selector2 )
    {
      DUF_OINV( pdi-> );
      r = duf_scan_db_vitems2( node_type, str_cb2, pdi, sccb, selector2, fieldset, args );
      DUF_OINV( pdi-> );
      DUF_TEST_R( r );
    }
    else
      r = DUF_ERROR_PTR;
    va_end( args );
  }
  DEBUG_ENDR( r );
  return r;
}
