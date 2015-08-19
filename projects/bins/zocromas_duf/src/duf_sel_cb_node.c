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
#include "duf_sccb_eval_dirs.h"

#include "duf_sccbh_shortcuts.h"
#include "duf_pstmt_levinfo.h"

/* ###################################################################### */
#include "duf_sel_cb_node.h"
/* ###################################################################### */


/*
 * str_cb2 can be
 *   -- duf_eval_sccbh_all(_wrap)
 *   ...
 */
int
duf_sel_cb2_node( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  assert( PDI );

  /* data from db at pstmt */

  DUF_TRACE( scan, 10, "  " DUF_DEPTH_PFMT ": =====> scan node2", duf_pdi_depth( PDI ) );
  DUF_TRACE( explain, 4, "@ sel cb2 node" );
  assert( str_cb2 == DUF_WRAPPED( duf_eval_sccbh_all ) );

#if 1
  DOR( r, duf_pstmt_levinfo_godown_openat_dh( pstmt, PDI, 0 /* is_leaf */  ) );


#elif 0

#else

#endif
  assert( PDI->depth >= 0 );

  if ( r >= 0 )                 /* levinfo_down OK */
  {
    PDI->seq++;
    PDI->seq_node++;

    DUF_TRACE( scan_dir, 0, "* qn%llu/q%llu T%llu %s", PDI->seq_node, PDI->seq, TOTITEMS, SCCB->title );
    if ( SCCB->count_nodes && !SCCB->no_progress && TOTITEMS > 0 && DUF_ACT_FLAG( progress ) )
    {
      long long m;

      m = TOTITEMS + duf_pdi_reldepth( PDI ) - duf_pdi_depth( PDI ) - 1;
      DUF_SCCB( DUF_TRACE, action, 0, "total_items: %llu; m: %llu rd:%d; d:%d", TOTITEMS, m, duf_pdi_reldepth( PDI ), duf_pdi_depth( PDI ) );
      /* assert( PDI->seq_node <= m ); FIXME counters! */
      if ( m > 0 )
      {
        duf_percent( PDI->seq_node, m, duf_uni_scan_action_title( SCCB ) );
      }
    }

    DUF_TRACE( seq, 0, "seq:%llu; seq_node:%llu", PDI->seq, PDI->seq_node );


    DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 5. leaf str cb2; r:%d; dfd:%d ; opendir:%d", r,
                  duf_levinfo_dfd( PDI ), PDI->opendir );

    if ( str_cb2 )
    {
      DUF_TRACE( explain, 2, "=> str cb2" );
      DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 5. node str cb2" );
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
