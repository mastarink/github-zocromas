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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo.h"                                             /* duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc. ✗ */
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_updown.h"                                      /* duf_levinfo_goup */

#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_levinfo; duf_pdi_*depth; ✗ */
#include "duf_pdi_ref.h"

#include "duf_sccb_def.h"                                            /* DUF_SCCB_PDI */
#include "duf_sccb.h"                                                /* duf_uni_scan_action_title, contained at DUF_SCCB_PDI */
/* #include "duf_sccb_row.h"                                            (* datarow_* ✗ *) */

#include "duf_sccbh_eval_leaf.h"                                     /* duf_sccbh_eval_db_leaf_str_cb; duf_sccbh_eval_db_leaf_fd_str_cb; ✗ */
#include "duf_sccbh_eval.h"                                          /* duf_sccbh_eval_db_leaf_str_cb; duf_sccbh_eval_db_leaf_fd_str_cb; ✗ */
#include "duf_sccbh_scanner.h"

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_structs.h"                                       /* duf_sccb_handle_s (from duf_sccbh_types: duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t) ✗ */
#include "duf_sccbh_row.h"                                           /* duf_sccbh_row_get_*; sccbh_rows_eval ✗ */
#include "duf_sccbh_pstmt.h"

#include "duf_sccb_structs.h"
#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */

#include "duf_levinfo_credel.h"                                      /* duf_levinfo_create; duf_levinfo_delete ✗ */
#include "duf_li_credel.h"

#include "duf_fmt_defs.h"

/* ###################################################################### */
#include "duf_sel_cb_leaf.h"
/* ###################################################################### */

/* 20151027.114003 */
/* DUF_WRAPSTATIC */
static
SR( SCCBH, sel_cb2_leaf_at, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_arg MAS_UNUSED, */ duf_str_cb2s_t str_cb2, duf_scanstage_t scanstage )
{
  if ( str_cb2 )
  {
    MAST_TRACE( explain, 20, "=> str cb2" );
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + CRX( pdi_reldepth, H_PDI ), H_PDI, " >>> 5. leaf str cb2" );

    assert( ( str_cb2 == F2ND( sccbh_eval_db_leaf_str_cb_new ) ) || str_cb2 == F2ND( sccbh_eval_db_leaf_fd_str_cb )
            || str_cb2 == F2ND( sccbh_eval_db_leaf_str_cb ) /* || str_cb2 == F2ND( sccbh_eval_db_leaf_qfd_pack_new ) */  );
    assert( CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) );

    if ( !( CRX( levinfo_dirid, H_PDI ) == CRX( levinfo_dirid_up, H_PDI ) && CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) ) )
    {
      duf_levinfo_t *pli = NULL;

    /* assert( duf_levinfo_closed( H_PDI ) ); */

      pli = CRP( nameid2li, CRX( levinfo_nameid, H_PDI ), CRX( pdi_maxdepth, H_PDI ) );
      if ( QNOERR )
        CRX( levinfo_set_li, H_PDI, pli, CRX( pdi_maxdepth, H_PDI ) );
      else
        CRX( li_delete_array, pli, CRX( pdi_maxdepth, H_PDI ) );
    }
    assert( CRX( pdi_depth, H_PDI ) > 0 );
    assert( CRX( pdi_levinfo, H_PDI ) );
    assert( CRX( levinfo_dirid, H_PDI ) == CRX( levinfo_dirid_up, H_PDI ) );
    assert( CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) );
  /* CR( sccbh_eval_db_leaf_qfd_pack_new, sccbh, scanstage ); */
    if ( str_cb2 )
    {
      assert( ( str_cb2 == F2ND( sccbh_eval_db_leaf_str_cb_new ) ) || str_cb2 == F2ND( sccbh_eval_db_leaf_fd_str_cb )
              || str_cb2 == F2ND( sccbh_eval_db_leaf_str_cb ) /* || str_cb2 == F2ND( sccbh_eval_db_leaf_qfd_pack_new ) */  );
      CRV( str_cb2, sccbh, /* pstmt_arg, */ scanstage );
    }

    if ( QNOERR )
    {
      CRX( pdi_seq_leaf_plus, H_PDI );
      MAST_TRACE( seq, 0, "seq:%llu; seq_leaf:%llu", CRX( pdi_seq_gen, H_PDI ) /* H_PDI->seq */ , CRX( pdi_seq_leaf, H_PDI ) /* H_PDI->seq_leaf */  );
    }
