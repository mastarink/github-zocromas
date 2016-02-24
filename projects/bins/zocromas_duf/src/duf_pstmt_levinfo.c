#undef MAS_TRACING
#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_util.h"


#include "duf_levinfo_ref.h"
#include "duf_levinfo_updown.h"

#include "duf_sccbh_shortcuts.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"


/* ###################################################################### */
#include "duf_pstmt_levinfo.h"
/* ###################################################################### */


/* 20150831.202009 */
int
duf_pstmt_levinfo_godown_dbopenat_dh( duf_stmnt_t * pstmt, duf_sccb_handle_t * sccbh, duf_node_type_t node_type )
{
  DUF_STARTR( r );

  /* Not here : assert( DUF_GET_UFIELD2( dirid) == duf_levinfo_dirid( pdi ) ); */
  DUF_TRACE( scan, 10, "before duf_levinfo_godown() : dirID:%llu", DUF_GET_UFIELD2( dirid ) );
  DUF_TRACE( explain, 2, "@ sel cb2 node" );

  DOR( r, duf_levinfo_godown_dbopenat_dh( PDI, node_type, pstmt ) );

  assert( r < 0 || DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( PDI ) ); /* was set by duf_levinfo_godown */

  DUF_ENDR( r );
}
