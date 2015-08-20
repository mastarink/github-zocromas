#include <assert.h>
#include <string.h>
#include <stddef.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"     /* DUF_TRACE */

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_item_scan2.h"
#include "duf_dirent_scan2.h"


#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "duf_option_defs.h"
#include "duf_option_names.h"

#include "duf_levinfo_openclose.h"

/* #include "duf_uni_scan2.h" */
#include "duf_sccb_eval_dirs.h"

#include "duf_leaf_scan2.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_dir_scan2_stages.h"
/* ###################################################################### */


/*
 *  - pstmt - for 1 node data, obtained from db
 *  - pdi
 *  - sccb
 *
 *  duf_sccbh_eval_db_node_before
 * */
#ifdef MAS_TRACING
#  define DUF_SCAN_DB_NODE_IMPLEMENT_FUNCTION(stagename) \
    int \
    duf_sccbh_eval_db_node_## stagename( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t *sccbh ) \
    { \
      DEBUG_STARTR( r ); \
      unsigned long long diridpdi; \
 \
      diridpdi = duf_levinfo_dirid( PDI ); \
      DUF_TRACE( scan, 4, "? scan node [" #stagename "]2 by %5llu", diridpdi ); \
      if ( DUF_ACT_FLAG( dirs ) ) \
      { \
	PDI->items.total++; \
	PDI->items.dirs++; \
 \
        DUF_TRACE( scan, 4, "? (dirs+) scan node [" #stagename "]2 by %5llu", diridpdi ); \
	if ( duf_levinfo_item_deleted( PDI ) ) \
	{ \
	  if ( SCCB->node_scan_ ## stagename ## 2_deleted ) \
	  { \
	    DUF_TRACE( scan, 4, "scan node " #stagename "2_deleted by %5llu", diridpdi ); \
	     /* scanner = SCCB->node_scan_ ## stagename ## 2_deleted */ \
	    DOR( r, SCCB->node_scan_ ## stagename ## 2_deleted( pstmt, /* diridpdi, */ PDI ) ); \
	  } \
	  DUF_TRACE( deleted, 0, "DELETED" ); \
	} \
	else if ( SCCB->node_scan_ ## stagename ## 2 ) \
	{ \
	  DUF_TRACE( scan, 4, "scan node " #stagename "2 by %5llu", diridpdi ); \
	  /* scanner = SCCB->node_scan_ ## stagename ## 2 */ \
	  DOR( r, SCCB->node_scan_ ## stagename ## 2( pstmt, /* diridpdi, */ PDI ) ); \
	} \
	else \
	{ \
	  DUF_TRACE( scan, 4, "NOT scan node " #stagename "2 by %5llu - sccb->node_scan_" #stagename "2 empty for %s", \
		      		diridpdi, duf_uni_scan_action_title( SCCB ) ); \
	} \
      } \
      else if ( SCCB->node_scan_ ## stagename ## 2 ) \
      { \
	DUF_TRACE( explain, 1, "to scan node " #stagename "2 use %s", DUF_OPT_NAME( FLAG_DIRS ) ); \
	DUF_TRACE( scan, 1, "to scan node " #stagename "2 use %s", DUF_OPT_NAME( FLAG_DIRS ) ); \
      } \
      else \
      { \
	DUF_TRACE( scan, 4, "NOT scan " #stagename "2 ( -d or --dirs absent )" ); \
      } \
      DEBUG_ENDR( r ); \
    }
#else
#  define DUF_SCAN_DB_NODE_IMPLEMENT_FUNCTION(stagename) \
    int \
    duf_scan_db_node_## stagename ## _with_sccbh( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t *sccbh ) \
    { \
      DEBUG_STARTR( r ); \
 \
      if ( DUF_ACT_FLAG( dirs ) ) \
      { \
	PDI->items.total++; \
	PDI->items.dirs++; \
 \
	if ( duf_levinfo_item_deleted( PDI ) ) \
	{ \
	  if ( SCCB->node_scan_ ## stagename ## 2_deleted ) \
	  { \
	     /* scanner = SCCB->node_scan_ ## stagename ## 2_deleted */ \
	    DOR( r, SCCB->node_scan_ ## stagename ## 2_deleted( pstmt, PDI ) ); \
	  } \
	  DUF_TRACE( deleted, 0, "DELETED" ); \
	} \
	else if ( SCCB->node_scan_ ## stagename ## 2 ) \
	{ \
	  /* scanner = SCCB->node_scan_ ## stagename ## 2 */ \
	  DOR( r, SCCB->node_scan_ ## stagename ## 2( pstmt,  PDI ) ); \
	} \
      } \
      DEBUG_ENDR( r ); \
    }
#endif

/* 20150820.085420 */
DUF_SCAN_DB_NODE_IMPLEMENT_FUNCTION( before );
DUF_SCAN_DB_NODE_IMPLEMENT_FUNCTION( middle );
DUF_SCAN_DB_NODE_IMPLEMENT_FUNCTION( after );

/*20150820.085447
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem with necessary info:
 *
 * call from duf_eval_sccbh_all(_wrap)
 *
 * fn of type: duf_str_cb2_t
 * */

int
duf_sccbh_eval_fs_items( duf_sqlite_stmt_t * pstmt_unused, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  if ( SCCB->dirent_dir_scan_before2 || SCCB->dirent_file_scan_before2 )
  {
    duf_pdi_set_opendir( PDI, 1 );
    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>>q +dirent" );
    DUF_TRACE( scan, 4, "@scan dirent by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

    DOR_NOE( r, duf_levinfo_if_statat_dh( PDI ), DUF_ERROR_FS_DISABLED );
    /* assert( duf_levinfo_dfd( PDI ) ); */
    assert( r < 0 || duf_levinfo_stat( PDI ) );
    /*
     *   -- call for each direntry
     *      - for directory                - sccb->dirent_dir_scan_before2
     *      - for other (~ regular) entry  - sccb->dirent_file_scan_before2
     * XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */
    DOR_NOE( r, duf_scan_fs_with2scanners( PDI, SCCB->dirent_file_scan_before2, SCCB->dirent_dir_scan_before2 ), DUF_ERROR_FS_DISABLED );
  }
  else
  {
    DUF_TRACE( scan, 10,
               "NOT scan dirent_dir by %5llu - sccb->dirent_dir_scan_before2 empty and sccb->dirent_file_scan_before2 for %s",
               duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ) );
  }
#if 0
  {
    const duf_sql_set_t *sql_set = NULL;

    sql_set = duf_get_leaf_sql_set( SCCB );
    DOR( r, duf_count_db_items2( NULL /* duf_match_leaf2 */ , sccbh, sql_set ) ); /* count for possibly --progress */
  }
#endif
  /* DUF_CLEAR_ERROR( r, DUF_ERROR_FS_DISABLED ); */
  DEBUG_ENDR( r );
}

/*20150820.085607
 *  - pstmt - for 1 node data, obtained from db
 *  - pdi
 *  - sccb
 *  */

int
duf_sccbh_eval_db_leaves( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh )
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
    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 2." );
    if ( SCCB->leaf_scan_fd2 )
    {
      /* duf_eval_sccbh_db_leaf_fd_str_cb is just a wrapper for sccb->leaf_scan_fd2 */
      DOR( r, duf_eval_sccbh_db_items_str_cb( DUF_NODE_LEAF, duf_eval_sccbh_db_leaf_fd_str_cb, sccbh ) );
    }
    if ( SCCB->leaf_scan2 )
    {
      /* duf_eval_sccbh_db_leaf_str_cb is just a wrapper for sccb->leaf_scan2 */
      DOR( r, duf_eval_sccbh_db_items_str_cb( DUF_NODE_LEAF, duf_eval_sccbh_db_leaf_str_cb, sccbh ) );
    }
  }
  else if ( SCCB->leaf_scan_fd2 )
  {
    DUF_TRACE( explain, 0, "to scan fd2 use %s", DUF_OPT_NAME( FLAG_FILES ) );
  }
  else
  {
    DUF_TRACE( scan, 10, "NOT scan leaf_scan_fd2 and leaf_scan2  ( -f or --files absent )" );
  }
  DEBUG_ENDR( r );
}

/* 20150820.085615 */
int
duf_sccbh_eval_db_subnodes( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  const char *node_selector2 = NULL;

  assert( sccbh );
  assert( SCCB );
  /* scan directories in this directory */
/* duf_eval_sccbh_db_items_str_cb:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
/* calling duf_sel_cb_(node|leaf) for each record by node.selector2 */
  /*
   *
   * str_cb2 (sub-item scanner):
   *       duf_eval_sccbh_all(_wrap)
   *     ( duf_eval_sccbh_db_leaf_str_cb    )
   *     ( duf_eval_sccbh_db_leaf_fd_str_cb )
   * */
  node_selector2 = duf_get_node_sql_set( SCCB )->selector2;

  DUF_TRACE( scan, 4, "scan dirent by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

  if ( node_selector2 )
    DORF( r, duf_eval_sccbh_db_items_str_cb, DUF_NODE_NODE, DUF_WRAPPED( duf_eval_sccbh_all ), sccbh );
  DEBUG_ENDR( r );
}
