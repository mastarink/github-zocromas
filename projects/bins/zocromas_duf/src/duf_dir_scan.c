#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_levinfo.h"


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


/* will be static! */
int
duf_scan_file_fd( void *str_cb_udata_notused, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb,
                  duf_record_t * precord /*, const duf_dirhandle_t * pdhu_unused_off */  )
{
  int r = 0;

  DUF_UFIELD( filesize );
  if ( filesize >= duf_config->u.minsize && ( !duf_config->u.maxsize || filesize < duf_config->u.maxsize ) )
  {
    /* fprintf( stderr, "+FILE: %llu ? %llu : %llu\n", filesize, duf_config->u.minsize, duf_config->u.maxsize ); */
    pdi->items.total++;
    pdi->items.files++;

    r = sccb->leaf_scan_fd( duf_levinfo_dfd( pdi ), duf_levinfo_stat( pdi ), pdi, precord );

    DUF_TEST_R( r );
    DUF_TRACE( scan, 0, "r:%d; sccb->leaf_scan:%s", r, DUF_FUNN( sccb->leaf_scan ) );

    /* DUF_TRACE( action, 0, "r=%d", r ); */
  }
  else
  {
    /* fprintf( stderr, "-FILE: %llu ? %llu : %llu\n", filesize, duf_config->u.minsize, duf_config->u.maxsize ); */
  }
  DUF_TEST_R( r );
  return r;
}

int
duf_scan_file( void *str_cb_udata_notused, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb,
               duf_record_t * precord /*, const duf_dirhandle_t * pdhu_unused_off */  )
{
  int r = 0;

  DUF_UFIELD( filesize );
  if ( filesize >= duf_config->u.minsize && ( !duf_config->u.maxsize || filesize < duf_config->u.maxsize ) )
  {
    /* fprintf( stderr, "+FILE: %llu ? %llu : %llu\n", filesize, duf_config->u.minsize, duf_config->u.maxsize ); */
    pdi->items.total++;
    pdi->items.files++;

    r = sccb->leaf_scan( pdi, precord /*, pdhu_unused_off */  );

    DUF_TEST_R( r );
    DUF_TRACE( scan, 0, "r:%d; sccb->leaf_scan:%s", r, DUF_FUNN( sccb->leaf_scan ) );

    /* DUF_TRACE( action, 0, "r=%d", r ); */
  }
  else
  {
    /* fprintf( stderr, "-FILE: %llu ? %llu : %llu\n", filesize, duf_config->u.minsize, duf_config->u.maxsize ); */
  }
  DUF_TEST_R( r );
  return r;
}

/* duf_scan_dir_by_pi:
 * call duf_scan_items_sql with str_cb + pdi (also) as str_cb_udata and <path> sql
 *   i.e. call str_cb + str_cb_udata 
 *        for each record by standard <path> sql by pathid (i.e. children of pathid) with  corresponding args
 *
 *
 * also call duf_scan_files_by_pathid wirh sccb->leaf_scan
 *   i.e. call sccb->leaf_scan + pdi (also) as str_cb_udata 
 *        for each <file> record by pathid (i.e. children of pathid) with corresponding args
 *
 * i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each file in <rrent> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 * */
