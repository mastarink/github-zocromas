#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"

#include "duf_status_ref.h"


#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_trace.h"
#include "duf_config_defs.h"
#include "duf_config_show_etc.h"

#include "duf_option_defs.h"
#include "duf_option_stage.h"
#include "duf_option_config.h"

#include "duf_options.h"

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_global.h"
#include "duf_pdi_credel.h"
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
            duf_levinfo_path( duf_pdi_global(  ) ) );
  return rl_prompt;
}

SR( TOP, treat_option_stage, duf_option_stage_t istage )
{
  DUF_TRACE( options, 0, "@@@@@before all options for %s stage;", duf_optstage_name( istage ) );
  /* TODO all (except INTERACTIVE) : call from paod, not real source */
  if ( istage == DUF_OPTION_STAGE_LOOP )
  {
    duf_option_stage_t istage_plus = istage;

    for ( int ia = duf_cli_options_get_targ_offset(  ); QNOERR && ia < duf_cli_options_get_targc(  ); ia++ )
    {
      const char *targia;

      targia = duf_cli_options_get_targi( ia );
      duf_ufilter_t *pu = ( duf_ufilter_t * ) NULL; /* take pu from config */
      const duf_sql_set_t *sql_set = NULL;

      DUF_TRACE( path, 0, "@%d/%d. %s", ia, duf_cli_options_get_targc(  ), targia );
      CR( pdi_reinit_anypath, duf_pdi_global(  ), targia, pu, sql_set, 7 /* caninsert */ , DUF_UG_FLAG( recursive ),
          DUF_ACTG_FLAG( allow_dirs ), DUF_UG_FLAG( linear ) );
      DUF_TRACE( path, 0, "@@@@@@path@pdi#LOOP: %s", duf_levinfo_path( duf_pdi_global(  ) ) );
      CR( all_options, istage_plus, cb_do_interactive, cb_prompt_interactive, &global_status.aod /* paod */ ,
          ( istage_plus > DUF_OPTION_STAGE_BOOT ) /* from_paod */  );
      istage_plus++;
    }
  }
  else
  {
    CR( all_options, istage, cb_do_interactive, cb_prompt_interactive, &global_status.aod /* paod */ ,
        ( istage > DUF_OPTION_STAGE_BOOT ) /* from_paod */  );
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

  CR( pdi_create_global, "selected" );

  CR( treat_option_stage, DUF_OPTION_STAGE_INIT );

  DUF_TRACE( path, 0, "@@@path@pdi#FIRST: %s", duf_levinfo_path( duf_pdi_global(  ) ) );
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
  fputs( "\n", stderr );
  TT( "∈1∋ One " );
  TT( "∈2∋ Two " );
  TT( "∈1∋ Three " );
  TT( "∈3 Four " );
  TT( "∈1∋ Five " );
  TT( "∈4∋ Six " );
  TT( "∈1∋ Seven " );
  TT( "∈6 Eight " );
  TT( "∈1∋ Nine " );
  TT( "∈7∋ Ten " );
  T( "@act: %d; i: %d; no: %d;", DUF_ACTG_FLAG( testflag ), DUF_ACTG_FLAG( testiflag ), DUF_ACTG_FLAG( testnoflag ) );
  T( "@recetc: %d; i: %d; no: %d;", DUF_UG_FLAG( testflag ), DUF_UG_FLAG( testiflag ), DUF_UG_FLAG( testnoflag ) );
  T( "@disable: %d; i: %d; no: %d;", DUF_CONFIGG( opt.disable.flag.testflag ), DUF_CONFIGG( opt.disable.flag.testiflag ),
     DUF_CONFIGG( opt.disable.flag.testnoflag ) );
  T( "@test-num: %lu", DUF_CONFIGG( testnum ) );

//          opt   disable  testnoflag
#define SFLAG_SET(_styp, _prf, _loc, _fld )  ((duf_ ## _styp ## _ ## _prf ## _ ## _loc ## _flags_combo_t) {.flag._fld = 1 }).sbit
  {
    unsigned long long t1 = ( unsigned long long ) SFLAG_SET( config, opt, disable, testnoflag );
    unsigned long long t2 = ( unsigned long long ) ( ( duf_option_anyflag_t ) {.disable.testnoflag = 1 } ).sbit;
    T( "@>>>> %llx : %llx <<<<", t1, t2 );
  }
  ER( TOP, treat_all_optstages );
}
