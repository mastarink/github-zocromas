#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_defs.h"
#include "duf_config_show_etc.h"

#include "duf_option_defs.h"
#include "duf_option_stage.h"
#include "duf_option_config.h"

#include "duf_options.h"

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_reinit.h"

/* #include "duf_option_clarify_new.h" */
#include "duf_option_source.h"

/* ###################################################################### */
#include "duf_maindb_options.h"
/* ###################################################################### */

static int
cb_do_interactive( void )
{
  return DUF_ACTG_FLAG( interactive );
}


static const char *
cb_prompt_interactive( void )
{
  static char rl_prompt[256 * 10] = "";

  snprintf( rl_prompt, sizeof( rl_prompt ), "A-F:%d;A-D:%d; %s:%s> ", DUF_ACTG_FLAG( allow_files ), DUF_ACTG_FLAG( allow_dirs ), "db",
            duf_levinfo_path( DUF_CONFIGG( scn.pdi ) ) );
  return rl_prompt;
}

SR( TOP, treat_option_stage, duf_option_stage_t istage )
{
  DUF_TRACE( options, 0, "@@@@@before all options for %s stage;", duf_optstage_name( istage ) );
  /* TODO all (except INTERACTIVE) : call from paod, not real source */
  if ( istage == DUF_OPTION_STAGE_LOOP )
  {
    for ( int ia = duf_cli_options_get_targ_offset(  ); QNOERR && ia < duf_cli_options_get_targc(  ); ia++ )
    {
      const char *targia;

      targia = duf_cli_options_get_targi( ia );
      duf_ufilter_t *pu = ( duf_ufilter_t * ) NULL; /* take pu from config */
      const duf_sql_set_t *sql_set = NULL;

      CR( pdi_reinit_anypath, DUF_CONFIGG( scn.pdi ), targia, pu, sql_set, 7 /* caninsert */ , DUF_UG_FLAG( recursive ),
          DUF_ACTG_FLAG( allow_dirs ), DUF_UG_FLAG( linear ) );
      DUF_TRACE( path, 0, "@@@@@@path@pdi#LOOP: %s", duf_levinfo_path( DUF_CONFIGG( scn.pdi ) ) );
      CR( all_options, istage, cb_do_interactive, cb_prompt_interactive, DUF_CONFIGA( aod ) /* paod */ ,
           (istage > DUF_OPTION_STAGE_BOOT) /* from_paod */  );
    }
  }
  else
  {
    CR( all_options, istage, cb_do_interactive, cb_prompt_interactive, DUF_CONFIGA( aod ) /* paod */ ,
         (istage > DUF_OPTION_STAGE_BOOT) /* from_paod */  );
  }
  DUF_TRACE( options, 0, "@@@@@after all options for %s stage;", duf_optstage_name( istage ) );
  /* T( "@@@@@############ ######### ######## aod:%lu : %lu", DUF_CONFIGG( aod.size ), DUF_CONFIGG( aod.count ) ); */

  ER( TOP, treat_option_stage, duf_option_stage_t istage );
}

SR( TOP, treat_option_stage_ne, duf_option_stage_t istage )
{
  DUF_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );

  CR( treat_option_stage, istage );

  DUF_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );

  ER( TOP, treat_option_stage_ne, duf_option_stage_t istage );
}

SR( TOP, treat_all_optstages )
{
  DUF_TRACE( options, 0, "@@@@@to do all options for all stages" );

  DUF_TRACE( options, 0, "@@I - stages from %s(presetup)", duf_optstage_name( DUF_OPTION_STAGE_PRESETUP ) );
  CR( treat_option_stage_ne, DUF_OPTION_STAGE_PRESETUP );

  DUF_TRACE( options, 0, "@@II - stages from %s(setup)", duf_optstage_name( DUF_OPTION_STAGE_SETUP ) );
  CR( treat_option_stage_ne, DUF_OPTION_STAGE_SETUP );

  CR( config_optionally_show ); /* FIXME similar to duf_show_options, called from duf_main_with_config after calling duf_main_db ??? FIXME */
  CR( pdi_init_at_config );
  DUF_TRACE( path, 0, "@@@path@pdi#FIRST: %s", duf_levinfo_path( DUF_CONFIGG( scn.pdi ) ) );

  DUF_TRACE( options, 0, "@@III %s - stages from first", QERRNAME );
  if ( DUF_ACTG_FLAG( interactive ) )
  {
    CR( treat_option_stage, DUF_OPTION_STAGE_INTERACTIVE );
  }
  else
  {
    CR( treat_option_stage, DUF_OPTION_STAGE_FIRST );
    CR( treat_option_stage, DUF_OPTION_STAGE_LOOP );
  }
  DUF_TRACE( options, 0, "@@@@@after all options for all stages" );

  TT("∈1∋ One ");
  TT("∈2∋ Two ");
  TT("∈1∋ Three ");
  TT("∈3 Four ");
  TT("∈1∋ Five ");
  TT("∈4∋ Six ");
  TT("∈1∋ Seven ");
  TT("∈6 Eight ");
  TT("∈1∋ Nine ");
  TT("∈7∋ Ten ");
  ER( TOP, treat_all_optstages );
}
