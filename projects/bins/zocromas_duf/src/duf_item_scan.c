#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_levinfo.h"
#include "duf_pdi.h"


#include "duf_sql_field.h"
#include "duf_sql1.h"
#include "duf_sql2.h"

#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_item_scan.h"
/* ###################################################################### */


int
duf_match_leaf( duf_record_t * precord )
{
  int r;

  DUF_SFIELD( filename );
  DUF_UFIELD( filesize );
  DUF_UFIELD( nsame );
  DUF_UFIELD( md5id );

  r = duf_filename_match( duf_config, filename ) && duf_filesize_match( duf_config, filesize ) && duf_filesame_match( duf_config, nsame )
        && duf_md5id_match( duf_config, md5id );
  return r;
}

int
duf_match_leaf2( duf_sqlite_stmt_t * pstmt )
{
  int r;

  DUF_SFIELD2( filename );
  DUF_UFIELD2( filesize );
  DUF_UFIELD2( nsame );
  DUF_UFIELD2( md5id );

  r = duf_filename_match( duf_config, filename ) && duf_filesize_match( duf_config, filesize ) && duf_filesame_match( duf_config, nsame )
        && duf_md5id_match( duf_config, md5id );
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
int
duf_sel_cb_leaf( duf_record_t * precord, void *sel_cb_udata_unused, duf_str_cb_t str_cb, void *str_cb_udata,
                 duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  DUF_SFIELD( filename );
  DUF_UFIELD( filesize );

  DEBUG_START(  );
  assert( pdi );
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );

  if ( duf_filename_match( duf_config, filename ) && duf_filesize_match( duf_config, filesize ) )
  {
    r = duf_levinfo_down( pdi, 0 /* dirid */ , filename, 0, 0, 1 /* is_leaf */  );
    DUF_TEST_R( r );
    DUF_OINVC( pdi-> );
    if ( r >= 0 )
    {
/*
 * 4. call function str_cb
 * */
      pdi->seq++;
      pdi->seq_leaf++;
      DUF_TEST_R( r );

      /* called both for leaves (files) and nodes (dirs) */
      if ( str_cb )
      {
        r = duf_levinfo_openat_dh( pdi );

        if ( r >= 0 && !duf_levinfo_item_deleted( pdi ) )
        {
          r = ( str_cb ) ( str_cb_udata, pdi, sccb, precord );
        }
        DUF_TEST_R( r );
        /* DUF_ERROR( "r:%d; str_cb:%s", r, DUF_FUNN( str_cb ) ); */
      }
      else
        DUF_TRACE( error, 0, "str_cb not set" );
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
  else
    DUF_TRACE( match, 0, "NOT MATCH %s (mode 1)", filename );
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r;
}

static int
duf_sel_cb2_leaf( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb )
{
  int r = 0;


  DEBUG_START(  );
  assert( pdi );
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );

  /* if ( duf_filename_match( duf_config, filename ) && duf_filesize_match( duf_config, filesize ) ) */
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
int
duf_sel_cb_node( duf_record_t * precord, void *sel_cb_udata_unused, duf_str_cb_t str_cb, void *str_cb_udata,
                 duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  DUF_UFIELD( dirid );
  DUF_UFIELD( ndirs );
  DUF_UFIELD( nfiles );
  DUF_SFIELD( dfname );

  DEBUG_START(  );
  assert( pdi );
  DUF_OINV_OPENED( pdi-> );

  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );

  /* Not here : assert( dirid == duf_levinfo_dirid( pdi ) );
   * */

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
 * 4. call function str_cb
 * */
    pdi->seq++;
    pdi->seq_node++;
    duf_levinfo_countdown_dirs( pdi );

    DUF_OINVC( pdi-> );
    /* called both for leaves (files) and nodes (dirs) */
    if ( str_cb )
    {
      if ( r >= 0 )
      {
        DUF_OINV_NOT_OPENED( pdi-> );
        r = duf_levinfo_openat_dh( pdi );

        if ( r >= 0 && !duf_levinfo_item_deleted( pdi ) )
        {
          DUF_OINV_OPENED( pdi-> );
          DUF_TEST_R( r );
          DUF_TRACE( scan, 1, "  L%u: str_cb node:     by %5llu", duf_pdi_depth( pdi ), dirid );

          if ( r >= 0 )
            r = ( str_cb ) ( str_cb_udata, pdi, sccb, precord );
          DUF_TEST_R( r );
          /* DUF_ERROR( "F:%s", DUF_FUNN( str_cb ) ); */
          DUF_OINV_OPENED( pdi-> );
        }
        DUF_OINV( pdi-> );
        DUF_TEST_R( r );
      }
    }
    else
      DUF_TRACE( error, 0, "str_cb not set" );
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
          DUF_TRACE( scan, 1, "  L%u: str_cb2 node:     by %5llu", duf_pdi_depth( pdi ), dirid );

          if ( r >= 0 )
          {
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
 * known str_cb for duf_scan_db_vitems (to pass to duf_sql_vselect and then to duf_sel_cb_leaf OR duf_sel_cb_node ):
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd;  duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd ; str_cb_udata_unused
 *   duf_str_cb_uni_scan_dir
 *   ...
 * */
static int
duf_scan_db_vitems( duf_node_type_t node_type, duf_str_cb_t str_cb, duf_depthinfo_t * pdi,
                    duf_scan_callbacks_t * sccb, const char *sql, va_list args )
{
  int r = 0;
  duf_sel_cb_t sel_cb = NULL;
  __attribute__ ( ( unused ) ) duf_sel_cb_match_t match_cb = NULL;

  DEBUG_START(  );
/* duf_sel_cb_(node|leaf):
 * this is callback of type: duf_sel_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 * */
  if ( node_type == DUF_NODE_LEAF )
  {
    sel_cb = duf_sel_cb_leaf;
    match_cb = duf_match_leaf;
  }
  else if ( node_type == DUF_NODE_NODE )
    sel_cb = duf_sel_cb_node;
  else
    r = DUF_ERROR_UNKNOWN_NODE;
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  DUF_TRACE( scan, 3, "scan items [%s] sel_cb%c; str_cb%c", node_type == DUF_NODE_LEAF ? "leaf" : "node", sel_cb ? '+' : '-',
             str_cb ? '+' : '-' );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( sel_cb )
    r = duf_sql_vselect( sel_cb, SEL_CB_UDATA_DEF, str_cb, pdi, pdi, sccb, sql, args );



  DUF_TRACE( scan, 3, "(%d) end scan items str_cb%c", r, str_cb ? '+' : '-' );
  DUF_OINV( pdi-> );

  /* DUF_ERROR( "r:%d; sel_cb:%s", r, DUF_FUNN( sel_cb ) ); */
  DUF_OINV_OPENED( pdi-> );
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r;
}

static int
duf_scan_db_vitems2( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi,
                     duf_scan_callbacks_t * sccb, const char *selector2, const char *fieldset, va_list args )
{
  int r = 0;
  duf_sel_cb2_t sel_cb2 = NULL;
  __attribute__ ( ( unused ) ) duf_sel_cb2_match_t match_cb2 = NULL;

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
  }
  else if ( node_type == DUF_NODE_NODE )
    sel_cb2 = duf_sel_cb2_node;
  else
    r = DUF_ERROR_UNKNOWN_NODE;
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  DUF_TRACE( scan, 3, "scan items [%s] sel_cb2%c; str_cb2%c", node_type == DUF_NODE_LEAF ? "leaf" : "node", sel_cb2 ? '+' : '-',
             str_cb2 ? '+' : '-' );
  DUF_TEST_R( r );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( selector2 )
  {
    duf_sqlite_stmt_t *pstmt = NULL;
    unsigned long long dirid = 0;
    dirid = va_arg( args, unsigned long long );

    /* DUF_ERROR( "(%d) selector2:%s", r, selector2 ); */
    {
      char *sql;

      sql = sqlite3_mprintf( selector2, fieldset );
      if ( r >= 0 && sql )
      {
        r = duf_sql_prepare( sql, &pstmt );
        /* DUF_TRACE( action, 0, "PREPARE %s;", r < 0 ? "FAIL" : "" ); */
      }
      DUF_TEST_R( r );

      DUF_TRACE( scan, 2, "sql:%s dirid:%llu", sql, dirid );
      DUF_TEST_R( r );
      {

        if ( r >= 0 )
          r = duf_sql_bind_long_long( pstmt, ":dirid", dirid );
        if ( r >= 0 )
        {
          r = duf_sql_bind_long_long_nz( pstmt, ":minsize", duf_config->u.minsize );
          if ( r == DUF_ERROR_BIND_NAME )
            r = 0;
        }
        if ( r >= 0 )
        {
          r = duf_sql_bind_long_long_nz( pstmt, ":maxsize", duf_config->u.maxsize );
          if ( r == DUF_ERROR_BIND_NAME )
            r = 0;
        }
        if ( r >= 0 )
        {
          r = duf_sql_bind_long_long_nz( pstmt, ":minsame", duf_config->u.minsame );
          if ( r == DUF_ERROR_BIND_NAME )
            r = 0;
        }
        if ( r >= 0 )
        {
          r = duf_sql_bind_long_long_nz( pstmt, ":maxsame", duf_config->u.maxsame );
          if ( r == DUF_ERROR_BIND_NAME )
            r = 0;
        }
        DUF_TEST_R( r );

        if ( r >= 0 )
        {
          int cnt = 0;

          r = duf_sql_reset( pstmt );
          DUF_TEST_R( r );
          /* if ( r >= 0 )                                            */
          /*   r = duf_sql_bind_string( pstmt, ":dirname", bd );      */
          while ( r >= 0 && r != DUF_SQL_DONE )
          {
            r = duf_sql_step( pstmt );
            DUF_TEST_RR( r );
            DUF_TRACE( scan, 1, "#%d sql_step : r:%d; %s", cnt, r, r == DUF_SQL_ROW ? "_ROW" : ( r == DUF_SQL_DONE ? "_DONE" : "_____" ) );

            if ( r == DUF_SQL_ROW )
            {
              if ( !match_cb2 || ( match_cb2 ) ( pstmt ) )
              {
                r = ( sel_cb2 ) ( pstmt, str_cb2, pdi, sccb );
                DUF_TEST_R( r );
                cnt++;
              }
              else
                r = 0;
            }
          }
          DUF_TEST_RR( r );
          {
            int rf = duf_sql_finalize( pstmt );

            pstmt = NULL;
            DUF_TEST_R( rf );
            /* DUF_TRACE( action, 0, "FINALIZE %s;", rf < 0 ? "FAIL" : "" ); */

            if ( r >= 0 || r == DUF_SQL_DONE )
              r = rf;
            DUF_TEST_R( r );
          }
          if ( r == DUF_SQL_DONE )
            r = 0;
          DUF_TEST_R( r );
        }
      }
      sqlite3_free( sql );
      sql = NULL;
    }
  }
  else
    r = DUF_ERROR_PTR;
  DUF_TRACE( scan, 3, "(%d) end scan items str_cb2%c", r, str_cb2 ? '+' : '-' );
  DUF_OINV( pdi-> );

  /* DUF_ERROR( "r:%d; sel_cb2:%s", r, DUF_FUNN( sel_cb2 ) ); */
  DUF_OINV_OPENED( pdi-> );
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r;
}

int
duf_count_db_vitems2( duf_sel_cb2_match_t match_cb2, duf_depthinfo_t * pdi,
                      duf_scan_callbacks_t * sccb, const char *selector2, const char *fieldset, unsigned long long dirid, int *pr )
{
  int cnt = 0;
  int r = 0;

  DEBUG_START(  );

  /* match_cb2 = duf_match_leaf2; */
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  DUF_TEST_R( r );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( selector2 )
  {
    duf_sqlite_stmt_t *pstmt = NULL;

    {
      char *sql;

      sql = sqlite3_mprintf( selector2, fieldset );
      if ( r >= 0 && sql )
        r = duf_sql_prepare( sql, &pstmt );
      DUF_TEST_R( r );

      DUF_TRACE( scan, 2, "sql:%s dirid:%llu", sql, dirid );
      DUF_TEST_R( r );
      {

        if ( r >= 0 )
          r = duf_sql_bind_long_long( pstmt, ":dirid", dirid );
        if ( r >= 0 )
        {
          r = duf_sql_bind_long_long_nz( pstmt, ":minsize", duf_config->u.minsize );
          if ( r == DUF_ERROR_BIND_NAME )
            r = 0;
        }
        if ( r >= 0 )
        {
          r = duf_sql_bind_long_long_nz( pstmt, ":maxsize", duf_config->u.maxsize );
          if ( r == DUF_ERROR_BIND_NAME )
            r = 0;
        }
        if ( r >= 0 )
        {
          r = duf_sql_bind_long_long_nz( pstmt, ":minsame", duf_config->u.minsame );
          if ( r == DUF_ERROR_BIND_NAME )
            r = 0;
        }
        if ( r >= 0 )
        {
          r = duf_sql_bind_long_long_nz( pstmt, ":maxsame", duf_config->u.maxsame );
          if ( r == DUF_ERROR_BIND_NAME )
            r = 0;
        }

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
        if ( r == DUF_SQL_DONE )
          r = 0;
        DUF_TEST_R( r );
      }
      sqlite3_free( sql );
      sql = NULL;
    }
  }
  else
    r = DUF_ERROR_PTR;
  DUF_OINV( pdi-> );

  /* DUF_ERROR( "r:%d; sel_cb2:%s", r, DUF_FUNN( sel_cb2 ) ); */
  DUF_OINV_OPENED( pdi-> );
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r >= 0 ? cnt : r;
}

/* duf_scan_db_items:
 * call str_cb + pdi as str_cb_udata for each record by sql with corresponding args
 *
 * known str_cb for duf_scan_db_items:
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan ; str_cb_udata_unused
 *   duf_str_cb_scan_file_fd;  duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd ; str_cb_udata_unused
 *   duf_str_cb_uni_scan_dir
 *   ...
 * */
int
duf_scan_db_items( duf_node_type_t node_type, duf_str_cb_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sql,
                   ... )
{
  int r = 0;
  va_list args;

  DEBUG_START(  );
  {
    va_start( args, sql );
    {
      DUF_OINV( pdi-> );
      r = duf_scan_db_vitems( node_type, str_cb, pdi, sccb, sql, args );
      DUF_OINV( pdi-> );
      DUF_TEST_R( r );
    }
    va_end( args );
  }
  DEBUG_ENDR( r );
  return r;
}

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
