/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli)
#include "muc_tracen_defs_preset.h"
#include "muc_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

/* #include "muc_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "muc_errorn_defs.h"                                         (* DUF_NOERROR; MUC_CLEAR_ERROR; DUF_E_(LOWER|UPPER); MUC_TEST_R ... ♠ *) */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */

#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

#include "muc_option_typed_gen.h"
#include "muc_option_typed_call.h"

/* ###################################################################### */
#include "muc_option_typed.h"
/* ###################################################################### */

muc_SR( OPTIONS, xoption_clarify_typed, muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *optargg,
    const muc_longval_extended_vtable_t * xvtable, unsigned noo, muc_option_stage_t istage, muc_option_source_t source )
{

#if 0
  muc_option_data_t od;

  od.stage = istage;
  od.source = source;
  od.optarg = optargg;
  od.xvtable = xvtable;
  od.noo = noo;
  od.extended = extended;
#endif

  if ( MUC_OPTION_CHECK_STAGE( cli, istage, extended, xvtable ) )
  {
    int nogen = 0;

    MAST_TRACE( options, 60, "to clarify typed:'%s'; `noo`:%d; stage:%s; source:%s", extended ? extended->o.name : "?", noo,
                muc_optstage_name( cli, istage ), muc_optsource_name( cli, source ) );
    if ( extended->calltype )
    {
      nogen = 1;
      muc_CR( xoption_clarify_typed_call, cli, extended, optargg, noo, istage, source );

    /* useless now: */
      if ( QISERR_N( OPTION_NOT_CLARIFIED ) )
      {
        ERRCLEAR( OPTION_NOT_CLARIFIED );
        nogen = 0;
      }
    }
    if ( !nogen )
    {
      muc_CR( xoption_clarify_typed_gen, cli, extended, optargg, noo, istage, source );
    }
  }
  else
  {
    MAST_TRACE( options, 60, "@--%s='%s'; `noo`:%d : NOT for this stage; istage:%s", extended ? extended->o.name : "?", optargg ? optargg : "", noo,
                muc_optstage_name( cli, istage ) );
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
  }

  muc_ER( OPTIONS, xoption_clarify_typed, muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *optargg,
      const muc_longval_extended_vtable_t * xvtable, unsigned noo, muc_option_stage_t istage, muc_option_source_t source );
}
