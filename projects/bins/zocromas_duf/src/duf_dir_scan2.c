#include <string.h>
#include <unistd.h>

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

#include "duf_options.h"
/* #include "duf_cli_options.h" */

#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_item_scan1.h"
#include "duf_item_scan2.h"

#include "duf_path.h"

#include "duf_dirent_scan2.h"

#include "duf_file_pathid.h"

#include "duf_dbg.h"

#include "duf_sccb.h"

/* ###################################################################### */
#include "duf_dir_scan2.h"
/* ###################################################################### */


/* 
 * this is callback of type: duf_str_cb_t (first range; str_cb) 
 *
 * duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd
 * */
static int
duf_str_cb2_scan_file_fd( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb )
{
  int r = 0;

  DUF_UFIELD2( filesize );
  DUF_TRACE( scan, 10, "+" );
  if ( filesize >= duf_config->u.size.min && ( !duf_config->u.size.max || filesize < duf_config->u.size.max ) )
  {
    pdi->items.total++;
    pdi->items.files++;

    if ( sccb->leaf_scan_fd2 )
      r = sccb->leaf_scan_fd2( pstmt, duf_levinfo_dfd( pdi ), duf_levinfo_stat( pdi ), pdi );

    DUF_TEST_R( r );
    DUF_TRACE( scan, 14, "r:%d; sccb->leaf_scan_fd:%s", r, DUF_FUNN( sccb->leaf_scan_fd2 ) );

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
duf_str_cb2_leaf_scan( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb )
{
  int r = 0;

  DUF_UFIELD2( filesize );
  DUF_SFIELD2( filename );
  DUF_TRACE( scan, 10, "+" );
  DUF_TRACE( explain, 0, "= ? ============ str cb2 leaf scan => ≪leaf_scan2≫" );
  if ( filesize >= duf_config->u.size.min && ( !duf_config->u.size.max || filesize < duf_config->u.size.max ) )
  {
    pdi->items.total++;
    pdi->items.files++;

    if ( duf_levinfo_item_deleted( pdi ) )
    {
      if ( sccb->leaf_scan2_deleted )
        r = sccb->leaf_scan2_deleted( pstmt, pdi );
      DUF_TRACE( scan, 13, "r:%d; sccb->leaf_scan2_deleted:%s", r, DUF_FUNN( sccb->leaf_scan2_deleted ) );
      DUF_TRACE( deleted, 0, "DELETED '%s%s'", duf_levinfo_path( pdi ), filename );
    }
    else
    {
      if ( sccb->leaf_scan2 )
        r = sccb->leaf_scan2( pstmt, pdi );
      DUF_TRACE( scan, 13, "r:%d; sccb->leaf_scan:%s", r, DUF_FUNN( sccb->leaf_scan2 ) );
    }

    DUF_TEST_R( r );
  }
  DUF_TEST_R( r );
  return r;
}

static int
duf_qscan_dirents2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;
  unsigned long long dirid;

  dirid = duf_levinfo_dirid( pdi );

  if ( DUF_ACT_FLAG( dirent ) )
  {
    duf_pdi_set_opendir( pdi, 1 );
    DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>>q +dirent" );
    if ( sccb->dirent_dir_scan_before2 || sccb->dirent_file_scan_before2 )
    {
      DUF_TRACE( scan, 10, "scan dirent_dir by %5llu", dirid );
      r = duf_scan_dirents2( pstmt, pdi, sccb->dirent_file_scan_before2, sccb->dirent_dir_scan_before2 );
    }
    else
    {
      DUF_TRACE( scan, 10, "NOT scan dirent_dir by %5llu - sccb->dirent_dir_scan_before2 empty and sccb->dirent_file_scan_before2 for %s",
                 dirid, duf_uni_scan_action_title( sccb ) );
    }
  }
  else if ( sccb->dirent_dir_scan_before2 || sccb->dirent_file_scan_before2 )
  {
    char *ona = NULL;

    DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> -dirent" );
    ona = duf_option_names( DUF_OPTION_FLAG_DIRENT );
    DUF_PRINTF( 0, "to scan dir / file before2 use %s", ona );
    mas_free( ona );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan dirent_dir ( -E or --dirent absent )" );
  }
  return r;
}

int
duf_qscan_node_scan_before2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;
  unsigned long long dirid;

  dirid = duf_levinfo_dirid( pdi );
  if ( DUF_ACT_FLAG( dirs ) )
  {
    unsigned long long n;

    n = duf_count_db_vitems2( duf_match_leaf2, pdi, sccb, sccb->leaf_selector2, sccb->leaf_fieldset, dirid, &r );
    DUF_TEST_R( r );
    if ( r >= 0 )
    {
      DUF_TRACE( scan, 10, "count items %llu", n );
      duf_levinfo_set_items_files( pdi, n );

      pdi->items.total++;
      pdi->items.dirs++;

      DUF_OINV_OPENED( pdi-> );
      if ( duf_levinfo_item_deleted( pdi ) )
      {
        if ( sccb->node_scan_before2_deleted )
        {
          DUF_TRACE( scan, 10, "scan node before2_deleted by %5llu", dirid );
          r = sccb->node_scan_before2_deleted( pstmt, dirid, pdi );
        }
        DUF_TRACE( deleted, 0, "DELETED" );
      }
      else if ( sccb->node_scan_before2 )
      {
        DUF_TRACE( scan, 10, "scan node before2 by %5llu", dirid );
        r = sccb->node_scan_before2( pstmt, dirid, pdi );
      }
      else
      {
        DUF_TRACE( scan, 10, "NOT scan node before2 by %5llu - sccb->node_scan_before2 empty for %s", dirid,
                   duf_uni_scan_action_title( sccb ) );
      }
      DUF_TEST_R( r );
    }
  }
  else if ( sccb->node_scan_before2 )
  {
    char *ona = NULL;

    ona = duf_option_names( DUF_OPTION_FLAG_DIRS );
    DUF_PRINTF( 0, "to scan node before2 use %s", ona );
    mas_free( ona );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan before2 ( -d or --dirs absent )" );
  }
  return r;
}

