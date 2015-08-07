#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_cli_options.h"

#include "duf_config_ref.h"

/* #include "duf_pdi.h" */
/* #include "duf_levinfo.h" */
/* #include "duf_levinfo_ref.h" */




#include "duf_option_file.h"
#include "duf_option_extended.h"
#include "duf_option_env.h"
#include "duf_option_names.h"
#include "duf_option_restore.h"
/* #include "duf_option.h" */

/* ###################################################################### */
#include "duf_options.h"
/* ###################################################################### */

/*
 * get get options/settings
 *    - global variable duf_config must be created/inited
 * ************************
 * 1. call duf_env_options, i.e. get options/settings from OS environment
 * 2. call duf_infile_options, i.e. get options/settings from configuration file(s)
 * 3. call duf_cli_options, i.e. get options/settings from command line
 * */
int
duf_all_options( int argc, char *argv[], duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  DUF_TRACE( temp, 0, "this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
#ifdef MAS_TRACING
  int er = 0, fr = 0, or = 0, ir = 0;
#else
  int DUF_UNUSED er = 0, fr = 0, or = 0, ir = 0;
#endif
  DEBUG_E_NO( DUF_ERROR_OPTION_NOT_FOUND );

  duf_config->carg.argc = argc;
  duf_config->carg.argv = argv;




  if ( !duf_config->cli.shorts )
    duf_config->cli.shorts = duf_cli_option_shorts( lo_extended_table_multi );


  if ( r >= 0 )
    er = r = duf_env_options( istage );
  DUF_TRACE( options, 0, "@got env options; er:%d (%c)  %s", er, er > ' ' && er < 'z' ? er : '-', duf_error_name( r ) );

#ifdef MAS_TRACING
  for ( int ia = 0; ia < duf_config->carg.argc; ia++ )
  {
    DUF_TRACE( temp, 2, "this is temp DUF_TRACE again - cargv[%d]='%s'", ia, duf_config->carg.argv[ia] );
  }
#endif


  if ( r >= 0 )
    fr = r = duf_infile_options( istage );
  DUF_TRACE( options, 0, "@got infile options; fr:%d (%c)  %s", fr, fr > ' ' && fr < 'z' ? fr : '-', duf_error_name( r ) );

  if ( r >= 0 )
    or = r = duf_cli_options( istage );
  DUF_TRACE( options, 0, "@got cli options; or:%d (%c)  %s", or, or > ' ' && or < 'z' ? or : '-', duf_error_name( r ) );

#if 0
/* duf_stdin_options - only for next stage, can be executed only once (direct stdin reading!)  */
  if ( r >= 0 )
    ir = r = duf_stdin_options( istage );
  DUF_TRACE( options, 0, "@got stdin options; or:%d (%c)  %s", ir, ir > ' ' && ir < 'z' ? ir : '-', duf_error_name( r ) );
#endif

#if 0
  if ( r >= 0 && duf_test_help( argc, argv, or ) < 0 && duf_test_help( argc, argv, fr ) < 0 && duf_test_help( argc, argv, er ) < 0 )
  {
    r = 1;
    if ( r == 0 && duf_config->db.dir )
    {
      DUF_TRACE( explain, 0, "to run main_db( argc, argv )" );
      r = 1;
    }
    else if ( r > 0 )
    {
      DUF_TRACE( explain, 1, "or: %d; fr: %d; er: %d; r: %d", or, fr, er, r );
      /* r=0; */
    }
  }
  else
  {
    r = 0;
  }
#endif
  DUF_TRACE( explain, 2, "or: %d; fr: %d; er: %d; ir: %d; r: %s", or, fr, er, ir, duf_error_name( r ) );
  DUF_TRACE( temp, 2, "@@@>>>>>>> targ_offset:%d", duf_config->targ_offset );
  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND );
}


int
duf_show_options( const char *a0 )
{
  DEBUG_STARTR( r );
  int oseq = 0;

  DUF_TRACE( options, 0, "%s", a0 );
  for ( duf_option_code_t codeval = DUF_OPTION_VAL_NONE; codeval < DUF_OPTION_VAL_MAX_LONG; codeval++ )
  {
#define BUFSZ 1024 * 4
    char buffer[BUFSZ] = "";

    duf_restore_some_option( buffer, codeval, BUFSZ );
    if ( *buffer )
    {
      oseq++;
      DUF_TRACE( options, 0, "%2d. %s", oseq, buffer );
    }
  }
  DUF_TRACE( options, 0, " --" );
  DEBUG_ENDR( r );
}
