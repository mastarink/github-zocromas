/* #undef MAS_TRACING */
#include <assert.h>
#include <unistd.h>
#include <libgen.h>
#include <stddef.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi.h"                                                 /* duf_pdi_init; duf_pdi_shut; duf_pdi_close ✗ */
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sccb.h"
#include "duf_sccb_def.h"
#include "duf_sccb_structs.h"
#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */
#include "duf_sccbh_eval.h"
#include "duf_sccbh_eval_node.h"
#include "duf_sccbh_eval_fs.h"

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_scanner.h"

#include "duf_pdi_structs.h"                                         /* H_PDI->items */
/* ###################################################################### */
#include "duf_sccbh_eval_all.h"                                      /* duf_sccbh_eval_all ✗ */
/* ###################################################################### */

/* duf_sccbh_eval_all:
 *  -= Walk (scan) dirs from DB =-
 *
 * consecutively call various scanner passes with the sccb
 * on current dir from pdi
 * 
 * i.e. [ evaluate sccb for current dir ]
 *
 * args:
 *   pstmt_x: sql(ite) prepared statement
 *   pdi:  depthinfo structure ptr
 *   sccb: module callbacs structure
 *
 * */

static
SR( SCCBH, eval_sccbh_scanstage, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_selector MAS_UNUSED, */ duf_scanstage_t scanstage )
{
  unsigned allow_fs = ! /* DUF_CONFIGG( opt.disable.flag.fs ) */ CRX( get_config_flag_disable_fs, );
  unsigned allow_dirs = /* DUF_ACTG_FLAG( allow_dirs ) */ CRX( get_config_flag_act_allow_dirs, );
  unsigned allow_files = /* DUF_ACTG_FLAG( allow_files ) */ CRX( get_config_flag_act_allow_files, );
  unsigned allow_sub = /* DUF_ACTG_FLAG( allow_sub ) */ CRX( get_config_flag_act_allow_sub, );
  unsigned linear = CRX( pdi_linear, H_PDI );

/* only sccbh_eval_db_subnodes needs pstmt_selector */
  duf_str_cb2s_t passes[] = {
    [DUF_SCANSTAGE_FS_ITEMS] /*        */  = /*      */ allow_fs /*           */ ? F2ND( sccbh_eval_fs ) : NULL,
    [DUF_SCANSTAGE_NODE_BEFORE] /*     */  = !linear && allow_dirs /*         */ ? F2ND(  /* sccbh_eval_db_node */ sccbh_eval_db_node_new ) : NULL,
    [DUF_SCANSTAGE_DB_LEAVES] /*       */  = /*      */ allow_files /*        */ ? F2ND( sccbh_eval_db_leaves_new ) : NULL,
    [DUF_SCANSTAGE_DB_LEAVES_NOFD] /*  */  = /*      */ allow_files /*        */ ? F2ND( sccbh_eval_db_leaves_nofd ) : NULL,
    [DUF_SCANSTAGE_DB_LEAVES_FD] /*    */  = /*      */ allow_files /*        */ ? F2ND( sccbh_eval_db_leaves_fd ) : NULL,
    [DUF_SCANSTAGE_DB_LEAVES_PACK] /*  */  = /*      */ allow_files /*        */ ? F2ND( sccbh_eval_db_leaves_new ) : NULL,
    [DUF_SCANSTAGE_NODE_MIDDLE] /*     */  = !linear && allow_dirs /*         */ ? F2ND(  /* sccbh_eval_db_node */ sccbh_eval_db_node_new ) : NULL,
    [DUF_SCANSTAGE_DB_SUBNODES] /*     */  = !linear && allow_sub /*          */ ? F2ND( sccbh_eval_db_subnodes ) : NULL,
    [DUF_SCANSTAGE_NODE_AFTER] /*      */  = !linear && allow_dirs /*         */ ? F2ND(  /* sccbh_eval_db_node */ sccbh_eval_db_node_new ) : NULL,
    NULL
  };

  MAST_TRACE( scan, 4, "scan stage %s(%d) by %5llu:%s; %s", CRX( scanstage_name, scanstage ), scanstage, CRX( levinfo_dirid, H_PDI ),
              CRX( uni_scan_action_title, H_SCCB ), CRX( levinfo_path, H_PDI ) );

  MAST_TRACE( sccbh, 2, "@%15s(%d). pass (%s)", CRX( scanstage_name, scanstage ), scanstage, CRX( uni_scan_action_title, H_SCCB ) );
/* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */
  if ( passes[scanstage] )
  {
  /* sccbh->current_statement = pstmt_selector; */
  /* sccbh->current_scanstage = scanstage; */
    H_PDI->items.total = 0;
    H_PDI->items.dirs = 0;
    /* QT( "@@scanstage:%s", CRX( scanstage_name, scanstage ) ); */
    CRV( ( passes[scanstage] ), sccbh, /* pstmt_selector, */ scanstage );
  /* QT( "@%d. %llu", scanstage, H_PDI->items.total ); */
    MAST_TRACE( scan, 4, "[%llu]", CRX( levinfo_dirid, H_PDI ) );
  /* sccbh->current_scanstage = DUF_SCANSTAGE_NONE; */
  /* sccbh->current_statement = NULL; */
  }
/* QT( "@@@@%s - %s : %s", CRX(scanstage_name, scanstage ), CRX(levinfo_relpath, H_PDI ), CRX(levinfo_itemtruename, H_PDI ) ); */

  ER( SCCBH, eval_sccbh_scanstage, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_selector, */ duf_scanstage_t scanstage );
}

