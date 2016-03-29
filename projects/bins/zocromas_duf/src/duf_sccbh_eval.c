/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */


#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_output_util.h"

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "duf_sccb_structs.h"
#include "duf_sccb_scanstage.h"

#include "duf_sccb_handle.h"
#include "duf_sccbh_eval_leaf.h"

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_sccbh_eval_all.h"
#include "duf_sccbh_eval_sql_set.h"
/* #include "duf_sccbh_eval_fs.h" */
#include "duf_sccbh_shortcuts.h"

#include "duf_sccb_row.h"

#include "duf_sql_positional.h"                                      /* duf_sql_column_long_long etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */

#include "duf_pdi_structs.h"
/* ###################################################################### */
#include "duf_sccbh_eval.h"
/* ###################################################################### */

SR( SCCBH, sccbh_call_scanner, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage, duf_scanner_t scanner,
    duf_node_type_t node_type )
{
  if ( scanner )
  {
    H_PDI->items.total++;
  /* sccbh->current_scanner = scanner; */
    sccbh->assert__current_node_type = node_type;
    if ( node_type == DUF_NODE_NODE )
    {
      H_PDI->items.dirs++;
    }
    else if ( node_type == DUF_NODE_LEAF )
    {
      H_PDI->items.files++;
    }
  /* QT( "@X %d : %s : %p", H_SCCBI, H_SCCB->name,scanner  ); */
    {
      int fd;

      fd = duf_levinfo_dfd( H_PDI );
      if ( fd > 0 )
        lseek( fd, 0, SEEK_SET );

      assert( fd <= 0 || lseek( fd, 0, SEEK_CUR ) == 0 );
    }
#if 0
    for ( int i = 0; i < duf_sql_column_count( pstmt ); i++ )        /* sqlite3_column_count( pstmt ) */
    {
      const char *s;
      const char *n;
      const char *st;
      duf_sqltype_t it;

      n = duf_sql_column_name( pstmt, i );                           /* sqlite3_column_name */
      s = duf_sql_column_string( pstmt, i );                         /* sqlite3_column_text( pstmt, i ) */
      st = duf_sql_column_decltype( pstmt, i );
      it = duf_sql_column_type( pstmt, i );
      if ( s )
        QT( "@@%d. %s/%d %s='%s'", i, st, it, n, s );
      else
        QT( "@@%d. %s/%d %s=NULL", i, st, it, n );
    }
#endif

    int eq = 0;
    duf_sccb_data_row_t *new_row = NULL;

    new_row = CRX( datarow_create, pstmt, CRX( pdi_pathinfo, H_PDI ) );
  /* QT( "@A %d : %d", sccbh->pdi->pathinfo.levinfo[17].node_type, new_row->pathinfo.levinfo[17].node_type ); */
    if ( sccbh->rows && sccbh->rows->prev )
    {
      unsigned long long sha1id0;
      unsigned long long sha1id1;

      sha1id0 = CRX( datarow_get_number, new_row, "sha1id" );
      sha1id1 = CRX( datarow_get_number, sccbh->rows, "sha1id" );
    /* QT( "@B %d : %d", sccbh->pdi->pathinfo.levinfo[17].node_type, new_row->pathinfo.levinfo[17].node_type ); */
      eq = ( sha1id0 == sha1id1 );
      {
        if ( !eq )
        {
          QT( "@@---A %lld ? %lld : %p:%p", sha1id0, sha1id1, sccbh->rows, sccbh->rows->prev );
        /* assert(0); */
          CRX( sccbh_rows_eval, sccbh );
        }
      }
    }

    CRV( ( scanner ), pstmt, H_PDI, sccbh );
    if ( new_row )
    {
      new_row->prev = sccbh->rows;
      sccbh->rows = new_row;
    }
    if ( sccbh->atom_cb )                                            /* atom is fs-direntry(dir or reg) or item(node or leaf) */
      sccbh->atom_cb( sccbh, pstmt, scanstage, scanner, node_type, QERRIND );
    assert( sccbh->assert__current_node_type == node_type );
  }
/* QT( "@@@ %p scanstage: %s @ %s (%s)", scanner, duf_scanstage_name( scanstage ), duf_uni_scan_action_title( H_SCCB ), QERRNAME ); */
  ER( SCCBH, sccbh_call_scanner, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_scanstage_t scanstage, duf_scanner_t scanner,
      duf_node_type_t node_type );
}

