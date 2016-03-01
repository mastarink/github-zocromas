/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include "duf_tracen_defs_preset.h"
#include "duf_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */
#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */

#include "duf_options.h"

/* ###################################################################### */
#include "duf_options_all_stages.h"
/* ###################################################################### */

SR( TOP, treat_option_stage, duf_config_cli_t * cli, duf_option_stage_t istage, duf_errc_cs_func_t cb_init_loop_optstage,
    duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive )
{
  assert( cli );
  MAST_TRACE( options, 0, "@@@@@before all options for %s stage;", duf_optstage_name( cli, istage ) );
/* TODO all (except INTERACTIVE) : call from paod, not real source */
  if ( istage == DUF_OPTION_STAGE_LOOP )
  {
    duf_option_stage_t istage_plus = istage;

    for ( int ia = duf_cli_options_get_targ_offset( cli ); QNOERR && ia < duf_cli_options_get_targc( cli ); ia++ )
    {
      const char *targia;

      targia = duf_cli_options_get_targi( cli, ia );

      MAST_TRACE( path, 0, "@%d/%d. %s", ia, duf_cli_options_get_targc( cli ), targia );

      if ( cb_init_loop_optstage )
        CRV( cb_init_loop_optstage, targia );                        /* optstage_cb */

      CR( all_options, cli, istage_plus, cb_do_interactive, cb_prompt_interactive, duf_cli_options_aod( cli ) /* paod */ ,
          ( istage_plus > DUF_OPTION_STAGE_BOOT ) /* from_paod */  );
      istage_plus++;
    }
  }
  else
  {
  /* TODO global_status.aod => global_cli_status.aod */
    CR( all_options, cli, istage, cb_do_interactive, cb_prompt_interactive, duf_cli_options_aod( cli ) /* paod */ ,
        ( istage > DUF_OPTION_STAGE_BOOT ) /* from_paod */  );
  }
  MAST_TRACE( options, 0, "@@@@@after all options for %s stage;", duf_optstage_name( cli, istage ) );

  ER( TOP, treat_option_stage, duf_config_cli_t * cli, duf_option_stage_t istage, duf_errc_cs_func_t cb_init_loop_optstage,
      duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
}

SR( TOP, treat_option_stage_ne, duf_config_cli_t * cli, duf_option_stage_t istage, duf_errc_cs_func_t cb_init_loop_optstage,
    duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive )
{
  MASE_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );

  CR( treat_option_stage, cli, istage, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );

  MASE_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );

  ER( TOP, treat_option_stage_ne, duf_config_cli_t * cli, duf_option_stage_t istage, duf_errc_cs_func_t cb_init_loop_optstage,
      duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
}

SR( OPTIONS, treat_all_optstages, duf_config_cli_t * cli, duf_errc_cs_func_t cb_init_all_optstages, duf_errc_cs_func_t cb_init_loop_optstage,
    duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive )
{
  MAST_TRACE( options, 0, "@@@@@to do all options for all stages (%p:%p)", ( void * ) &cb_init_all_optstages, ( void * ) &cb_init_loop_optstage );

  MAST_TRACE( options, 0, "@@I - stages from %s(presetup)", duf_optstage_name( cli, DUF_OPTION_STAGE_PRESETUP ) );
  CR( treat_option_stage_ne, cli, DUF_OPTION_STAGE_PRESETUP, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );

  MAST_TRACE( options, 0, "@@II - stages from %s(setup)", duf_optstage_name( cli, DUF_OPTION_STAGE_SETUP ) );
  CR( treat_option_stage_ne, cli, DUF_OPTION_STAGE_SETUP, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );
#if 0
  CR( config_optionally_show );                                      /* FIXME similar to ..._show_options, called from ..._main_with_config after calling ..._main_db ??? FIXME */
#endif
#if 0
  CR( pdi_create_global, "selected" );                               /* all_optstages_cb */
#else
  if ( cb_init_all_optstages )
    CRV( cb_init_all_optstages, NULL );
#endif
/* XXX already there : this is reinit, after init! XXX */

  CR( treat_option_stage, cli, DUF_OPTION_STAGE_INIT, NULL, cb_do_interactive, cb_prompt_interactive );

  MAST_TRACE( options, 0, "@@III %s - stages from first", QERRNAME );
#if 0
  if ( DUF_ACTG_FLAG( interactive ) )
#else
  if ( cb_do_interactive && cb_do_interactive(  ) )
#endif
  {
    CR( treat_option_stage, cli, DUF_OPTION_STAGE_INTERACTIVE, NULL, cb_do_interactive, cb_prompt_interactive );
  }
  else
  {
    CR( treat_option_stage, cli, DUF_OPTION_STAGE_FIRST, NULL, cb_do_interactive, cb_prompt_interactive );
    CR( treat_option_stage, cli, DUF_OPTION_STAGE_LOOP, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );
  }
  MAST_TRACE( options, 0, "@@@@@after all options for all stages" );
  ER( OPTIONS, treat_all_optstages, duf_config_cli_t * cli, duf_errc_cs_func_t cb_init_all_optstages, duf_errc_cs_func_t cb_init_loop_optstage,
      duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
}