int
duf_qscan_node_scan_middle2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;
  unsigned long long dirid;

  dirid = duf_levinfo_dirid( pdi );
  if ( DUF_ACT_FLAG( dirs ) )
  {
    DUF_OINV_OPENED( pdi-> );
    if ( duf_levinfo_item_deleted( pdi ) )
    {
      if ( sccb->node_scan_middle2_deleted )
      {
        DUF_TRACE( scan, 10, "scan node middle2_deleted by %5llu", dirid );
        r = sccb->node_scan_middle2_deleted( pstmt, dirid, pdi );
      }
      DUF_TRACE( deleted, 0, "DELETED" );
    }
    else if ( sccb->node_scan_middle2 )
    {
      DUF_TRACE( scan, 10, "scan node middle2 by %5llu", dirid );
      r = sccb->node_scan_middle2( pstmt, dirid, pdi );
    }
    else
    {
      DUF_TRACE( scan, 10, "NOT scan node middle2 by %5llu - sccb->node_scan_middle2 empty for %s", dirid,
                 duf_uni_scan_action_title( sccb ) );
    }
    DUF_TEST_R( r );
  }
  else if ( sccb->node_scan_middle2 )
  {
    char *ona = NULL;

    ona = duf_option_names( DUF_OPTION_FLAG_DIRS );
    DUF_PRINTF( 0, "to scan node middle2 use %s", ona );
    mas_free( ona );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan middle2 ( -d or --dirs absent )" );
  }
  return r;
}

