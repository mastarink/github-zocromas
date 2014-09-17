#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
/* #include "duf_levinfo.h" */
#include "duf_levinfo_ref.h"

#include "duf_item_scan2.h"

/* #include "duf_db2path.h" */

#include "duf_sccb.h"

#include "duf_dir_scan2_stages.h"

/* #include "duf_item_match2.h" */

/* #include "duf_uni_scan2.h" */
/* ###################################################################### */
#include "duf_dir_scan2.h"
/* ###################################################################### */



/* duf_scan_dirs_by_pdi:
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
static int
duf_scan_dirs_by_pdi( duf_sqlite_stmt_t * pstmt_selector, /* duf_str_cb2_t str_cb2_unused, */ duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );
  assert( sccb );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid(pdi) );
/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem
 * */
  DOR( r, duf_qscan_dirents2( pstmt_selector, pdi, sccb ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid(pdi) );
  DOR( r, duf_count_db_items2( NULL /* duf_match_leaf2 */ , pdi, sccb, &sccb->leaf ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid(pdi) );
  DOR( r, duf_qscan_node_scan_before2(  /*        */ pstmt_selector, pdi, sccb ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid(pdi) );
  DOR( r, duf_qscan_files_by_dirid2(  /*          */ pstmt_selector, pdi, sccb ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid(pdi) );
  DOR( r, duf_qscan_node_scan_middle2(  /*        */ pstmt_selector, pdi, sccb ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid(pdi) );
/* assert( duf_scan_dirs_by_pdi_maxdepth == str_cb2_unused ); */
  /* if ( DUF_U_FLAG( recursive ) ) */
  DOR( r, duf_qscan_dirs_by_dirid2(  /*         */ pstmt_selector, pdi, sccb /* , duf_scan_dirs_by_pdi_maxdepth *//* str_cb2_unused */  ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid(pdi) );
  DOR( r, duf_qscan_node_scan_after2(  /*         */ pstmt_selector, pdi, sccb ) );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid(pdi) );

  DEBUG_ENDR( r );
}

/* duf_scan_dirs_by_pdi_wrap          ( duf_scan_dirs_by_parentid )
 *
 * see duf_scan_dirs_by_pdi
 *
 * str_cb2 (sub-item scanner) one of:
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */

int
duf_scan_dirs_by_pdi_wrap( duf_sqlite_stmt_t * pstmt_selector, /* duf_str_cb2_t str_cb2_unused, */ duf_depthinfo_t * pdi,
                           duf_scan_callbacks_t * sccb )
{
  DEBUG_STARTR( r );
  unsigned long long diridpid;

  diridpid = duf_levinfo_dirid( pdi );

  /* assert( duf_scan_dirs_by_pdi_maxdepth == str_cb2_unused ); */

  DUF_TRACE( scan, 3, "[%llu]", diridpid);
  DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, "** depth:%d/%d; diridpid:%llu", duf_pdi_depth( pdi ),
                duf_pdi_reldepth( pdi ), diridpid );

  DUF_TRACE( scan, 3, "[%llu]  : scan start      +" DUF_DEPTH_PFMT "", diridpid, duf_pdi_depth( pdi ) );

  DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 1." );

  DOR( r, duf_scan_dirs_by_pdi( pstmt_selector, /* str_cb2_unused, */ pdi, sccb ) );

  DUF_TRACE( scan, 3, "[%llu]  : scan end      +" DUF_DEPTH_PFMT "", diridpid, duf_pdi_depth( pdi ) );
  DEBUG_ENDR( r );
}
