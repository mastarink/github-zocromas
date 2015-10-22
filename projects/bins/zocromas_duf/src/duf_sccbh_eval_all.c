#include <unistd.h>
#include <libgen.h>
#include <stddef.h>

#include "duf_maintenance.h"

#include "duf_option_defs.h"

#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"

#include "duf_levinfo_ref.h"

#include "duf_dir_scan2_stages.h" /* rename to duf_dir_scan2_passes.h */
/* #include "duf_item_scan2.h" */

#include "duf_sccb.h"
#include "duf_sccb_def.h"
#include "duf_sccbh_eval.h"
#include "duf_sccbh_eval_fs.h"

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

DUF_WRAPSTATIC int
duf_eval_sccbh_all( duf_stmnt_t * pstmt_selector, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  assert( SCCB );
  /* duf_scan_qbeginning_sql( SCCB ); */
/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem
 *                                     -- see duf_dir_scan2_passs.c
 * */
  duf_str_cb2_t passes[] = {
    [DUF_SCANSTAGE_FS_ITEMS] /*        */  = /*                   */ DUF_WRAPPED( duf_sccbh_eval_fs ), /* SCCB->dirent_file_scan_before2, SCCB->dirent_dir_scan_before2 */
    [DUF_SCANSTAGE_NODE_BEFORE] /*     */  = DUF_ACTG_FLAG( allow_dirs ) /*  */ ? duf_sccbh_eval_db_node : NULL /* duf_sccbh_eval_db_node_before */ ,
    [DUF_SCANSTAGE_DB_LEAVES_NOFD] /*  */  = DUF_ACTG_FLAG( allow_files ) /* */ ? duf_sccbh_eval_db_leaves_nofd : NULL,
    [DUF_SCANSTAGE_DB_LEAVES_FD] /*    */  = DUF_ACTG_FLAG( allow_files ) /* */ ? duf_sccbh_eval_db_leaves_fd : NULL,
    [DUF_SCANSTAGE_NODE_MIDDLE] /*     */  = DUF_ACTG_FLAG( allow_dirs ) /*  */ ? duf_sccbh_eval_db_node : NULL /* duf_sccbh_eval_db_node_middle */ ,
    [DUF_SCANSTAGE_DB_SUBNODES] /*     */  = DUF_ACTG_FLAG( allow_sub ) /*   */ ? duf_sccbh_eval_db_subnodes : NULL,
    [DUF_SCANSTAGE_NODE_AFTER] /*      */  = DUF_ACTG_FLAG( allow_dirs ) /*  */ ? duf_sccbh_eval_db_node : NULL /* duf_sccbh_eval_db_node_after */ ,
    NULL
  };
  DUF_TRACE( scan, 3, "scan passes by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );
  DUF_TRACE( sccbh, 4, "(pstmt:%d) passes (%s)", pstmt_selector ? 1 : 0, duf_uni_scan_action_title( SCCB ) );
#if 0
  {
    int nn = 0;

    for ( duf_str_cb2_t * ppass = passes; DUF_NOERROR( r ) && *ppass; nn++, ppass++ )
    {
      DUF_TRACE( scan, 4, "scan pass %d by %5llu:%s; %s", nn, duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

      DUF_TRACE( sccbh, 2, "%d. pass (%s) %s", nn, duf_uni_scan_action_title( SCCB ), SCCB->name );
      /* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */
      DOR( r, ( *ppass ) ( scanstage_fake, pstmt_selector, sccbh ) );
      /*                                                     */ DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
    }
  }
#else
  for ( duf_scanstage_t scanstage = DUF_SCANSTAGE_MIN; scanstage < DUF_SCANSTAGE_MAX; scanstage++ )
  {
    DUF_TRACE( scan, 4, "scan pass %d by %5llu:%s; %s", scanstage, duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ),
               duf_levinfo_path( PDI ) );

    DUF_TRACE( sccbh, 2, "%d. pass (%s) %s", scanstage, duf_uni_scan_action_title( SCCB ), SCCB->name );
    /* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */
    if ( passes[scanstage] )
      DOR( r, ( passes[scanstage] ) ( scanstage, pstmt_selector, sccbh ) );
    /*                                                     */ DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
  }
#endif
  DUF_TRACE( sccbh, 4, "(pstmt:%d) /passes (%s)", pstmt_selector ? 1 : 0, duf_uni_scan_action_title( SCCB ) );
  DUF_CLEAR_ERROR( r, DUF_ERROR_TOO_DEEP ); /* reset error if it was `MAX_DEPTH` */

  DUF_TRACE( scan, 3, "/scan passes by %5llu:%s; %s", duf_levinfo_dirid( PDI ), duf_uni_scan_action_title( SCCB ), duf_levinfo_path( PDI ) );

  DEBUG_ENDR( r );
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
  DEBUG_STARTR( r );

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

  DUF_TRACE( sccbh, 0, "eval sccbh all %s at %llu:%s", SCCB->name, duf_levinfo_dirid( PDI ), duf_levinfo_path( PDI ) );

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
  DEBUG_ENDR( r );
}
#endif
