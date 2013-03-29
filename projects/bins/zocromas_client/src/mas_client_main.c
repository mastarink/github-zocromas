#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/init/mas_init.h>
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
  int r = 0;

  WMSG( "MAIN" );
#ifndef MAS_CLIENT_LOG
  if ( ctrl.is_client )
    ctrl.log_disabled = 1;
#endif

#ifdef MAS_INIT_SEPARATE
  /* r = mas_init_client( argc, argv, env ); */
  IEVAL( r, mas_init_client( argc, argv, env ) );
#else
  /* r = mas_init_plus( argc, argv, env, mas_client_init_readline, NULL ); */
  IEVAL( r, mas_init_plus( argc, argv, env, mas_client_init_readline, NULL ) );
#endif
  for ( int ia = opts.hostsv.c; r >= 0 && ia > 0; ia-- )
  {
      int maxit = 0;
    /* mas_client( opts.hostsv.v[ia - 1] ); */
    if (!(r<0))do
    {
      r = 0;
      IEVAL( r, mas_client( opts.hostsv.v[ia - 1] ) );
      if ( r < 0 && ctrl.restart_cnt > 0 )
      {
        HMSG( "RESTART %s DELAY %10.5fs", opts.argvv.v[0], opts.restart_sleep );
        mas_nanosleep( opts.restart_sleep );
      }
      maxit++;
    }
    while ( r < 0 && ctrl.restart_cnt && maxit < 25 );

    break;
  }
  return 0;
}
