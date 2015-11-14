#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_updown.h"
#include "duf_li.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_filters.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_stmt_defs.h"
#include "duf_sql_defs.h"
#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"

#include "evsql_selector.h"
#include "sql_tables_defs.h"    /* for duf_levinfo_childs* only */
#include "sql_selected_defs.h"
#include "duf_ufilter_bind.h"


#include "sql_beginning_selected.h"

#include "duf_sql_field.h"


/* ###################################################################### */
#include "duf_pstmt_levinfo.h"
/* ###################################################################### */


/* 20150831.202009 */
int
duf_pstmt_levinfo_godown_dbopenat_dh( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_node_type_t node_type )
{
  DEBUG_STARTR( r );

  /* Not here : assert( DUF_GET_UFIELD2( dirid) == duf_levinfo_dirid( pdi ) ); */
  DUF_TRACE( scan, 10, "before duf_levinfo_godown() : dirID:%llu", DUF_GET_UFIELD2( dirid ) );
  DUF_TRACE( explain, 2, "@ sel cb2 node" );

  DOR( r, duf_levinfo_godown_dbopenat_dh( pdi, node_type, pstmt ) );

  assert( r < 0 || DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( pdi ) ); /* was set by duf_levinfo_godown */

  DEBUG_ENDR( r );
}
