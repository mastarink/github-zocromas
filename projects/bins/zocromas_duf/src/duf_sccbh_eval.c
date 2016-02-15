#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
#include "duf_printn_defs.h"

#include "duf_config.h"
#include "duf_config_trace.h"

#include "duf_config_util.h"
#include "duf_config_ref.h"     /* needed for DUF_PRINTF */

#include "duf_pdi_filters.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "duf_sccb_scanstage.h"

#include "duf_sccb_handle.h"
#include "duf_sccbh_eval_leaf.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_sccbh_eval_all.h"
#include "duf_sccbh_eval_sql_set.h"
/* #include "duf_sccbh_eval_fs.h" */
#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccbh_eval.h"
/* ###################################################################### */

/* 20150819.164652 */
DUF_WRAPSTATIC int
duf_eval_sccbh_all_and_summary( duf_sccb_handle_t * sccbh, bool f_summary )
{
  DEBUG_STARTR( r );

  DUF_E_LOWER( DUF_ERROR_TOO_DEEP );
  assert( sccbh );
  assert( SCCB );

  DUF_TRACE( sccbh, 0, "eval sccbh ALL (%d) %s", sccbh ? 1 : 0, SCCB->name );
  DORF( r, DUF_WRAPPED( duf_eval_sccbh_all ), ( duf_scanstage_t ) NULL, ( duf_stmnt_t * ) NULL, sccbh ); /* XXX XXX XXX XXX XXX XXX */
  DUF_TRACE( sccbh, 0, "(%s) eval sccbh ALL done (%d) %s", mas_error_name_i( r ), sccbh ? 1 : 0, SCCB->name );

  if ( DUF_NOERROR( r ) && f_summary )
  {
    DUF_PRINTF( 0, "%s", duf_uni_scan_action_title( SCCB ) );

    DUF_PRINTF( 0, " summary; seq:     %llu", PDI->seq );
    DUF_PRINTF( 0, " summary; seq-leaf:%llu", PDI->seq_leaf );
    DUF_PRINTF( 0, " summary; seq-node:%llu", PDI->seq_node );
    DUF_PRINTF( 0, " summary;  seq-row:%llu", PDI->seq_row );
    if ( PU->max_seq )
      DUF_PRINTF( 0, " of %llu (max-seq)", PU->max_seq );
    DUF_PRINTF( 0, " summary; changes:%llu", HCHANGES );
    DUF_PRINTF( 0, " summary; total-bytes:%llu", PDI->total_bytes );
  }
  DUF_E_UPPER( DUF_ERROR_TOO_DEEP );
  DEBUG_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
/* 20150819.164643 */
int DUF_WRAPPED( duf_eval_sccbh_all_and_summary ) ( duf_sccb_handle_t * sccbh, bool f_summary ) /* duf_eval_sccbh_all */
{
  DEBUG_STARTR( r );
  if ( sccbh && SCCB )
  {
    DUF_TRACE( explain, 0, "scan targ; action title: %s", duf_uni_scan_action_title( SCCB ) );
    DUF_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan", duf_uni_scan_action_title( SCCB ) );
    DUF_TRACE( sccbh, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan %s", duf_uni_scan_action_title( SCCB ), SCCB->name );

    {
      HCHANGES = 0;
      if ( duf_levinfo_path( PDI ) )
      {
        DOR( r, duf_eval_sccbh_all_and_summary( sccbh, f_summary ) ); /* XXX XXX XXX XXX XXX XXX */
      }
      else
      {
        /* assert( 0 ); */
      }
    }
  }
  else
  {
    DUF_MAKE_ERROR( r, DUF_ERROR_SCCB );
  }
  DEBUG_ENDR( r );
}
#endif

/* 20160212.130816 */
static int
duf_eval_sccbh_db_items_str_cb( duf_scanstage_t scanstage, duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  duf_sql_set_pair_t sql_set_pair = { NULL, NULL };

  assert( str_cb2 == DUF_WRAPPED( duf_eval_sccbh_all ) || ( str_cb2 == duf_eval_sccbh_db_leaf_fd_str_cb )
          || ( str_cb2 == duf_eval_sccbh_db_leaf_str_cb ) );
#ifdef MAS_TRACING
  const char *set_type_title = duf_nodetype_name( node_type );
#endif
  sql_set_pair = duf_sccbh_get_sql_set_f( sccbh, node_type );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( sql_set_pair.active
       /* && sql_set_pair.active->selector2 && sql_set_pair.active->fieldset  FIXME for !sql_set_pair.active->selector2 && sql_set_pair.active->selector2_cte */
         )
  {
    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 4. set '%s' str_cb2%c", set_type_title, str_cb2 ? '+' : '-' );
    DUF_TRACE( scan, 10, "ql%llu / qn%llu / w%llu / q%llu %s", PDI->seq_leaf, PDI->seq_node, PDI->seq_row, PDI->seq, SCCB->title );

    DUF_TRACE( sccbh, 2, "@@@@has selector for %s(%d) dirid:%llu (%s) %s", set_type_title, node_type, duf_levinfo_dirid( PDI ),
               duf_uni_scan_action_title( SCCB ), SCCB->name );
#if 0
    DOR( r, duf_scan_db_items_with_str_cb_sql_set( sql_set_pair.active, str_cb2, sccbh, node_type ) );
#else
    DOR( r, duf_eval_sccbh_sql_set_str_cb( scanstage, node_type, sql_set_pair, str_cb2, sccbh ) );
#endif
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
  DEBUG_ENDR( r );
}

/*20150820.085607
 *  - pstmt - for 1 node data, obtained from db
 *  - pdi
 *  - sccb
 *  */
int
duf_sccbh_eval_db_leaves( duf_scanstage_t scanstage, duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
/*
 * call sccb->leaf_scan (or sccb->leaf_scan wrapped by duf_str_cb_leaf_scan) + pdi (also) as str_cb_udata
 * 			for each <file> record by dirID with corresponding args
 *
 * */
  /* scan this files in this directory */
  DUF_TRACE( sccbh, 10, "%s: leaf_scan_fd2:%d; leaf_scan:%d;", SCCB->name, SCCB->leaf_scan_fd2 ? 1 : 0, SCCB->leaf_scan2 ? 1 : 0 );
  /* if ( DUF_ACTG_FLAG( allow_files ) ) */
  {
    DUF_TRACE( sccbh, 2, "@@@scan files (%s) %s", duf_uni_scan_action_title( SCCB ), SCCB->name );
    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 2." );
    if ( scanstage == DUF_SCANSTAGE_DB_LEAVES_FD && SCCB->leaf_scan_fd2 )
    {
      DUF_TRACE( sccbh, 4, "%s: leaf_scan_fd2:%d; leaf_scan:%d;", SCCB->name, SCCB->leaf_scan_fd2 ? 1 : 0, SCCB->leaf_scan2 ? 1 : 0 );
      /* duf_eval_sccbh_db_leaf_fd_str_cb is just a wrapper for sccb->leaf_scan_fd2 */
      DOR( r, duf_eval_sccbh_db_items_str_cb( scanstage, DUF_NODE_LEAF, duf_eval_sccbh_db_leaf_fd_str_cb, sccbh ) );
    }
    if ( scanstage == DUF_SCANSTAGE_DB_LEAVES_NOFD && SCCB->leaf_scan2 )
    {
      DUF_TRACE( sccbh, 10, "%s: leaf_scan_fd2:%d; leaf_scan:%d;", SCCB->name, SCCB->leaf_scan_fd2 ? 1 : 0, SCCB->leaf_scan2 ? 1 : 0 );
      /* duf_eval_sccbh_db_leaf_str_cb is just a wrapper for sccb->leaf_scan2 */
      DOR( r, duf_eval_sccbh_db_items_str_cb( scanstage, DUF_NODE_LEAF, duf_eval_sccbh_db_leaf_str_cb, sccbh ) );
    }
  }
  DEBUG_ENDR( r );
}

int
duf_sccbh_eval_db_leaves_nofd( duf_scanstage_t scanstage, duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  /* scan this files in this directory */
  /* if ( DUF_ACTG_FLAG( allow_files ) ) */
  {
    DUF_TRACE( sccbh, 2, "@@@@scan files (%s) %s", duf_uni_scan_action_title( SCCB ), SCCB->name );
    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 2." );
    DUF_TRACE( sccbh, 2, "@@@@@%s: leaf scan:%d;", duf_uni_scan_action_title( SCCB ), SCCB->leaf_scan2 ? 1 : 0 );
    if ( SCCB->leaf_scan2 )
    {
      /* duf_eval_sccbh_db_leaf_str_cb is just a wrapper for sccb->leaf_scan2 */
      DOR( r, duf_eval_sccbh_db_items_str_cb( scanstage, DUF_NODE_LEAF, duf_eval_sccbh_db_leaf_str_cb, sccbh ) );
    }
  }
  DEBUG_ENDR( r );
}

int
duf_sccbh_eval_db_leaves_fd( duf_scanstage_t scanstage, duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  /* scan this files in this directory */
  /* if ( DUF_ACTG_FLAG( allow_files ) ) */
  {
    DUF_TRACE( sccbh, 2, "@@@@scan files fd (%s) %s", duf_uni_scan_action_title( SCCB ), SCCB->name );
    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 2." );
    DUF_TRACE( sccbh, 2, "@@@@@%s: leaf scan fd:%d;", duf_uni_scan_action_title( SCCB ), SCCB->leaf_scan_fd2 ? 1 : 0 );
    if ( SCCB->leaf_scan_fd2 )
    {
      /* duf_eval_sccbh_db_leaf_fd_str_cb is just a wrapper for sccb->leaf_scan_fd2 */
      DOR( r, duf_eval_sccbh_db_items_str_cb( scanstage, DUF_NODE_LEAF, duf_eval_sccbh_db_leaf_fd_str_cb, sccbh ) );
    }
  }
  DEBUG_ENDR( r );
}

/* 20151027.144606 */
int
duf_sccbh_eval_db_node( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  unsigned long long diridpdi;

  diridpdi = duf_levinfo_dirid( PDI );
  DUF_TRACE( scan, 4, "? scan node [%s] by %5llu", duf_scanstage_name( scanstage ), diridpdi );
  /* if ( DUF_ACTG_FLAG( allow_dirs ) ) */
  {
    duf_scanner_t scanner = NULL;

    PDI->items.total++;
    PDI->items.dirs++;

    DUF_TRACE( scan, 4, "? (dirs+) scan node [%s] by %5llu", duf_scanstage_name( scanstage ), diridpdi );
#if 1
    if ( ( scanner = duf_scanstage_scanner( SCCB, scanstage, 1 /* deleted */ , DUF_NODE_NODE ) ) && duf_levinfo_if_deleted( PDI ) )
    {
      sccbh->current_scanner = scanner;
      sccbh->current_node_type = DUF_NODE_NODE;
      DUF_TRACE( scan, 4, "scan node %s_deleted by %5llu", duf_scanstage_name( scanstage ), diridpdi );
      DOR( r, ( scanner ) ( pstmt, PDI ) );
      assert( sccbh->current_node_type == DUF_NODE_NODE );
      if ( sccbh->atom_cb )     /* atom is fs-direntry(dir or reg) or item(node or leaf) */
        sccbh->atom_cb( sccbh, scanstage, pstmt, scanner, DUF_NODE_NODE, r );
    }
    else if ( ( scanner = duf_scanstage_scanner( SCCB, scanstage, 0 /* deleted */ , DUF_NODE_NODE ) ) )
    {
      sccbh->current_scanner = scanner;
      sccbh->current_node_type = DUF_NODE_NODE;
      DUF_TRACE( scan, 4, "scan node %s by %5llu", duf_scanstage_name( scanstage ), diridpdi );
      DOR( r, ( scanner ) ( pstmt, PDI ) );
      assert( sccbh->current_node_type == DUF_NODE_NODE );
      if ( sccbh->atom_cb )     /* atom is fs-direntry(dir or reg) or item(node or leaf) */
        sccbh->atom_cb( sccbh, scanstage, pstmt, scanner, DUF_NODE_NODE, r );
    }
#else
    if ( ( scanner = duf_scanstage_scanner( SCCB, scanstage, duf_levinfo_if_deleted( PDI ), DUF_NODE_NODE ) ) )
    {
      sccbh->current_scanner = scanner;
      DUF_TRACE( scan, 4, "scan node %s_deleted by %5llu", duf_scanstage_name( scanstage ), diridpdi );
      DOR( r, ( scanner ) ( pstmt, PDI ) );
    }
#endif
    else
    {
      DUF_TRACE( scan, 4, "NOT scan node %s by %5llu - sccb->node_scan_%s empty for %s", duf_scanstage_name( scanstage ),
                 diridpdi, duf_scanstage_name( scanstage ), duf_uni_scan_action_title( SCCB ) );
    }
  }
  DEBUG_ENDR( r );
}

/* 20150820.085615 */
/* TODO duf_sccb_handle_t * sccbh must be first in all such funcs 20160210.120016 */
int
duf_sccbh_eval_db_subnodes( duf_scanstage_t scanstage, duf_stmnt_t * pstmt DUF_UNUSED, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  assert( sccbh );
  assert( SCCB );

#ifdef DUF_NO_RNUMS
  DUF_TRACE( sql, 0, "@@@EACH SUB %llu ... %s", duf_levinfo_dirid( PDI ), sqlite3_sql( pstmt ) );
#else
  DUF_TRACE( sql, 0, "@@@EACH SUB %llu {%llu}... %s", duf_levinfo_dirid( PDI ), pstmt ? DUF_GET_UFIELD2( rnfiles ) : 0, sqlite3_sql( pstmt ) );
#endif
  DUF_TRACE( scan, 4, "scan dirent by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

  DORF( r, duf_eval_sccbh_db_items_str_cb, scanstage, DUF_NODE_NODE, DUF_WRAPPED( duf_eval_sccbh_all ), sccbh );
  DEBUG_ENDR( r );
}
