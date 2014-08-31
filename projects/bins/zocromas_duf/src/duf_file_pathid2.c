#include <stdio.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>



#include "duf_maintenance.h"
#include "duf_scan_types.h"

#include "duf_config_ref.h"

#include "duf_utils.h"
#include "duf_service.h"

#include "duf_options.h"


#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_path.h"
/* #include "duf_item_scan1.h" */
#include "duf_item_scan2.h"

#include "duf_sccb.h"

/* ###################################################################### */
#include "duf_file_pathid2.h"
/* ###################################################################### */


/* duf_scan_files_by_pi:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by dirID (i.e. children of dirID) with corresponding args
 *
 * known str_cb for duf_scan_files_by_di:
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd
 *
 * str_cb2 (sub-item scanner):
 *       duf_str_cb2_uni_scan_dir
 *       duf_str_cb2_leaf_scan
 *       duf_str_cb2_scan_file_fd
 * */
static int
duf_scan_files_by_di2( duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = DUF_ERROR_NO_FILE_SELECTOR;

  DUF_TRACE( scan, 11, "  " DUF_DEPTH_PFMT ": scan leaves -->  by %5llu", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ) );

  if ( sccb && sccb->leaf.selector2 )
  {
    DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 3." );

/* calling duf_sel_cb_(node|leaf) for each record by sccb->leaf.selector2 */
    /* 
     * call str_cb2 + str_cb_udata for each record by this sql with corresponding args
     *
     * DUF_NODE_NODE => sccb->node.selector2, sccb->node.fieldset
     * DUF_NODE_LEAF => sccb->leaf.selector2, sccb->leaf.fieldset
     * 
     * str_cb2 (sub-item scanner):
     *       duf_str_cb2_uni_scan_dir
     *     ( duf_str_cb2_leaf_scan    )
     *     ( duf_str_cb2_scan_file_fd )
     * */
    r = duf_scan_db_items2( DUF_NODE_LEAF, str_cb2, pdi, sccb /*, sccb->leaf.selector2, sccb->leaf.fieldset */ );

    DUF_TRACE( scan, 11, "  " DUF_DEPTH_PFMT ": scan leaves <--  by %5llu", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ) );

    DUF_TEST_R( r );
  }
  else
  {
    DUF_ERROR( "sccb->leaf.selector2 must be set for %s", sccb->title );
    r = DUF_ERROR_PTR;
  }

  return r;
}

/* duf_scan_files_by_dirid:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by dirID with corresponding args
 *
 * known str_cb for duf_scan_files_by_dirid:
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd
 *
 * str_cb2 (sub-item scanner):
 *     ( duf_str_cb2_uni_scan_dir )
 *       duf_str_cb2_leaf_scan
 *       duf_str_cb2_scan_file_fd
 * */
int
duf_scan_files_by_dirid2( duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  if ( DUF_ACT_FLAG( files ) )
    r = duf_scan_files_by_di2( str_cb2, pdi, sccb );
  else
    DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": skip scan leaves by %5llu : no '--files'", duf_pdi_depth( pdi ), duf_levinfo_dirid( pdi ) );
  return r;
}
