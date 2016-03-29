/* #undef MAS_TRACING */
#include <assert.h>
#include <dlfcn.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
/* #include <mastar/error/mas_error_defs_make.h> */
#include <mastar/error/mas_error_defs.h>

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_action_table.h"

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
duf_scan_callbacks_t *
duf_first_sccb( void )
{
  duf_action_table_t *at = NULL;

  at = duf_actions_table_std(  );
  return at ? at->sccb : NULL;
}

static void duf_sccb_dlclose_std( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
duf_sccb_dlclose_std( void )
{
  duf_sccb_dlclose( duf_first_sccb(  ) );
}

const duf_scan_callbacks_t *
duf_find_sccb_by_evname_std( const char *name )
{
  const duf_scan_callbacks_t *sccb;

  sccb = duf_find_sccb_by_evname( name, duf_first_sccb(  ) );
  return sccb;
}

#if 0
const duf_scan_callbacks_t *
duf_finor_load_d_sccb_by_evname_std( const char *name )
{
  const duf_scan_callbacks_t *sccb;

  sccb = duf_find_or_load_sccb_by_evname( name, duf_first_sccb(  ) );
  return sccb;
}
#endif

SR( OTHER, ev_evnamed_list_std, const char *names )
{
/* DUF_STARTR( r ); */
  /* assert( duf_pdi_global_ufilter(  ) == DUF_CONFIGX( vars.puz ) ); */
#if 0
  DOR( r, duf_ev_atable_evnamed_list( names, duf_action_table(  ) ) );
#else
/* QT( "names:%s; dirid:%llu", names, duf_levinfo_dirid( duf_pdi_global(  ) ) ); */
  CR( ev_evnamed_list, names, duf_first_sccb(  ) );
#endif
/* DUF_ENDR( r ); */
  ER( OTHER, ev_evnamed_list_std, const char *names );
}

#if 0
int
duf_ev_evnamen_std( const char *name, size_t len )
{
  DUF_STARTR( r );
  assert( duf_pdi_global_ufilter(  ) == DUF_CONFIGX( vars.puz ) );
# if 0
  DOR( r, duf_ev_atable_evnamen( name, len, duf_action_table(  ) ) );
# else
  DOR( r, duf_ev_evnamen( name, len, duf_first_sccb(  ) ) );
# endif
  DUF_ENDR( r );
}

int
duf_ev_evname_std( const char *name )
{
  DUF_STARTR( r );
  assert( duf_pdi_global_ufilter(  ) == DUF_CONFIGX( vars.puz ) );
# if 0
  DOR( r, duf_ev_atable_evname( name, duf_action_table(  ) ) );
# else
  DOR( r, duf_ev_evname( name, duf_first_sccb(  ) ) );
# endif
  DUF_ENDR( r );
}

int
duf_ev_pdi_evnamen_std_pt( duf_depthinfo_t * pdi, const char *name, size_t len, const mas_argvc_t * ptarg )
{
  DUF_STARTR( r );
# if 0
  DOR( r, duf_ev_pdi_atable_evnamen( pdi, name, len, duf_action_table(  ), ptarg, DUF_ACTG_FLAG( summary ) ) );
# else
  DOR( r, duf_ev_pdi_evnamen( pdi, name, len, duf_first_sccb(  ), ptarg, DUF_ACTG_FLAG( summary ) ) );
# endif
  DUF_ENDR( r );
}

int
duf_ev_pdi_evname_std_pt( duf_depthinfo_t * pdi, const char *name, const mas_argvc_t * ptarg )
{
  DUF_STARTR( r );
# if 0
  DOR( r, duf_ev_pdi_atable_evname( pdi, name, duf_action_table(  ), ptarg, DUF_ACTG_FLAG( summary ) ) );
# else
  DOR( r, duf_ev_pdi_evname( pdi, name, duf_first_sccb(  ), ptarg, DUF_ACTG_FLAG( summary ) ) );
# endif
  DUF_ENDR( r );
}

int
duf_ev_pdi_evname_std_at( duf_depthinfo_t * pdi, const char *name, const char *arg )
{
  DUF_STARTR( r );
# if 0
  DOR( r, duf_ev_pdi_atable_evname_at( pdi, name, duf_action_table(  ), arg, DUF_ACTG_FLAG( summary ) ) );
# else
  DOR( r, duf_ev_pdi_evname_at( pdi, name, duf_first_sccb(  ), arg, DUF_ACTG_FLAG( summary ) ) );
# endif
  DUF_ENDR( r );
}
#endif

SR( PDI, ev_pdi_evname_std, duf_depthinfo_t * pdi, const char *name )
{
/* DUF_STARTR( r ); */
/* assert( 0 );                                             */
/* assert( duf_levinfo_node_type( pdi ) == DUF_NODE_LEAF ); */

#if 0
  DOR( r, duf_ev_pdi_atable_evname_at( pdi, name, duf_action_table(  ), NULL, DUF_ACTG_FLAG( summary ) ) );
#else
  CR( ev_pdi_evname_at, pdi, name, duf_first_sccb(  ), NULL, /* DUF_ACTG_FLAG( summary ) */ duf_get_config_flag_act_summary() );
#endif
/* DUF_ENDR( r ); */
  ER( PDI, ev_pdi_evname_std, duf_depthinfo_t * pdi, const char *name );
}
