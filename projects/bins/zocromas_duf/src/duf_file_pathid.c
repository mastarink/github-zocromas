#include <stdio.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>

#include "duf_trace_defs.h"
#include "duf_debug_defs.h"


#include "duf_types.h"
#include "duf_config_ref.h"

#include "duf_utils.h"
#include "duf_service.h"

#include "duf_options.h"


#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_path.h"
#include "duf_item_scan1.h"
#include "duf_item_scan2.h"

#include "duf_dbg.h"
#include "duf_sccb.h"

/* ###################################################################### */
#include "duf_file_pathid.h"
/* ###################################################################### */


/* duf_scan_files_by_pi:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by dirid (i.e. children of dirid) with corresponding args
 *
 * known str_cb for duf_scan_files_by_di:
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd
 * */
static int
duf_scan_files_by_di( unsigned long long dirid, duf_str_cb_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = DUF_ERROR_NO_FILE_SELECTOR;

  DUF_TRACE( scan, 11, "  " DUF_DEPTH_PFMT ": scan leaves -->  by %5llu", duf_pdi_depth( pdi ), dirid );

/* duf_scan_db_items:
 * call str_cb + str_cb_udata for each record by this sql with corresponding args
 * */
  DUF_OINV( pdi-> );

  if ( sccb && sccb->leaf_selector )
  {
    DUF_OINV( pdi-> );

/* calling duf_sel_cb_(node|leaf) for each record by sccb->leaf_selector */
    r = duf_scan_db_items( DUF_NODE_LEAF, str_cb, pdi, sccb, sccb->leaf_selector, /* ... */ sccb->leaf_fieldset,
                           dirid /* for WHERE */  );




    DUF_TRACE( scan, 11, "  " DUF_DEPTH_PFMT ": scan leaves <--  by %5llu", duf_pdi_depth( pdi ), dirid );


    DUF_OINV( pdi-> );
    DUF_TEST_R( r );
  }
  else
  {
    DUF_ERROR( "sccb->leaf_selector must be set for %s", sccb->title );
  }

  DUF_OINV( pdi-> );
  return r;
}

static int
duf_scan_files_by_di2( unsigned long long dirid, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = DUF_ERROR_NO_FILE_SELECTOR;

  DUF_TRACE( scan, 11, "  " DUF_DEPTH_PFMT ": scan leaves -->  by %5llu", duf_pdi_depth( pdi ), dirid );

/* duf_scan_db_items2:
 * call str_cb2 + str_cb_udata for each record by this sql with corresponding args
 * */
  DUF_OINV( pdi-> );

  if ( sccb && sccb->leaf_selector2 )
  {
    DUF_OINV( pdi-> );
    
    DUF_SCCB_PDI( DUF_TRACE, scan, duf_pdi_reldepth( pdi ), pdi, " >>> 3." );

/* calling duf_sel_cb_(node|leaf) for each record by sccb->leaf_selector2 */
    r = duf_scan_db_items2( DUF_NODE_LEAF, str_cb2, pdi, sccb, sccb->leaf_selector2, sccb->leaf_fieldset, /* ... */
                            dirid /* for WHERE */  );

    DUF_TRACE( scan, 11, "  " DUF_DEPTH_PFMT ": scan leaves <--  by %5llu", duf_pdi_depth( pdi ), dirid );

    DUF_OINV( pdi-> );
    DUF_TEST_R( r );
  }
  else
  {
    DUF_ERROR( "sccb->leaf_selector2 must be set for %s", sccb->title );
    r = DUF_ERROR_PTR;
  }

  DUF_OINV( pdi-> );
  return r;
}

/* duf_scan_files_by_dirid:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by dirid with corresponding args
 *
 * known str_cb for duf_scan_files_by_dirid:
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd
 * */
int
duf_scan_files_by_dirid( unsigned long long dirid, duf_str_cb_t str_cb, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  if ( DUF_ACT_FLAG( files ) )
    r = duf_scan_files_by_di( dirid, str_cb, pdi, sccb );
  else
    DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": skip scan leaves by %5llu : no '--files'", duf_pdi_depth( pdi ), dirid );
  return r;
}

int
duf_scan_files_by_dirid2( unsigned long long dirid, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb )
{
  int r = 0;

  if ( DUF_ACT_FLAG( files ) )
    r = duf_scan_files_by_di2( dirid, str_cb2, pdi, sccb );
  else
    DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": skip scan leaves by %5llu : no '--files'", duf_pdi_depth( pdi ), dirid );
  return r;
}
