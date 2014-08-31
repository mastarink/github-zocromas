#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_service.h"
#include "duf_config_ref.h"
/* #include "duf_match.h" */

#include "duf_pdi.h"
#include "duf_levinfo.h"

/* #include "duf_sql_defs.h" */
/* #include "duf_sql_field.h" */

#include "duf_item_scan2.h"

#include "duf_path.h"

/* #include "duf_dirent_scan2.h" */

/* #include "duf_file_pathid2.h" */

#include "duf_sccb.h"

#include "duf_dir_scan2_stages.h"

/* ###################################################################### */
#include "duf_dir_scan2.h"
/* ###################################################################### */



/* duf_scan_dir_by_pi:
 * call duf_scan_db_items with str_cb + pdi (also) as str_cb_udata and <path> sql
 *   i.e. call str_cb + str_cb_udata 
 *        for each record by standard <path> sql by dirID (i.e. children of dirID) with  corresponding args
 *
 *
 * also call duf_scan_files_by_pathid wirh sccb->leaf_scan
 *   i.e. call sccb->leaf_scan + pdi (also) as str_cb_udata 
 *        for each <file> record by dirID (i.e. children of dirID) with corresponding args
 *
 * i.e.
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each file in <rrent> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 *
 * known str_cb for duf_scan_dir_by_pi:
 *   duf_str_cb_uni_scan_dir
 *
 * */

static int
duf_scan_dir_by_pi2( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  DEBUG_START(  );
  assert( sccb );

  /* scan dirent's */
  r = duf_qscan_dirents2( pstmt, pdi, sccb );
  DUF_TEST_R( r );

  if ( r >= 0 )
    r = duf_count_db_items2( duf_match_leaf2, pdi, sccb, sccb->leaf_selector2, sccb->leaf_fieldset );
  DUF_TEST_R( r );

  if ( r >= 0 )
    r = duf_qscan_node_scan_before2( pstmt, pdi, sccb );
  DUF_TEST_R( r );

  if ( r >= 0 )
    r = duf_qscan_files_by_dirid2( pstmt, pdi, sccb );
  DUF_TEST_R( r );

  if ( r >= 0 )
    r = duf_qscan_node_scan_middle2( pstmt, pdi, sccb );
  DUF_TEST_R( r );

  if ( r >= 0 )
    r = duf_qscan_dirs_by_dirid2( pstmt, pdi, sccb, str_cb2 );
  DUF_TEST_R( r );

  if ( r >= 0 )
    r = duf_qscan_node_scan_after2( pstmt, pdi, sccb );
  DUF_TEST_R( r );

  DEBUG_ENDR( r );
  return r;
}

/* duf_scan_dirs_by_parentid
 *     1. for <current> dir call sccb->node_scan_before
 *     2. for each leaf in <current> dir call sccb->leaf_scan
 *     3. for <current> dir call sccb->node_scan_middle
 *     4. for each dir in <current> dir call str_cb + str_cb_udata
 *     5. for <current> dir call sccb->node_scan_after
 *
 * known str_cb for duf_scan_dirs_by_parentid:
 *   duf_str_cb_uni_scan_dir
 *
 * see duf_scan_dir_by_pi
 * */

int
duf_scan_dirs_by_pi2_msg( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;
  unsigned long long diridpid;

  diridpid = duf_levinfo_dirid( pdi );

  DUF_TRACE( scan, 10, "+" );
  DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, "** depth:%d/%d; diridpid:%llu:%s", duf_pdi_depth( pdi ),
                duf_pdi_reldepth( pdi ), diridpid, DUF_PATHID2PATH( diridpid, pdi ) );

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan start       by %5llu", duf_pdi_depth( pdi ), diridpid );

  DUF_TRACE( scan, 11, "scan dir by pi2 ID%5llu", diridpid );
  DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 1." );
  r = duf_scan_dir_by_pi2( pstmt, str_cb2, pdi, sccb );
  DUF_TEST_R( r );

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": scan end         by %5llu", duf_pdi_depth( pdi ), diridpid );
  return r;
}
