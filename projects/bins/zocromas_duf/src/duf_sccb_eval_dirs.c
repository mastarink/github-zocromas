#include <unistd.h>
#include <assert.h>
#include <libgen.h>
#include <stddef.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"

#include "duf_dir_scan2_stages.h"

#include "duf_sccb.h"
#include "duf_sccb_def.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccb_eval_dirs.h"
/* ###################################################################### */


/* duf_sccbh_eval_pdi_dirs:
 *  -= Walk (scan) dirs from DB =-
 *
 * consecutively call various scanner stages with the sccb
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
duf_sccbh_eval_pdi_dirs( duf_sqlite_stmt_t * pstmt_selector, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );


  assert( SCCB );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
  /* duf_scan_qbeginning_sql( SCCB ); */
/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem
 *                                     -- see duf_dir_scan2_stages.c
 * */
  duf_str_cb2_t stages[] = {
    duf_scan_fs_items_with_sccb,         /* SCCB->dirent_file_scan_before2, SCCB->dirent_dir_scan_before2 */
    duf_scan_db_node_before_with_sccb,
    duf_scan_db_items_with_sccb,
    duf_scan_db_node_middle_with_sccb,
    duf_scan_db_subnodes_with_sccb,
    duf_scan_db_node_after_with_sccb,
    NULL
  };
  for ( duf_str_cb2_t * pstage = stages; *pstage; pstage++ )
  {
#if 0
    DUF_TRACE( temp, 0, "%lu : %lu : %lu : %lu : %p : %p : %p @ %p : %p", ( offsetof( duf_scan_callbacks_t, leaf ) ),
               ( offsetof( duf_scan_callbacks_t, init_scan ) ), ( offsetof( duf_scan_callbacks_t, node_scan_before2 ) ),
               ( offsetof( duf_scan_callbacks_t, node_scan_after2 ) ),
               /* */
               SCCB, &( SCCB->node_scan_after2 ),
               ( duf_scan_hook2_dir_t * ) ( ( ( char * ) SCCB ) + ( offsetof( duf_scan_callbacks_t, node_scan_after2 ) ) ), SCCB->node_scan_after2,
               *( ( duf_scan_hook2_dir_t * ) ( ( ( char * ) SCCB ) + ( offsetof( duf_scan_callbacks_t, node_scan_after2 ) ) ) ) );
#endif
    DOR( r, ( *pstage ) ( pstmt_selector, sccbh ) );
    /*                                                     */ DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
  }

  DEBUG_ENDR( r );
}

/* duf_scan_dirs_by_pdi_wrap          ( duf_scan_dirs_by_parentid )
 *  -= Walk (scan) dirs from DB =-
 *
 * see duf_sccbh_eval_pdi_dirs
 *
 * str_cb2 (sub-item scanner) one of:
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */
#ifdef MAS_WRAP_FUNC
int DUF_WRAPPED( duf_sccbh_eval_pdi_dirs ) ( duf_sqlite_stmt_t * pstmt_selector, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  assert( PDI );
  assert( PDI->depth >= 0 );
  /* assert( pstmt_selector ); */

#  ifdef MAS_TRACING
  unsigned long long diridpid;

  diridpid = duf_levinfo_dirid( PDI );
  DUF_TRACE( scan, 3, "[%llu]", diridpid );
  DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, "** depth:%d/%d; diridpid:%llu", duf_pdi_depth( PDI ),
                duf_pdi_reldepth( PDI ), diridpid );

  DUF_TRACE( scan, 3, "[%llu]  : scan start      +" DUF_DEPTH_PFMT "", diridpid, duf_pdi_depth( PDI ) );

  DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 1." );

  DUF_TRACE( explain, 0,
             "≫≫≫≫≫≫≫≫≫≫  to scan %" "s" /* DUF_ACTION_TITLE_FMT */ " ≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪≪",
             duf_uni_scan_action_title( SCCB ) );
  DUF_SCCB( DUF_TRACE, scan, 0, "scanning: top dirID: %llu; path: %s;", duf_levinfo_dirid( PDI ), duf_levinfo_path( PDI ) );

#  endif
  if ( !SCCB->disabled )
  {                             /* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */
    DOR( r, duf_sccbh_eval_pdi_dirs( pstmt_selector, /* str_cb2_unused, */ sccbh ) );
  }

  DUF_TRACE( scan, 3, "[%llu]  : scan end      +" DUF_DEPTH_PFMT "", diridpid, duf_pdi_depth( PDI ) );
  DEBUG_ENDR( r );
}
#endif