/* 20150819.164652 */
SR( SCCBH, sccbh_eval_all_and_summary_i, duf_sccb_handle_t * sccbh, bool f_summary )
{
/* DUF_STARTR( r ); */

  ERRLOWER( TOO_DEEP );
  assert( sccbh );
  assert( H_SCCB );

  MAST_TRACE( sccbh, 0, "eval sccbh ALL (%d) %s", sccbh ? 1 : 0, H_SCCB->name );
  CR( sccbh_eval_all, sccbh, ( duf_stmnt_t * ) NULL, ( duf_scanstage_t ) NULL ); /* XXX XXX XXX XXX XXX XXX */
  MAST_TRACE( sccbh, 0, "(%s) eval sccbh ALL done (%d) %s", QERRNAME, sccbh ? 1 : 0, H_SCCB->name );

  if ( QNOERR && f_summary )
  {
    DUF_PRINTF( 0, "%s", duf_uni_scan_action_title( H_SCCB ) );

    DUF_PRINTF( 0, " summary; seq:     %llu", H_PDI->seq );
    DUF_PRINTF( 0, " summary; seq-leaf:%llu", H_PDI->seq_leaf );
    DUF_PRINTF( 0, " summary; seq-node:%llu", H_PDI->seq_node );
    DUF_PRINTF( 0, " summary;  seq-row:%llu", H_PDI->seq_row );
    if ( H_PU->max_seq )
      DUF_PRINTF( 0, " of %llu (max-seq)", H_PU->max_seq );
    DUF_PRINTF( 0, " summary; changes:%llu", H_HCHANGES );
    DUF_PRINTF( 0, " summary; total-bytes:%llu", H_PDI->total_bytes );
  }
  ERRUPPER( TOO_DEEP );
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_all_and_summary_i, duf_sccb_handle_t * sccbh, bool f_summary );
}

