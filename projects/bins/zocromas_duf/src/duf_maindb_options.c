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

#include "duf_option_clarify_new.h"
#include "duf_option_source.h"

/* ###################################################################### */
#include "duf_maindb_options.h"
/* ###################################################################### */

static int
cb_do_interactive( void )
{
  return DUF_ACTG_FLAG( interactive );
}

void
list_booted_option_source( duf_option_stage_t istage, duf_option_source_t source )
{
  if ( istage > DUF_OPTION_STAGE_BOOT )
  {
    const duf_option_adata_t *paod;

    paod = DUF_CONFIGA( aod );
    if ( duf_pod_source_count( paod, istage, source ) )
      T( "@@@@@@%s:%s:source_count: %lu", duf_optstage_name( istage ), duf_optsource_name( source ), duf_pod_source_count( paod, istage, source ) );
    for ( size_t npod = 0; npod < duf_pod_source_count( paod, istage, source ); npod++ )
    {
      const duf_option_data_t *pod DUF_UNUSED;

      pod = duf_pod_from_paod_n( paod, DUF_OPTION_STAGE_BOOT, source, npod );
      /* T( "@%lu. %p", npod, pod ); */
    }
  }
}

void
list_booted_option_stage( duf_option_stage_t istage )
{
  /* if ( istage > DUF_OPTION_STAGE_BOOT ) */
  {
    duf_option_source_t source = { 0 };
    for ( duf_option_source_code_t isource = DUF_OPTION_SOURCE_MIN; isource <= DUF_OPTION_SOURCE_MAX; isource++ )
    {
      source.sourcecode = isource;
      list_booted_option_source( istage, source );
    }
  }
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
  T( "@%s", "TODO all (except INTERACTIVE) : call from paod, not real source" );
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
      CR( all_options, istage, cb_do_interactive, cb_prompt_interactive, DUF_CONFIGA( aod ) /* paod */  );
    }
  }
  else
  {
    CR( all_options, istage, cb_do_interactive, cb_prompt_interactive, DUF_CONFIGA( aod ) /* paod */  );
  }
  DUF_TRACE( options, 0, "@@@@@after all options for %s stage;", duf_optstage_name( istage ) );
  /* T( "@@@@@############ ######### ######## aod:%lu : %lu", DUF_CONFIGG( aod.size ), DUF_CONFIGG( aod.count ) ); */

  list_booted_option_stage( istage );
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

  ER( TOP, treat_all_optstages );
}
