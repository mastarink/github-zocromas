/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include "duf_tracen_defs_preset.h"
#include "duf_errorn_defs_preset.h"

#include <string.h>
#include <stdlib.h>                                                  /* getenv */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */
#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */

#include "duf_option_clarify_batch.h"

/* ###################################################################### */
#include "duf_options_env.h"
/* ###################################################################### */

static
SR( OPTIONS, env_options_at_var, duf_config_cli_t * cli, duf_option_stage_t istage, const char *envvarname, duf_option_adata_t * paod )
{

  const char *eo = NULL;

  eo = getenv( envvarname );

  MAST_TRACE( options, 1, "@@@@@@@stage:%s; source: env(%s='%s')", duf_optstage_name( cli, istage ), envvarname, eo );
  CR( boption_xclarify_at_stdx, cli, '=', istage, DUF_OPTION_SOURCE( ENV ), eo, 0, paod );

  MAST_TRACE( explain, 0, "got env options from %s", envvarname );

  ER( OPTIONS, env_options_at_var, duf_config_cli_t * cli, duf_option_stage_t istage, const char *envvarname, duf_option_adata_t * paod );
}

/*
 * get options/settings from OS environment
 * ****************************************
 * 1. set OS environment variable name
 * 2. call ..._env_options_at_var
 * */
SR( OPTIONS, source_env_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
    __attribute__ ( ( unused ) ), duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod,
    duf_option_source_code_t sourcecode MAS_UNUSED )
{
  const char *evname = NULL;

  MAST_TRACE( optsource, 0, "@   source:%s", duf_optsourcecode_name( cli, sourcecode ) );
  evname = duf_cli_options_config_env_var_name( cli );
#if 0
  CR( r, env_options_at_var, cli, istage, "MSH_DUF_AOPTIONS", paod );
  CR( r, env_options_at_var, cli, istage, "MSH_DUF_ZOPTIONS", paod );
  CR( r, env_options_at_var, cli, istage, "MSH_DUF_OPTIONS", paod );
#else
  CR( env_options_at_var, cli, istage, evname, paod );
#endif

  ER( OPTIONS, source_env_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
      __attribute__ ( ( unused ) ), duf_cpchar_void_func_t cb_prompt_interactive, duf_option_adata_t * paod, duf_option_source_code_t sourcecode );
}
