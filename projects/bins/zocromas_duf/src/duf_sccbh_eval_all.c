#undef MAS_TRACING
#include <unistd.h>
#include <libgen.h>
#include <stddef.h>

#include "duf_maintenance.h"


#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_option_defs.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"

#include "duf_levinfo_ref.h"

/* #include "duf_dir_scan2_stages.h" */
/* #include "duf_item_scan2.h" */

#include "duf_sccb.h"
#include "duf_sccb_def.h"
#include "duf_sccbh_eval.h"
#include "duf_sccbh_eval_fs.h"
#include "duf_sccb_scanstage.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccbh_eval_all.h"
/* ###################################################################### */


/* duf_eval_sccbh_all:
 *  -= Walk (scan) dirs from DB =-
 *
 * consecutively call various scanner passes with the sccb
 * on current dir from pdi
 * 
 * i.e. [ evaluate sccb for current dir ]
 *
 * args:
 *   pstmt: sql(ite) prepared statement
 *   pdi:  depthinfo structure ptr
 *   sccb: module callbacs structure
 *
 * */

int
duf_eval_sccbh_scanstage( duf_stmnt_t * pstmt_selector, duf_sccb_handle_t * sccbh, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );
  unsigned allow_fs = !DUF_CONFIGG( opt.disable.flag.fs );
  unsigned allow_dirs = DUF_ACTG_FLAG( allow_dirs );
  unsigned allow_files = DUF_ACTG_FLAG( allow_files );
  unsigned allow_sub = DUF_ACTG_FLAG( allow_sub );
  unsigned linear = duf_pdi_linear( PDI );

  duf_str_cb2_t passes[] = {
    [DUF_SCANSTAGE_FS_ITEMS] /*        */  = /*      */ allow_fs /*           */ ? DUF_WRAPPED( duf_sccbh_eval_fs ) : NULL,
    [DUF_SCANSTAGE_NODE_BEFORE] /*     */  = !linear && allow_dirs /*         */ ? duf_sccbh_eval_db_node : NULL,
    [DUF_SCANSTAGE_DB_LEAVES_NOFD] /*  */  = /*      */ allow_files /*        */ ? duf_sccbh_eval_db_leaves_nofd : NULL,
    [DUF_SCANSTAGE_DB_LEAVES_FD] /*    */  = /*      */ allow_files /*        */ ? duf_sccbh_eval_db_leaves_fd : NULL,
    [DUF_SCANSTAGE_NODE_MIDDLE] /*     */  = !linear && allow_dirs /*         */ ? duf_sccbh_eval_db_node : NULL,
    [DUF_SCANSTAGE_DB_SUBNODES] /*     */  = !linear && allow_sub /*          */ ? duf_sccbh_eval_db_subnodes : NULL,
    [DUF_SCANSTAGE_NODE_AFTER] /*      */  = !linear && allow_dirs /*         */ ? duf_sccbh_eval_db_node : NULL,
    NULL
  };


  DUF_TRACE( scan, 4, "scan stage %s(%d) by %5llu:%s; %s", duf_scanstage_name( scanstage ), scanstage, duf_levinfo_dirid( PDI ),
             duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

  DUF_TRACE( sccbh, 2, "@%15s(%d). pass (%s)", duf_scanstage_name( scanstage ), scanstage, duf_uni_scan_action_title( SCCB ) );
  /* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */
  if ( passes[scanstage] )
  {
    sccbh->current_statement = pstmt_selector;
    sccbh->current_scanstage = scanstage;
    /* T( "@@@############# hhh: %s @ %s", duf_scanstage_name( scanstage ), duf_uni_scan_action_title( SCCB ) ); */
    DOR( r, ( passes[scanstage] ) ( scanstage, pstmt_selector, sccbh ) );
    DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
    sccbh->current_scanstage = DUF_SCANSTAGE_NONE;
    sccbh->current_statement = NULL;
  }
  /* T( "@@@@%s - %s : %s", duf_scanstage_name( scanstage ), duf_levinfo_relpath( PDI ), duf_levinfo_itemtruename( PDI ) ); */
  DUF_ENDR( r );

}

DUF_WRAPSTATIC int
duf_eval_sccbh_all( duf_stmnt_t * pstmt_selector, duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );

  assert( SCCB );
  /* T( "@@@%s", duf_levinfo_path( PDI ) ); */
  /* duf_scan_qbeginning_sql( SCCB ); */
