/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_se.h"                                                  /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_debug_defs.h"                                          /* DUF_WRAPSTATIC; DUF_WRAPPED ...  ♠ */

#include "duf_status_ref.h"
#include "duf_option_defs.h"
#include "duf_optimpl_defs.h"

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_defs.h"

#include "duf_pdi_credel.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_reinit.h"
/* ###################################################################### */
#include "duf_pdi_global.h"
/* ###################################################################### */

/* 20150914.114508 */
int
duf_pdi_init_global( void )
{
  DUF_STARTR( r );
  MAST_TRACE( pdi, 0, "@@@recursive:%d; NO real_path", DUF_UG_FLAG( recursive ) );
#if 0
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( global_status.scn.pdi, global_status.scn.pdi->pup, NULL /* real_path */ , NULL /* sql_set */ ,
                                        0 /* caninsert */ ,
                                        DUF_UG_FLAG( recursive ) /* frecursive */ ,
                                        1 /* opendir */  ) );
#else
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( global_status.scn.pdi, DUF_CONFIGG( vars.puz ), NULL /* real_path */ , NULL /* sql_set */ ,
                                        0 /* caninsert */ ,
                                        DUF_UG_FLAG( recursive ) /* frecursive */ ,
                                        DUF_ACTG_FLAG( allow_dirs ) /* fallow_dirs */ ,
                                        DUF_UG_FLAG( linear ) /* frecursive */ ,
                                        1 /* opendir */  ) );
#endif
  assert( global_status.scn.pdi->pup == DUF_CONFIGX( vars.puz ) );
  DUF_ENDR( r );
}

SR( PDI, pdi_create_global, const char *name )
{
  global_status.scn.pdi = duf_pdi_create( name ? name : "selected" );
  CR( pdi_init_global );

  assert( DUF_UG_FLAG( recursive ) == duf_pdi_recursive( duf_pdi_global(  ) ) );
  assert( DUF_UG_FLAG( linear ) == duf_pdi_linear( duf_pdi_global(  ) ) );
  assert( DUF_ACTG_FLAG( allow_dirs ) == duf_pdi_allow_dirs( duf_pdi_global(  ) ) );

  ER( PDI, pdi_create_global, const char *name );
}

duf_depthinfo_t *
_duf_pdi_global( int *pr )
{
  int rpr = 0;

  if ( !global_status.scn.pdi )
    DORF( rpr, duf_pdi_create_global, "selected" );
  if ( pr )
    *pr = rpr;
  return global_status.scn.pdi;
}

duf_depthinfo_t *
duf_pdi_global( void )
{
  return _duf_pdi_global( NULL );
}

const char *
_duf_pdi_global_name( int *pr )
{
  duf_depthinfo_t *pdi;

  pdi = _duf_pdi_global( pr );
  return pdi ? pdi->pdi_name : NULL;
}

const char *
duf_pdi_global_name( void )
{
  return _duf_pdi_global_name( NULL );
}

const duf_ufilter_t *
_duf_pdi_global_ufilter( int *pr )
{
  duf_depthinfo_t *pdi;

  pdi = _duf_pdi_global( pr );
  return pdi ? pdi->pup : NULL;
}

const duf_ufilter_t *
duf_pdi_global_ufilter( void )
{
  return _duf_pdi_global_ufilter( NULL );
}

SR( PDI, pdi_kill_global, void )
{
  CR( pdi_kill, &global_status.scn.pdi );
  ER( PDI, pdi_kill_global, void );
}

SR( PDI, pdi_reinit_pu_anypath_global, const char *cpath, const duf_ufilter_t * pu )
{
/* CR( pdi_reinit_defflags_anypath, duf_pdi_global(  ), cpath, DUF_CONFIGG( vars.puz ), sql_set ); */
  CR( pdi_reinit_defflags_anypath, duf_pdi_global(  ), cpath, pu, NULL /* sql_set */  );
  ER( PDI, pdi_reinit_pu_anypath_global, const char *cpath, const duf_ufilter_t * pu );
}

SR( PDI, pdi_reinit_anypath_global, const char *cpath )
{
  CR( pdi_reinit_pu_anypath_global, cpath, DUF_CONFIGG( vars.puz ) );
  ER( PDI, pdi_reinit_anypath_global, const char *cpath );
}
