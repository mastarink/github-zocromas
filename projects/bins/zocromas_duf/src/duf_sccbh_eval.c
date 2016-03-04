/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* T; TT; TR ✗ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ✗ */
#include "duf_dodefs.h"                                              /* DOR ✗ */

#include "duf_debug_defs.h"                                          /* DUF_WRAPSTATIC; DUF_WRAPPED ...  ✗ */

#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_ref.h"                                          /* needed for DUF_PRINTF */
#include "duf_config_output_util.h"

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "duf_sccb_scanstage.h"

#include "duf_sccb_handle.h"
#include "duf_sccbh_eval_leaf.h"

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_sccbh_eval_all.h"
#include "duf_sccbh_eval_sql_set.h"
/* #include "duf_sccbh_eval_fs.h" */
#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccbh_eval.h"
/* ###################################################################### */

/* 20150819.164652 */
DUF_WRAPSTATIC int
duf_sccbh_eval_all_and_summary( duf_sccb_handle_t * sccbh, bool f_summary )
{
  DUF_STARTR( r );

  DUF_E_LOWER( DUF_ERROR_TOO_DEEP );
  assert( sccbh );
  assert( SCCB );

  MAST_TRACE( sccbh, 0, "eval sccbh ALL (%d) %s", sccbh ? 1 : 0, SCCB->name );
  DORF( r, duf_sccbh_eval_all, sccbh, ( duf_stmnt_t * ) NULL, ( duf_scanstage_t ) NULL ); /* XXX XXX XXX XXX XXX XXX */
  MAST_TRACE( sccbh, 0, "(%s) eval sccbh ALL done (%d) %s", mas_error_name_i( r ), sccbh ? 1 : 0, SCCB->name );

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
  DUF_ENDR( r );
}