static int
duf_scan_dir_by_pi( unsigned long long dirid /*, const duf_dirhandle_t * pdh_off */ , duf_scan_callback_file_t str_cb,
                    duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( r >= 0 && ( sccb->entry_dir_scan_before || sccb->entry_file_scan_before ) )
  {
    r = duf_scan_entries_by_pathid_and_record( dirid, pdi, precord, sccb->entry_file_scan_before, sccb->entry_dir_scan_before );
  }

  if ( r >= 0 && sccb && sccb->node_scan_before && duf_config->cli.act.dirs )
  {
    pdi->items.total++;
    pdi->items.dirs++;

    DUF_OINV_OPENED( pdi-> );
    DUF_TRACE( scan, 0, "scan node before:%llu", dirid );
    r = sccb->node_scan_before( dirid, /*&pdi->levinfo[pdi->depth].lev_dh, */ pdi, precord );
    DUF_OINV_OPENED( pdi-> );
    DUF_TEST_R( r );
    if ( r == DUF_ERROR_MAX_REACHED )
    {
      if ( pdi->depth == 0 )
        DUF_TRACE( action, 0, "Maximum reached ........" );
    }
    /* else if ( r < 0 )             */
    /*   DUF_ERROR( "code: %d", r ); */
  }
  if ( pdi->depth > 0 && !pdi->levinfo[pdi->depth - 1].ndirs )
    pdi->levinfo[pdi->depth - 1].eod = 1;




  if ( r >= 0 && sccb )
  {
    {
/* duf_scan_files_by_pathid:
 * call sccb->leaf_scan (or sccb->leaf_scan wrapped by duf_scan_file) + pdi (also) as str_cb_udata
 * 			for each <file> record by dirid with corresponding args
 *
 * duf_scan_file is called from  duf_leaf_scan.c:duf_sel_cb_items()
 * */
      if ( r >= 0 && sccb && sccb->leaf_scan_fd )
      {
        DUF_OINV_OPENED( pdi-> );
        DUF_TRACE( scan, 0, "scan files by:%llu", dirid );

        r = duf_scan_files_by_dirid( dirid, duf_scan_file_fd /* str_cb */ , pdi, sccb /*, pdh_off */  );

        /* r = duf_scan_files_by_dirid( dirid, sccb->leaf_scan (* str_cb *) , pdi, sccb ); */
        DUF_OINV_OPENED( pdi-> );
        DUF_TEST_R( r );
      }
      if ( r >= 0 && sccb && sccb->leaf_scan )
      {
        DUF_OINV_OPENED( pdi-> );
        DUF_TRACE( scan, 0, "scan files by:%llu", dirid );

        r = duf_scan_files_by_dirid( dirid, duf_scan_file /* str_cb */ , pdi, sccb /*, pdh_off */  );

        /* r = duf_scan_files_by_dirid( dirid, sccb->leaf_scan (* str_cb *) , pdi, sccb ); */
        DUF_OINV_OPENED( pdi-> );
        DUF_TEST_R( r );
      }

      if ( r == DUF_ERROR_MAX_REACHED )
      {
        if ( pdi->depth == 0 )
          DUF_TRACE( action, 0, "Maximum reached ........" );
      }
      /* else if ( r < 0 )             */
      /*   DUF_ERROR( "code: %d", r ); */
    }
    DUF_OINV_OPENED( pdi-> );
    if ( sccb && r >= 0 && sccb->node_scan_middle && duf_config->cli.act.dirs )
      r = sccb->node_scan_middle( dirid, pdi, precord );
    DUF_OINV_OPENED( pdi-> );
    DUF_TEST_R( r );

    if ( r >= 0 && sccb )
    {
/* duf_scan_items_sql:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
      if ( r >= 0 && sccb )
      {
        DUF_OINV( pdi-> );
        DUF_OINV_OPENED( pdi-> );
        r = duf_scan_items_sql( DUF_NODE_NODE, str_cb, pdi, pdi, sccb /*, pdh_off */ , sccb->node_selector,
                                /* pdi->u.minsize,                                                              */
                                /* pdi->u.maxsize ? pdi->u.maxsize : ( unsigned long long ) -1, pdi->u.minsame, */
                                /* pdi->u.maxsame ? pdi->u.maxsame : ( unsigned long long ) -1,                 */
                                dirid );
        DUF_OINV_OPENED( pdi-> );
        DUF_TEST_R( r );
      }

      if ( r == DUF_ERROR_MAX_REACHED )
      {
        if ( pdi->depth == 0 )
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
 * see duf_scan_dir_by_pi
 * */
int
duf_scan_dirs_by_parentid( unsigned long long dirid /*, const duf_dirhandle_t * pdh_off */ , duf_scan_callback_file_t str_cb,
                           duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_record_t * precord )
{
  int r = 0;

  DUF_UFIELD_OPT( nfiles );
  DUF_UFIELD_OPT( minsize );
  DUF_UFIELD_OPT( maxsize );
  DUF_TRACE( scan, 0, "by parentid %llu : %llu : %llu : %llu", dirid, nfiles, minsize, maxsize );

  DUF_OINV( pdi-> );
  DUF_OINV_OPENED( pdi-> );
  /* {                                                                    */
  /*   struct stat stt;                                                   */
  /*                                                                      */
  /*   if ( pdh_off && pdh_off->dfd && 0 == fstat( pdh_off->dfd, &stt ) )             */
  /*   {                                                                  */
  /*     DUF_TRACE( fs, 0, "fstat OK %d: %ld ", pdi->depth, stt.st_ino ); */
  /*   }                                                                  */
  /* }                                                                    */
  /* DUF_OINV_OPENED( pdi-> );                                            */

  if (  /* !nfiles || */ !dirid
       || ( ( ( nfiles >= duf_config->u.mindirfiles ) && ( !duf_config->u.maxdirfiles || nfiles < duf_config->u.maxdirfiles ) )
            /* && ( nfiles == 0
               || ( ( maxsize >= duf_config->u.minsize ) && ( !duf_config->u.maxsize || minsize <= duf_config->u.maxsize ) ) ) */
        ) )
  {
    DUF_OINV_OPENED( pdi-> );
    DUF_TRACE( scan, 0, "scan dirid:%llu", dirid );
    r = duf_scan_dir_by_pi( dirid /*, pdh_off */ , str_cb, pdi, sccb, precord );
    DUF_OINV_OPENED( pdi-> );
    DUF_TEST_R( r );
  }
  else
  {
    DUF_TRACE( scan, 0, "off; dirid:%llu", dirid );
    /* fprintf( stderr, "----dirid: %llu : ( N=%llu : %llu ) : ( %llu : %llu ) : ( %llu : %llu )\n", dirid, nfiles, duf_config->u.mindirfiles, */
    /*          minsize, duf_config->u.minsize, maxsize, duf_config->u.maxsize );                                                              */
  }
  DUF_TEST_R( r );
  DUF_OINV( pdi-> );
  DUF_OINV_OPENED( pdi-> );
  DUF_TRACE( scan, 0, "end scan dirid %llu", dirid );
  return r;
}
