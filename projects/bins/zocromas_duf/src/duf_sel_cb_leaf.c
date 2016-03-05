#include <assert.h>                                                  /* assert */
#include <string.h>
#include <unistd.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* T; TT; TR ✗ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ✗ */
#include "duf_dodefs.h"                                              /* DOR ✗ */

#include "duf_debug_defs.h"                                          /* DUF_WRAPSTATIC; DUF_WRAPPED ...  ✗ */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_levinfo.h"                                             /* duf_levinfo_calc_depth; duf_levinfo_clear_level_d; etc. ✗ */
#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_updown.h"                                      /* duf_levinfo_goup */

#include "duf_pdi_ref.h"                                             /* duf_pdi_depth */
#include "duf_pdi_pi_ref.h"                                          /* duf_pdi_depth */

#include "duf_sccb_def.h"                                            /* DUF_SCCB_PDI */
#include "duf_sccb.h"                                                /* duf_uni_scan_action_title, contained at DUF_SCCB_PDI */

#include "duf_sccbh_eval_leaf.h"                                     /* duf_sccbh_eval_db_leaf_fd_str_cb and duf_sccbh_eval_db_leaf_str_cb */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_sccbh_shortcuts.h"                                     /* H_PDI etc. */
#include "duf_sccbh_pstmt.h"

#include "duf_sccb_scanstage.h"
#include "duf_pdi_credel.h"                                          /* duf_pdi_create; duf_pdi_kill ✗ */
#include "duf_levinfo_credel.h"                                      /* duf_levinfo_create; duf_levinfo_delete ✗ */
#include "duf_li_credel.h"
#include "duf_li.h"

/* ###################################################################### */
#include "duf_sel_cb_leaf.h"
/* ###################################################################### */

/* 20151027.114003 */
/* DUF_WRAPSTATIC */
static int
duf_sel_cb2_leaf_at( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );
  if ( str_cb2 )
  {
    MAST_TRACE( explain, 2, "=> str cb2" );
    DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( H_PDI ), H_PDI, " >>> 5. leaf str cb2" );
    assert( str_cb2 == duf_sccbh_eval_db_leaf_fd_str_cb || str_cb2 == duf_sccbh_eval_db_leaf_str_cb );

