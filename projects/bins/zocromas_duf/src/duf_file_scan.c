#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
/* #include <unistd.h> */
#include <sys/stat.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

/* #include <mastar/tools/mas_arg_tools.h> */

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"
#include "duf_levinfo.h"
#include "duf_pdi.h"


#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_file.h"
#include "duf_path.h"
/* #include "duf_file_pathid.h" */
/* #include "duf_file.h" */

#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_file_scan.h"
/* ###################################################################### */




/* duf_sel_cb_leaves:
 * this is callback of type: duf_sql_select_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 *
 * called both for leaves (files) and nodes (dirs)
 * */
/* will be static! */
int
duf_sel_cb_leaf( duf_record_t * precord, va_list args, void *sel_cb_udata,
                 duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                 const duf_dirhandle_t * pdhu )
{
  int r = 0;

  DEBUG_START(  );
  assert( pdi );
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  r = duf_levinfo_down( pdi, 0, NULL, 0, 0, 1 );
  DUF_OINV( pdi-> );
  if ( r >= 0 )
  {
    DEBUG_STEPULL( pdi->levinfo[pdi->depth].dirid );

/*
 * 4. call function str_cb
 * */
    pdi->seq++;
    pdi->seq_leaf++;
    if ( !duf_pdi_max_filter( pdi ) )
      r = DUF_ERROR_MAX_REACHED;
    DUF_TEST_R( r );

    /* called both for leaves (files) and nodes (dirs) */
    if ( str_cb )
    {
      if ( r >= 0 )
        r = ( str_cb ) ( str_cb_udata, pdi, sccb, precord, pdhu );
      DUF_TEST_R( r );
      /* DUF_ERROR( "r:%d; str_cb:%s", r, DUF_FUNN( str_cb ) ); */
    }
    else
      DUF_TRACE( error, 0, "str_cb not set" );
    DUF_OINV( pdi-> );
    DUF_OINV( pdi-> );
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
 * this is callback of type: duf_sql_select_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 *
 * called both for leaves (files) and nodes (dirs)
 * */
/* will be static! */
int
duf_sel_cb_node( duf_record_t * precord, va_list args, void *sel_cb_udata,
                 duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                 const duf_dirhandle_t * pdhu )
{
  int r = 0;

  DUF_UFIELD( dirid );
  DUF_UFIELD( ndirs );
  DUF_UFIELD( nfiles );
  DUF_SFIELD( dfname );

  DEBUG_START(  );
  assert( pdi );
  DUF_OINV_OPENED( pdi-> );

  if ( 0 == strcmp( dfname, "platforms" ) )
  {
    DUF_TRACE( scan, 0, "dirid:%llu", pdi->levinfo[pdi->depth].dirid );
  }
  DUF_OINV_OPENED( pdi-> );
  DUF_OINV( pdi-> );
  r = duf_levinfo_down( pdi, dirid, dfname, ndirs, nfiles, 0 );
  if ( r != DUF_ERROR_MAX_DEPTH )
    DUF_TEST_R( r );
  /* DUF_ERROR( "r:%d;", r ); */
  if ( r >= 0 )
  {
  DUF_OINV_NOT_OPENED( pdi-> );
    DUF_TRACE( scan, 0, "dirid:%llu", pdi->levinfo[pdi->depth].dirid );

    DEBUG_STEPULL( pdi->levinfo[pdi->depth].dirid );
    DUF_VERBOSE( 1, "call str_cb> %p", ( void * ) ( unsigned long long ) str_cb );
/*
 * 4. call function str_cb
 * */
    pdi->seq++;
    pdi->seq_node++;
    duf_levinfo_countdown_dirs( pdi );
    if ( !duf_pdi_max_filter( pdi ) )
      r = DUF_ERROR_MAX_REACHED;
    DUF_TEST_R( r );

    DUF_OINV( pdi-> );
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
          /* {                                                                                                         */
          /*   struct stat stt;                                                                                        */
          /*   const duf_dirhandle_t *pdht = &pdi->levinfo[pdi->depth].lev_dh;                                         */
          /*                                                                                                           */
          /*   if ( pdht->dfd && 0 == fstat( pdht->dfd, &stt )  )                                                      */
          /*   {                                                                                                       */
          /*     DUF_ERROR( "@@@@@@@@@@@@@@ L %d: %ld %s", pdi->depth, stt.st_ino, pdi->levinfo[pdi->depth].dirname ); */
          /*   }                                                                                                       */
          /* }                                                                                                         */


          if ( r >= 0 )
            r = ( str_cb ) ( str_cb_udata, pdi, sccb, precord, pdhu );
          DUF_TEST_R( r );
          /* DUF_ERROR( "F:%s", DUF_FUNN( str_cb ) ); */
  DUF_OINV_OPENED( pdi-> );
        }
        DUF_OINV( pdi-> );
        {
          int rc;

          rc = duf_levinfo_closeat_dh( pdi );
          if ( r >= 0 )
            r = rc;
        }
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
duf_scan_vitems_sql( duf_node_type_t node_type, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi,
                     duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu, const char *sql, va_list args )
{
  int r = DUF_ERROR_UNKNOWN_NODE;
  duf_sql_select_cb_t sel_cb = NULL;

  DEBUG_START(  );
/* duf_sel_cb_(node|leaf):
 * this is callback of type: duf_sql_select_cb_t (first range): 
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
  if ( sel_cb )
    r = duf_sql_vselect( sel_cb, SEL_CB_UDATA_DEF, str_cb, str_cb_udata, pdi, sccb, pdhu, sql, args );
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
duf_scan_items_sql( duf_node_type_t node_type, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi,
                    duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu, const char *sql, ... )
{
  int r = 0;
  va_list args;

  DEBUG_START(  );
  {
    va_start( args, sql );
    {
      DUF_OINV( pdi-> );
      r = duf_scan_vitems_sql( node_type, str_cb, str_cb_udata, pdi, sccb, pdhu, sql, args );
      DUF_OINV( pdi-> );
      DUF_TEST_R( r );
    }
    va_end( args );
  }
  DEBUG_ENDR( r );
  return r;
}