/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem
 *                                     -- see duf_dir_scan2_passs.c
 * */
  DUF_TRACE( scan, 3, "scan passes by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );
  DUF_TRACE( sccbh, 4, "(pstmt:%d) passes (%s)", pstmt_selector ? 1 : 0, duf_uni_scan_action_title( SCCB ) );
  /* T( "Q: TOTCOUNTED:%d; TOTITEMS:%llu for %s", TOTCOUNTED, TOTITEMS, duf_uni_scan_action_title( SCCB ) ); */
  if ( !SCCB->disabled /* && ( !TOTCOUNTED || TOTITEMS ) TODO FIXME */ )
  {
    /* T( "DO: TOTCOUNTED:%d; TOTITEMS:%llu for %s", TOTCOUNTED, TOTITEMS, duf_uni_scan_action_title( SCCB ) ); */

    for ( duf_scanstage_t scanstage = DUF_SCANSTAGE_MIN; scanstage <= DUF_SCANSTAGE_MAX; scanstage++ )
    {
      DORF( r, duf_eval_sccbh_scanstage, pstmt_selector, sccbh, scanstage );
    }
  }
  else
  {
    /* T( "SKIP: TOTCOUNTED:%d; TOTITEMS:%llu [%s] for %s", TOTCOUNTED, TOTITEMS, duf_scanstage_name( scanstage ), duf_uni_scan_action_title( SCCB ) ); */

    T( "SKIP: TOTCOUNTED:%d; TOTITEMS:%llu for %s", TOTCOUNTED, TOTITEMS, duf_uni_scan_action_title( SCCB ) );

    /* assert( 0 );                                                                                               */
  }

  DUF_TRACE( sccbh, 4, "(pstmt:%d) /passes (%s)", pstmt_selector ? 1 : 0, duf_uni_scan_action_title( SCCB ) );


  /* FIXME */
  DUF_CLEAR_ERROR( r, DUF_ERROR_TOO_DEEP ); /* reset error if it was `MAX_DEPTH` */


  DUF_TRACE( scan, 3, "/scan passes by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

  DUF_ENDR( r );
}

/* duf_scan_dirs_by_pdi_wrap          ( duf_scan_dirs_by_parentid )
 *  -= Walk (scan) dirs from DB =-
 *
 * see duf_eval_sccbh_all
 *
 * str_cb2 (sub-item scanner) one of:
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
#ifdef MAS_WRAP_FUNC
int DUF_WRAPPED( duf_eval_sccbh_all ) ( duf_scanstage_t scanstage_fake DUF_UNUSED, duf_stmnt_t * pstmt_selector, duf_sccb_handle_t * sccbh )
{
  DUF_STARTR( r );

  assert( PDI );
  assert( PDI->pathinfo.depth >= 0 );
  assert( sccbh );
  /* assert( pstmt_selector ); */

#  ifdef MAS_TRACING
  unsigned long long diridpid;

  diridpid = duf_levinfo_dirid( PDI );
  DUF_TRACE( scan, 3, "[%llu]", diridpid );
  DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, "** depth:%d/%d; diridpid:%llu", duf_pdi_depth( PDI ),
                duf_pdi_reldepth( PDI ), diridpid );

  DUF_TRACE( scan, 3, "[%llu]  : scan start      +" DUF_DEPTH_PFMT "", diridpid, duf_pdi_depth( PDI ) );

  DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 1." );

  DUF_TRACE( explain, 2,
             "≫≫≫≫≫≫≫≫≫≫  to scan %" "s" /* DUF_ACTION_TITLE_FMT */ " ≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪",
             duf_uni_scan_action_title( SCCB ) );

  DUF_TRACE( sccbh, 1, "eval sccbh all %s at %llu:%s", SCCB->name, duf_levinfo_dirid( PDI ), duf_levinfo_path( PDI ) );

#  endif
  if ( !SCCB->disabled )
  {                             /* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */
    DOR( r, duf_eval_sccbh_all( pstmt_selector, sccbh ) );
  }
  else
  {
    DUF_TRACE( sccbh, 0, "@eval sccbh all disabled %s", SCCB->name );
  }

  DUF_TRACE( scan, 3, "[%llu]  : scan end      +" DUF_DEPTH_PFMT "", diridpid, duf_pdi_depth( PDI ) );
  DUF_ENDR( r );
}
#endif
