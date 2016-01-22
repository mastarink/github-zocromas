#include <string.h>
#include <unistd.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h" /* duf_levinfo_goup */

#include "duf_pdi_ref.h"        /* duf_pdi_depth */
#include "duf_pdi_pi_ref.h"        /* duf_pdi_depth */

#include "duf_sccb_def.h"       /* DUF_SCCB_PDI */
#include "duf_sccb.h"           /* duf_uni_scan_action_title, contained at DUF_SCCB_PDI */

#include "duf_sccbh_eval_leaf.h" /* duf_eval_sccbh_db_leaf_fd_str_cb and duf_eval_sccbh_db_leaf_str_cb */

#include "duf_sql_defs.h"       /* DUF_GET_UFIELD2 */
#include "duf_sql_field.h"      /* __duf_sql_ull_by_name2 for DUF_GET_UFIELD2 */

#include "duf_sccbh_shortcuts.h" /* PDI etc. */
#include "duf_pstmt_levinfo.h"



#include "duf_sccb_scanstage.h"
#include "duf_pdi_credel.h"
#include "duf_levinfo_credel.h"
#include "duf_li_credel.h"
#include "duf_li.h"


/* ###################################################################### */
#include "duf_sel_cb_leaf.h"
/* ###################################################################### */

/* 20151027.114003 */
DUF_WRAPSTATIC int
duf_sel_cb2_leaf_at( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  if ( str_cb2 )
  {
    DUF_TRACE( explain, 2, "=> str cb2" );
    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 5. leaf str cb2" );
    assert( str_cb2 == duf_eval_sccbh_db_leaf_fd_str_cb || str_cb2 == duf_eval_sccbh_db_leaf_str_cb );

#if 1
    {
      assert( PDI->pathinfo.depth == duf_levinfo_calc_depth( PDI ) );
      {
#  if 0
        duf_levinfo_t *pli = NULL;

        /* pli = duf_dirid2li( duf_levinfo_dirid( PDI ), duf_levinfo_itemtruename( PDI ), PDI->pathinfo.maxdepth , &r ); */
        pli = duf_nameid2li( duf_levinfo_nameid( PDI ), PDI->pathinfo.maxdepth, &r );
        assert( pli[0].itemname );

        {
          duf_depthinfo_t *pdi2;

          pdi2 = duf_pdi_clone( PDI, 1 /* no_li */  );
/*   duf_levinfo_create( pdi2 ); */
#    if 1
          /* T( "B %d -- %d/%d", pdi2->pathinfo.depth, duf_levinfo_calc_depth( pdi2 ), duf_li_calc_depth( pli ) ); */
          duf_levinfo_set_li( pdi2, pli, PDI->pathinfo.maxdepth );
          {
            if ( duf_levinfo_dirid( pdi2 ) != duf_levinfo_dirid_up( pdi2 ) || pdi2->pathinfo.depth != duf_levinfo_calc_depth( pdi2 ) )
            {
              __duf_sql_dump_row( pstmt );
              T( "P  nameid:%llu; (dirid:%3llu/%3llu) depth(%d/%d) [%s - %s:%llu/%s:%llu] %s(%d)", duf_levinfo_nameid( PDI ),
                 duf_levinfo_dirid( PDI ), duf_levinfo_dirid_up( PDI ), PDI->pathinfo.depth, duf_levinfo_calc_depth( PDI ),
                 duf_levinfo_path( PDI ), duf_levinfo_itemtruename( PDI ), duf_levinfo_nameid( PDI ), duf_levinfo_itemtruename_up( PDI ),
                 duf_levinfo_nameid_up( PDI ), duf_nodetype_name( duf_levinfo_node_type( PDI ) ), duf_levinfo_node_type( PDI ) );
              T( "p2 nameid:%llu; (dirid:%3llu/%3llu) depth(%d/%d) [%s - %s:%llu/%s:%llu] %s(%d)", duf_levinfo_nameid( pdi2 ),
                 duf_levinfo_dirid( pdi2 ), duf_levinfo_dirid_up( pdi2 ), pdi2->pathinfo.depth, duf_levinfo_calc_depth( pdi2 ),
                 duf_levinfo_path( pdi2 ), duf_levinfo_itemtruename( pdi2 ), duf_levinfo_nameid( pdi2 ), duf_levinfo_itemtruename_up( pdi2 ),
                 duf_levinfo_nameid_up( pdi2 ), duf_nodetype_name( duf_levinfo_node_type( pdi2 ) ), duf_levinfo_node_type( pdi2 ) );
            }
            assert( pdi2->pathinfo.levinfo );
            assert( duf_levinfo_dirid( pdi2 ) == duf_levinfo_dirid_up( pdi2 ) );
            assert( pdi2->pathinfo.depth == duf_levinfo_calc_depth( pdi2 ) );

            assert( 0 == strcmp( duf_levinfo_path_up( PDI ), duf_levinfo_path_up( pdi2 ) ) );
            assert( 0 == strcmp( duf_levinfo_path( PDI ), duf_levinfo_path( pdi2 ) ) );
            assert( duf_levinfo_nameid( PDI ) == duf_levinfo_nameid( pdi2 ) );
            assert( duf_levinfo_dirid( PDI ) == duf_levinfo_dirid_up( PDI ) );
          }


          DUF_TRACE( sccbh, 0, "TEST %llu; %s(%d) @ %s @ %s @ %s", duf_levinfo_dirid( pdi2 ), duf_nodetype_name( DUF_NODE_LEAF ), DUF_NODE_LEAF,
                     duf_levinfo_path_up( pdi2 ), DUF_GET_STMT_SFIELD2( pstmt, dfname ), duf_levinfo_itemtruename( pdi2 ) );
#    else
          duf_li_delete( pli, PDI->pathinfo.maxdepth );
          pli = NULL;
#    endif
          duf_pdi_delete( pdi2 );
        }
#  else

        if ( !( duf_levinfo_dirid( PDI ) == duf_levinfo_dirid_up( PDI ) && PDI->pathinfo.depth == duf_levinfo_calc_depth( PDI ) ) )
        {
          duf_levinfo_t *pli = NULL;

          pli = duf_nameid2li( duf_levinfo_nameid( PDI ), PDI->pathinfo.maxdepth, &r );
          if ( DUF_NOERROR( r ) )
            duf_levinfo_set_li( PDI, pli, PDI->pathinfo.maxdepth );
          else
            duf_li_delete( pli, PDI->pathinfo.maxdepth );
        }
        assert( PDI->pathinfo.depth > 0 );
        assert( PDI->pathinfo.levinfo );
        assert( duf_levinfo_dirid( PDI ) == duf_levinfo_dirid_up( PDI ) );
        assert( PDI->pathinfo.depth == duf_levinfo_calc_depth( PDI ) );
#  endif
      }
    }
#endif



    DOR( r, ( str_cb2 ) ( scanstage, pstmt, sccbh ) );
    if ( DUF_NOERROR( r ) )
    {
      PDI->seq++;
      PDI->seq_leaf++;
      DUF_TRACE( seq, 0, "seq:%llu; seq_leaf:%llu", PDI->seq, PDI->seq_leaf );
    }
    if ( sccbh->progress_leaf_cb )
      ( sccbh->progress_leaf_cb ) ( sccbh );
  }
  DEBUG_ENDR( r );
}

