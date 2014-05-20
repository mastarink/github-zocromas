#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"


#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"
#include "duf_pdi.h"

#include "duf_options.h"
/* #include "duf_cli_options.h" */

#include "duf_sql_defs.h"
#include "duf_sql_field1.h"
#include "duf_item_scan1.h"
/* #include "duf_item_scan2.h" */
#include "duf_dirent_scan.h"
#include "duf_file_pathid.h"

#include "duf_dbg.h"


/* ###################################################################### */
#include "duf_dir_scan1.h"
/* ###################################################################### */





/* 
 * this is callback of type: duf_str_cb_t (first range; str_cb) 
 *
 * duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd
 * */
static int
duf_str_cb_scan_file_fd( void *str_cb_udata_unused, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb, duf_record_t * precord )
{
  int r = 0;

  DUF_UFIELD( filesize );
  DUF_TRACE( scan, 0, "+" );
  if ( filesize >= duf_config->u.size.min && ( !duf_config->u.size.max || filesize < duf_config->u.size.max ) )
  {
    pdi->items.total++;
    pdi->items.files++;
    if ( sccb->leaf_scan_fd )
      r = sccb->leaf_scan_fd( duf_levinfo_dfd( pdi ), duf_levinfo_stat( pdi ), pdi, precord );

    DUF_TEST_R( r );
    DUF_TRACE( scan, 4, "r:%d; sccb->leaf_scan_fd:%s", r, DUF_FUNN( sccb->leaf_scan_fd ) );

    /* DUF_TRACE( action, 0, "r=%d", r ); */
  }
  DUF_TEST_R( r );
  return r;
}

/* 
 * this is callback of type: duf_str_cb_t (first range; str_cb) 
 *
 * duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 * */
int
duf_str_cb_leaf_scan( void *str_cb_udata_unused, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb, duf_record_t * precord )
{
  int r = 0;

  DUF_UFIELD( filesize );
  DUF_TRACE( scan, 0, "+" );
  if ( filesize >= duf_config->u.size.min && ( !duf_config->u.size.max || filesize < duf_config->u.size.max ) )
  {
    pdi->items.total++;
    pdi->items.files++;

    if ( sccb->leaf_scan )
      r = sccb->leaf_scan( pdi, precord );

    DUF_TEST_R( r );
    DUF_TRACE( scan, 3, "r:%d; sccb->leaf_scan:%s", r, DUF_FUNN( sccb->leaf_scan ) );
  }
  DUF_TEST_R( r );
  return r;
}

/* duf_scan_dir_by_pi:
 * call duf_scan_db_items with str_cb + pdi (also) as str_cb_udata and <path> sql
 *   i.e. call str_cb + str_cb_udata 
 *        for each record by standard <path> sql by dirid (i.e. children of dirid) with  corresponding args
 *
 *
 * also call duf_scan_files_by_pathid wirh sccb->leaf_scan
 *   i.e. call sccb->leaf_scan + pdi (also) as str_cb_udata 
 *        for each <file> record by dirid (i.e. children of dirid) with corresponding args
 *
 * i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each file in <rrent> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 *
 * known str_cb for duf_scan_dir_by_pi:
 *   duf_str_cb_uni_scan_dir
 *
 * */
