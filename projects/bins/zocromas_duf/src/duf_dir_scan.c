#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"
#include "duf_pdi.h"


#include "duf_sql.h"
#include "duf_sql_field.h"
#include "duf_path.h"
#include "duf_item_scan.h"
#include "duf_entry_scan.h"

#include "duf_dbg.h"

#include "duf_file_pathid.h"

/* ###################################################################### */
#include "duf_dir_scan.h"
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
  if ( filesize >= duf_config->u.minsize && ( !duf_config->u.maxsize || filesize < duf_config->u.maxsize ) )
  {
    pdi->items.total++;
    pdi->items.files++;

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
  if ( filesize >= duf_config->u.minsize && ( !duf_config->u.maxsize || filesize < duf_config->u.maxsize ) )
  {
    pdi->items.total++;
    pdi->items.files++;

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

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  dirid = duf_levinfo_dirid( pdi );

  if ( r >= 0 && ( sccb->entry_dir_scan_before || sccb->entry_file_scan_before ) )
    r = duf_scan_entries_by_pathid_and_record( pdi, precord, sccb->entry_file_scan_before, sccb->entry_dir_scan_before );

  if ( r >= 0 && sccb && sccb->node_scan_before && duf_config->cli.act.dirs )
  {
    pdi->items.total++;
    pdi->items.dirs++;

    DUF_OINV_OPENED( pdi-> );
    DUF_TRACE( scan, 0, "scan node before by %5llu", dirid );
    r = sccb->node_scan_before( dirid, pdi, precord );
    DUF_OINV_OPENED( pdi-> );
    DUF_TEST_R( r );
    if ( r == DUF_ERROR_MAX_REACHED )
    {
      if ( pdi->depth == 0 )
        DUF_TRACE( action, 0, "Maximum reached ........" );
      if ( duf_pdi_reldepth( pdi ) == 0 )
        DUF_TRACE( action, 0, "Maximum reached ...." );
    }
  }
  {
    int d = duf_pdi_depth( pdi ) - 1;

    if ( d >= 0 && pdi->levinfo[d].numdir == 0 && !duf_levinfo_is_leaf( pdi ) )
      duf_levinfo_set_eod( pdi );
  }
  /* assert( pdi->depth > 0 ); */
  /* if ( duf_levinfo_numdir_d( pdi, pdi->depth - 1 ) <= 0 ) */
  /* {                                                       */
  /*   DUF_PRINTF(0, "EOD for L%u", pdi->depth - 1 );        */
  /*   pdi->levinfo[pdi->depth - 1].eod = 1;                 */
  /* }                                                       */


  if ( r >= 0 && sccb )
  {
/* duf_scan_files_by_pathid:
 * call sccb->leaf_scan (or sccb->leaf_scan wrapped by duf_str_cb_leaf_scan) + pdi (also) as str_cb_udata
 * 			for each <file> record by dirid with corresponding args
 *
 * */
    if ( r >= 0 && sccb && sccb->leaf_scan_fd )
    {
      DUF_OINV_OPENED( pdi-> );
      DUF_TRACE( scan, 1, "  L%u: scan leaves fd   by %5llu", duf_pdi_depth( pdi ), dirid );

      /* duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd */
      r = duf_scan_files_by_dirid( dirid, duf_str_cb_scan_file_fd, pdi, sccb );

      DUF_OINV_OPENED( pdi-> );
      DUF_TEST_R( r );
    }
    if ( r >= 0 && sccb && sccb->leaf_scan )
    {
      DUF_OINV_OPENED( pdi-> );
      DUF_TRACE( scan, 1, "  L%u: scan leaves ..   by %5llu", duf_pdi_depth( pdi ), dirid );
/* duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan */
      r = duf_scan_files_by_dirid( dirid, duf_str_cb_leaf_scan, pdi, sccb );

      DUF_OINV_OPENED( pdi-> );
      DUF_TEST_R( r );
    }

    if ( r == DUF_ERROR_MAX_REACHED )
    {
      if ( pdi->depth == 0 )
        DUF_TRACE( action, 0, "Maximum reached ........" );
      if ( duf_pdi_reldepth( pdi ) == 0 )
        DUF_TRACE( action, 0, "Maximum reached ...." );
    }
    /* else if ( r < 0 )             */
    /*   DUF_ERROR( "code: %d", r ); */
    DUF_OINV_OPENED( pdi-> );
    if ( sccb && r >= 0 && sccb->node_scan_middle && duf_config->cli.act.dirs )
      r = sccb->node_scan_middle( dirid, pdi, precord );
    DUF_OINV_OPENED( pdi-> );
    DUF_TEST_R( r );

    if ( r >= 0 && sccb )
    {
/* duf_scan_db_items:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
      DUF_OINV( pdi-> );
      DUF_OINV_OPENED( pdi-> );
  
      DUF_TRACE( scan, 0, "  L%u: scan node selector: [%s]", duf_pdi_depth( pdi ), sccb->node_selector );

      r = duf_scan_db_items( DUF_NODE_NODE, str_cb, pdi, sccb, sccb->node_selector,
                             /* pdi->u.minsize,                                                              */
                             /* pdi->u.maxsize ? pdi->u.maxsize : ( unsigned long long ) -1, pdi->u.minsame, */
                             /* pdi->u.maxsame ? pdi->u.maxsame : ( unsigned long long ) -1,                 */
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
    if ( sccb && r >= 0 && sccb->node_scan_after && duf_config->cli.act.dirs )
      r = sccb->node_scan_after( dirid, pdi, precord );


    DUF_OINV_OPENED( pdi-> );
    DUF_TEST_R( r );
  }
  else if ( r >= 0 )
    DUF_ERROR( "no sccb - is it an error? r: %d", r );
  /* if ( r < 0 )                  */
  /*   DUF_ERROR( "code: %d", r ); */
  DUF_OINV_OPENED( pdi-> );
  DUF_TEST_R( r );
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
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
  DUF_TRACE( scan, 3, "by parentid %5llu : %llu : %llu : %llu", dirid, nfiles, minsize, maxsize );
  DUF_TRACE( scan, 0, "  L%u: scan start       by %5llu", duf_pdi_depth( pdi ), dirid );

  if (  /* !nfiles || */ !dirid
       || ( ( ( nfiles >= duf_config->u.mindirfiles ) && ( !duf_config->u.maxdirfiles || nfiles < duf_config->u.maxdirfiles ) )
            /* && ( nfiles == 0
               || ( ( maxsize >= duf_config->u.minsize ) && ( !duf_config->u.maxsize || minsize <= duf_config->u.maxsize ) ) ) */
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
  DUF_TRACE( scan, 0, "  L%u: scan end         by %5llu", duf_pdi_depth( pdi ), dirid );
  return r;
}
