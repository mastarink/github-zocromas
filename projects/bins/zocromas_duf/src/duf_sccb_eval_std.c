#define DUF_SQL_PDI_STMT

#include <assert.h>


#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/wrap/mas_memory.h> */


#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_action_table.h"
#include "duf_sccb_eval.h"
#include "duf_pdi_sccb_eval.h"
/* ###################################################################### */
#include "duf_sccb_eval_std.h"
/* ###################################################################### */


int
duf_ev_evnamed_list_std( const char *names )
{
  assert( DUF_CONFIGX( pdi )->pup == DUF_CONFIGX( puz ) );
  return duf_ev_evnamed_list( names, duf_action_table(  ) );
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
duf_ev_pdi_evnamen_std_pt( duf_depthinfo_t * pdi, const char *name, size_t len, duf_argvc_t * ptarg /*, duf_ufilter_t * pu */  )
{
  return duf_ev_pdi_evnamen( pdi, name, len, duf_action_table(  ), ptarg /*, pu */  );
}

int
duf_ev_pdi_evname_std_pt( duf_depthinfo_t * pdi, const char *name, duf_argvc_t * ptarg /*, duf_ufilter_t * pu */  )
{
  return duf_ev_pdi_evname( pdi, name, duf_action_table(  ), ptarg /*, pu */  );
}

int
duf_ev_pdi_evname_std_at( duf_depthinfo_t * pdi, const char *name, const char *arg /*, duf_ufilter_t * pu */  )
{
  return duf_ev_pdi_evname_at( pdi, name, duf_action_table(  ), arg /*, pu */  );
}

int
duf_ev_pdi_evname_std( duf_depthinfo_t * pdi, const char *name /*, duf_ufilter_t * pu */  )
{
  DEBUG_STARTR( r );

  DOR( r, duf_ev_pdi_evname_at( pdi, name, duf_action_table(  ), NULL /*, pu */  ) );
  DEBUG_ENDR( r );
}
