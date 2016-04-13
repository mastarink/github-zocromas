/* #undef MAS_TRACING */
#include <assert.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_sccb_structs.h"
/* ###################################################################### */
#include "duf_sccbh_ref.h"
/* ###################################################################### */

void
duf_sccbh_set_pdi( duf_sccb_handle_t * sccbh, duf_depthinfo_t * pdi )
{
  if ( sccbh )
    sccbh->pdi = pdi;
}

duf_depthinfo_t *
duf_sccbh_pdi( const duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->pdi : NULL;
}

duf_depthinfo_t *
duf_sccbh_pdi_p( duf_sccb_handle_t * sccbh )
{
  return sccbh ? sccbh->pdi : NULL;
}
