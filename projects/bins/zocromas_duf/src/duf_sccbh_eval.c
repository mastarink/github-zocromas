#define DUF_SQL_PDI_STMT

#include <assert.h>
#include <string.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h"
#include "duf_option_defs.h"
#include "duf_option_names.h"

#include "duf_sccb_def.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"

#include "duf_levinfo_openclose.h"

#include "duf_sccb.h"
#include "duf_sccb_eval_dirs.h"
/* #include "duf_sccb_handle.h" */

#include "duf_levinfo_ref.h"

#include "duf_item_scan2.h"
#include "duf_leaf_scan2.h"
#include "duf_dirent_scan2.h"

#include "duf_option_defs.h"


#include "duf_maindb.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccbh_eval.h"
/* ###################################################################### */

/* 20150819.164652 */
static int
duf_eval_sccbh_all_and_summary( duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  DUF_E_NO( DUF_ERROR_TOO_DEEP );
  assert( sccbh );
  assert( SCCB );

  DUF_TRACE( scan, 2, "scan a+s by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

  DORF( r, DUF_WRAPPED( duf_eval_sccbh_all ), ( duf_sqlite_stmt_t * ) NULL, sccbh ); /* XXX XXX XXX XXX XXX XXX */

  if ( r >= 0 && DUF_ACTG_FLAG( summary ) )
  {
    DUF_PRINTF( 0, "%s", duf_uni_scan_action_title( SCCB ) );

    DUF_PRINTF( 0, " summary; seq:     %llu", PDI->seq );
    DUF_PRINTF( 0, " summary; seq-leaf:%llu", PDI->seq_leaf );
    DUF_PRINTF( 0, " summary; seq-node:%llu", PDI->seq_node );
    if ( PU->max_seq )
      DUF_PRINTF( 0, " of %llu (max-seq)", PU->max_seq );
    if ( DUF_ACTG_FLAG( summary ) )
      DUF_PRINTF( 0, " summary; changes:%llu", HCHANGES );
  }
  DUF_TRACE( scan, 2, "/scan a+s by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );
  DUF_E_YES( DUF_ERROR_TOO_DEEP );
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
/* 20150819.164643 */
int DUF_WRAPPED( duf_eval_sccbh_all_and_summary ) ( duf_sccb_handle_t * sccbh ) /* duf_eval_sccbh_all */
{
  DEBUG_STARTR( r );
  if ( sccbh && SCCB )
  {
    DUF_TRACE( explain, 0, "scan targ; action title: %s", duf_uni_scan_action_title( SCCB ) );
    DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan", duf_uni_scan_action_title( SCCB ) );

    {
      HCHANGES = 0;
      if ( duf_levinfo_path( PDI ) )
        DOR( r, duf_eval_sccbh_all_and_summary( sccbh ) ); /* XXX XXX XXX XXX XXX XXX */
    }
  }
  else
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_SCCB );
  }
  DEBUG_ENDR( r );
}
#endif

/* 20150819.133354 */
int
duf_eval_sccbh_db_items_str_cb( duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  const duf_sql_set_t *sql_set = NULL;

  assert( str_cb2 == DUF_WRAPPED( duf_eval_sccbh_all ) || ( str_cb2 == duf_eval_sccbh_db_leaf_fd_str_cb )
          || ( str_cb2 == duf_eval_sccbh_db_leaf_str_cb ) );
#ifdef MAS_TRACING
  const char *set_type_title = node_type == DUF_NODE_LEAF ? "leaf" : ( node_type == DUF_NODE_LEAF ? "node" : "UNDEF" );
#endif
  sql_set = duf_get_sql_set( SCCB, node_type );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( sql_set && sql_set->selector2 && sql_set->fieldset )
  {
    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 4. set '%s' str_cb2%c", set_type_title, str_cb2 ? '+' : '-' );
    DUF_TRACE( scan, 2, "cb2 %s", set_type_title );
    DUF_TRACE( scan, 2, "ql%llu / qn%llu / q%llu %s", PDI->seq_leaf, PDI->seq_node, PDI->seq, SCCB->title );


    DOR( r, duf_scan_db_items_with_str_cb_sql_set( sql_set, str_cb2, sccbh, node_type ) );
    if ( DUF_NOERROR( r ) )
    {
      if ( r > 0 )
        DUF_TRACE( explain, 2, "%u records processed of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                   r, set_type_title, duf_uni_scan_action_title( SCCB ), duf_levinfo_dirid( PDI ) );
      else
        DUF_TRACE( explain, 3, "no records found of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                   set_type_title, duf_uni_scan_action_title( SCCB ), duf_levinfo_dirid( PDI ) );
    }
  }
  else
  {
    DUF_TRACE( explain, 0, "= ? ============ NO selector2 ≪%s≫", set_type_title );
    DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
  }
  DUF_TRACE( scan, 13, "(%d) end scan items str_cb2%c", r, str_cb2 ? '+' : '-' );

  DEBUG_ENDR( r );
}

/* 20150820.085615 */
int
duf_sccbh_eval_db_subnodes( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  assert( sccbh );
  assert( SCCB );

  DUF_TRACE( scan, 4, "scan dirent by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

  DORF( r, duf_eval_sccbh_db_items_str_cb, DUF_NODE_NODE, DUF_WRAPPED( duf_eval_sccbh_all ), sccbh );
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
  if ( DUF_ACTG_FLAG( files ) )
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

    /* DOR_NOE( r, duf_levinfo_if_statat_dh( PDI ), DUF_ERROR_FS_DISABLED ); */
    DOR( r, duf_levinfo_if_statat_dh( PDI ) );

    /* assert( duf_levinfo_dfd( PDI ) ); */
    assert( r < 0 || duf_levinfo_stat( PDI ) );
    /*
     *   -- call for each direntry
     *      - for directory                - sccb->dirent_dir_scan_before2
     *      - for other (~ regular) entry  - sccb->dirent_file_scan_before2
     * XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */

    /* DOR_NOE( r, duf_scan_fs_with2scanners( PDI, SCCB->dirent_file_scan_before2, SCCB->dirent_dir_scan_before2 ), DUF_ERROR_FS_DISABLED ); */
    DOR( r, duf_scan_fs_with2scanners( PDI, SCCB->dirent_file_scan_before2, SCCB->dirent_dir_scan_before2 ) );
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
