#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_action_table.h"
#include "duf_sccb_eval.h"
#include "duf_pdi_sccb_eval.h"
/* ###################################################################### */
#include "duf_sccb_eval_std.h"
/* ###################################################################### */



const duf_action_table_t *
duf_find_sccb_by_evname_std( const char *name )
{
  const duf_action_table_t *act;

  act = duf_find_sccb_by_evname( name, duf_action_table(  ) );
  return act;
}

int
duf_ev_evnamed_list_std( const char *names )
{
  DEBUG_STARTR( r );
  assert( DUF_CONFIGX( pdi )->pup == DUF_CONFIGX( puz ) );
  DOR( r, duf_ev_evnamed_list( names, duf_action_table(  ) ) );
  DEBUG_ENDR( r );
}

int
duf_ev_evnamen_std( const char *name, size_t len )
{
  assert( DUF_CONFIGX( pdi )->pup == DUF_CONFIGX( puz ) );
  return duf_ev_evnamen( name, len, duf_action_table(  ) );
}

int
duf_ev_evname_std( const char *name )
{
  assert( DUF_CONFIGX( pdi )->pup == DUF_CONFIGX( puz ) );
  return duf_ev_evname( name, duf_action_table(  ) );
}

int
duf_ev_pdi_evnamen_std_pt( duf_depthinfo_t * pdi, const char *name, size_t len, duf_argvc_t * ptarg )
{
  DEBUG_STARTR( r );
  DOR( r, duf_ev_pdi_evnamen( pdi, name, len, duf_action_table(  ), ptarg ) );
  DEBUG_ENDR( r );
}

int
duf_ev_pdi_evname_std_pt( duf_depthinfo_t * pdi, const char *name, duf_argvc_t * ptarg )
{
  return duf_ev_pdi_evname( pdi, name, duf_action_table(  ), ptarg );
}

int
duf_ev_pdi_evname_std_at( duf_depthinfo_t * pdi, const char *name, const char *arg )
{
  return duf_ev_pdi_evname_at( pdi, name, duf_action_table(  ), arg );
}

int
duf_ev_pdi_evname_std( duf_depthinfo_t * pdi, const char *name )
{
  DEBUG_STARTR( r );

  DOR( r, duf_ev_pdi_evname_at( pdi, name, duf_action_table(  ), NULL ) );
  DEBUG_ENDR( r );
}