static
SR( SCCBH, sccbh_eval_all_i, duf_sccb_handle_t * sccbh /* , duf_stmnt_t * pstmt_selector MAS_UNUSED */  )
{
  assert( H_SCCB );
/* QT( "@@@%s", CRX(levinfo_path, H_PDI ) ); */
/* CRX(scan_qbeginning_sql, H_SCCB ); */
/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem
 *                                     -- see duf_dir_scan2_passs.c
 * */
  MAST_TRACE( scan, 3, "scan passes by %5llu:%s; %s", CRX( levinfo_dirid, H_PDI ), CRX( uni_scan_action_title, H_SCCB ), CRX( levinfo_path, H_PDI ) );
  MAST_TRACE( sccbh, 4, "passes (%s)", CRX( uni_scan_action_title, H_SCCB ) );
/* QT( "Q: H_TOTCOUNTED:%d; H_TOTITEMS:%llu for %s", H_TOTCOUNTED, H_TOTITEMS, CRX(uni_scan_action_title, H_SCCB ) ); */
  if ( !H_SCCB->disabled /* && ( !H_TOTCOUNTED || H_TOTITEMS ) TODO FIXME */  )
  {
  /* QT( "DO: H_TOTCOUNTED:%d; H_TOTITEMS:%llu for %s", H_TOTCOUNTED, H_TOTITEMS, CRX(uni_scan_action_title, H_SCCB ) ); */

    for ( duf_scanstage_t scanstage = DUF_SCANSTAGE_MIN; scanstage <= DUF_SCANSTAGE_MAX; scanstage = scanstage << 1 )
    {
      /* QT( "@@@scanstage:%s", CRX( scanstage_name, scanstage ) ); */
      assert( scanstage );
      CR( eval_sccbh_scanstage, sccbh, /* pstmt_selector, */ scanstage );
    }
  }
  else
  {
  /* QT( "SKIP: H_TOTCOUNTED:%d; H_TOTITEMS:%llu [%s] for %s", H_TOTCOUNTED, H_TOTITEMS, CRX(scanstage_name, scanstage ), CRX(uni_scan_action_title, H_SCCB ) ); */

    QT( "SKIP: H_TOTCOUNTED:%d; H_TOTITEMS:%llu for %s", H_TOTCOUNTED, H_TOTITEMS, CRX( uni_scan_action_title, H_SCCB ) );
  /* assert( 0 );                                                                                               */
  }

  MAST_TRACE( sccbh, 4, "/passes (%s)", CRX( uni_scan_action_title, H_SCCB ) );
/* FIXME */
  ERRCLEAR( TOO_DEEP );                                              /* reset error if it was `MAX_DEPTH` */
  MAST_TRACE( scan, 3, "/scan passes by %5llu:%s; %s", CRX( levinfo_dirid, H_PDI ), CRX( uni_scan_action_title, H_SCCB ),
              CRX( levinfo_path, H_PDI ) );
  ER( SCCBH, sccbh_eval_all_i, duf_sccb_handle_t * sccbh /* , duf_stmnt_t * pstmt_selector */  );
}

/* duf_scan_dirs_by_pdi_wrap          ( duf_scan_dirs_by_parentid )
 *  -= Walk (scan) dirs from DB =-
 *
 * see duf_sccbh_eval_all
 *
 * str_cb2 (sub-item scanner) one of:
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
SR( SCCBH, sccbh_eval_all, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ duf_scanstage_t scanstage_fake MAS_UNUSED )
{
  assert( H_PDI );
  assert( CRX( pdi_depth, H_PDI ) /* H_PDI->pathinfo.depth */  >= 0 );
  assert( sccbh );
/* assert( pstmt_selector ); */
/* assert( pstmt_selector == duf_pdi_each_stmt( H_PDI, 1 ) ); */
#ifdef MAS_TRACING
  unsigned long long diridpdi;

  diridpdi = CRX( levinfo_dirid, H_PDI );

  MAST_TRACE( scan, 3, "[%llu]", diridpdi );
  DUF_SCCB_PDI( MAST_TRACE, scan, 10 + CRX( pdi_reldepth, H_PDI ), H_PDI, "** depth:%d/%d; diridpdi:%llu", CRX( pdi_depth, H_PDI ),
                CRX( pdi_reldepth, H_PDI ), diridpdi );
  MAST_TRACE( scan, 3, "[%llu]  : scan start      +" DUF_DEPTH_PFMT "", diridpdi, CRX( pdi_depth, H_PDI ) );
  DUF_SCCB_PDI( MAST_TRACE, scan, 10 + CRX( pdi_reldepth, H_PDI ), H_PDI, " >>> 1." );
  MAST_TRACE( explain, 20, "≫≫≫≫≫≫≫≫≫≫  to scan %" "s" /* DUF_ACTION_TITLE_FMT */
              " ≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪", CRX( uni_scan_action_title, H_SCCB ) );
  MAST_TRACE( sccbh, 1, "eval sccbh all %s at %llu:%s", H_SCCB->name, CRX( levinfo_dirid, H_PDI ), CRX( levinfo_path, H_PDI ) );

#endif
  if ( !H_SCCB->disabled )
  {                                                                  /* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */
    CR( sccbh_eval_all_i, sccbh /* , pstmt_selector */  );
  }
  else
  {
    MAST_TRACE( sccbh, 0, "@eval sccbh all disabled %s", H_SCCB->name );
  }

#ifdef MAS_TRACING
  MAST_TRACE( scan, 3, "[%llu]  : scan end      +" DUF_DEPTH_PFMT "", diridpdi, CRX( pdi_depth, H_PDI ) );
#endif
  ER( SCCBH, sccbh_eval_all, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_selector, */ duf_scanstage_t scanstage_fake );
}
