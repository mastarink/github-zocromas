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

/* #include "duf_file_pathid2.h" */

#include "duf_sccb.h"
#include "duf_option_names.h"

#include "duf_uni_scan2.h"
/* ###################################################################### */
#include "duf_dir_scan2_stages.h"
/* ###################################################################### */



#define DUF_QSCAN_NODE_DECLARE_FUNCTION(stagename) \
    int \
    duf_qscan_node_scan_## stagename ## 2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb ) \
    { \
      DEBUG_STARTR( r ); \
      unsigned long long diridpdi; \
 \
      diridpdi = duf_levinfo_dirid( pdi ); \
      if ( DUF_ACT_FLAG( dirs ) ) \
      { \
	pdi->items.total++; \
	pdi->items.dirs++; \
 \
	if ( duf_levinfo_item_deleted( pdi ) ) \
	{ \
	  if ( sccb->node_scan_ ## stagename ## 2_deleted ) \
	  { \
	    DUF_TRACE( scan, 10, "scan node " #stagename "2_deleted by %5llu", diridpdi ); \
	    DOR( r, sccb->node_scan_ ## stagename ## 2_deleted( pstmt, /* diridpdi, */ pdi ) ); \
	  } \
	  DUF_TRACE( deleted, 0, "DELETED" ); \
	} \
	else if ( sccb->node_scan_ ## stagename ## 2 ) \
	{ \
	  DUF_TRACE( scan, 10, "scan node " #stagename "2 by %5llu", diridpdi ); \
	  DOR( r, sccb->node_scan_ ## stagename ## 2( pstmt, /* diridpdi, */ pdi ) ); \
	} \
	else \
	{ \
	  DUF_TRACE( scan, 10, "NOT scan node " #stagename "2 by %5llu - sccb->node_scan_" #stagename "2 empty for %s", \
		      		diridpdi, duf_uni_scan_action_title( sccb ) ); \
	} \
	DUF_TEST_R( r ); \
      } \
      else if ( sccb->node_scan_ ## stagename ## 2 ) \
      { \
	DUF_TRACE( explain, 1, "to scan node " #stagename "2 use %s", DUF_OPT_NAME( FLAG_DIRS ) ); \
      } \
      else \
      { \
	DUF_TRACE( scan, 10, "NOT scan " #stagename "2 ( -d or --dirs absent )" ); \
      } \
      DEBUG_ENDR( r ); \
    }

/* *INDENT-OFF*  */
DUF_QSCAN_NODE_DECLARE_FUNCTION(before)
DUF_QSCAN_NODE_DECLARE_FUNCTION(middle)
DUF_QSCAN_NODE_DECLARE_FUNCTION(after)
/* *INDENT-ON*  */

/*
 * this is callback of type: duf_str_cb_t (first range; str_cb)
 *
 * duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd
 * */
static int
duf_str_cb2_leaf_scan_fd( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );

  pdi->items.total++;
  pdi->items.files++;
  assert( duf_levinfo_dfd( pdi ) );
  if ( sccb->leaf_scan_fd2 )
    DOR( r, sccb->leaf_scan_fd2( pstmt, duf_levinfo_dfd( pdi ), duf_levinfo_stat( pdi ), pdi ) );
  DEBUG_ENDR( r );
}

/*
 * this is callback of type: duf_str_cb_t (first range; str_cb)
 *
 * duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 * */
static int
duf_str_cb2_leaf_scan( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );

  DUF_SFIELD2( filename );

  pdi->items.total++;
  pdi->items.files++;

  if ( duf_levinfo_item_deleted( pdi ) )
  {
    if ( sccb->leaf_scan2_deleted )
      DOR( r, sccb->leaf_scan2_deleted( pstmt, pdi ) );
    DUF_TRACE( deleted, 0, "DELETED '%s%s'", duf_levinfo_path( pdi ), filename );
  }
  else
  {
    if ( sccb->leaf_scan2 )
      DOR( r, sccb->leaf_scan2( pstmt, pdi ) );
  }

  DEBUG_ENDR( r );
}

/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem with necessary info:
 * */
int
duf_qscan_dirents2( duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );

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
      DOR( r, duf_scan_dirents2( pdi, sccb->dirent_file_scan_before2, sccb->dirent_dir_scan_before2 ) );
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
  DEBUG_ENDR( r );
}

int
duf_qscan_files_by_dirid2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );

/* duf_scan_files_by_pathid:
 * call sccb->leaf_scan (or sccb->leaf_scan wrapped by duf_str_cb_leaf_scan) + pdi (also) as str_cb_udata
 * 			for each <file> record by dirID with corresponding args
 *
 * */
  /* scan this files in this directory */
  if ( DUF_ACT_FLAG( files ) )
  {
    DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 2." );
    if ( sccb->leaf_scan_fd2 )
    {
      /* duf_str_cb2_leaf_scan_fd is just a wrapper for sccb->leaf_scan_fd2 */
      DOR( r, duf_scan_db_items2( DUF_NODE_LEAF, duf_str_cb2_leaf_scan_fd, pdi, sccb ) );
    }
    if ( sccb->leaf_scan2 )
    {
      /* duf_str_cb2_leaf_scan is just a wrapper for sccb->leaf_scan2 */
      DOR( r, duf_scan_db_items2( DUF_NODE_LEAF, duf_str_cb2_leaf_scan, pdi, sccb ) );
    }
  }
  else if ( sccb->leaf_scan_fd2 )
  {
    DUF_TRACE( explain, 0, "to scan fd2 use %s", DUF_OPT_NAME( FLAG_FILES ) );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan leaf_scan_fd2 and leaf_scan2  ( -f or --files absent )" );
  }
  DEBUG_ENDR( r );
}

int
duf_qscan_dirs_by_dirid2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb /*, duf_str_cb2_t str_cb2 */  )
{
  DEBUG_STARTR( r );

  /* scan directories in this directory */
/* duf_scan_db_items2:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */

/* calling duf_sel_cb_(node|leaf) for each record by sccb->node.selector2 */
  /*
   * DUF_NODE_NODE => sccb->node.selector2, sccb->node.fieldset
   * DUF_NODE_LEAF => sccb->leaf.selector2, sccb->leaf.fieldset
   *
   * str_cb2 (sub-item scanner):
   *       duf_scan_dirs_by_pdi_maxdepth
   *     ( duf_str_cb2_leaf_scan    )
   *     ( duf_str_cb2_leaf_scan_fd )
   * */
  if ( sccb->node.selector2 )
    DOR( r, duf_scan_db_items2( DUF_NODE_NODE, duf_scan_dirs_by_pdi_maxdepth /* str_cb2 */ , pdi,
                                sccb /*, sccb->node.selector2, sccb->node.fieldset */  ) );
  DEBUG_ENDR( r );
}