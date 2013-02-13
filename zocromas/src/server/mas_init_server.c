#include "mas_server_def.h"
#include "mas_basic_def.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/modules/mas_modules_load_module.h>

#include "listener/inc/mas_listener_control_list.h"
#include "listener/inc/mas_listeners.h"

#include "init/inc/mas_opts.h"
#include "mas_ticker.h"

#include <mastar/msg/mas_curses.h>

#include <mastar/log/mas_logger.h>
#include <mastar/log/mas_log.h>

#include <mastar/thtools/mas_thread_tools.h>
#include "mas_init_threads.h"

#include "mas_init_server.h"

/*
this:
  mas_init_server.c
related:
  mas_init_client.c
  mas_init.c
  mas_init_threads.c
more:
  mas_init_client.h
  mas_init.h
  mas_init_server.h
  mas_init_threads.h


*/

static int
mas_init_load_protos( void )
{
  int protos_num = 0;
  mas_transaction_protodesc_t *proto_descs;

  tMSG( "@@@@@@@@@@ protodir:%s [%u]", opts.protodir, opts.protos_num );
  proto_descs = mas_calloc( opts.protos_num, sizeof( mas_transaction_protodesc_t ) );
  for ( int ipr = 0; ipr < opts.protos_num; ipr++ )
  {
    /* from one */
    proto_descs[ipr].proto_id = protos_num + 1;
    proto_descs[ipr].name = mas_strdup( opts.protos[ipr] );
    proto_descs[ipr].function = mas_modules_load_proto_func( opts.protos[ipr], "mas_proto_main" );
    tMSG( "@@@@@@@@@@ %u proto:%s : %p", ipr, opts.protos[ipr], ( void * ) ( unsigned long long ) proto_descs[ipr].function );
    protos_num++;
  }
  ctrl.protos_num = protos_num;
  ctrl.proto_descs = proto_descs;
  return proto_descs ? 0 : -1;
}

/*
Creating a daemon
   1 Create a normal process (Parent process)
   2 Create a child process from within the above parent process
   3 The process hierarchy at this stage looks like :  TERMINAL -> PARENT PROCESS -> CHILD PROCESS
   4 Terminate the the parent process.
   5 The child process now becomes orphan and is taken over by the init process.
   6 Call setsid() function to run the process in new session and have a new group.
   7 After the above step we can say that now this process becomes a daemon process without having a controlling terminal.
   8 Change the working directory of the daemon process to root and close stdin, stdout and stderr file descriptors.
   9 Let the main logic of daemon process run.
*/
static int
mas_init_daemon( void )
{
  int r = 0;
  pid_t pid_child;

  HMSG( "DAEMONIZE" );
  pid_child = mas_fork(  );
  if ( pid_child == 0 )
  {
    ctrl.child_pid = getpid(  );
    if ( opts.msgfilename )
      mas_msg_set_file( opts.msgfilename );
    HMSG( "CHILD : %u @ %u @ %u - %s : %d", pid_child, getpid(  ), getppid(  ), opts.msgfilename, ctrl.msgfile ? 1 : 0 );
    /* sleep(200); */
    if ( ctrl.redirect_std )
    {
      int foutd = -1;
      int ferrd = -1;

      foutd = mas_open( "daemon_stdout.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0777 );
      ferrd = mas_open( "daemon_stderr.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0777 );
      ctrl.saved_stderr = dup( STDERR_FILENO );
      ctrl.saved_stderr_file = fdopen( ctrl.saved_stderr, "w" );
      ctrl.saved_stdout = dup( STDOUT_FILENO );
      dup2( foutd, STDOUT_FILENO );
      dup2( ferrd, STDERR_FILENO );
      mas_close( foutd );
      mas_close( ferrd );
    }
    if ( ctrl.close_std && ctrl.daemon )
    {
      mas_close( STDIN_FILENO );
      mas_close( STDOUT_FILENO );
      mas_close( STDERR_FILENO );
    }
    /* mas_destroy_server(  ); */
  }
  else if ( pid_child > 0 )
  {
    ctrl.child_pid = pid_child;
    HMSG( "PARENT : %u @ %u @ %u", pid_child, getpid(  ), getppid(  ) );
    r = -2;
  }
  else
  {
    r = -1;
  }
  return r;
}

int
mas_init_server( void ( *atexit_fun ) ( void ), int initsig, int argc, char **argv, char **env )
{
  int r = 0;
  int k;

  ctrl.status = MAS_STATUS_START;
  ctrl.start_time = mas_double_time(  );
  /* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server */
  ctrl.is_client = 0;
  ctrl.is_server = 1;
  mas_pre_init( argc, argv, env );

  MAS_LOG( "init server" );
  HMSG( "PPID: %u", getppid(  ) );
  HMSG( "BASH: %s", getenv( "MAS_PID_AT_BASHRC" ) );
  MAS_LOG( "PPID: %u BASH: %s", getppid(  ), getenv( "MAS_PID_AT_BASHRC" ) );
  k = mas_opts_restore_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL );
  if ( k <= 0 )
    mas_opts_restore( NULL, ctrl.progname ? ctrl.progname : "Unknown" );
  /* r = mas_init_curses(  ); */
  if ( r < 0 && use_curses )
  {
    mas_close_curses(  );
    fprintf( stderr, "ERROR curses %d\n", r );
    /* exit( 33 ); */
  }
  if ( r >= 0 )
    r = mas_init( atexit_fun, initsig, argc, argv, env );
  /* mMSG( "ARGV_NONOPTIND :%d", ctrl.argv_nonoptind ); */
  /* HMSG( "DAEMON -%d +%d", opts.nodaemon, ctrl.daemon ); */
  if ( r >= 0 && ctrl.daemon )
    r = mas_init_daemon(  );

  if ( r >= 0 )
    r = mas_threads_init(  );
  if ( r >= 0 && !ctrl.proto_descs )
    r = mas_init_load_protos(  );

  if ( r >= 0 )
    mas_lcontrols_list_create(  );
  MAS_LOG( "init server done" );
  if ( r >= 0 )
    r = mas_post_init( argc, argv, env );
  return r;
}

void
mas_destroy_server( void )
{
  MAS_LOG( "destroy server" );
  MAS_LOG( "to save opts" );
  if ( !ctrl.opts_saved )
    mas_opts_save( NULL, ctrl.progname ? ctrl.progname : "Unknown" );
  if ( !ctrl.opts_saved_plus )
    mas_opts_save_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL );

  if ( ctrl.lcontrols_list )
  {
    MAS_LOG( "to cancel listeners" );
    FMSG( "TO CANCEL LISTENERS" );
    mas_listeners_cancel(  );
  }
  /* mas_channel_deaf( &ctrl, ctrl.pchannel ); */

  if ( use_curses )
    sleep( 1 );
  mas_close_curses(  );

  mas_lcontrols_delete_list(  );
  mas_in_thread_end(  );
  mas_threads_destroy(  );
  mas_destroy(  );
  MAS_LOG( "to cancel ticker" );
  MAS_LOG( "to cancel logger" );
  if ( ctrl.logger_thread )
  {
    FMSG( "TO STOP LOGGER" );
    mas_logger_stop(  );
  }
  if ( ctrl.ticker_thread )
  {
    FMSG( "TO STOP TICKER" );
    mas_ticker_stop(  );
  }
  MAS_LOG( "destroy server done" );
  FMSG( "DESTROY SERVER DONE" );
}
