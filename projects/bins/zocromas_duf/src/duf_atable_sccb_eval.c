#include "duf_maintenance.h"


#include "duf_config.h"
#include "duf_config_trace.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_option_defs.h"    /* DUF_ACTG_FLAG( summary ) */

#include "duf_pdi_global.h"
#include "duf_pdi_atable_sccb_eval.h"
/* ###################################################################### */
#include "duf_atable_sccb_eval.h"
/* ###################################################################### */



int
duf_ev_atable_evnamen( const char *name, size_t len, const duf_action_table_t * table )
{
  DEBUG_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  DOR( r, duf_ev_pdi_atable_evnamen( duf_pdi_global(  ), name, len, table, DUF_CONFIGA( cli.targ ), DUF_ACTG_FLAG( summary ) ) );
  DEBUG_ENDR( r );
}

int
duf_ev_atable_evname( const char *name, const duf_action_table_t * table )
{
  DEBUG_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  DUF_TRACE( sccb, 0, "evaluate name %s [%s]", name, duf_pdi_global_name(  ) );
  DOR( r, duf_ev_pdi_atable_evname( duf_pdi_global(  ), name, table, DUF_CONFIGA( cli.targ ), DUF_ACTG_FLAG( summary ) ) );
  DEBUG_ENDR( r );
}

int
duf_ev_atable_evnamed_list( const char *names, const duf_action_table_t * table )
{
  DEBUG_STARTR( r );
  assert( duf_pdi_global_name(  ) );
  /* assert( duf_pdi_global()->pyp ); */
  DUF_TRACE( sccb, 0, "evaluate sccb list '%s' [%s]", names, duf_pdi_global_name(  ) );
  DOR( r, duf_ev_pdi_atable_evnamed_list( duf_pdi_global(  ), names, table, DUF_CONFIGA( cli.targ ), DUF_ACTG_FLAG( summary ) ) );
  DEBUG_ENDR( r );
}
