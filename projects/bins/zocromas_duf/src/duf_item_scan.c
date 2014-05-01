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


#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_item_scan.h"
/* ###################################################################### */




/* duf_sel_cb_leaves:
 * this is callback of type: duf_sel_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 *
 * called both for leaves (files) and nodes (dirs)
 * */
/* will be static! */
int
duf_sel_cb_leaf( duf_record_t * precord, void *sel_cb_udata_unused, duf_str_cb_t str_cb, void *str_cb_udata,
                 duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;
  int rc;

  DUF_SFIELD( filename );

  DEBUG_START(  );
  assert( pdi );
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  DUF_TRACE( scan, 0, "scan leaf L%u", duf_pdi_depth( pdi ) );

  r = duf_levinfo_down( pdi, 0 /* dirid */ , filename, 0, 0, 1 /* is_leaf */  );
  DUF_OINVC( pdi-> );
  if ( r >= 0 )
  {
/*
 * 4. call function str_cb
 * */
    pdi->seq++;
    pdi->seq_leaf++;
    if ( !duf_pdi_max_filter( pdi ) )
      r = DUF_ERROR_MAX_REACHED;
    DUF_TEST_R( r );

    DUF_TRACE( scan, 0, "scan leaf" );
    /* called both for leaves (files) and nodes (dirs) */
    if ( str_cb )
    {
      r = duf_levinfo_openat_dh( pdi );
      if ( r >= 0 )
        r = ( str_cb ) ( str_cb_udata, pdi, sccb, precord   );
      DUF_TEST_R( r );
      /* DUF_ERROR( "r:%d; str_cb:%s", r, DUF_FUNN( str_cb ) ); */
      rc = duf_levinfo_closeat_dh( pdi );
      if ( r >= 0 && rc < 0 )
        r = rc;
    }
    else
      DUF_TRACE( error, 0, "str_cb not set" );
    DUF_OINVC( pdi-> );
    duf_levinfo_up( pdi );
  }
  if ( r == DUF_ERROR_MAX_DEPTH )
    r = 0;
  DUF_OINV_OPENED( pdi-> );
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
 * called both for leaves (files) and nodes (dirs)
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
  DUF_TRACE( scan, 0, "scan node" );
  r = duf_levinfo_down( pdi, dirid, dfname, ndirs, nfiles, 0 /*is_leaf */  );
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
    if ( !duf_pdi_max_filter( pdi ) )
      r = DUF_ERROR_MAX_REACHED;
    DUF_TEST_R( r );

    DUF_OINVC( pdi-> );
    /* called both for leaves (files) and nodes (dirs) */
    if ( str_cb )
    {
      if ( r >= 0 )
      {
        DUF_OINV_NOT_OPENED( pdi-> );
        r = duf_levinfo_openat_dh( pdi );
        {
          DUF_OINV_OPENED( pdi-> );
          DUF_TEST_R( r );
          DUF_TRACE( scan, 0, "scan node" );

          if ( r >= 0 )
            r = ( str_cb ) ( str_cb_udata, pdi, sccb, precord   );
          DUF_TEST_R( r );
          /* DUF_ERROR( "F:%s", DUF_FUNN( str_cb ) ); */
          DUF_OINV_OPENED( pdi-> );
        }
        DUF_OINV( pdi-> );
        if ( r >= 0 )
          r = duf_levinfo_closeat_dh( pdi );
        DUF_OINVC( pdi-> );
        DUF_OINV_NOT_OPENED( pdi-> );
        DUF_TEST_R( r );
      }
    }
    else
      DUF_TRACE( error, 0, "str_cb not set" );
    DUF_OINV_NOT_OPENED( pdi-> );
    duf_levinfo_up( pdi );
    DUF_OINV_OPENED( pdi-> );
  }
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  if ( r == DUF_ERROR_MAX_DEPTH )
    r = 0;
  DUF_OINV_OPENED( pdi-> );
  DUF_TEST_R( r );
  DEBUG_END(  );
  return r;
}

/* 
 * call str_cb + str_cb_udata for each record by sql with corresponding args
 * */
static int
duf_scan_vitems_sql( duf_node_type_t node_type, duf_str_cb_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi,
                     duf_scan_callbacks_t * sccb  , const char *sql, va_list args )
{
  int r = DUF_ERROR_UNKNOWN_NODE;
  duf_sel_cb_t sel_cb = NULL;

  DEBUG_START(  );
/* duf_sel_cb_(node|leaf):
 * this is callback of type: duf_sel_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 * */
  if ( node_type == DUF_NODE_LEAF )
    sel_cb = duf_sel_cb_leaf;
  else if ( node_type == DUF_NODE_NODE )
    sel_cb = duf_sel_cb_node;
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  DUF_TRACE( scan, 0, "scan items [%s] sel_cb%c; str_cb%c", node_type == DUF_NODE_LEAF ? "leaf" : "node", sel_cb ? '+' : '-',
             str_cb ? '+' : '-' );

  if ( sel_cb )
    r = duf_sql_vselect( sel_cb, SEL_CB_UDATA_DEF, str_cb, str_cb_udata, pdi, sccb, sql, args );

  DUF_TRACE( scan, 0, "(%d) end scan items str_cb%c", r, str_cb ? '+' : '-' );
  DUF_OINV( pdi-> );

  /* DUF_ERROR( "r:%d; sel_cb:%s", r, DUF_FUNN( sel_cb ) ); */
  DUF_OINV_OPENED( pdi-> );
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r;
}

/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by sql with corresponding args
 * */
int
duf_scan_items_sql( duf_node_type_t node_type, duf_str_cb_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi,
                    duf_scan_callbacks_t * sccb, const char *sql, ... )
{
  int r = 0;
  va_list args;

  DEBUG_START(  );
  {
    va_start( args, sql );
    {
      DUF_OINV( pdi-> );
      r = duf_scan_vitems_sql( node_type, str_cb, str_cb_udata, pdi, sccb, sql, args );
      DUF_OINV( pdi-> );
      DUF_TEST_R( r );
    }
    va_end( args );
  }
  DEBUG_ENDR( r );
  return r;
}
