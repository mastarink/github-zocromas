/* #undef MAS_TRACING */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include "muc_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_descr.h"                                        /* muc_xoption_description_tmp */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */
#include "muc_option_typed.h"

#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

#include "muc_config_cli_structs.h"
/* ###################################################################### */
#include "muc_option_clarify.h"
/* ###################################################################### */

/* 20160227.123618 */
muc_SR( OPTIONS, xoption_clarify, muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *optargg,
        const muc_longval_extended_vtable_t * xtable, unsigned noo, muc_option_stage_t istage, muc_option_source_t source )
{

  if ( extended )
  {
    MAST_TRACE( options, 55, "@@@fully clarify option: [--%s='%s'] stage:%s; `noo`:%d; source:%s",
                extended ? extended->o.name : "?", optargg, muc_optstage_name( cli, istage ), noo, muc_optsource_name( cli, source ) );
    muc_CR( xoption_clarify_typed, cli, extended, optargg, xtable, noo, istage, source );
    MAST_TRACE( options, 55, "@clarified typed: --%s / [%s]; (%d:%s)", extended->o.name, muc_xoption_description_tmp( cli, -1, extended ), QERRIND,
                QERRNAME );

#if 0
    if ( QISERR_N( OPTION_NOT_PARSED ) && !noo )
    {
      assert( 0 );
      MAST_TRACE( explain, 1, "@old opt for %s", extended ? extended->o.name : NULL );
      muc_CR( clarify_xcmd_old, extended, optargg, istage, xtable );
    }
#endif
    MAST_TRACE( options, 55, "@clarified cli option: %s (%d:%s)", muc_xoption_description_tmp( cli, -1, extended ), QERRIND, QERRNAME );
  }
  else
  {
    ERRMAKE( OPTION_NOT_PARSED );
    assert( 0 );
  }
  MAST_TRACE( options, +170, "@[%s]; arg:%s; istage:%d; `noo`:%d", muc_xoption_description_tmp( cli, -1, extended ), optargg, istage, noo );

  muc_ER( OPTIONS, xoption_clarify, muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *optargg,
          const muc_longval_extended_vtable_t * xtable, unsigned noo, muc_option_stage_t istage, muc_option_source_t source );
}

/* 20150924.144102 */
muc_SR( OPTIONS, argv_clarify, muc_config_cli_t * cli, mas_argvc_t * ptarg, mas_cargvc_t * pcarg, int pos )
{

  mas_del_argv( ptarg->argc, ptarg->argv, 0 );
  ptarg->argc = 0;
  ptarg->argv = NULL;

  ptarg->argc = mas_add_argv_argv( ptarg->argc, &ptarg->argv, pcarg->argc, pcarg->argv, pos );
  MAST_TRACE( options, 150, "@@@@@argc:%d", ptarg->argc );

/* targ.argv becomes valid here - may init pdi etc. */

  muc_ER( OPTIONS, argv_clarify, muc_config_cli_t * cli, mas_argvc_t * ptarg, mas_cargvc_t * pcarg, int pos );
}
