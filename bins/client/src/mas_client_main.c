#include <mastar/wrap/mas_std_def.h>
/* #include "mas_client_def.h" */
/* #include "mas_basic_def.h"  */

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
  int r = -1;

  HMSG( "MAIN" );
#ifdef MAS_INIT_SEPARATE
  r = mas_init_client( argc, argv, env );
#else
  r = mas_init_plus( argc, argv, env, mas_client_init_readline, NULL );
#endif
  HMSG( "Inited (%d hosts)", opts.hosts_num );
  if ( r >= 0 )
    for ( int ia = opts.hosts_num; ia > 0; ia-- )
    {
      mas_client( opts.hosts[ia - 1] );
      break;
    }
  return 0;
}