#ifdef MAS_WRAP_FUNC
/* 20150819.164643 */
SR( SCCBH, sccbh_eval_all_and_summary, duf_sccb_handle_t * sccbh, bool f_summary )
{
/* DUF_STARTR( r ); */
  if ( sccbh && H_SCCB )
  {
    MAST_TRACE( explain, 10, "scan targ; action title: %s", duf_uni_scan_action_title( H_SCCB ) );
    MAST_TRACE( action, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan", duf_uni_scan_action_title( H_SCCB ) );
    MAST_TRACE( sccbh, 1, "%" DUF_ACTION_TITLE_FMT ": inited scan %s", duf_uni_scan_action_title( H_SCCB ), H_SCCB->name );

    {
      H_HCHANGES = 0;
      if ( duf_levinfo_path( H_PDI ) )
      {
        CR( sccbh_eval_all_and_summary_i, sccbh, f_summary );
      }
      else
      {
      /* assert( 0 ); */
      }
    }
  }
  else
  {
    ERRMAKE( SCCB );
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_all_and_summary, duf_sccb_handle_t * sccbh, bool f_summary );
}
#endif

/* 20160212.130816 */

SR( SCCBH, sccbh_eval_db_items_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */

  duf_sql_set_pair_t sql_set_pair = {.orderid = 0, NULL, NULL };

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
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( H_PDI ), H_PDI, " >>> 4. set '%s' str_cb2%c", set_type_title, str_cb2 ? '+' : '-' );
    MAST_TRACE( scan, 10, "ql%llu / qn%llu / w%llu / q%llu %s", H_PDI->seq_leaf, H_PDI->seq_node, H_PDI->seq_row, H_PDI->seq, H_SCCB->title );

    MAST_TRACE( sccbh, 2, "@@@@has selector for %s(%d) dirid:%llu (%s) %s", set_type_title, node_type, duf_levinfo_dirid( H_PDI ),
                duf_uni_scan_action_title( H_SCCB ), H_SCCB->name );
#if 0
    DOR( r, duf_scan_db_items_with_str_cb_sql_set( sql_set_pair.active, str_cb2, sccbh, node_type ) );
#else
    CR( eval_sccbh_sql_set_str_cb, sccbh, node_type, sql_set_pair, str_cb2, scanstage );
#endif
    if ( QNOERR )
    {
      if ( QERRIND > 0 )
      {
        MAST_TRACE( explain, 20, "%u records processed of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                    QERRIND, set_type_title, duf_uni_scan_action_title( H_SCCB ), duf_levinfo_dirid( H_PDI ) );
      }
      else
      {
        MAST_TRACE( explain, 30, "no records found of type ≪%s≫ ; action ≪%s≫; diridpid:%llu",
                    set_type_title, duf_uni_scan_action_title( H_SCCB ), duf_levinfo_dirid( H_PDI ) );
      }
    }
  }
  else
  {
    MAST_TRACE( explain, 10, "= ? ============ NO selector2 ≪%s≫", set_type_title );
    ERRMAKE( PTR );
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_db_items_str_cb, duf_sccb_handle_t * sccbh, duf_node_type_t node_type, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage );
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
  MAST_TRACE( sccbh, 10, "%s: leaf_scan_fd2:%d; leaf_scan:%d;", H_SCCB->name, H_SCCB->leaf_scan_fd2 ? 1 : 0, H_SCCB->leaf_scan2 ? 1 : 0 );
/* if ( DUF_ACTG_FLAG( allow_files ) ) */
  {
    MAST_TRACE( sccbh, 2, "@@@scan files (%s) %s", duf_uni_scan_action_title( H_SCCB ), H_SCCB->name );
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( H_PDI ), H_PDI, " >>> 2." );
    if ( scanstage == DUF_SCANSTAGE_DB_LEAVES_FD && H_SCCB->leaf_scan_fd2 )
    {
      MAST_TRACE( sccbh, 4, "%s: leaf_scan_fd2:%d; leaf_scan:%d;", H_SCCB->name, H_SCCB->leaf_scan_fd2 ? 1 : 0, H_SCCB->leaf_scan2 ? 1 : 0 );
    /* duf_sccbh_eval_db_leaf_fd_str_cb is just a wrapper for sccb->leaf_scan_fd2 */
      DOR( r, duf_sccbh_eval_db_items_str_cb( sccbh, DUF_NODE_LEAF, duf_sccbh_eval_db_leaf_fd_str_cb, scanstage ) );
    }
    if ( scanstage == DUF_SCANSTAGE_DB_LEAVES_NOFD && H_SCCB->leaf_scan2 )
    {
      MAST_TRACE( sccbh, 10, "%s: leaf_scan_fd2:%d; leaf_scan:%d;", H_SCCB->name, H_SCCB->leaf_scan_fd2 ? 1 : 0, H_SCCB->leaf_scan2 ? 1 : 0 );
    /* duf_sccbh_eval_db_leaf_str_cb is just a wrapper for sccb->leaf_scan2 */
      DOR( r, duf_sccbh_eval_db_items_str_cb( sccbh, DUF_NODE_LEAF, duf_sccbh_eval_db_leaf_str_cb, scanstage ) );
    }
  }
  DUF_ENDR( r );
}
#endif
SR( SCCBH, sccbh_eval_db_leaves_nofd, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */
/* scan this files in this directory */
/* if ( DUF_ACTG_FLAG( allow_files ) ) */
  {
    MAST_TRACE( sccbh, 2, "@@@@scan files (%s) %s", duf_uni_scan_action_title( H_SCCB ), H_SCCB->name );
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( H_PDI ), H_PDI, " >>> 2." );
    MAST_TRACE( sccbh, 2, "@@@@@%s: leaf scan:%d;", duf_uni_scan_action_title( H_SCCB ), H_SCCB->leaf_scan2 ? 1 : 0 );
    if ( H_SCCB->leaf_scan2 )
    {
    /* duf_sccbh_eval_db_leaf_str_cb is just a wrapper for sccb->leaf_scan2 */
      CR( sccbh_eval_db_items_str_cb, sccbh, DUF_NODE_LEAF, duf_sccbh_eval_db_leaf_str_cb, scanstage );
    }
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_db_leaves_nofd, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage );
}

SR( SCCBH, sccbh_eval_db_leaves_fd, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */
/* scan this files in this directory */
/* if ( DUF_ACTG_FLAG( allow_files ) ) */
  {
    MAST_TRACE( sccbh, 2, "@@@@scan files fd (%s) %s", duf_uni_scan_action_title( H_SCCB ), H_SCCB->name );
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( H_PDI ), H_PDI, " >>> 2." );
    MAST_TRACE( sccbh, 2, "@@@@@%s: leaf scan fd:%d;", duf_uni_scan_action_title( H_SCCB ), H_SCCB->leaf_scan_fd2 ? 1 : 0 );
    if ( H_SCCB->leaf_scan_fd2 )
    {
    /* duf_sccbh_eval_db_leaf_fd_str_cb is just a wrapper for sccb->leaf_scan_fd2 */
      CR( sccbh_eval_db_items_str_cb, sccbh, DUF_NODE_LEAF, duf_sccbh_eval_db_leaf_fd_str_cb, scanstage );
    }
  }
/* DUF_ENDR( r ); */
  ER( SCCBH, sccbh_eval_db_leaves_fd, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_scanstage_t scanstage );
}
