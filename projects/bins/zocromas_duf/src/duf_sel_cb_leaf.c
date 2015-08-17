#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"     /* for DUF_ACT_FLAG( progress ) !ONLY! */
#include "duf_option_defs.h"    /* for DUF_ACT_FLAG( progress ) !ONLY! */

#include "duf_utils.h"          /* duf_percent */

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"

#include "duf_pdi.h"
#include "duf_sccb_def.h"
#include "duf_sccb.h"

#include "duf_sccbh_shortcuts.h"
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
int
duf_sel_cb2_leaf( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  assert( PDI );

  /* data from db at pstmt */

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan leaf2", duf_pdi_depth( PDI ) );
  DUF_TRACE( explain, 4, "@ sel cb2 leaf" );


#if 1
  DOR( r, duf_pstmt_levinfo_godown_openat_dh( pstmt, PDI, 1 /* is_leaf */  ) );
/* TODO  duf_pstmt_levinfo_godown_openat_dh can be used if sql contains dirid, ndirs and nfiles
 * ( something like " 0 as ndirs, 0 as nfiles" ) */
#elif 0
  DOR( r, duf_levinfo_godown_openat_dh( PDI, 0 /* dir_id */ , DUF_GET_SFIELD2( filename ), 0, 0, 1 /* is_leaf */  ) );
#else
  DOR( r, duf_levinfo_godown_dbopenat_dh( PDI, 0, DUF_GET_SFIELD2( filename ), 0 /* ndirs */ , 0 /* nfiles */ , 1 /* is_leaf */ , pstmt ) );
#endif
  assert( PDI->depth >= 0 );

  if ( r >= 0 )                 /* levinfo_down OK */
  {
    PDI->seq++;
    PDI->seq_leaf++;

    DUF_TRACE( scan_reg, 0, "* qn%llu/q%llu T%llu %s", PDI->seq_leaf, PDI->seq, TOTITEMS, SCCB->title );
    if ( !SCCB->count_nodes && !SCCB->no_progress && TOTITEMS > 0 && DUF_ACT_FLAG( progress ) )
    {
      long long m;

      m = TOTITEMS;
      DUF_SCCB( DUF_TRACE, action, 0, "total_items: %llu; m: %llu rd:%d; d:%d", TOTITEMS, m, duf_pdi_reldepth( PDI ), duf_pdi_depth( PDI ) );
      /* assert( PDI->seq_node <= m ); FIXME counters! */
      if ( m > 0 )
      {
        duf_percent( PDI->seq_leaf, m, duf_uni_scan_action_title( SCCB ) );
      }
    }

    DUF_TRACE( seq, 0, "seq:%llu; seq_leaf:%llu", PDI->seq, PDI->seq_leaf );


    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 5. leaf str cb2; r:%d; dfd:%d ; opendir:%d", r,
                  duf_levinfo_dfd( PDI ), PDI->opendir );

    if ( str_cb2 )
    {
      DUF_TRACE( explain, 2, "=> str cb2" );
      DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 5. leaf str cb2" );
      DOR( r, ( str_cb2 ) ( pstmt, sccbh ) );
      if ( r == DUF_ERROR_OPENAT_ENOENT )
        r = 0;
    }

    DOR( r, duf_levinfo_goup( PDI ) );
  }

  DUF_CLEAR_ERROR( r, DUF_ERROR_TOO_DEEP ); /* reset error if it was `MAX_DEPTH` */

  DOR( r, duf_pdi_max_filter( PDI ) ); /* check if any of max's reached */

  DEBUG_ENDR( r );
}
