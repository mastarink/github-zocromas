/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ♠ */

#include <mastar/trace/mas_trace.h>

#include "duf_tracen_defs.h"                                         /* T; TT; TR ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ♠ */
#include "duf_levinfo_updown.h"

#include "duf_sccbh_shortcuts.h"

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ♠ */

/* ###################################################################### */
#include "duf_sccbh_pstmt.h"
/* ###################################################################### */

/* 20150831.202009 */
int
duf_sccbh_pstmt_godown_dbopenat_dh( duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt, duf_node_type_t node_type )
{
  DUF_STARTR( r );

/* Not here : assert( DUF_GET_UFIELD2( dirid) == duf_levinfo_dirid( pdi ) ); */
  MAST_TRACE( scan, 10, "before duf_levinfo_godown() : dirID:%llu", DUF_GET_UFIELD2( dirid ) );
  MAST_TRACE( explain, 2, "@ sel cb2 node" );

  DOR( r, duf_levinfo_godown_dbopenat_dh( PDI, node_type, pstmt ) );

  assert( r < 0 || DUF_GET_UFIELD2( dirid ) == duf_levinfo_dirid( PDI ) ); /* was set by duf_levinfo_godown */

  DUF_ENDR( r );
}
