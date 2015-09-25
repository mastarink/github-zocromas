#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "duf_option_defs.h"    /* for DUF_ACTG_FLAG( progress ) !ONLY! */

#include "duf_utils.h"          /* duf_percent */

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h" /* duf_levinfo_goup */

#include "duf_pdi_ref.h"        /* duf_pdi_depth */

#include "duf_sccb_def.h"       /* DUF_SCCB_PDI */
#include "duf_sccb.h"           /* duf_uni_scan_action_title */
#include "duf_leaf_scan2.h"     /* duf_eval_sccbh_db_leaf_fd_str_cb and duf_eval_sccbh_db_leaf_str_cb */

#include "duf_sql_defs.h"       /* DUF_GET_UFIELD2 */
#include "duf_sql_field.h"      /* __duf_sql_ull_by_name2 for DUF_GET_UFIELD2 */

#include "duf_sccbh_shortcuts.h" /* PDI etc. */
#include "duf_pstmt_levinfo.h"

/* ###################################################################### */
#include "duf_sel_cb_leaf.h"
/* ###################################################################### */


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
duf_sel_cb2_leaf( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  assert( PDI );

  /* data from db at pstmt */

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", duf_pdi_depth( PDI ) );
  DUF_TRACE( explain, 4, "@ sel cb2 leaf" );

  assert( str_cb2 == duf_eval_sccbh_db_leaf_fd_str_cb || str_cb2 == duf_eval_sccbh_db_leaf_str_cb );


  DOR( r, duf_pstmt_levinfo_godown_dbopenat_dh( pstmt, PDI, 1 /* is_leaf */  ) );
  assert( PDI->pathinfo.depth >= 0 );

  if ( 0 && DUF_GET_UFIELD2( md5id ) )
  {
    T( "@@@MD5:%llu - to take list of same files here", DUF_GET_UFIELD2( md5id ) );
  }

  if ( DUF_NOERROR( r ) )       /* levinfo_down OK */
  {

    DUF_TRACE( scan_reg, 0, "* qn%llu/q%llu T%llu %s", PDI->seq_leaf, PDI->seq, TOTITEMS, SCCB->title );


    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 5. leaf str cb2; r:%d; dfd:%d ; opendir:%d", r,
                  duf_levinfo_dfd( PDI ), PDI->opendir );
#ifdef MAS_TRACING
    {
      DUF_SFIELD2( filename );
      const char *dfn = duf_levinfo_itemshowname( PDI );

      /* filename from db same as duf_levinfo_itemname( pdi ) */
      assert( 0 == strcmp( filename, dfn ) );
      /* DOR( r, duf_levinfo_pstmt2levinfo_dh( PDI, pstmt, 1 (* is_leaf *)  ) ); */
      {
        duf_levinfo_t *l = duf_levinfo_ptr( PDI );

        assert( l );
        assert( duf_levinfo_dbstat( PDI ) );
        assert( duf_levinfo_dbstat_dev( PDI ) );
        assert( duf_levinfo_dbstat_inode( PDI ) );
      }
    }
#endif



    if ( str_cb2 )
    {
      DUF_TRACE( explain, 2, "=> str cb2" );
      DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 5. leaf str cb2" );
      assert( str_cb2 == duf_eval_sccbh_db_leaf_fd_str_cb || str_cb2 == duf_eval_sccbh_db_leaf_str_cb );
#if 1
      DOR( r, ( str_cb2 ) ( pstmt, sccbh ) );
      if ( DUF_NOERROR( r ) )
      {
        PDI->seq++;
        PDI->seq_leaf++;
        DUF_TRACE( seq, 0, "seq:%llu; seq_leaf:%llu", PDI->seq, PDI->seq_leaf );
      }
      if ( !SCCB->count_nodes && !SCCB->no_progress && TOTITEMS > 0 && DUF_ACTG_FLAG( progress ) )
      {
        long long m;

        m = TOTITEMS;
        DUF_SCCB( DUF_TRACE, action, 0, "total_items: %llu; m: %llu rd:%d; d:%d", TOTITEMS, m, duf_pdi_reldepth( PDI ), duf_pdi_depth( PDI ) );
        /* assert( PDI->seq_node <= m ); FIXME counters! */
        if ( m > 0 )
        {
          duf_percent( PDI->seq_leaf, m, duf_uni_scan_action_title( SCCB ) );
          DUF_TRACE( seq, 0, "PROGRESS: seq:%llu; seq_leaf:%llu OF %llu", PDI->seq, PDI->seq_leaf, m );
        }
      }
      DUF_CLEAR_ERROR( r, DUF_ERROR_OPENAT_ENOENT );
#else
      DOR_NOE( r, ( str_cb2 ) ( pstmt, sccbh ), DUF_ERROR_OPENAT_ENOENT );
#endif
    }

    DOR( r, duf_levinfo_goup( PDI ) );
  }

  DEBUG_ENDR( r );
}