#if 1
    {
      assert( H_PDI->pathinfo.depth == duf_levinfo_calc_depth( H_PDI ) );
      {
# if 0
        duf_levinfo_t *pli = NULL;

      /* pli = duf_dirid2li( duf_levinfo_dirid( H_PDI ), duf_levinfo_itemtruename( H_PDI ), H_PDI->pathinfo.maxdepth , &r ); */
        pli = duf_nameid2li( duf_levinfo_nameid( H_PDI ), H_PDI->pathinfo.maxdepth, &r );
        assert( pli[0].itemname );

        {
          duf_depthinfo_t *pdi2;

          pdi2 = duf_pdi_clone( H_PDI, 1 /* no_li */  );
/*   duf_levinfo_create( pdi2 ); */
#  if 1
        /* T( "B %d -- %d/%d", pdi2->pathinfo.depth, duf_levinfo_calc_depth( pdi2 ), duf_li_calc_depth( pli ) ); */
          duf_levinfo_set_li( pdi2, pli, H_PDI->pathinfo.maxdepth );
          {
            if ( duf_levinfo_dirid( pdi2 ) != duf_levinfo_dirid_up( pdi2 ) || pdi2->pathinfo.depth != duf_levinfo_calc_depth( pdi2 ) )
            {
              __duf_sql_dump_row( pstmt );
              T( "P  nameid:%llu; (dirid:%3llu/%3llu) depth(%d/%d) [%s - %s:%llu/%s:%llu] %s(%d)", duf_levinfo_nameid( H_PDI ),
                 duf_levinfo_dirid( H_PDI ), duf_levinfo_dirid_up( H_PDI ), H_PDI->pathinfo.depth, duf_levinfo_calc_depth( H_PDI ),
                 duf_levinfo_path( H_PDI ), duf_levinfo_itemtruename( H_PDI ), duf_levinfo_nameid( H_PDI ), duf_levinfo_itemtruename_up( H_PDI ),
                 duf_levinfo_nameid_up( H_PDI ), duf_nodetype_name( duf_levinfo_node_type( H_PDI ) ), duf_levinfo_node_type( H_PDI ) );
              T( "p2 nameid:%llu; (dirid:%3llu/%3llu) depth(%d/%d) [%s - %s:%llu/%s:%llu] %s(%d)", duf_levinfo_nameid( pdi2 ),
                 duf_levinfo_dirid( pdi2 ), duf_levinfo_dirid_up( pdi2 ), pdi2->pathinfo.depth, duf_levinfo_calc_depth( pdi2 ),
                 duf_levinfo_path( pdi2 ), duf_levinfo_itemtruename( pdi2 ), duf_levinfo_nameid( pdi2 ), duf_levinfo_itemtruename_up( pdi2 ),
                 duf_levinfo_nameid_up( pdi2 ), duf_nodetype_name( duf_levinfo_node_type( pdi2 ) ), duf_levinfo_node_type( pdi2 ) );
            }
            assert( pdi2->pathinfo.levinfo );
            assert( duf_levinfo_dirid( pdi2 ) == duf_levinfo_dirid_up( pdi2 ) );
            assert( pdi2->pathinfo.depth == duf_levinfo_calc_depth( pdi2 ) );

            assert( 0 == strcmp( duf_levinfo_path_up( H_PDI ), duf_levinfo_path_up( pdi2 ) ) );
            assert( 0 == strcmp( duf_levinfo_path( H_PDI ), duf_levinfo_path( pdi2 ) ) );
            assert( duf_levinfo_nameid( H_PDI ) == duf_levinfo_nameid( pdi2 ) );
            assert( duf_levinfo_dirid( H_PDI ) == duf_levinfo_dirid_up( H_PDI ) );
          }

          MAST_TRACE( sccbh, 0, "TEST %llu; %s(%d) @ %s @ %s @ %s", duf_levinfo_dirid( pdi2 ), duf_nodetype_name( DUF_NODE_LEAF ), DUF_NODE_LEAF,
                      duf_levinfo_path_up( pdi2 ), DUF_GET_STMT_SFIELD2( pstmt, dfname ), duf_levinfo_itemtruename( pdi2 ) );
#  else
          duf_li_delete( pli, H_PDI->pathinfo.maxdepth );
          pli = NULL;
#  endif
          duf_pdi_delete( pdi2 );
        }
# else

        if ( !( duf_levinfo_dirid( H_PDI ) == duf_levinfo_dirid_up( H_PDI ) && H_PDI->pathinfo.depth == duf_levinfo_calc_depth( H_PDI ) ) )
        {
          duf_levinfo_t *pli = NULL;

          pli = duf_nameid2li( duf_levinfo_nameid( H_PDI ), H_PDI->pathinfo.maxdepth, &r );
          if ( DUF_NOERROR( r ) )
            duf_levinfo_set_li( H_PDI, pli, H_PDI->pathinfo.maxdepth );
          else
            duf_li_delete( pli, H_PDI->pathinfo.maxdepth );
        }
        assert( H_PDI->pathinfo.depth > 0 );
        assert( H_PDI->pathinfo.levinfo );
        assert( duf_levinfo_dirid( H_PDI ) == duf_levinfo_dirid_up( H_PDI ) );
        assert( H_PDI->pathinfo.depth == duf_levinfo_calc_depth( H_PDI ) );
# endif
      }
    }
