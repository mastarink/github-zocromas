#include "duf_maintenance.h"

#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

#include "duf_action_table.h"

#include "duf_option_defs.h"    /* DUF_ACTG_FLAG( summary ) */

#include "duf_sccb.h"
#include "duf_sccb_eval.h"
#include "duf_pdi_sccb_eval.h"
/* ###################################################################### */
#include "duf_sccb_eval_std.h"
/* ###################################################################### */


#if 0
const duf_action_table_t *
duf_find_atable_sccb_by_evname_std( const char *name )
{
  const duf_action_table_t *act;

  act = duf_find_atable_sccb_by_evname( name, _duf_action_table(  ) );
  return act;
}
#endif

const duf_scan_callbacks_t *
duf_find_sccb_by_evname_std( const char *name )
{
  const duf_scan_callbacks_t *sccb;

  sccb = duf_find_sccb_by_evname( name, duf_first_sccb(  ) );
  return sccb;
}
const duf_scan_callbacks_t *
duf_finor_load_d_sccb_by_evname_std( const char *name )
{
  const duf_scan_callbacks_t *sccb;

  sccb = duf_find_or_load_sccb_by_evname( name, duf_first_sccb(  ) );
  return sccb;
}



int
duf_ev_evnamed_list_std( const char *names )
{
  DEBUG_STARTR( r );
  assert( DUF_CONFIGX( scn.pdi )->pup == DUF_CONFIGX( scn.puz ) );
#if 0
  DOR( r, duf_ev_atable_evnamed_list( names, duf_action_table(  ) ) );
#else
  DOR( r, duf_ev_evnamed_list( names, duf_first_sccb(  ) ) );
#endif
  DEBUG_ENDR( r );
}

int
duf_ev_evnamen_std( const char *name, size_t len )
{
  DEBUG_STARTR( r );
  assert( DUF_CONFIGX( scn.pdi )->pup == DUF_CONFIGX( scn.puz ) );
#if 0
  DOR( r, duf_ev_atable_evnamen( name, len, duf_action_table(  ) ) );
#else
  DOR( r, duf_ev_evnamen( name, len, duf_first_sccb(  ) ) );
#endif
  DEBUG_ENDR( r );
}

int
duf_ev_evname_std( const char *name )
{
  DEBUG_STARTR( r );
  assert( DUF_CONFIGX( scn.pdi )->pup == DUF_CONFIGX( scn.puz ) );
#if 0
  DOR( r, duf_ev_atable_evname( name, duf_action_table(  ) ) );
#else
  DOR( r, duf_ev_evname( name, duf_first_sccb(  ) ) );
#endif
  DEBUG_ENDR( r );
}

int
duf_ev_pdi_evnamen_std_pt( duf_depthinfo_t * pdi, const char *name, size_t len, const mas_argvc_t * ptarg )
{
  DEBUG_STARTR( r );
#if 0
  DOR( r, duf_ev_pdi_atable_evnamen( pdi, name, len, duf_action_table(  ), ptarg, DUF_ACTG_FLAG( summary ) ) );
#else
  DOR( r, duf_ev_pdi_evnamen( pdi, name, len, duf_first_sccb(  ), ptarg, DUF_ACTG_FLAG( summary ) ) );
#endif
  DEBUG_ENDR( r );
}

int
duf_ev_pdi_evname_std_pt( duf_depthinfo_t * pdi, const char *name, const mas_argvc_t * ptarg )
{
  DEBUG_STARTR( r );
#if 0
  DOR( r, duf_ev_pdi_atable_evname( pdi, name, duf_action_table(  ), ptarg, DUF_ACTG_FLAG( summary ) ) );
#else
  DOR( r, duf_ev_pdi_evname( pdi, name, duf_first_sccb(  ), ptarg, DUF_ACTG_FLAG( summary ) ) );
#endif
  DEBUG_ENDR( r );
}

int
duf_ev_pdi_evname_std_at( duf_depthinfo_t * pdi, const char *name, const char *arg )
{
  DEBUG_STARTR( r );
#if 0
  DOR( r, duf_ev_pdi_atable_evname_at( pdi, name, duf_action_table(  ), arg, DUF_ACTG_FLAG( summary ) ) );
#else
  DOR( r, duf_ev_pdi_evname_at( pdi, name, duf_first_sccb(  ), arg, DUF_ACTG_FLAG( summary ) ) );
#endif
  DEBUG_ENDR( r );
}

int
duf_ev_pdi_evname_std( duf_depthinfo_t * pdi, const char *name )
{
  DEBUG_STARTR( r );
#if 0
  DOR( r, duf_ev_pdi_atable_evname_at( pdi, name, duf_action_table(  ), NULL, DUF_ACTG_FLAG( summary ) ) );
#else
  DOR( r, duf_ev_pdi_evname_at( pdi, name, duf_first_sccb(  ), NULL, DUF_ACTG_FLAG( summary ) ) );
#endif
  DEBUG_ENDR( r );
}
