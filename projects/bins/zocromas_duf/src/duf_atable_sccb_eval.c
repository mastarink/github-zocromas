/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */

#include "duf_option_defs.h"                                         /* DUF_ACTG_FLAG( summary ) */
#include "duf_optimpl_defs.h"                                         /* DUF_ACTG_FLAG( summary ) */

#include "duf_pdi_global.h"
#include "duf_pdi_atable_sccb_eval.h"
/* ###################################################################### */
#include "duf_atable_sccb_eval.h"
/* ###################################################################### */

int
duf_ev_atable_evnamen( const char *name, size_t len, const duf_action_table_t * table )
{
  DUF_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  DOR( r, duf_ev_pdi_atable_evnamen( duf_pdi_global(  ), name, len, table, DUF_CONFIGA( pcli->targ ), DUF_ACTG_FLAG( summary ) ) );
  DUF_ENDR( r );
}

int
duf_ev_atable_evname( const char *name, const duf_action_table_t * table )
{
  DUF_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  DUF_TRACE( sccb, 0, "evaluate name %s [%s]", name, duf_pdi_global_name(  ) );
  DOR( r, duf_ev_pdi_atable_evname( duf_pdi_global(  ), name, table, DUF_CONFIGA( pcli->targ ), DUF_ACTG_FLAG( summary ) ) );
  DUF_ENDR( r );
}

int
duf_ev_atable_evnamed_list( const char *names, const duf_action_table_t * table )
{
  DUF_STARTR( r );
  assert( duf_pdi_global_name(  ) );
/* assert( duf_pdi_global()->pyp ); */
  DUF_TRACE( sccb, 0, "evaluate sccb list '%s' [%s]", names, duf_pdi_global_name(  ) );
  DOR( r, duf_ev_pdi_atable_evnamed_list( duf_pdi_global(  ), names, table, DUF_CONFIGA( pcli->targ ), DUF_ACTG_FLAG( summary ) ) );
  DUF_ENDR( r );
}
