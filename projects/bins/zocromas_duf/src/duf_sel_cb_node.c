#include <string.h>
#include <unistd.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"        /* duf_pdi_depth */

#include "duf_sccb_def.h"
#include "duf_sccb.h"
#include "duf_sccbh_eval_all.h"

#include "duf_sccbh_shortcuts.h"
#include "duf_pstmt_levinfo.h"

/* ###################################################################### */
#include "duf_sel_cb_node.h"
/* ###################################################################### */

/* 20151027.113952 */
DUF_WRAPSTATIC int
duf_sel_cb2_node_at( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  /*@ 1. go down + dbopenat */
  PDI->seq++;
  PDI->seq_node++;

  DUF_TRACE( scan_dir, 0, "* qn%llu/q%llu T%llu %s", PDI->seq_node, PDI->seq, TOTITEMS, SCCB->title );
  if ( sccbh->progress_node_cb )
    ( sccbh->progress_node_cb ) ( sccbh );
  DUF_TRACE( seq, 0, "seq:%llu; seq_node:%llu", PDI->seq, PDI->seq_node );

  if ( str_cb2 )
  {
    DUF_TRACE( explain, 2, "=> str cb2" );
    /*@ 3. str_cb2 */
    DOR( r, ( str_cb2 ) ( scanstage, pstmt, sccbh ) );

    DUF_CLEAR_ERROR( r, DUF_ERROR_OPENAT_ENOENT, DUF_ERROR_STATAT_ENOENT );
  }

  /*@ 4. go up */
  DEBUG_ENDR( r );
}

/* 20151027.114000 */
int DUF_WRAPPED( duf_sel_cb2_node_at ) ( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  assert( PDI );
  assert( PDI->pathinfo.depth >= 0 );

  /* data from db at pstmt */

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan node2", duf_pdi_depth( PDI ) );
  DUF_TRACE( explain, 4, "@ sel cb2 node" );
  assert( str_cb2 == DUF_WRAPPED( duf_eval_sccbh_all ) || str_cb2 == NULL );

  DUF_TRACE( scan, 6, "NODE %s", duf_levinfo_path( PDI ) );
  DUF_TRACE( scan, 6, "(%s) NODE down %s", mas_error_name_i( r ), duf_levinfo_path( PDI ) );
  assert( PDI->pathinfo.depth >= 0 );
  {
    DOR( r, duf_sel_cb2_node_at( scanstage, pstmt, str_cb2, sccbh ) );
  }
  DUF_TRACE( scan, 6, "/NODE %s", duf_levinfo_path( PDI ) );

  DEBUG_ENDR( r );
}

/*
 * str_cb2 can be
 *   -- duf_eval_sccbh_all(_wrap)
 *   ...
 */
/* 20150820.085950 */
int
duf_sel_cb2_node( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  assert( PDI );
  assert( PDI->pathinfo.depth >= 0 );


  assert( PDI->pathinfo.depth == duf_levinfo_calc_depth( PDI ) );


  /* data from db at pstmt */

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan node2", duf_pdi_depth( PDI ) );
  DUF_TRACE( explain, 4, "@ sel cb2 node" );
  assert( str_cb2 == DUF_WRAPPED( duf_eval_sccbh_all ) || str_cb2 == NULL );


  DUF_TRACE( scan, 6, "NODE %s", duf_levinfo_path( PDI ) );
  {
    /*@ 1. go down + dbopenat */
    DOR( r, duf_pstmt_levinfo_godown_dbopenat_dh( pstmt, sccbh, DUF_NODE_NODE /* node_type */  ) );
    DUF_TRACE( scan, 6, "(%s) NODE down %s", mas_error_name_i( r ), duf_levinfo_path( PDI ) );
    assert( PDI->pathinfo.depth >= 0 );

    DOR( r, duf_sel_cb2_node_at( scanstage, pstmt, str_cb2, sccbh ) );

    assert( PDI->pathinfo.depth == duf_levinfo_calc_depth( PDI ) );

    DOR( r, duf_levinfo_goup( PDI ) );
  }
  DUF_TRACE( scan, 6, "/NODE %s", duf_levinfo_path( PDI ) );

  DEBUG_ENDR( r );
}