#if 0
    if ( sccbh->progress_leaf_cb )
      ( sccbh->progress_leaf_cb ) ( sccbh );
#elif 0
    IF_CRV( CRX( sccbh_progress_leaf_cb, sccbh ), sccbh );
    ERRCLEAR( NO_FUNC );
#else
    {
      duf_sccbh_fun_t cb;

      cb = duf_sccbh_progress_leaf_cb( sccbh );
      if ( cb )
        cb( sccbh );
    }
#endif
  }
  else
  {
    assert( 0 );
  }
  ER( SCCBH, sel_cb2_leaf_at, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_arg, */ duf_str_cb2s_t str_cb2, duf_scanstage_t scanstage );
}

/* duf_sel_cb_leaves:
 * this is callback of type: duf_sel_cb_t (first range): 
 *
 * called with precord
 * str_cb + str_cb_udata to be called for precord with correspondig args
 *
 * known str_cb for duf_sel_cb_leaf:
 *   duf_str_cb_leaf_scan;   duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *   duf_str_cb_scan_file_fd;  duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd ; str_cb_udata_unused
 *   duf_str_cb_uni_scan_dir (in theory ?!)
 *   ...
 * */
/* 20150820.085847 */
SR( SCCBH, sel_cb2_leaf, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_str_cb2s_t str_cb2, duf_scanstage_t scanstage )
{
  assert( H_PDI );
  assert( CRX( pdi_depth, H_PDI ) >= 0 );
  assert( CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) );
/* data from db at pstmt_arg */
  MAST_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", CRX( pdi_depth, H_PDI ) );
  MAST_TRACE( explain, 40, "@ sel cb2 leaf" );
  assert( ( str_cb2 == F2ND( sccbh_eval_db_leaf_str_cb_new ) ) || str_cb2 == F2ND( sccbh_eval_db_leaf_str_cb )
          || str_cb2 == F2ND( sccbh_eval_db_leaf_fd_str_cb ) /* || str_cb2 == F2ND( sccbh_eval_db_leaf_qfd_pack_new ) */  || str_cb2 == NULL );
  MAST_TRACE( scan, 9, "LEAF %s", CRX( levinfo_path, H_PDI ) );
  {
  /*@ 1. go down + dbopenat */
    assert( pstmt_arg == duf_pdi_each_stmt( H_PDI, 0 ) );
  /* QT( "@pstmt_arg:%p - %p", pstmt_arg, duf_pdi_each_stmt( H_PDI, 0 ) ); */
    {
      CR( sccbh_pstmt_godown_dbopenat_dh, sccbh, pstmt_arg, DUF_NODE_LEAF /* node_type */  );
      assert( QISERR_N( TOO_DEEP ) || pstmt_arg == duf_pdi_each_stmt( H_PDI, 1 ) );
      if ( QNOERR )
      {
        MAST_TRACE( scan, 9, "(%s) LEAF down %s", QERRNAME, CRX( levinfo_path, H_PDI ) );
        assert( CRX( pdi_depth, H_PDI ) >= 0 );

        CRX( sccbh_row_add, sccbh, pstmt_arg );
        CR( sel_cb2_leaf_at, sccbh, /* pstmt_arg, */ str_cb2, scanstage );

        assert( CRX( pdi_depth, H_PDI ) == CRX( levinfo_calc_depth, H_PDI ) );

        assert( QNOERR );
        {
          mas_error_index_t ei;

          ei = QERRIND;
          CR_( levinfo_goup, H_PDI );
          if ( QNOERR )
            QERRIND = ei;
        }
      }
    }
  }

  MAST_TRACE( scan, 9, "/LEAF %s", CRX( levinfo_path, H_PDI ) );

  ER( SCCBH, sel_cb2_leaf, duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_str_cb2s_t str_cb2, duf_scanstage_t scanstage );
}
