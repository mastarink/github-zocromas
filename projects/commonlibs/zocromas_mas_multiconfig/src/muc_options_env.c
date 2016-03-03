/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include "muc_errorn_defs_preset.h"

#include <string.h>
#include <stdlib.h>                                                  /* getenv */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */
#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

#include "muc_option_clarify_batch.h"

/* ###################################################################### */
#include "muc_options_env.h"
/* ###################################################################### */

static
muc_SR( OPTIONS, env_options_at_var, muc_config_cli_t * cli, muc_option_stage_t istage, const char *envvarname, muc_option_adata_t * paod )
{

  const char *eo = NULL;

  eo = getenv( envvarname );

  MAST_TRACE( options, 1, "@@@@@@@stage:%s; source: env(%s='%s')", muc_optstage_name( cli, istage ), envvarname, eo );
  muc_CR( boption_xclarify_at_stdx, cli, '=', istage, MUC_OPTION_SOURCE( ENV ), eo, 0, paod );

  MAST_TRACE( explain, 0, "got env options from %s", envvarname );

  muc_ER( OPTIONS, env_options_at_var, muc_config_cli_t * cli, muc_option_stage_t istage, const char *envvarname, muc_option_adata_t * paod );
}

/*
 * get options/settings from OS environment
 * ****************************************
 * 1. set OS environment variable name
 * 2. call ..._env_options_at_var
 * */
muc_SR( OPTIONS, source_env_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive
        __attribute__ ( ( unused ) ), muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod,
        muc_option_source_code_t sourcecode MAS_UNUSED )
{
  const char *evname = NULL;

  MAST_TRACE( optsource, 0, "@   source:%s", muc_optsourcecode_name( cli, sourcecode ) );
  evname = muc_cli_options_config_env_var_name( cli );
#if 0
  muc_CR( r, env_options_at_var, cli, istage, "MSH_DUF_AOPTIONS", paod );
  muc_CR( r, env_options_at_var, cli, istage, "MSH_DUF_ZOPTIONS", paod );
  muc_CR( r, env_options_at_var, cli, istage, "MSH_DUF_OPTIONS", paod );
#else
  muc_CR( env_options_at_var, cli, istage, evname, paod );
#endif

  muc_ER( OPTIONS, source_env_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive
          __attribute__ ( ( unused ) ), muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod,
          muc_option_source_code_t sourcecode );
}
