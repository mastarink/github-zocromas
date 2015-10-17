#include "duf_maintenance.h"


#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_sccb.h"
#include "duf_levinfo_ref.h"

#include "duf_option_defs.h"    /* DUF_ACTG_FLAG( summary ) */

#include "duf_pdi_atable_sccb_eval.h"
/* ###################################################################### */
#include "duf_atable_sccb_eval.h"
/* ###################################################################### */



int
duf_ev_atable_evnamen( const char *name, size_t len, const duf_action_table_t * table )
{
  DEBUG_STARTR( r );
  assert( DUF_CONFIGX( scn.pdi )->pdi_name );
  DOR( r, duf_ev_pdi_atable_evnamen( DUF_CONFIGG( scn.pdi ), name, len, table, DUF_CONFIGA( cli.targ ), DUF_ACTG_FLAG( summary ) ) );
  DEBUG_ENDR( r );
}

int
duf_ev_atable_evname( const char *name, const duf_action_table_t * table )
{
  DEBUG_STARTR( r );
  assert( DUF_CONFIGX( scn.pdi )->pdi_name );
  DUF_TRACE( sccb, 0, "evaluate name %s [%s]", name, DUF_CONFIGX( scn.pdi )->pdi_name );
  DOR( r, duf_ev_pdi_atable_evname( DUF_CONFIGG( scn.pdi ), name, table, DUF_CONFIGA( cli.targ ), DUF_ACTG_FLAG( summary ) ) );
  DEBUG_ENDR( r );
}

int
duf_ev_atable_evnamed_list( const char *names, const duf_action_table_t * table )
{
  DEBUG_STARTR( r );
  assert( DUF_CONFIGX( scn.pdi )->pdi_name );
  /* assert( DUF_CONFIGX( scn.pdi )->pyp ); */
  DUF_TRACE( sccb, 0, "evaluate sccb list '%s' [%s]", names, DUF_CONFIGX( scn.pdi )->pdi_name );
  DOR( r, duf_ev_pdi_atable_evnamed_list( DUF_CONFIGG( scn.pdi ), names, table, DUF_CONFIGA( cli.targ ), DUF_ACTG_FLAG( summary ) ) );
  DEBUG_ENDR( r );
}
