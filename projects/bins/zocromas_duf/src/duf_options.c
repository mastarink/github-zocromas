#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_cli_options.h"

#include "duf_config_ref.h"


#include "duf_option_file.h"
#include "duf_option_extended.h"
#include "duf_option_env.h"
#include "duf_option_names.h"
#include "duf_option_restore.h"
#include "duf_option.h"

/* ###################################################################### */
#include "duf_options.h"
/* ###################################################################### */


int
duf_all_options( int argc, char *argv[] )
{
  int r = 0, er = 0, fr = 0, or = 0;

  if ( r >= 0 )
    er = r = duf_env_options( argc, argv );
  DUF_TRACE( options, 0, "got env options; er:%d (%c)  %s", er, er > ' ' && er < 'z' ? er : '-', duf_error_name( r ) );

  if ( r >= 0 )
    fr = r = duf_infile_options( argc, argv );
  DUF_TRACE( options, 0, "got infile options; fr:%d (%c)  %s", fr, fr > ' ' && fr < 'z' ? fr : '-', duf_error_name( r ) );

  if ( r >= 0 )
    or = r = duf_cli_options( argc, argv );
  DUF_TRACE( options, 0, "got cli options; or:%d (%c)  %s", or, or > ' ' && or < 'z' ? or : '-', duf_error_name( r ) );

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
  DUF_TRACE( explain, 2, "or: %d; fr: %d; er: %d; r: %s", or, fr, er, duf_error_name( r ) );
  return r;
}


int
duf_show_options( const char *a0 )
{
  int r = 0;
  int seq = 0;

  DUF_TRACE( options, 0, "%s", a0 );
  for ( duf_option_code_t codeval = DUF_OPTION_NONE; codeval < DUF_OPTION_MAX_LONG; codeval++ )
  {
#define BUFSZ 1024 * 4
    char buffer[BUFSZ] = "";

    duf_restore_some_option( buffer, codeval, BUFSZ );
    if ( *buffer )
    {
      seq++;
      DUF_TRACE( options, 0, "%2d. %s", seq, buffer );
    }
  }
  DUF_TRACE( options, 0, " --" );
  return r;
}
