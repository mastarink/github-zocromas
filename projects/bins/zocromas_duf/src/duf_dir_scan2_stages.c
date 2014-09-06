#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_service.h"
#include "duf_config_ref.h"
#include "duf_match.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_item_scan2.h"

#include "duf_dirent_scan2.h"

#include "duf_file_pathid2.h"

#include "duf_sccb.h"
#include "duf_option_names.h"

/* ###################################################################### */
#include "duf_dir_scan2_stages.h"
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

  /*
   * 20140831.142148
   * DUF_UFIELD2( filesize );
   * if ( dufOFF_lim_matchll( duf_config->u.size, filesize ) ) */
  {
    pdi->items.total++;
    pdi->items.files++;
    assert( duf_levinfo_dfd( pdi ) );
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

static int
duf_str_cb2_leaf_scan( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb )
{
  int r = 0;

  DUF_SFIELD2( filename );
  DUF_TRACE( scan, 10, "+" );
  DUF_TRACE( explain, 5, "= ? ============ str cb2 leaf scan => ≪leaf_scan2≫" );

  /* if ( filesize >= duf_config->u.size.min && ( !duf_config->u.size.max || filesize < duf_config->u.size.max ) ) */

  /* 20140823.101349
   * DUF_UFIELD2( filesize );
   * if ( dufOFF_lim_matchll( duf_config->u.size, filesize ) )
   * */
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

/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem with necessary info:
 * */
int
duf_qscan_dirents2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  if ( DUF_ACT_FLAG( dirent ) )
  {
    duf_pdi_set_opendir( pdi, 1 );
    DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>>q +dirent" );
    if ( sccb->dirent_dir_scan_before2 || sccb->dirent_file_scan_before2 )
    {
      DUF_TRACE( scan, 10, "scan dirent_dir by %5llu", duf_levinfo_dirid( pdi ) );
      /*
       *   -- call for each direntry
       *      - for directory                - sccb->dirent_dir_scan_before2
       *      - for other (~ regular) entry  - sccb->dirent_file_scan_before2
       * */
      r = duf_scan_dirents2( pdi, sccb->dirent_file_scan_before2, sccb->dirent_dir_scan_before2 );
    }
    else
    {
      DUF_TRACE( scan, 10, "NOT scan dirent_dir by %5llu - sccb->dirent_dir_scan_before2 empty and sccb->dirent_file_scan_before2 for %s",
                 duf_levinfo_dirid( pdi ), duf_uni_scan_action_title( sccb ) );
    }
  }
  else if ( sccb->dirent_dir_scan_before2 || sccb->dirent_file_scan_before2 )
  {
    char *ona = NULL;

    DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> -dirent" );
    ona = duf_option_names( DUF_OPTION_FLAG_DIRENT );
    DUF_TRACE( explain, 0, "to scan dir / file before2 use %s", ona );
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
  unsigned long long diridpdi;

  diridpdi = duf_levinfo_dirid( pdi );
  if ( DUF_ACT_FLAG( dirs ) )
  {
    pdi->items.total++;
    pdi->items.dirs++;

    if ( duf_levinfo_item_deleted( pdi ) )
    {
      if ( sccb->node_scan_before2_deleted )
      {
        DUF_TRACE( scan, 10, "scan node before2_deleted by %5llu", diridpdi );
        r = sccb->node_scan_before2_deleted( pstmt, diridpdi, pdi );
      }
      DUF_TRACE( deleted, 0, "DELETED" );
    }
    else if ( sccb->node_scan_before2 )
    {
      DUF_TRACE( scan, 10, "scan node before2 by %5llu", diridpdi );
      r = sccb->node_scan_before2( pstmt, diridpdi, pdi );
    }
    else
    {
      DUF_TRACE( scan, 10, "NOT scan node before2 by %5llu - sccb->node_scan_before2 empty for %s", diridpdi, duf_uni_scan_action_title( sccb ) );
    }
    DUF_TEST_R( r );
  }
  else if ( sccb->node_scan_before2 )
  {
    char *ona = NULL;

    ona = duf_option_names( DUF_OPTION_FLAG_DIRS );
    DUF_TRACE( explain, 1, "to scan node before2 use %s", ona );
    mas_free( ona );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan before2 ( -d or --dirs absent )" );
  }
  return r;
}

int
duf_qscan_files_by_dirid2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

/* duf_scan_files_by_pathid:
 * call sccb->leaf_scan (or sccb->leaf_scan wrapped by duf_str_cb_leaf_scan) + pdi (also) as str_cb_udata
 * 			for each <file> record by dirID with corresponding args
 *
 * */
  /* scan this files in this directory */
  if ( r >= 0 && DUF_ACT_FLAG( files ) )
  {
    DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 2." );
    if ( r >= 0 && sccb->leaf_scan_fd2 )
    {
      DUF_TRACE( scan, 11, "  " DUF_DEPTH_PFMT ": scan leaves_scan2 fd   by %5llu", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ) );

      /* duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd */
      r = duf_scan_files_by_dirid2( duf_str_cb2_scan_file_fd, pdi, sccb );

      DUF_TEST_R( r );
    }
    else
    {
      DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": NOT scan leaves_scan2 fd   by %5llu", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ) );
    }
    if ( r >= 0 && sccb->leaf_scan2 )
    {
      DUF_TRACE( scan, 11, "  " DUF_DEPTH_PFMT ": scan leaves_scan2 ..   by %5llu", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ) );
/* duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan */
      r = duf_scan_files_by_dirid2( duf_str_cb2_leaf_scan, pdi, sccb );

      DUF_TEST_R( r );
    }
    else
    {
      DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": NOT scan leaves_scan2    by %5llu", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ) );
    }
  }
  else if ( sccb->leaf_scan_fd2 )
  {
    char *ona = NULL;

    ona = duf_option_names( DUF_OPTION_FLAG_FILES );
    DUF_TRACE( explain, 0, "to scan fd2 use %s", ona );
    mas_free( ona );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan leaf_scan_fd2 and leaf_scan2  ( -f or --files absent )" );
  }
  return r;
}

int
duf_qscan_node_scan_middle2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;
  unsigned long long diridpdi;

  diridpdi = duf_levinfo_dirid( pdi );
  if ( DUF_ACT_FLAG( dirs ) )
  {
    if ( duf_levinfo_item_deleted( pdi ) )
    {
      if ( sccb->node_scan_middle2_deleted )
      {
        DUF_TRACE( scan, 10, "scan node middle2_deleted by %5llu", diridpdi );
        r = sccb->node_scan_middle2_deleted( pstmt, diridpdi, pdi );
      }
      DUF_TRACE( deleted, 0, "DELETED" );
    }
    else if ( sccb->node_scan_middle2 )
    {
      DUF_TRACE( scan, 10, "scan node middle2 by %5llu", diridpdi );
      r = sccb->node_scan_middle2( pstmt, diridpdi, pdi );
    }
    else
    {
      DUF_TRACE( scan, 10, "NOT scan node middle2 by %5llu - sccb->node_scan_middle2 empty for %s", diridpdi, duf_uni_scan_action_title( sccb ) );
    }
    DUF_TEST_R( r );
  }
  else if ( sccb->node_scan_middle2 )
  {
    char *ona = NULL;

    ona = duf_option_names( DUF_OPTION_FLAG_DIRS );
    DUF_TRACE( explain, 0, "to scan node middle2 use %s", ona );
    mas_free( ona );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan middle2 ( -d or --dirs absent )" );
  }
  return r;
}

/*
 * str_cb2 (sub-item scanner):
 *       duf_str_cb2_uni_scan_dir
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
int
duf_qscan_dirs_by_dirid2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_str_cb2_t str_cb2 )
{
  int r = 0;

  /* scan directories in this directory */
/* duf_scan_db_items2:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */

  DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan node selector2: [%s]", duf_pdi_depth( pdi ), sccb->node.selector2 );

  /* if ( !sccb->node.selector2 )                                                           */
  /* {                                                                                      */
  /*   DUF_ERROR( "sccb->node.selector2 empty for %s", duf_uni_scan_action_title( sccb ) ); */
  /*   r = DUF_ERROR_PTR;                                                                   */
  /*   DUF_TEST_R( r );                                                                     */
  /* }                                                                                      */

/* calling duf_sel_cb_(node|leaf) for each record by sccb->node.selector2 */
  /*
   * DUF_NODE_NODE => sccb->node.selector2, sccb->node.fieldset
   * DUF_NODE_LEAF => sccb->leaf.selector2, sccb->leaf.fieldset
   *
   * str_cb2 (sub-item scanner):
   *       duf_str_cb2_uni_scan_dir
   *     ( duf_str_cb2_leaf_scan    )
   *     ( duf_str_cb2_scan_file_fd )
   * */
  if ( r >= 0 && sccb->node.selector2 )
    r = duf_scan_db_items2( DUF_NODE_NODE, str_cb2, pdi, sccb /*, sccb->node.selector2, sccb->node.fieldset */  );
  return r;
}

int
duf_qscan_node_scan_after2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;
  unsigned long long diridpdi;

  diridpdi = duf_levinfo_dirid( pdi );
  if ( DUF_ACT_FLAG( dirs ) )
  {
    if ( duf_levinfo_item_deleted( pdi ) )
    {
      if ( sccb->node_scan_after2_deleted )
      {
        DUF_TRACE( scan, 10, "scan node after2_deleted by %5llu", diridpdi );
        r = sccb->node_scan_after2_deleted( pstmt, diridpdi, pdi );
      }
      DUF_TRACE( deleted, 0, "DELETED" );
    }
    else if ( sccb->node_scan_after2 )
    {
      DUF_TRACE( scan, 10, "scan node after2 by %5llu", diridpdi );
      r = sccb->node_scan_after2( pstmt, diridpdi, pdi );
    }
    else
    {
      DUF_TRACE( scan, 10, "NOT scan node after2 by %5llu - sccb->node_scan_after2 empty for %s", diridpdi, duf_uni_scan_action_title( sccb ) );
    }
    DUF_TEST_R( r );
  }
  else if ( sccb->node_scan_after2 )
  {
    char *ona = NULL;

    ona = duf_option_names( DUF_OPTION_FLAG_DIRS );
    DUF_TRACE( explain, 0, "to scan node after2  use %s", ona );
    mas_free( ona );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan after2 ( -d or --dirs absent )" );
  }
  return r;
}
