/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli)
#include "muc_tracen_defs_preset.h"
#include "muc_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

/* #include "muc_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "muc_errorn_defs.h"                                         (* DUF_NOERROR; MUC_CLEAR_ERROR; DUF_E_(LOWER|UPPER); MUC_TEST_R ... ♠ *) */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */
#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */

#include "muc_options.h"

/* ###################################################################### */
#include "muc_options_all_stages.h"
/* ###################################################################### */

muc_SR( TOP, treat_option_stage, muc_config_cli_t * cli, muc_option_stage_t istage, muc_errc_cs_func_t cb_init_loop_optstage,
    muc_int_void_func_t cb_do_interactive, muc_cpchar_void_func_t cb_prompt_interactive )
{
  assert( cli );
  MAST_TRACE( options, 0, "@@@@@before all options for %s stage;", muc_optstage_name( cli, istage ) );
/* TODO all (except INTERACTIVE) : call from paod, not real source */
  if ( istage == MUC_OPTION_STAGE_LOOP )
  {
    muc_option_stage_t istage_plus = istage;

    for ( int ia = muc_cli_options_get_targ_offset( cli ); QNOERR && ia < muc_cli_options_get_targc( cli ); ia++ )
    {
      const char *targia;

      targia = muc_cli_options_get_targi( cli, ia );

      MAST_TRACE( path, 0, "@%d/%d. %s", ia, muc_cli_options_get_targc( cli ), targia );

      if ( cb_init_loop_optstage )
        muc_CRV( cb_init_loop_optstage, targia );                        /* optstage_cb */

      muc_CR( all_options, cli, istage_plus, cb_do_interactive, cb_prompt_interactive, muc_cli_options_aod( cli ) /* paod */ ,
          ( istage_plus > MUC_OPTION_STAGE_BOOT ) /* from_paod */  );
      istage_plus++;
    }
  }
  else
  {
  /* TODO global_status.aod => global_cli_status.aod */
    muc_CR( all_options, cli, istage, cb_do_interactive, cb_prompt_interactive, muc_cli_options_aod( cli ) /* paod */ ,
        ( istage > MUC_OPTION_STAGE_BOOT ) /* from_paod */  );
  }
  MAST_TRACE( options, 0, "@@@@@after all options for %s stage;", muc_optstage_name( cli, istage ) );

  muc_ER( TOP, treat_option_stage, muc_config_cli_t * cli, muc_option_stage_t istage, muc_errc_cs_func_t cb_init_loop_optstage,
      muc_int_void_func_t cb_do_interactive, muc_cpchar_void_func_t cb_prompt_interactive );
}

muc_SR( TOP, treat_option_stage_ne, muc_config_cli_t * cli, muc_option_stage_t istage, muc_errc_cs_func_t cb_init_loop_optstage,
    muc_int_void_func_t cb_do_interactive, muc_cpchar_void_func_t cb_prompt_interactive )
{
  MASE_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );

  muc_CR( treat_option_stage, cli, istage, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );

  MASE_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );

  muc_ER( TOP, treat_option_stage_ne, muc_config_cli_t * cli, muc_option_stage_t istage, muc_errc_cs_func_t cb_init_loop_optstage,
      muc_int_void_func_t cb_do_interactive, muc_cpchar_void_func_t cb_prompt_interactive );
}

muc_SR( OPTIONS, treat_all_optstages, muc_config_cli_t * cli, muc_errc_cs_func_t cb_init_all_optstages, muc_errc_cs_func_t cb_init_loop_optstage,
    muc_int_void_func_t cb_do_interactive, muc_cpchar_void_func_t cb_prompt_interactive )
{
  MAST_TRACE( options, 0, "@@@@@to do all options for all stages (%p:%p)", ( void * ) &cb_init_all_optstages, ( void * ) &cb_init_loop_optstage );

  MAST_TRACE( options, 0, "@@I - stages from %s(presetup)", muc_optstage_name( cli, MUC_OPTION_STAGE_PRESETUP ) );
  muc_CR( treat_option_stage_ne, cli, MUC_OPTION_STAGE_PRESETUP, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );

  MAST_TRACE( options, 0, "@@II - stages from %s(setup)", muc_optstage_name( cli, MUC_OPTION_STAGE_SETUP ) );
  muc_CR( treat_option_stage_ne, cli, MUC_OPTION_STAGE_SETUP, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );
#if 0
  muc_CR( config_optionally_show );                                      /* FIXME similar to ..._show_options, called from ..._main_with_config after calling ..._main_db ??? FIXME */
#endif
#if 0
  muc_CR( pdi_create_global, "selected" );                               /* all_optstages_cb */
#else
  if ( cb_init_all_optstages )
    muc_CRV( cb_init_all_optstages, NULL );
#endif
/* XXX already there : this is reinit, after init! XXX */

  muc_CR( treat_option_stage, cli, MUC_OPTION_STAGE_INIT, NULL, cb_do_interactive, cb_prompt_interactive );

  MAST_TRACE( options, 0, "@@III %s - stages from first", QERRNAME );
#if 0
  if ( DUF_ACTG_FLAG( interactive ) )
#else
  if ( cb_do_interactive && cb_do_interactive(  ) )
#endif
  {
    muc_CR( treat_option_stage, cli, MUC_OPTION_STAGE_INTERACTIVE, NULL, cb_do_interactive, cb_prompt_interactive );
  }
  else
  {
    muc_CR( treat_option_stage, cli, MUC_OPTION_STAGE_FIRST, NULL, cb_do_interactive, cb_prompt_interactive );
    muc_CR( treat_option_stage, cli, MUC_OPTION_STAGE_LOOP, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );
  }
  MAST_TRACE( options, 0, "@@@@@after all options for all stages" );
  muc_ER( OPTIONS, treat_all_optstages, muc_config_cli_t * cli, muc_errc_cs_func_t cb_init_all_optstages, muc_errc_cs_func_t cb_init_loop_optstage,
      muc_int_void_func_t cb_do_interactive, muc_cpchar_void_func_t cb_prompt_interactive );
}
