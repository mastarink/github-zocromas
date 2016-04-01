#include <assert.h>                                                  /* assert */
#include <string.h>
#include <unistd.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */


#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo.h"                                             /* duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc. ✗ */
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_updown.h"

#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_depth */
#include "duf_pdi_ref.h"

#include "duf_sccb_structs.h"
#include "duf_sccbh_eval_all.h"

#include "duf_sccbh_shortcuts.h"
#include "duf_sccbh_pstmt.h"

/* #include "duf_pdi_structs.h" */
/* ###################################################################### */
#include "duf_sel_cb_node.h"
/* ###################################################################### */

/* 20151027.113952 */
static
SR( SCCBH, sel_cb2_node_at, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */
/*@ 1. go down + dbopenat */
#if 0
  H_PDI->seq++;
  H_PDI->seq_node++;
#else
  CRX( pdi_seq_node_plus, H_PDI );
#endif
  MAST_TRACE( scan_dir, 0, "* qn%llu/q%llu T%llu %s", CRX( pdi_seq_node, H_PDI ) /* H_PDI->seq_node */ , CRX( pdi_seq, H_PDI ) /* H_PDI->seq */ ,
              H_TOTITEMS, H_SCCB->title );
  if ( sccbh->progress_node_cb )
    ( sccbh->progress_node_cb ) ( sccbh );
  MAST_TRACE( seq, 0, "seq:%llu; seq_node:%llu", CRX( pdi_seq, H_PDI ) /* H_PDI->seq */ , CRX( pdi_seq_node, H_PDI ) /* H_PDI->seq_node */  );

  if ( str_cb2 )
  {
    MAST_TRACE( explain, 20, "=> str cb2" );
  /*@ 3. str_cb2 */
    CRV( ( str_cb2 ), sccbh, pstmt_arg, scanstage );

    ERRCLEAR( OPENAT_ENOENT );
    ERRCLEAR( STATAT_ENOENT );
  }

/*@ 4. go up */
/* DUF_ENDR( r ); */
  ER( SCCBH, sel_cb2_node_at, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage );
}

#if 0
/* 20151027.114000 */
int DUF_WRAPPED( duf_sel_cb2_node_at ) ( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );
  assert( H_PDI );
  assert( CRX( pdi_depth, H_PDI ) >= 0 );

/* data from db at pstmt_arg */

  MAST_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan node2", CRX( pdi_depth, H_PDI ) );
  MAST_TRACE( explain, 40, "@ sel cb2 node" );
  assert( str_cb2 == duf_sccbh_eval_all || str_cb2 == NULL );

  MAST_TRACE( scan, 6, "NODE %s", CRX( levinfo_path, H_PDI ) );
  MAST_TRACE( scan, 6, "(%s) NODE down %s", mas_error_name_i( r ), CRX( levinfo_path, H_PDI ) );
  assert( CRX( pdi_depth, H_PDI ) >= 0 );
  {
    DOR( r, duf_sel_cb2_node_at( sccbh, pstmt_arg, str_cb2, scanstage ) );
  }
  MAST_TRACE( scan, 6, "/NODE %s", CRX( levinfo_path, H_PDI ) );

  DUF_ENDR( r );
}
#endif
/*
 * str_cb2 can be
 *   -- CRX(sccbh_eval_all,_wrap)
 *   ...
 */
/* 20150820.085950 */
SR( SCCBH, sel_cb2_node, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage )
{
/* DUF_STARTR( r ); */
  assert( H_PDI );
  assert( CRX( pdi_depth, H_PDI ) >= 0 );

  assert( CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) );

/* data from db at pstmt_arg */

  MAST_TRACE( scan, 10, "   %d: =====> scan node2", CRX( pdi_depth, H_PDI ) );
  MAST_TRACE( explain, 40, "@ sel cb2 node" );
  assert( str_cb2 == duf_sccbh_eval_all || str_cb2 == NULL );

  MAST_TRACE( scan, 6, "NODE %s", CRX( levinfo_path, H_PDI ) );
  {
  /*@ 1. go down + dbopenat */
    CR( sccbh_pstmt_godown_dbopenat_dh, sccbh, pstmt_arg, DUF_NODE_NODE /* node_type */  );
    MAST_TRACE( scan, 6, "(%s) NODE down %s", QERRNAME, CRX( levinfo_path, H_PDI ) );
    assert( CRX( pdi_depth, H_PDI ) >= 0 );

    CR( sel_cb2_node_at, sccbh, pstmt_arg, str_cb2, scanstage );

    assert( CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) );

    CR( levinfo_goup, H_PDI );
  }
  MAST_TRACE( scan, 6, "/NODE %s", CRX( levinfo_path, H_PDI ) );

/* DUF_ENDR( r ); */
  ER( SCCBH, sel_cb2_node, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage );
}