#endif

    DOR( r, ( str_cb2 ) ( sccbh, pstmt, scanstage ) );
    if ( DUF_NOERROR( r ) )
    {
      H_PDI->seq++;
      H_PDI->seq_leaf++;
      MAST_TRACE( seq, 0, "seq:%llu; seq_leaf:%llu", H_PDI->seq, H_PDI->seq_leaf );
    }
    if ( sccbh->progress_leaf_cb )
      ( sccbh->progress_leaf_cb ) ( sccbh );
  }
  DUF_ENDR( r );
}

#if 0
/* 20151027.114007 */
int DUF_WRAPPED( duf_sel_cb2_leaf_at ) ( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );
  assert( H_PDI );

/* data from db at pstmt */

  MAST_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", duf_pdi_depth( H_PDI ) );
  MAST_TRACE( explain, 4, "@ sel cb2 leaf" );

  MAST_TRACE( scan_reg, 0, "* qn%llu/q%llu T%llu %s", H_PDI->seq_leaf, H_PDI->seq, H_TOTITEMS, H_SCCB->title );

  DUF_SCCB_PDI( MAST_TRACE, scan, 10 + duf_pdi_reldepth( H_PDI ), H_PDI, " >>> 5. leaf str cb2; r:%d; dfd:%d ; opendir:%d", r,
                duf_levinfo_dfd( H_PDI ), H_PDI->opendir );
/* #ifdef MAS_ASSERT */
  {
  /* DUF_SFIELD2( fname ); */
    const char *dfn = duf_levinfo_itemshowname( H_PDI );

  /* filename from db same as duf_levinfo_itemname( pdi ) */
    assert( 0 == strcmp( DUF_GET_SFIELD2( fname ), dfn ) );
  /* DOR( r, duf_levinfo_pstmt2levinfo_dh( H_PDI, pstmt, node_type ) ); */
    {
      duf_levinfo_t *l = duf_levinfo_ptr( H_PDI );

      assert( l );
      assert( duf_levinfo_dbstat( H_PDI ) );
      assert( duf_levinfo_dbstat_dev( H_PDI ) );
      assert( duf_levinfo_dbstat_inode( H_PDI ) );
    }
  }
/* #endif */

  duf_sel_cb2_leaf_at( sccbh, pstmt, str_cb2, scanstage );

  DUF_ENDR( r );
}
#endif

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
int
duf_sel_cb2_leaf( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_scanstage_t scanstage )
{
  DUF_STARTR( r );
  assert( H_PDI );
  assert( H_PDI->pathinfo.depth >= 0 );

  assert( H_PDI->pathinfo.depth == duf_levinfo_calc_depth( H_PDI ) );

/* data from db at pstmt */

  MAST_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", duf_pdi_depth( H_PDI ) );
  MAST_TRACE( explain, 4, "@ sel cb2 leaf" );
  assert( str_cb2 == duf_sccbh_eval_db_leaf_str_cb || str_cb2 == duf_sccbh_eval_db_leaf_fd_str_cb || str_cb2 == NULL );

  MAST_TRACE( scan, 9, "LEAF %s", duf_levinfo_path( H_PDI ) );
  {
  /*@ 1. go down + dbopenat */
    DOR( r, duf_sccbh_pstmt_godown_dbopenat_dh( sccbh, pstmt, DUF_NODE_LEAF /* node_type */  ) );
    MAST_TRACE( scan, 9, "(%s) LEAF down %s", mas_error_name_i( r ), duf_levinfo_path( H_PDI ) );
    assert( H_PDI->pathinfo.depth >= 0 );

    DOR( r, duf_sel_cb2_leaf_at( sccbh, pstmt, str_cb2, scanstage ) );

    assert( H_PDI->pathinfo.depth == duf_levinfo_calc_depth( H_PDI ) );

    DOR( r, duf_levinfo_goup( H_PDI ) );
  }
  MAST_TRACE( scan, 9, "/LEAF %s", duf_levinfo_path( H_PDI ) );

  DUF_ENDR( r );
}