static int
duf_scan_dir_by_pi( duf_str_cb_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  int r = 0;
  unsigned long long dirid;

  DEBUG_START(  );
  dirid = duf_levinfo_dirid( pdi );
  DUF_TRACE( scan, 0, "by pi" );

  if ( r >= 0 && ( sccb->dirent_dir_scan_before || sccb->dirent_file_scan_before ) )
    r = duf_scan_dirents_by_pathid_and_record( pdi, precord, sccb->dirent_file_scan_before, sccb->dirent_dir_scan_before );

  if ( r >= 0 && sccb && DUF_ACT_FLAG( dirs ) )
  {
    pdi->items.total++;
    pdi->items.dirs++;

    DUF_OINV_OPENED( pdi-> );
    DUF_TRACE( scan, 0, "scan node before by %5llu", dirid );
    if ( sccb->node_scan_before )
      r = sccb->node_scan_before( dirid, pdi, precord );
    DUF_TEST_R( r );
  }
  {
    int d = duf_pdi_depth( pdi ) - 1;

    if ( d >= 0 && duf_levinfo_numdir_d( pdi, d ) == 0 && !duf_levinfo_is_leaf( pdi ) )
      duf_levinfo_set_eod( pdi );
  }

  if ( r >= 0 && sccb )
  {
/* duf_scan_files_by_pathid:
 * call sccb->leaf_scan (or sccb->leaf_scan wrapped by duf_str_cb_leaf_scan) + pdi (also) as str_cb_udata
 * 			for each <file> record by dirid with corresponding args
 *
 * */
    if ( DUF_ACT_FLAG( files ) )
    {
      if ( r >= 0 && sccb->leaf_scan_fd )
      {
        DUF_OINV_OPENED( pdi-> );
        DUF_TRACE( scan, 1, "  " DUF_DEPTH_PFMT ": scan leaves fd   by %5llu", duf_pdi_depth( pdi ), dirid );

        /* duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd */
        r = duf_scan_files_by_dirid( dirid, duf_str_cb_scan_file_fd, pdi, sccb );

        DUF_OINV_OPENED( pdi-> );
        DUF_TEST_R( r );
      }
      if ( r >= 0 && sccb->leaf_scan )
      {
        DUF_OINV_OPENED( pdi-> );
        DUF_TRACE( scan, 1, "  " DUF_DEPTH_PFMT ": scan leaves ..   by %5llu", duf_pdi_depth( pdi ), dirid );
/* duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan */
        r = duf_scan_files_by_dirid( dirid, duf_str_cb_leaf_scan, pdi, sccb );

        DUF_OINV_OPENED( pdi-> );
        DUF_TEST_R( r );
      }
    }
    DUF_OINV_OPENED( pdi-> );
    if ( r >= 0 && sccb->node_scan_middle && DUF_ACT_FLAG( dirs ) )
      r = sccb->node_scan_middle( dirid, pdi, precord );
    DUF_OINV_OPENED( pdi-> );
    DUF_TEST_R( r );

    if ( r >= 0 )
    {
/* duf_scan_db_items:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
      DUF_OINV( pdi-> );
      DUF_OINV_OPENED( pdi-> );

      DUF_TRACE( scan, 0, "  " DUF_DEPTH_PFMT ": scan node selector: [%s]", duf_pdi_depth( pdi ), sccb->node_selector );

/* calling duf_sel_cb_(node|leaf) for each record by sccb->node_selector */
      r = duf_scan_db_items( DUF_NODE_NODE, str_cb, pdi, sccb, sccb->node_selector,
                             /* pdi->u.size.min,                                                              */
                             /* pdi->u.size .max? pdi->u.size .max: ( unsigned long long ) -1, pdi->u.same.min, */
                             /* pdi->u.same .max? pdi->u.same .max: ( unsigned long long ) -1,                 */
                             dirid );
      DUF_OINV_OPENED( pdi-> );
      DUF_TEST_R( r );

      if ( r == DUF_ERROR_MAX_REACHED )
      {
        if ( pdi->depth == 0 )
          DUF_TRACE( action, 0, "Maximum reached ...." );
        if ( duf_pdi_reldepth( pdi ) == 0 )
          DUF_TRACE( action, 0, "Maximum reached ...." );
      }
      /* else if ( r < 0 )             */
      /*   DUF_ERROR( "code: %d", r ); */
    }
    DUF_OINV_OPENED( pdi-> );
    if ( r >= 0 && DUF_ACT_FLAG( dirs ) )
    {
      if ( sccb->node_scan_after )
        r = sccb->node_scan_after( dirid, pdi, precord );
    }
    else
    {
      char *ona = NULL;

      ona = duf_option_names( DUF_OPTION_FLAG_DIRS );
      DUF_PRINTF( 0, "to scan dir / file before2 use %s", ona );
      mas_free( ona );
    }

    DUF_OINV_OPENED( pdi-> );
    DUF_TEST_R( r );
  }
  else if ( r >= 0 )
    DUF_ERROR( "no sccb - is it an error? r: %d", r );
  /* if ( r < 0 )                  */
  /*   DUF_ERROR( "code: %d", r ); */
  DUF_OINV_OPENED( pdi-> );
  DUF_TEST_R( r );
  DEBUG_ENDR( r );
  return r;
}

/* duf_scan_dirs_by_parentid
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 *
 * known str_cb for duf_scan_dirs_by_parentid:
 *   duf_str_cb_uni_scan_dir
 *
 * see duf_scan_dir_by_pi
 * */
int
duf_scan_dirs_by_parentid( duf_str_cb_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  int r = 0;
  unsigned long long dirid;

  DUF_UFIELD_OPT( nfiles );
  DUF_UFIELD_OPT( minsize );
  DUF_UFIELD_OPT( maxsize );

  DUF_OINV( pdi-> );
  DUF_OINV_OPENED( pdi-> );


  dirid = duf_levinfo_dirid( pdi );
  DUF_TRACE( scan, 0, "by_parentid" );
  DUF_TRACE( scan, 3, "by parentid %5llu : %llu : %llu : %llu", dirid, nfiles, minsize, maxsize );
  DUF_TRACE( scan, 0, "  " DUF_DEPTH_PFMT ": scan start       by %5llu", duf_pdi_depth( pdi ), dirid );

  if (  /* !nfiles || */ !dirid
       || ( ( ( nfiles >= duf_config->u.dirfiles.min ) && ( !duf_config->u.dirfiles.max || nfiles < duf_config->u.dirfiles.max ) )
            /* && ( nfiles == 0
               || ( ( maxsize >= duf_config->u.size .min) && ( !duf_config->u.size .max|| minsize <= duf_config->u.size .max) ) ) */
        ) )
  {
    DUF_OINV_OPENED( pdi-> );
    DUF_TRACE( scan, 3, "scan by %5llu", dirid );
    r = duf_scan_dir_by_pi( str_cb, pdi, sccb, precord );
    DUF_OINV_OPENED( pdi-> );
    DUF_TEST_R( r );
  }
  else
  {
    DUF_TRACE( scan, 0, "off; by %5llu", dirid );
  }
  DUF_TEST_R( r );
  DUF_OINV( pdi-> );
  DUF_OINV_OPENED( pdi-> );
  DUF_TRACE( scan, 0, "  " DUF_DEPTH_PFMT ": scan end         by %5llu", duf_pdi_depth( pdi ), dirid );
  return r;
}
