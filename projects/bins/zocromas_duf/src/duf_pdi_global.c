#include <string.h>

#include "duf_maintenance.h"


#include "duf_status_ref.h"
#include "duf_option_defs.h"

#include "duf_config.h"
#include "duf_config_trace.h"
#include "duf_config_defs.h"

#include "duf_pdi_credel.h"

#include "duf_pdi.h"
/* ###################################################################### */
#include "duf_pdi_global.h"
/* ###################################################################### */




/* 20150914.114508 */
int
duf_pdi_init_global( void )
{
  DEBUG_STARTR( r );
  DUF_TRACE( pdi, 0, "@@@recursive:%d; NO real_path", DUF_UG_FLAG( recursive ) );
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
  DEBUG_ENDR( r );
}

SR( PDI, pdi_create_global, const char *name )
{
  global_status.scn.pdi = duf_pdi_create( name );
  CR( pdi_init_global );

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
