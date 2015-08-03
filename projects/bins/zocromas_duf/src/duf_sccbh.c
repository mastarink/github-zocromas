#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_option_defs.h"
#include "duf_levinfo_ref.h"

#include "duf_dir_scan2.h"

/* #include "duf_sccb_def.h" */

#include "duf_sccbh_shortcuts.h"
#include "duf_sccb.h"
/* ###################################################################### */
#include "duf_sccbh.h"
/* ###################################################################### */

int
duf_sccbh_eval_pdi_and_summary( duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );

  DUF_E_NO( DUF_ERROR_TOO_DEEP );

  DOR( r, DUF_WRAPPED( duf_scan_dirs_by_pdi ) ( ( duf_sqlite_stmt_t * ) NULL, sccbh ) );

  if ( r >= 0 && DUF_ACT_FLAG( summary ) )
  {
    DUF_PRINTF( 0, "%s", duf_uni_scan_action_title( SCCB ) );

    DUF_PRINTF( 0, " summary; seq:     %llu", PDI->seq );
    DUF_PRINTF( 0, " summary; seq-leaf:%llu", PDI->seq_leaf );
    DUF_PRINTF( 0, " summary; seq-node:%llu", PDI->seq_node );
    if ( PU->max_seq )
      DUF_PRINTF( 0, " of %llu (max-seq)", PU->max_seq );
  }
  DUF_E_YES( DUF_ERROR_TOO_DEEP );
  DEBUG_ENDR( r );
}