int
duf_qscan_node_scan_after2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;
  unsigned long long dirid;

  dirid = duf_levinfo_dirid( pdi );
  if ( DUF_ACT_FLAG( dirs ) )
  {
    DUF_OINV_OPENED( pdi-> );
    if ( duf_levinfo_item_deleted( pdi ) )
    {
      if ( sccb->node_scan_after2_deleted )
      {
        DUF_TRACE( scan, 10, "scan node after2_deleted by %5llu", dirid );
        r = sccb->node_scan_after2_deleted( pstmt, dirid, pdi );
      }
      DUF_TRACE( deleted, 0, "DELETED" );
    }
    else if ( sccb->node_scan_after2 )
    {
      DUF_TRACE( scan, 10, "scan node after2 by %5llu", dirid );
      r = sccb->node_scan_after2( pstmt, dirid, pdi );
    }
    else
    {
      DUF_TRACE( scan, 10, "NOT scan node after2 by %5llu - sccb->node_scan_after2 empty for %s", dirid,
                 duf_uni_scan_action_title( sccb ) );
    }
    DUF_TEST_R( r );
  }
  else if ( sccb->node_scan_after2 )
  {
    char *ona = NULL;

    ona = duf_option_names( DUF_OPTION_FLAG_DIRS );
    DUF_PRINTF( 0, "to scan node after2  use %s", ona );
    mas_free( ona );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan after2 ( -d or --dirs absent )" );
  }
  return r;
}

int
duf_qscan_files_by_dirid2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;
  unsigned long long dirid;

  dirid = duf_levinfo_dirid( pdi );

/* duf_scan_files_by_pathid:
 * call sccb->leaf_scan (or sccb->leaf_scan wrapped by duf_str_cb_leaf_scan) + pdi (also) as str_cb_udata
 * 			for each <file> record by dirid with corresponding args
 *
 * */
  /* scan this files in this directory */
  if ( r >= 0 && DUF_ACT_FLAG( files ) )
  {
    DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 2." );
    if ( r >= 0 && sccb->leaf_scan_fd2 )
    {
      DUF_OINV_OPENED( pdi-> );
      DUF_TRACE( scan, 11, "  " DUF_DEPTH_PFMT ": scan leaves_scan2 fd   by %5llu", duf_pdi_depth( pdi ), dirid );

      /* duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd */
      r = duf_scan_files_by_dirid2( dirid, duf_str_cb2_scan_file_fd, pdi, sccb );

      DUF_OINV_OPENED( pdi-> );
      DUF_TEST_R( r );
    }
    else
    {
      DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": NOT scan leaves_scan2 fd   by %5llu", duf_pdi_depth( pdi ), dirid );
    }
    if ( r >= 0 && sccb->leaf_scan2 )
    {
      DUF_OINV_OPENED( pdi-> );
      DUF_TRACE( scan, 11, "  " DUF_DEPTH_PFMT ": scan leaves_scan2 ..   by %5llu", duf_pdi_depth( pdi ), dirid );
/* duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan */
      r = duf_scan_files_by_dirid2( dirid, duf_str_cb2_leaf_scan, pdi, sccb );

      DUF_OINV_OPENED( pdi-> );
      DUF_TEST_R( r );
    }
    else
    {
      DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": NOT scan leaves_scan2    by %5llu", duf_pdi_depth( pdi ), dirid );
    }
  }
  else if ( sccb->leaf_scan_fd2 )
  {
    char *ona = NULL;

    ona = duf_option_names( DUF_OPTION_FLAG_FILES );
    DUF_PRINTF( 0, "to scan fd2 use %s", ona );
    mas_free( ona );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan leaf_scan_fd2 and leaf_scan2  ( -f or --files absent )" );
  }
  return r;
}

