/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include "duf_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */

#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */

#include "duf_option_typed_gen.h"
#include "duf_option_typed_call.h"

/* ###################################################################### */
#include "duf_option_typed.h"
/* ###################################################################### */

SR( OPTIONS, xoption_clarify_typed, duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg,
    const duf_longval_extended_vtable_t * xvtable, unsigned noo, duf_option_stage_t istage, duf_option_source_t source )
{


#if 0
  duf_option_data_t od;

  od.stage = istage;
  od.source = source;
  od.optarg = optargg;
  od.xvtable = xvtable;
  od.noo = noo;
  od.extended = extended;
#endif

  if ( DUF_OPTION_CHECK_STAGE( cli, istage, extended, xvtable ) )
  {
    int nogen = 0;

    DUF_TRACE( options, 60, "to clarify typed:'%s'; `noo`:%d; stage:%s; source:%s", extended ? extended->o.name : "?", noo,
               duf_optstage_name( cli, istage ), duf_optsource_name( cli, source ) );
    if ( extended->calltype )
    {
      nogen = 1;
      CR( xoption_clarify_typed_call, cli, extended, optargg, noo, istage, source );

    /* useless now: */
      if ( QISERR_N( OPTION_NOT_CLARIFIED ) )
      {
        ERRCLEAR( OPTION_NOT_CLARIFIED );
        nogen = 0;
      }
    }
    if ( !nogen )
    {
      CR( xoption_clarify_typed_gen, cli, extended, optargg, noo, istage, source );
    }
  }
  else
  {
    DUF_TRACE( options, 60, "@--%s='%s'; `noo`:%d : NOT for this stage; istage:%s", extended ? extended->o.name : "?", optargg ? optargg : "", noo,
               duf_optstage_name( cli, istage ) );
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
  }


  ER( OPTIONS, xoption_clarify_typed, duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg,
      const duf_longval_extended_vtable_t * xvtable, unsigned noo, duf_option_stage_t istage, duf_option_source_t source );
}
