#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;

#include "mas_control.h"

/*
this:
  mas_control.c
related:
  mas_control_data.c
  mas_common.h
  mas_tools.c
  mas_opts.c
  mas_cs.c
  mas_cs.h

more:
  mas_rcontrol_object.c
*/

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;
  if ( !ctrl.msgfile )
    ctrl.msgfile = ctrl.stderrfile;
  /* fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno ); */
}

__attribute__ ( ( destructor ) )
     static void master_destructor( void )
{
  /* fprintf( stderr, "******************** DESTRUCTOR %s e%d\n", __FILE__, errno ); */
}

/*
 * */
int
mas_ctrl_init( mas_options_t * popts )
{
  /* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server */
  ctrl.in_client = 0;
  ctrl.keep_listening = 0;
  if ( ctrl.is_client )
    ctrl.in_client = 1;
  else
    ctrl.keep_listening = 1;

  ctrl.messages = !popts->nomessages;
  if ( !ctrl.msgfile )
    ctrl.msgfile = ctrl.stderrfile;

  ctrl.daemon = !popts->nodaemon;
  ctrl.ticker_mode = popts->ticker_mode;
  ctrl.redirect_std = !popts->noredirect_std;
  ctrl.close_std = !popts->noclose_std;
  return 0;
}

void
mas_ctrl_destroy( void )
{
  mas_del_argv( ctrl.commandsv.c, ctrl.commandsv.v, 0 );
  ctrl.commandsv.c = 0;
  ctrl.commandsv.v = NULL;

  mas_del_argv( ctrl.loaded_optsv.c, ctrl.loaded_optsv.v, 0 );
  ctrl.loaded_optsv.c = 0;
  ctrl.loaded_optsv.v = NULL;

  if ( ctrl.threads.n.daemon.pid && ctrl.threads.n.daemon.pid == getpid(  ) )
    for ( int ifil = 0; ifil < ctrl.pidfilesv.c; ifil++ )
    {
      /* HMSG( "PID FILE %d. %s", ifil, ctrl.pidfilesv.v[ifil] ); */
      unlink( ctrl.pidfilesv.v[ifil] );
    }

  mas_del_argv( ctrl.pidfilesv.c, ctrl.pidfilesv.v, 0 );
  ctrl.pidfilesv.c = 0;
  ctrl.pidfilesv.v = NULL;

  for ( int ipd = 0; ipd < ctrl.protos_num; ipd++ )
    if ( ctrl.proto_descs[ipd].name )
      mas_free( ctrl.proto_descs[ipd].name );
  if ( ctrl.proto_descs )
    mas_free( ctrl.proto_descs );
  ctrl.protos_num = 0;
  ctrl.proto_descs = NULL;
}

void
mas_ctrl_add_command( const char *s )
{
  /* const char *se;              */
  /*                              */
  /* se = mas_find_eq_value( s ); */
  ctrl.commandsv.c = mas_add_argv_arg( ctrl.commandsv.c, &ctrl.commandsv.v, s );
}