int
duf_qscan_dirs_by_dirid2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_str_cb2_t str_cb2 )
{
  int r;
  unsigned long long dirid;

  dirid = duf_levinfo_dirid( pdi );

  /* scan directories in this directory */
/* duf_scan_db_items2:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
  DUF_OINV( pdi-> );
  DUF_OINV_OPENED( pdi-> );

  DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan node selector2: [%s]", duf_pdi_depth( pdi ), sccb->node_selector2 );

  if ( !sccb->node_selector2 )
  {
    DUF_ERROR( "sccb->node_selector2 empty for %s", duf_uni_scan_action_title( sccb ) );
    r = DUF_ERROR_PTR;
    DUF_TEST_R( r );
  }

/* calling duf_sel_cb_(node|leaf) for each record by sccb->node_selector2 */
  if ( r >= 0 && sccb->node_selector2 )
    r = duf_scan_db_items2( DUF_NODE_NODE, str_cb2, pdi, sccb, sccb->node_selector2, sccb->node_fieldset /* fieldset */ , dirid );
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
duf_scan_dir_by_pi2( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  DEBUG_START(  );
  assert( sccb );
  DUF_TRACE( scan, 10, "by pi2" );


  DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 1." );

  /* scan dirent's */
  r = duf_qscan_dirents2( pstmt, pdi, sccb );


  if ( r >= 0 )
    r = duf_qscan_node_scan_before2( pstmt, pdi, sccb );

  {
    int d = duf_pdi_depth( pdi ) - 1;

    if ( d >= 0 && duf_levinfo_numdir_d( pdi, d ) == 0 && !duf_levinfo_is_leaf( pdi ) )
      duf_levinfo_set_eod( pdi );
  }
  if ( r >= 0 )
    r = duf_qscan_files_by_dirid2( pstmt, pdi, sccb );

  DUF_OINV_OPENED( pdi-> );
  if ( r >= 0 )
    r = duf_qscan_node_scan_middle2( pstmt, pdi, sccb );

  if ( r >= 0 )
    r = duf_qscan_dirs_by_dirid2( pstmt, pdi, sccb, str_cb2 );

  DUF_OINV_OPENED( pdi-> );
  if ( r >= 0 )
    r = duf_qscan_node_scan_after2( pstmt, pdi, sccb );


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
duf_scan_dirs_by_parentid2( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;
  unsigned long long dirid;

  DUF_UFIELD2( nfiles );
  DUF_UFIELD2( minsize );
  DUF_UFIELD2( maxsize );

  DUF_OINV( pdi-> );
  DUF_OINV_OPENED( pdi-> );


  dirid = duf_levinfo_dirid( pdi );

  DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, "** depth:%d/%d; dirid:%llu:%s", duf_pdi_depth( pdi ),
                duf_pdi_reldepth( pdi ), dirid, duf_pathid_to_path2_tmp( dirid, pdi ) );

  DUF_TRACE( scan, 10, "by_parentid2" );
  DUF_TRACE( scan, 13, "by parentid2 %5llu : %llu : %llu : %llu", dirid, nfiles, minsize, maxsize );
  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan start       by %5llu", duf_pdi_depth( pdi ), dirid );

  if (  /* !nfiles || */ !dirid
       || ( ( ( nfiles >= duf_config->u.dirfiles.min ) && ( !duf_config->u.dirfiles.max || nfiles < duf_config->u.dirfiles.max ) )
            /* && ( nfiles == 0
               || ( ( maxsize >= duf_config->u.size .min) && ( !duf_config->u.size .max|| minsize <= duf_config->u.size .max) ) ) */
        ) )
  {
    DUF_OINV_OPENED( pdi-> );
    DUF_TRACE( scan, 11, "scan dir by pi2 id%5llu", dirid );
    r = duf_scan_dir_by_pi2( pstmt, str_cb2, pdi, sccb );
    DUF_OINV_OPENED( pdi-> );
    DUF_TEST_R( r );
  }
  else
  {
    DUF_TRACE( scan, 10, "off; by %5llu", dirid );
  }
  DUF_TEST_R( r );
  DUF_OINV( pdi-> );
  DUF_OINV_OPENED( pdi-> );
  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan end         by %5llu", duf_pdi_depth( pdi ), dirid );
  return r;
}