#ifdef MAS_WRAP_FUNC
/* 20150819.164643 */
int DUF_WRAPPED( duf_sccbh_eval_all_and_summary ) ( duf_sccb_handle_t * sccbh, bool f_summary ) /* duf_sccbh_eval_all */
{
  DUF_STARTR( r );
  if ( sccbh && SCCB )
  {
    MAST_TRACE( explain, 0, "scan targ; action title: %s", duf_uni_scan_action_title( SCCB ) );
    MAST_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan", duf_uni_scan_action_title( SCCB ) );
    MAST_TRACE( sccbh, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan %s", duf_uni_scan_action_title( SCCB ), SCCB->name );

    {
      HCHANGES = 0;
      if ( duf_levinfo_path( PDI ) )
      {
        DOR( r, duf_sccbh_eval_all_and_summary( sccbh, f_summary ) ); /* XXX XXX XXX XXX XXX XXX */
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
  DUF_ENDR( r );
}
#endif

/* 20160212.130816 */
static int
duf_sccbh_eval_db_items_str_cb( duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );

  duf_sql_set_pair_t sql_set_pair = { NULL, NULL };

  assert( str_cb2 == duf_sccbh_eval_all || ( str_cb2 == duf_sccbh_eval_db_leaf_fd_str_cb ) || ( str_cb2 == duf_sccbh_eval_db_leaf_str_cb ) );
#ifdef MAS_TRACING
  const char *set_type_title = duf_nodetype_name( node_type );
#endif
  sql_set_pair = duf_sccbh_get_sql_set_f( sccbh, node_type );

/* calling duf_sel_cb_(node|leaf) for each record by sql */
  if ( sql_set_pair.active
     /* && sql_set_pair.active->selector2 && sql_set_pair.active->fieldset  FIXME for !sql_set_pair.active->selector2 && sql_set_pair.active->selector2_cte */
           )
  {
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 4. set '%s' str_cb2%c", set_type_title, str_cb2 ? '+' : '-' );
    MAST_TRACE( scan, 10, "ql%llu / qn%llu / w%llu / q%llu %s", PDI->seq_leaf, PDI->seq_node, PDI->seq_row, PDI->seq, SCCB->title );

    MAST_TRACE( sccbh, 2, "@@@@has selector for %s(%d) dirid:%llu (%s) %s", set_type_title, node_type, duf_levinfo_dirid( PDI ),
                duf_uni_scan_action_title( SCCB ), SCCB->name );
#if 0
    DOR( r, duf_scan_db_items_with_str_cb_sql_set( sql_set_pair.active, str_cb2, sccbh, node_type ) );
#else
    DOR( r, duf_eval_sccbh_sql_set_str_cb( sccbh, node_type, sql_set_pair, str_cb2, scanstage ) );
#endif
    if ( DUF_NOERROR( r ) )
    {
      if ( r > 0 )
      {
        MAST_TRACE( explain, 2, "%u records processed of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                    r, set_type_title, duf_uni_scan_action_title( SCCB ), duf_levinfo_dirid( PDI ) );
      }
      else
      {
        MAST_TRACE( explain, 3, "no records found of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                    set_type_title, duf_uni_scan_action_title( SCCB ), duf_levinfo_dirid( PDI ) );
      }
    }
  }
  else
  {
    MAST_TRACE( explain, 0, "= ? ============ NO selector2 ≪%s≫", set_type_title );
    DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
  }
  DUF_ENDR( r );
}

/*20150820.085607
 *  - pstmt - for 1 node data, obtained from db
 *  - pdi
 *  - sccb
 *  */
#if 0
int
duf_sccbh_eval_db_leaves( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );
/*
 * call sccb->leaf_scan (or sccb->leaf_scan wrapped by duf_str_cb_leaf_scan) + pdi (also) as str_cb_udata
 * 			for each <file> record by dirID with corresponding args
 *
 * */
/* scan this files in this directory */
  MAST_TRACE( sccbh, 10, "%s: leaf_scan_fd2:%d; leaf_scan:%d;", SCCB->name, SCCB->leaf_scan_fd2 ? 1 : 0, SCCB->leaf_scan2 ? 1 : 0 );
/* if ( DUF_ACTG_FLAG( allow_files ) ) */
  {
    MAST_TRACE( sccbh, 2, "@@@scan files (%s) %s", duf_uni_scan_action_title( SCCB ), SCCB->name );
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 2." );
    if ( scanstage == DUF_SCANSTAGE_DB_LEAVES_FD && SCCB->leaf_scan_fd2 )
    {
      MAST_TRACE( sccbh, 4, "%s: leaf_scan_fd2:%d; leaf_scan:%d;", SCCB->name, SCCB->leaf_scan_fd2 ? 1 : 0, SCCB->leaf_scan2 ? 1 : 0 );
    /* duf_sccbh_eval_db_leaf_fd_str_cb is just a wrapper for sccb->leaf_scan_fd2 */
      DOR( r, duf_sccbh_eval_db_items_str_cb( sccbh, DUF_NODE_LEAF, duf_sccbh_eval_db_leaf_fd_str_cb, scanstage ) );
    }
    if ( scanstage == DUF_SCANSTAGE_DB_LEAVES_NOFD && SCCB->leaf_scan2 )
    {
      MAST_TRACE( sccbh, 10, "%s: leaf_scan_fd2:%d; leaf_scan:%d;", SCCB->name, SCCB->leaf_scan_fd2 ? 1 : 0, SCCB->leaf_scan2 ? 1 : 0 );
    /* duf_sccbh_eval_db_leaf_str_cb is just a wrapper for sccb->leaf_scan2 */
      DOR( r, duf_sccbh_eval_db_items_str_cb( sccbh, DUF_NODE_LEAF, duf_sccbh_eval_db_leaf_str_cb, scanstage ) );
    }
  }
  DUF_ENDR( r );
}
#endif
int
duf_sccbh_eval_db_leaves_nofd( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );
/* scan this files in this directory */
/* if ( DUF_ACTG_FLAG( allow_files ) ) */
  {
    MAST_TRACE( sccbh, 2, "@@@@scan files (%s) %s", duf_uni_scan_action_title( SCCB ), SCCB->name );
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 2." );
    MAST_TRACE( sccbh, 2, "@@@@@%s: leaf scan:%d;", duf_uni_scan_action_title( SCCB ), SCCB->leaf_scan2 ? 1 : 0 );
    if ( SCCB->leaf_scan2 )
    {
    /* duf_sccbh_eval_db_leaf_str_cb is just a wrapper for sccb->leaf_scan2 */
      DOR( r, duf_sccbh_eval_db_items_str_cb( sccbh, DUF_NODE_LEAF, duf_sccbh_eval_db_leaf_str_cb, scanstage ) );
    }
  }
  DUF_ENDR( r );
}

int
duf_sccbh_eval_db_leaves_fd( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );
/* scan this files in this directory */
/* if ( DUF_ACTG_FLAG( allow_files ) ) */
  {
    MAST_TRACE( sccbh, 2, "@@@@scan files fd (%s) %s", duf_uni_scan_action_title( SCCB ), SCCB->name );
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 2." );
    MAST_TRACE( sccbh, 2, "@@@@@%s: leaf scan fd:%d;", duf_uni_scan_action_title( SCCB ), SCCB->leaf_scan_fd2 ? 1 : 0 );
    if ( SCCB->leaf_scan_fd2 )
    {
    /* duf_sccbh_eval_db_leaf_fd_str_cb is just a wrapper for sccb->leaf_scan_fd2 */
      DOR( r, duf_sccbh_eval_db_items_str_cb( sccbh, DUF_NODE_LEAF, duf_sccbh_eval_db_leaf_fd_str_cb, scanstage ) );
    }
  }
  DUF_ENDR( r );
}

/* 20151027.144606 */
int
duf_sccbh_eval_db_node( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );

  MAST_TRACE( scan, 4, "? scan node [%s] by %5llu", duf_scanstage_name( scanstage ), duf_levinfo_dirid( PDI ) );
/* if ( DUF_ACTG_FLAG( allow_dirs ) ) */
  {
    duf_scanner_t scanner = NULL;

    PDI->items.total++;
    PDI->items.dirs++;

    MAST_TRACE( scan, 4, "? (dirs+) scan node [%s] by %5llu", duf_scanstage_name( scanstage ), duf_levinfo_dirid( PDI ) );
#if 1
    if ( ( scanner = duf_scanstage_scanner( SCCB, scanstage, 1 /* deleted */ , DUF_NODE_NODE ) ) && duf_levinfo_if_deleted( PDI ) )
    {
      sccbh->current_scanner = scanner;
      sccbh->current_node_type = DUF_NODE_NODE;
      MAST_TRACE( scan, 4, "scan node %s_deleted by %5llu", duf_scanstage_name( scanstage ), duf_levinfo_dirid( PDI ) );
      DOR( r, ( scanner ) ( pstmt, PDI ) );
      assert( sccbh->current_node_type == DUF_NODE_NODE );
      if ( sccbh->atom_cb )                                          /* atom is fs-direntry(dir or reg) or item(node or leaf) */
        sccbh->atom_cb( sccbh, scanstage, pstmt, scanner, DUF_NODE_NODE, r );
    }
    else if ( ( scanner = duf_scanstage_scanner( SCCB, scanstage, 0 /* deleted */ , DUF_NODE_NODE ) ) )
    {
      sccbh->current_scanner = scanner;
      sccbh->current_node_type = DUF_NODE_NODE;
      MAST_TRACE( scan, 4, "scan node %s by %5llu", duf_scanstage_name( scanstage ), duf_levinfo_dirid( PDI ) );
      DOR( r, ( scanner ) ( pstmt, PDI ) );
      assert( sccbh->current_node_type == DUF_NODE_NODE );
      if ( sccbh->atom_cb )                                          /* atom is fs-direntry(dir or reg) or item(node or leaf) */
        sccbh->atom_cb( sccbh, scanstage, pstmt, scanner, DUF_NODE_NODE, r );
    }
#else
    if ( ( scanner = duf_scanstage_scanner( SCCB, scanstage, duf_levinfo_if_deleted( PDI ), DUF_NODE_NODE ) ) )
    {
      sccbh->current_scanner = scanner;
      MAST_TRACE( scan, 4, "scan node %s_deleted by %5llu", duf_scanstage_name( scanstage ), duf_levinfo_dirid( PDI ) );
      DOR( r, ( scanner ) ( pstmt, PDI ) );
    }
#endif
    else
    {
      MAST_TRACE( scan, 4, "NOT scan node %s by %5llu - sccb->node_scan_%s empty for %s", duf_scanstage_name( scanstage ),
                  duf_levinfo_dirid( PDI ), duf_scanstage_name( scanstage ), duf_uni_scan_action_title( SCCB ) );
    }
  }
  DUF_ENDR( r );
}

/* 20150820.085615 */
/* TODO duf_sccb_handle_t * sccbh must be first in all such funcs 20160210.120016 */
int
duf_sccbh_eval_db_subnodes( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt MAS_UNUSED, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );

  assert( sccbh );
  assert( SCCB );

#ifdef DUF_NO_RNUMS
  MAST_TRACE( sql, 0, "@@@EACH SUB %llu ... %s", duf_levinfo_dirid( PDI ), sqlite3_sql( pstmt ) );
#else
  MAST_TRACE( sql, 0, "@@@EACH SUB %llu {%llu}... %s", duf_levinfo_dirid( PDI ), pstmt ? DUF_GET_UFIELD2( rnfiles ) : 0, sqlite3_sql( pstmt ) );
#endif
  MAST_TRACE( scan, 4, "scan dirent by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

  DORF( r, duf_sccbh_eval_db_items_str_cb, sccbh, DUF_NODE_NODE, duf_sccbh_eval_all, scanstage );
  DUF_ENDR( r );
}
