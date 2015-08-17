#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_sel_cb_leaf.h"
#include "duf_sel_cb_node.h"

/* ###################################################################### */
#include "duf_pstmt_levinfo.h"
/* ###################################################################### */

int
duf_pstmt_levinfo_godown_openat_dh( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, int is_leaf )
{
  DEBUG_STARTR( r );

  if ( r >= 0 )
  {
    DUF_UFIELD2( dirid );       /* FIXME only for test / debug!? */
    DUF_UFIELD2( ndirs );
    DUF_UFIELD2( nfiles );
    /* Not here : assert( dirid == duf_levinfo_dirid( pdi ) );
     * */
    DUF_TRACE( scan, 10, "before duf_levinfo_godown() : dirID:%llu", dirid );
    DUF_TRACE( explain, 2, "@ sel cb2 node" );

    /*!! dirid need not be same as duf_levinfo_dirid( pdi ) before duf_levinfo_godown */
#if 0
    DOR( r, duf_levinfo_godown_openat_dh( pdi, dirid, DUF_GET_SFIELD2( dfname ), ndirs, nfiles, 0 /*is_leaf */  ) );
#else
    DOR( r, duf_levinfo_godown_dbopenat_dh( pdi, 0, DUF_GET_SFIELD2( dfname ), ndirs, nfiles, is_leaf /* is_leaf */ , pstmt ) );
#endif
    {
      assert( r < 0 || dirid == duf_levinfo_dirid( pdi ) ); /* was set by duf_levinfo_godown */
    }
  }
  DEBUG_ENDR( r );
}
