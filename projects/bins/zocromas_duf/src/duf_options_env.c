/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include <string.h>
#include <stdlib.h>                                                  /* getenv */

#include <mastar/wrap/mas_std_def.h>

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *)                   */

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
/* DUF_STARTR( r ); */

  const char *eo = NULL;

  eo = getenv( envvarname );

  DUF_TRACE( options, 1, "@@@@@@@stage:%s; source: env(%s='%s')", duf_optstage_name( cli, istage ), envvarname, eo );
  CR( boption_xclarify_at_stdx, cli, '=', istage, DUF_OPTION_SOURCE( ENV ), eo, 0, paod );

  DUF_TRACE( explain, 0, "got env options from %s", envvarname );

/* DUF_ENDR( r ); */
  ER( OPTIONS, env_options_at_var, duf_config_cli_t * cli, duf_option_stage_t istage, const char *envvarname, duf_option_adata_t * paod );
}

/*
 * get options/settings from OS environment
 *    - global variable duf_config must be created/inited
 * ****************************************
 * 1. set OS environment variable name
 * 2. call duf_env_options_at_var
 * */
SR( OPTIONS, source_env_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
    __attribute__ ( ( unused ) ), duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod,
    duf_option_source_code_t sourcecode MAS_UNUSED )
{
/* extern duf_config_t *duf_config; */
  const char *evname = NULL;

/* DUF_STARTR( r ); */
  DUF_TRACE( optsource, 0, "@   source:%s", duf_optsourcecode_name( cli, sourcecode ) );
  evname = duf_cli_options_config_env_var_name( cli );
#if 0
  CR( r, env_options_at_var, cli, istage, "MSH_DUF_AOPTIONS", paod );
  CR( r, env_options_at_var, cli, istage, "MSH_DUF_ZOPTIONS", paod );
  CR( r, env_options_at_var, cli, istage, "MSH_DUF_OPTIONS", paod );
#else
  CR( env_options_at_var, cli, istage, evname, paod );
#endif
/* DUF_ENDR( r ); */
  ER( OPTIONS, source_env_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
      __attribute__ ( ( unused ) ), duf_cpchar_void_func_t cb_prompt_interactive, duf_option_adata_t * paod, duf_option_source_code_t sourcecode );
}
