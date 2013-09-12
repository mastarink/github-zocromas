#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>

#include <mastar/init/mas_sig.h>

#include <mastar/options/mas_cli_opts.h>
#include <mastar/options/mas_cli_opts_data.h>


#include <mastar/control/mas_control.h>


/* #include <mastar/init/mas_opts_common.h> */

#include <mastar/init/mas_init.h>
#include <mastar/init/mas_init_modules.h>

#include "mas_init_client.h"
#include "mas_client_readline.h"

#include "mas_client.h"


/*
this:
  mas_client_main.c
related:
  mas_client.c
  mas_client_session.c
  mas_client_readline.c
  mas_client_sig.c
  mas_server_main.c
more:
  mas_client.h
  mas_client_readline.h
  mas_client_session.h
  mas_cmdmod_client.c
  mas_init_client.c
  mas_init_client.h
  mas_listener_wait_client.c
  mas_listener_wait_client.h

*/


int
main( int argc, char *argv[], char *env[] )
{
  CTRL_PREPARE;
  /* extern mas_options_t g_opts; */
  extern mas_options_t *gpopts;
  int r = 0;

  WMSG( "MAIN" );
#ifndef MAS_CLIENT_LOG
  if ( ctrl.is_client )
    ctrl.log_disabled = 1;
#endif

  /* r = mas_init_plus( argc, argv, env, mas_client_init_readline, NULL ); */
  HMSG( "INIT CLIENT (%p:%p:%p)", ( void * ) ( unsigned long ) mas_init_sig,
        ( void * ) ( unsigned long ) mas_ctrl_init, ( void * ) ( unsigned long ) mas_client_init_readline );

  /* uuid BEFORE opt_files !! */
  IEVAL( r, mas_init_plus( gpopts, argc, argv, env, /* mas_init_proc, */ mas_init_uuid, mas_init_opt_files, mas_init_sig,
                           mas_cli_options_data_init, mas_cli_options_init, mas_ctrl_init,
                           mas_client_init_readline, mas_post_init, NULL ) );
  for ( int ia = gpopts->hostsv.c; r >= 0 && ia > 0; ia-- )
  {
    int maxit = 0;

    /* mas_client( gpopts-> hostsv.v[ia - 1] ); */
    if ( !( r < 0 ) )
      do
      {
        r = 0;
        IEVAL( r, mas_client( gpopts->hostsv.v[ia - 1] ) );
        if ( r < 0 && ctrl.restart_cnt > 0 )
        {
          HMSG( "RESTART %s DELAY %10.5fs", gpopts->argvv.v[0], gpopts->restart_sleep );
          mas_nanosleep( gpopts->restart_sleep );
        }
        maxit++;
      }
      while ( r < 0 && ctrl.restart_cnt && maxit < 25 );

    break;
  }
  return 0;
}
