#include <mastar/wrap/mas_std_def.h>

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/prctl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>


#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifndef MAS_SERVER_NOLOG
#  include <mastar/log/mas_logger.h>
#  include <mastar/log/mas_log.h>
#endif

#include <mastar/modules/mas_modules_load_module.h>

#include <mastar/listener/mas_listener_control_list.h>
#include <mastar/listener/mas_listeners.h>

#ifdef MAS_USE_CURSES
#  include <mastar/msg/mas_curses.h>
#endif

#include <mastar/thtools/mas_thread_tools.h>
#include "mas_init_threads.h"
#include "mas_ticker.h"

#include <mastar/init/mas_init.h>
#include <mastar/init/mas_opts.h>

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

int
mas_init_load_protos( void )
{
  int r = 0;
  int protos_num = 0;
  mas_transaction_protodesc_t *proto_descs = NULL;

  HMSG( "INIT PROTOS" );
  MAS_LOG( "(%d) init / load protos", r );
  if ( !ctrl.proto_descs )
  {
    proto_descs = mas_calloc( opts.protos_num, sizeof( mas_transaction_protodesc_t ) );
    for ( int ipr = 0; ipr < opts.protos_num; ipr++ )
    {
      /* from one */
      proto_descs[ipr].proto_id = protos_num + 1;
      proto_descs[ipr].name = mas_strdup( opts.protos[ipr] );
      proto_descs[ipr].function = mas_modules_load_proto_func( opts.protos[ipr], "mas_proto_main" );
      if ( !proto_descs[ipr].function )
      {
        EMSG( "PROTO LOAD %s FAIL", proto_descs[ipr].name );
      }
      protos_num++;
      MAS_LOG( "(%d) init / load protos #%d", r, protos_num );
    }
    ctrl.protos_num = protos_num;
    ctrl.proto_descs = proto_descs;
  }
  r = ctrl.proto_descs ? 0 : -1;
  MAS_LOG( "(%d) init / load protos done", r );
  return r;
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
mas_init_pid( int indx, const char *name )
{
  int r = -1;

  if ( name && *name && indx < MAS_MAX_PIDFD )
  {
    char *pidpath;

    pidpath = mas_strdup( opts.pidsdir );
    pidpath = mas_strcat_x( pidpath, name );
    HMSG( "PIDPATH: %s", pidpath );
    ctrl.pidfd[indx] = open( pidpath, O_CREAT | O_WRONLY | O_TRUNC /* | O_EXCL */ , S_IWUSR | S_IRUSR );
    if ( ctrl.pidfd[indx] > 0 )
    {
      int w, lck;

      lck = lockf( ctrl.pidfd[indx], F_TLOCK, 0 );
      HMSG( "PIDLCK: %d (%d)", lck, ctrl.pidfd[indx] );
      /* setvbuf( ctrl.pidfile, NULL, _IONBF, 0 ); */
      if ( lck >= 0 )
      {
        /* w = fwrite( pidpath, 1, strlen( pidpath ), f ); */
        /* w = fprintf( f, "%u", getpid(  ) ); */
        w = write( ctrl.pidfd[indx], &ctrl.main_pid, sizeof( ctrl.main_pid ) );
        HMSG( "PIDW: %d", w );
        if ( w > 0 )
          r = 0;
      }
      else
      {
        P_ERR;
        close( ctrl.pidfd[indx] );
        ctrl.pidfd[indx] = -1;
      }
    }
    else
    {
      P_ERR;
    }
    mas_free( pidpath );
  }
  HMSG( "(%d) INIT %s", r, __func__ );
  return r;
}

int
mas_init_pids( void )
{
  int r = 0;
  char *namebuf = NULL;

  ctrl.server_pid = getpid(  );
  ctrl.server_tid = mas_gettid(  );
  ctrl.server_thread = mas_pthread_self(  );

  namebuf = mas_malloc( 512 );
  MAS_LOG( "(%d) init pids", r );
  if ( namebuf )
  {
    int indx = -1;

    *namebuf = 0;
    HMSG( "PIDSDIR: %s", opts.pidsdir );
    if ( opts.single_instance && opts.pidsdir )
    {
      snprintf( namebuf, sizeof( namebuf ), "/%s.pid", ctrl.is_client ? "client" : "server" );
      indx = 0;
    }
    else if ( opts.single_child && opts.pidsdir )
    {
      snprintf( namebuf, sizeof( namebuf ), "/%s.%u.pid", ctrl.is_client ? "client" : "server", getppid(  ) );
      indx = 1;
    }
    if ( indx >= 0 )
    {
      if ( *namebuf )
        r = mas_init_pid( indx, namebuf );
      else
        r = -1;
    }
    mas_free( namebuf );
  }
  MAS_LOG( "(%d) init pids done", r );
  HMSG( "(%d) INIT %s", r, __func__ );
  return r;
}

int
mas_init_daemon( void )
{
  int r = 0;
  pid_t pid_child;

  HMSG( "INIT DAEMON" );
  MAS_LOG( "(%d) init daemon", r );
  if ( ctrl.daemon )
  {
    HMSG( "DAEMONIZE" );
    MAS_LOG( "(%d) init daemonize", r );
    pid_child = mas_fork(  );
    MAS_LOG( "(%d) init fork", r );
    if ( pid_child == 0 )
    {
      ctrl.child_pid = getpid(  );
      ctrl.child_tid = mas_gettid(  );
      ctrl.child_thread = mas_pthread_self(  );
      ctrl.server_pid = getpid(  );
      ctrl.server_tid = mas_gettid(  );
      ctrl.server_thread = mas_pthread_self(  );

      if ( prctl( PR_SET_NAME, ( unsigned long ) "zocchild" ) < 0 )
      {
        P_ERR;
      }

      for ( int i = 0; i < MAS_MAX_PIDFD; i++ )
      {
        if ( ctrl.pidfd[i] > 0 )
        {
          int lck;

          lck = lockf( ctrl.pidfd[i], F_LOCK, 0 );
          MAS_LOG( "(%d) init daemon; lock", r );
          HMSG( "PIDLCK+: %d (%d)", lck, ctrl.pidfd[i] );
        }
      }
      HMSG( "CHILD : %u @ %u @ %u - %s : %d", pid_child, getpid(  ), getppid(  ), opts.msgfilename, ctrl.msgfile ? 1 : 0 );
      /* sleep(200); */
      if ( ctrl.redirect_std )
      {
        int foutd = -1;
        int ferrd = -1;

        foutd = mas_open( "daemon_stdout.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0777 );
        MAS_LOG( "(%d) init daemon; open foutd", r );
        ferrd = mas_open( "daemon_stderr.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0777 );
        MAS_LOG( "(%d) init daemon; open ferrd", r );
        ctrl.old_stderr = dup( STDERR_FILENO );
        ctrl.old_stderrfile = fdopen( ctrl.old_stderr, "w" );
        setvbuf( ctrl.old_stderrfile, NULL, _IONBF, 0 );
        MAS_LOG( "(%d) init daemon; fdopen", r );
        ctrl.old_stdout = dup( STDOUT_FILENO );
        MAS_LOG( "(%d) init daemon; dup", r );
        dup2( foutd, STDOUT_FILENO );
        MAS_LOG( "(%d) init daemon; dup2 / out", r );
        dup2( ferrd, STDERR_FILENO );
        MAS_LOG( "(%d) init daemon; dup2 / err", r );
        mas_close( foutd );
        MAS_LOG( "(%d) init daemon; close foutd", r );
        mas_close( ferrd );
        MAS_LOG( "(%d) init daemon; close ferrd", r );
        setvbuf( stderr, NULL, _IONBF, 0 );
        MFP( "MFP to redirected stderr\n" );
        fprintf( stderr, "fprintf to redirected stderr\n" );
      }
      if ( ctrl.close_std && ctrl.daemon )
      {
        int same;
/* Instead of close:                        */
/* int tmpfd = open("/dev/null", O_WRONLY); */
/* dup2(tmpfd, 2);                          */
/* close(tmpfd);                            */
        same = ( ctrl.stderrfile == ctrl.msgfile );
        mas_close( STDIN_FILENO );
        MAS_LOG( "(%d) init daemon; close STDIN", r );
        mas_close( STDOUT_FILENO );
        MAS_LOG( "(%d) init daemon; close STDOUT", r );
        MAS_LOG( "(%d) init to close stderr", r );
        mas_close( STDERR_FILENO );
        ctrl.stderrfile = NULL;
        if ( same )
        {
          ctrl.msgfile = ctrl.old_stderrfile;
          MAS_LOG( "(%d) init daemon; same msgfile!!", r );
        }
        MAS_LOG( "(%d) init daemon; close STDERR", r );
      }
      /* mas_destroy_server(  ); */
      setsid(  );
    }
    else if ( pid_child > 0 )
    {
      ctrl.child_pid = pid_child;
      HMSG( "PARENT : %u @ %u @ %u", pid_child, getpid(  ), getppid(  ) );
      ctrl.is_parent = 1;
    }
    else
    {
      r = -1;
    }
  }
  MAS_LOG( "(%d) init daemon almost done", r );
  HMSG( "(%d) INIT %s", r, __func__ );
  MAS_LOG( "(%d) init daemon done", r );
  return r;
}

#ifdef MAS_INIT_SEPARATE
int
mas_init_server( void ( *atexit_fun ) ( void ), int initsig, int argc, char **argv, char **env )
{
  int r = 0;

  HMSG( "INIT SERVER" );
  ctrl.status = MAS_STATUS_START;
  ctrl.start_time = mas_double_time(  );
  
  ctrl.server_pid = getpid(  );
  ctrl.server_tid = mas_gettid(  );
  ctrl.server_thread = mas_pthread_self(  );

#  ifdef MAS_SERVER_NOLOG
  ctrl.log_disabled = 1;
#  endif
  /* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server */
  ctrl.is_client = 0;
  ctrl.is_server = 1;
  r = mas_pre_init( argc, argv, env );
  HMSG( "(%d) INIT %s", r, __func__ );

  MAS_LOG( "init server" );
#  ifdef MAS_USE_CURSES
  /* if ( r >= 0 )              */
  /*   r = mas_init_curses(  ); */
#  endif
  if ( r >= 0 )
    r = mas_init( atexit_fun, initsig, argc, argv, env );
  HMSG( "(%d) INIT %s", r, __func__ );
  if ( r >= 0 )
    r = mas_init_daemon(  );
  HMSG( "(%d) INIT %s", r, __func__ );
  /* if ( ctrl.is_parent )       */
  /* {                           */
  /*   HMSG( "PARENT to exit" ); */
  /* }                           */
  /* else                        */
  {
    MAS_LOG( "(%d) init server: to init threads", r );
    if ( r >= 0 )
      r = mas_threads_init(  );
    HMSG( "(%d) INIT %s", r, __func__ );
    MAS_LOG( "(%d) init server: to load protos", r );
    if ( r >= 0 )
      r = mas_init_load_protos(  );
    HMSG( "(%d) INIT %s", r, __func__ );
    MAS_LOG( "(%d) init server: to create lcontrols", r );
    if ( r >= 0 )
      mas_lcontrols_list_create(  );
    HMSG( "(%d) INIT %s", r, __func__ );
    MAS_LOG( "init server done" );
    MAS_LOG( "(%d) init server: to post-init", r );
    if ( r >= 0 )
      r = mas_post_init(  );
    HMSG( "(%d) INIT %s", r, __func__ );
    MAS_LOG( "(%d) end init server", r );
  }
  return r;
}
#endif
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

#ifdef MAS_USE_CURSES
  if ( use_curses )
    sleep( 1 );
  mas_close_curses(  );
#endif
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
  mas_modules_destroy(  );
  {
    for ( int i = 0; i < MAS_MAX_PIDFD; i++ )
    {
      if ( ctrl.pidfd[i] > 0 )
      {
        close( ctrl.pidfd[i] );
      }
      ctrl.pidfd[i] = 0;
    }
  }

  MAS_LOG( "destroy server done" );
  FMSG( "DESTROY SERVER DONE" );
}
