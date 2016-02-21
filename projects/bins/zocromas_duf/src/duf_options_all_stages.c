#undef MAS_TRACING
/* #include <stddef.h> */
#include <string.h>

#include "duf_maintenance_z.h"
#include "duf_maintenance_tracen.h"
#include "duf_maintenance_errors.h"

/* #include "duf_status_ref.h" */
#include "duf_config_util.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"
#include "duf_option_config.h"

#include "duf_options.h"

/* ###################################################################### */
#include "duf_options_all_stages.h"
/* ###################################################################### */

SR( TOP, treat_option_stage, duf_config_cli_t *cli, duf_option_stage_t istage, duf_errc_cs_func_t cb_init_loop_optstage, duf_int_void_func_t cb_do_interactive,
    duf_cpchar_void_func_t cb_prompt_interactive )
{
  DUF_TRACE( options, 0, "@@@@@before all options for %s stage;", duf_optstage_name( istage ) );
/* TODO all (except INTERACTIVE) : call from paod, not real source */
  if ( istage == DUF_OPTION_STAGE_LOOP )
  {
    duf_option_stage_t istage_plus = istage;

    for ( int ia = duf_cli_options_get_targ_offset( cli ); QNOERR && ia < duf_cli_options_get_targc( cli );
          ia++ )
    {
      const char *targia;

      targia = duf_cli_options_get_targi( cli, ia );

      DUF_TRACE( path, 0, "@%d/%d. %s", ia, duf_cli_options_get_targc( cli ), targia );

#if 0
      CR( pdi_reinit_anypath_global, targia );                       /* optstage_cb */
#else
      if ( cb_init_loop_optstage )
        CRV( cb_init_loop_optstage, targia );                        /* optstage_cb */
#endif
    /* DUF_TRACE( path, 0, "@@@@@@path@pdi#LOOP: %s", duf_levinfo_path( duf_pdi_global(  ) ) ); */
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
  DUF_TRACE( options, 0, "@@@@@after all options for %s stage;", duf_optstage_name( istage ) );

  ER( TOP, treat_option_stage, duf_config_cli_t *cli, duf_option_stage_t istage, duf_errc_cs_func_t cb_init_loop_optstage, duf_int_void_func_t cb_do_interactive,
      duf_cpchar_void_func_t cb_prompt_interactive );
}

SR( TOP, treat_option_stage_ne, duf_config_cli_t *cli, duf_option_stage_t istage, duf_errc_cs_func_t cb_init_loop_optstage, duf_int_void_func_t cb_do_interactive,
    duf_cpchar_void_func_t cb_prompt_interactive )
{
  DUF_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );

  CR( treat_option_stage,cli, istage, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );

  DUF_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );

  ER( TOP, treat_option_stage_ne, duf_config_cli_t *cli,duf_option_stage_t istage, duf_errc_cs_func_t cb_init_loop_optstage, duf_int_void_func_t cb_do_interactive,
      duf_cpchar_void_func_t cb_prompt_interactive );
}

SR( OPTIONS, treat_all_optstages,duf_config_cli_t *cli, duf_errc_cs_func_t cb_init_all_optstages, duf_errc_cs_func_t cb_init_loop_optstage,
    duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive )
{
  DUF_TRACE( options, 0, "@@@@@to do all options for all stages (%p:%p)", ( void * ) &cb_init_all_optstages, ( void * ) &cb_init_loop_optstage );

  DUF_TRACE( options, 0, "@@I - stages from %s(presetup)", duf_optstage_name( DUF_OPTION_STAGE_PRESETUP ) );
  CR( treat_option_stage_ne,cli, DUF_OPTION_STAGE_PRESETUP, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );

  DUF_TRACE( options, 0, "@@II - stages from %s(setup)", duf_optstage_name( DUF_OPTION_STAGE_SETUP ) );
  CR( treat_option_stage_ne, cli,DUF_OPTION_STAGE_SETUP, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );
#if 0
  CR( config_optionally_show );                                      /* FIXME similar to duf_show_options, called from duf_main_with_config after calling duf_main_db ??? FIXME */
#endif
#if 0
  CR( pdi_create_global, "selected" );                               /* all_optstages_cb */
#else
  CRV( cb_init_all_optstages, NULL );
#endif
/* XXX already there : this is reinit, after init! XXX */

  CR( treat_option_stage, cli, DUF_OPTION_STAGE_INIT, NULL, cb_do_interactive, cb_prompt_interactive );

/* DUF_TRACE( path, 0, "@@@path@pdi#FIRST: %s", duf_levinfo_path( duf_pdi_global(  ) ) ); */
  DUF_TRACE( options, 0, "@@III %s - stages from first", QERRNAME );
#if 0
  if ( DUF_ACTG_FLAG( interactive ) )
#else
  if ( cb_do_interactive(  ) )
#endif
  {
    CR( treat_option_stage, cli, DUF_OPTION_STAGE_INTERACTIVE, NULL, cb_do_interactive, cb_prompt_interactive );
  }
  else
  {
    CR( treat_option_stage, cli, DUF_OPTION_STAGE_FIRST, NULL, cb_do_interactive, cb_prompt_interactive );
    CR( treat_option_stage,cli,  DUF_OPTION_STAGE_LOOP, cb_init_loop_optstage, cb_do_interactive, cb_prompt_interactive );
  }
  DUF_TRACE( options, 0, "@@@@@after all options for all stages" );
  ER( OPTIONS, treat_all_optstages,duf_config_cli_t *cli, duf_errc_cs_func_t cb_init_all_optstages, duf_errc_cs_func_t cb_init_loop_optstage,
      duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
}