/* 20151027.114007 */
int DUF_WRAPPED( duf_sel_cb2_leaf_at ) ( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  assert( PDI );

  /* data from db at pstmt */

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", duf_pdi_depth( PDI ) );
  DUF_TRACE( explain, 4, "@ sel cb2 leaf" );


  DUF_TRACE( scan_reg, 0, "* qn%llu/q%llu T%llu %s", PDI->seq_leaf, PDI->seq, TOTITEMS, SCCB->title );


  DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 5. leaf str cb2; r:%d; dfd:%d ; opendir:%d", r,
                duf_levinfo_dfd( PDI ), PDI->opendir );
/* #ifdef MAS_ASSERT */
  {
    /* DUF_SFIELD2( fname ); */
    const char *dfn = duf_levinfo_itemshowname( PDI );

    /* filename from db same as duf_levinfo_itemname( pdi ) */
    assert( 0 == strcmp( DUF_GET_SFIELD2( fname ), dfn ) );
    /* DOR( r, duf_levinfo_pstmt2levinfo_dh( PDI, pstmt, node_type ) ); */
    {
      duf_levinfo_t *l = duf_levinfo_ptr( PDI );

      assert( l );
      assert( duf_levinfo_dbstat( PDI ) );
      assert( duf_levinfo_dbstat_dev( PDI ) );
      assert( duf_levinfo_dbstat_inode( PDI ) );
    }
  }
/* #endif */


  duf_sel_cb2_leaf_at( scanstage, pstmt, str_cb2, sccbh );


  DEBUG_ENDR( r );
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
int
duf_sel_cb2_leaf( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  assert( PDI );
  assert( PDI->pathinfo.depth >= 0 );


  assert( PDI->pathinfo.depth == duf_levinfo_calc_depth( PDI ) );


  /* data from db at pstmt */

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", duf_pdi_depth( PDI ) );
  DUF_TRACE( explain, 4, "@ sel cb2 leaf" );
  assert( str_cb2 == duf_eval_sccbh_db_leaf_str_cb || str_cb2 == duf_eval_sccbh_db_leaf_fd_str_cb || str_cb2 == NULL );


  DUF_TRACE( scan, 9, "LEAF %s", duf_levinfo_path( PDI ) );
  {
    /*@ 1. go down + dbopenat */
    DOR( r, duf_pstmt_levinfo_godown_dbopenat_dh( pstmt, sccbh, DUF_NODE_LEAF /* node_type */  ) );
    DUF_TRACE( scan, 9, "(%s) LEAF down %s", mas_error_name_i( r ), duf_levinfo_path( PDI ) );
    assert( PDI->pathinfo.depth >= 0 );

    DOR( r, duf_sel_cb2_leaf_at( scanstage, pstmt, str_cb2, sccbh ) );

    assert( PDI->pathinfo.depth == duf_levinfo_calc_depth( PDI ) );

    DOR( r, duf_levinfo_goup( PDI ) );
  }
  DUF_TRACE( scan, 9, "/LEAF %s", duf_levinfo_path( PDI ) );

  DEBUG_ENDR( r );
}
