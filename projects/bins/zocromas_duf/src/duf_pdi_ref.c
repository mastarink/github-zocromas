/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */

#include <mastar/trace/mas_trace.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

/* ###################################################################### */
#include "duf_pdi_ref.h"
/* ###################################################################### */

duf_depthinfo_t *
duf_pdi_root( duf_depthinfo_t * pdi )
{
  return pdi->root_pdi ? pdi->root_pdi : pdi;
}

int
duf_pdi_seq( const duf_depthinfo_t * pdi )
{
  return pdi ? pdi->seq : 0;
}

int
duf_pdi_recursive( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->recursive ? 1 : 0 ) : 0;
}

int
duf_pdi_allow_dirs( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->allow_dirs ? 1 : 0 ) : 0;
}

int
duf_pdi_linear( const duf_depthinfo_t * pdi )
{
  return pdi ? ( pdi->linear ? 1 : 0 ) : 0;
}

void
duf_pdi_reg_changes( duf_depthinfo_t * pdi, int changes )
{
  if ( pdi )
    pdi->changes += changes;
}

int
duf_pdi_opendir( const duf_depthinfo_t * pdi )
{
  return pdi && pdi->opendir ? 1 : 0;
}

int
duf_pdi_set_opendir( duf_depthinfo_t * pdi, int od )
{
  int rd = 0;

  assert( pdi );
  rd = pdi->opendir;
  pdi->opendir = od;
  /* QT( "@SET OPENDIR: %d", od ); */
  MAST_TRACE( fs, 3, "set opendir:%d", od );

  return rd;
}
