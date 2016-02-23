/* #undef MAS_TRACING */
#include <string.h>

#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_utils_path.h>

#include "duf_maintenance_z.h"
#include "duf_maintenance_tracen.h"
#include "duf_maintenance_errors.h"

/* #include "duf_config_util.h" */

#include "duf_option_defs.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"

#include "duf_option_typed_gen.h"
#include "duf_option_typed_call.h"

/* ###################################################################### */
#include "duf_option_typed.h"
/* ###################################################################### */

int
duf_xoption_clarify_typed( duf_config_cli_t * cli,const duf_longval_extended_t * extended, const char *optargg, const duf_longval_extended_vtable_t * xvtable, unsigned noo,
                           duf_option_stage_t istage, duf_option_source_t source )
{
  DUF_STARTR( r );
  duf_option_data_t od DUF_UNUSED;

#if 0
  od.stage = istage;
  od.source = source;
  od.optarg = optargg;
  od.xvtable = xvtable;
  od.noo = noo;
  od.extended = extended;
#endif

  if ( DUF_OPTION_CHECK_STAGE( istage, extended, xvtable ) )
  {
    int nogen = 0;

    DUF_TRACE( options, 60, "to clarify typed:'%s'; `noo`:%d; stage:%s; source:%s", extended ? extended->o.name : "?", noo,
               duf_optstage_name( istage ), duf_optsource_name( source ) );
    if ( extended->calltype )
    {
      nogen = 1;
      DOR( r, duf_xoption_clarify_typed_call(  cli,extended, optargg, noo, istage, source ) );

    /* useless now: */
      if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_CLARIFIED ) )
      {
        DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED );
        nogen = 0;
      }
    }
    if ( !nogen )
    {
      DOR( r, duf_xoption_clarify_typed_gen( extended, optargg, noo, istage, source ) );
    }
  }
  else
  {
    DUF_TRACE( options, 60, "@--%s='%s'; `noo`:%d : NOT for this stage; istage:%s", extended ? extended->o.name : "?", optargg ? optargg : "", noo,
               duf_optstage_name( istage ) );
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
  }

  DUF_ENDR( r );
}
