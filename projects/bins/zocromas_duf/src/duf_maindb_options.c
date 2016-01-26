#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config_defs.h"
#include "duf_config.h"

#include "duf_option_defs.h"
#include "duf_option_stage.h"
#include "duf_option_config.h"

#include "duf_options.h"

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_reinit.h"


/* ###################################################################### */
#include "duf_maindb_options.h"
/* ###################################################################### */

SR( OPTIONS, treat_all_optstages, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive )
{
  DUF_TRACE( options, 0, "@@@@@to do all options for all stages" );

  DUF_TRACE( options, 0, "@@I - stages from %s(presetup)", duf_optstage_name( DUF_OPTION_STAGE_PRESETUP ) );
  CR( treat_option_stage, DUF_OPTION_STAGE_PRESETUP, cb_do_interactive, cb_prompt_interactive );

  DUF_TRACE( options, 0, "@@II - stages from %s(setup)", duf_optstage_name( DUF_OPTION_STAGE_SETUP ) );
  CR( treat_option_stage, DUF_OPTION_STAGE_SETUP, cb_do_interactive, cb_prompt_interactive );
  CR( config_optionally_show ); /* FIXME similar to duf_show_options, called from duf_main_with_config after calling duf_main_db ??? FIXME */
  CR( pdi_init_at_config );
  DUF_TRACE( path, 0, "@@@path@pdi#FIRST: %s", duf_levinfo_path( DUF_CONFIGG( scn.pdi ) ) );

  DUF_TRACE( options, 0, "@@III %s - stages from first", QERRNAME );
  if ( DUF_ACTG_FLAG( interactive ) )
  {
    CR( treat_option_stage, DUF_OPTION_STAGE_INTERACTIVE, cb_do_interactive, cb_prompt_interactive );
  }
  else
  {
    CR( treat_option_stage, DUF_OPTION_STAGE_FIRST, cb_do_interactive, cb_prompt_interactive );
    CR( treat_option_stage, DUF_OPTION_STAGE_LOOP, cb_do_interactive, cb_prompt_interactive );
  }
  DUF_TRACE( options, 0, "@@@@@after all options for all stages" );

  ER( OPTIONS, treat_all_optstages, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
}

SR( OPTIONS, treat_option_stage, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive )
{
#if 0
  DOR_LOWERE( r, duf_all_options( istage, DUF_ACTG_FLAG( interactive ) ), DUF_ERROR_OPTION_NOT_FOUND );
#else
  DUF_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );
  DUF_TRACE( options, 0, "@@@@@before all options for %s stage;", duf_optstage_name( istage ) );
  if ( istage == DUF_OPTION_STAGE_LOOP )
  {
    for ( int ia = duf_cli_options_get_targ_offset(  ); QNOERR && ia < duf_cli_options_get_targc(  ); ia++ )
    {
      CR( pdi_reinit_anypath, DUF_CONFIGG( scn.pdi ), duf_cli_options_get_targi( ia ), ( duf_ufilter_t * ) NULL /* take pu from config */ ,
          NULL /* node_selector2 */ , 7 /* caninsert */ , DUF_UG_FLAG( recursive ), DUF_ACTG_FLAG( allow_dirs ),
          DUF_UG_FLAG( linear ) );
      DUF_TRACE( path, 0, "@@@@@@path@pdi#LOOP: %s", duf_levinfo_path( DUF_CONFIGG( scn.pdi ) ) );
      CR( all_options, istage, cb_do_interactive, cb_prompt_interactive );
    }
  }
  else
  {
    CR( all_options, istage, /* DUF_ACTG_FLAG( interactive ) , */ cb_do_interactive, cb_prompt_interactive );
  }
  DUF_TRACE( options, 0, "@@@@@after all options for %s stage;", duf_optstage_name( istage ) );
  DUF_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );
#endif

  ER( OPTIONS, treat_option_stage, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
}
