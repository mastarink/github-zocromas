#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


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

  MAS_LOG( "(%d) init / load protos", r );
  if ( !opts.protos_num )
  {
    HMSG("NO PROTCOLOS DEFINED");
    WMSG("no protcolos defined");
    IEVAL( r, -1 );
  }
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
        IEVAL( r, -1 );
        WMSG( "INIT PROTOS - #%d: %s", ipr, opts.protos[ipr] );
      }
      else
      {
        WMSG( "INIT PROTOS + #%d: %s", ipr, opts.protos[ipr] );
      }
      protos_num++;
      MAS_LOG( "(%d) init / load protos #%d", r, protos_num );
    }
    ctrl.protos_num = protos_num;
    ctrl.proto_descs = proto_descs;
    if ( opts.protos_num && !ctrl.protos_num )
    {
      IEVAL( r, -1 );
    }
  }
  else
  {
    IEVAL( r, -1 );
  }
  /* r = ctrl.proto_descs ? 0 : -1; */
  IEVAL( r, ctrl.proto_descs ? 0 : -1 );
  MAS_LOG( "(%d) init / load protos done", r );
  HMSG( "(%d) INIT S PROTOS %d of %d", r, protos_num, opts.protos_num );
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
  int r = 0;

  if ( name && *name && indx < MAS_MAX_PIDFD )
  {
    char *pidpath;

    pidpath = mas_strdup( opts.pidsdir );
    pidpath = mas_strcat_x( pidpath, name );
    WMSG( "PIDPATH: %s", pidpath );
    IEVAL( r, open( pidpath, O_CREAT | O_WRONLY | O_TRUNC /* | O_EXCL */ , S_IWUSR | S_IRUSR ) );
    if ( r > 0 )
    {
      ctrl.pidfd[indx] = r;
      IEVAL( r, lockf( ctrl.pidfd[indx], F_TLOCK, 0 ) );
      WMSG( "PIDLCK: %d (%d)", r, ctrl.pidfd[indx] );
      IEVAL( r, write( ctrl.pidfd[indx], &ctrl.threads.n.main.pid, sizeof( ctrl.threads.n.main.pid ) ) );
      WMSG( "PIDW: %d", r );
      if ( r < 0 )
      {
        close( ctrl.pidfd[indx] );
        ctrl.pidfd[indx] = -1;
      }
    }
    mas_free( pidpath );
  }
  else
  {
    IEVAL( r, -1 );
  }
  return r;
}

int
mas_init_pids( void )
{
  int r = 0;
  char *namebuf = NULL;

  ctrl.threads.n.main.tid = mas_gettid(  );
  ctrl.threads.n.main.pid = getpid(  );
  ctrl.threads.n.main.thread = mas_pthread_self(  );
  ctrl.pserver_thread = &ctrl.threads.n.main;

  namebuf = mas_malloc( 512 );
  MAS_LOG( "(%d) init pids", r );
  if ( namebuf )
  {
    int indx = -1;

    *namebuf = 0;
    WMSG( "PIDSDIR: %s", opts.pidsdir );
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
      IEVAL( r, *namebuf ? mas_init_pid( indx, namebuf ) : -1 );
    }
    mas_free( namebuf );
  }
  MAS_LOG( "(%d) init pids done", r );
  return r;
}

int
mas_init_daemon( void )
{
  int r = 0;
  pid_t pid_child;

  MAS_LOG( "(%d) init daemon", r );
  if ( ctrl.daemon )
  {
    WMSG( "DAEMONIZE" );
    MAS_LOG( "(%d) init daemonize", r );
    IEVAL( r, mas_fork(  ) );
    pid_child = r;
    MAS_LOG( "(%d) init fork", r );
    if ( pid_child == 0 )
    {
      ctrl.threads.n.child.pid = getpid(  );
      ctrl.threads.n.child.tid = mas_gettid(  );
      ctrl.threads.n.child.thread = mas_pthread_self(  );
      ctrl.pserver_thread = &ctrl.threads.n.child;

      /* if ( prctl( PR_SET_NAME, ( unsigned long ) "zocchild" ) < 0 ) */
      /* {                                                             */
      /*   P_ERR;                                                      */
      /* }                                                             */

      IEVAL( r, prctl( PR_SET_NAME, ( unsigned long ) "zocchild" ) );

      for ( int i = 0; i < MAS_MAX_PIDFD; i++ )
      {
        if ( ctrl.pidfd[i] > 0 )
        {
          int lck;

          IEVAL( lck, lockf( ctrl.pidfd[i], F_LOCK, 0 ) );
          MAS_LOG( "(%d) init daemon; lock", r );
          WMSG( "PIDLCK+: %d (%d)", lck, ctrl.pidfd[i] );
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
      ctrl.threads.n.child.pid = pid_child;
      HMSG( "PARENT : %u @ %u @ %u", pid_child, getpid(  ), getppid(  ) );
      ctrl.is_parent = 1;
    }
  }
  MAS_LOG( "(%d) init daemon almost done", r );
  MAS_LOG( "(%d) init daemon done", r );
  return r;
}

#ifdef MAS_INIT_SEPARATE
int
mas_init_server( void ( *atexit_fun ) ( void ), int initsig, int argc, char **argv, char **env )
{
  int r = 0;

  ctrl.status = MAS_STATUS_START;
  ctrl.start_time = mas_double_time(  );


#  ifdef MAS_SERVER_NOLOG
  ctrl.log_disabled = 1;
#  endif
  /* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server */
  ctrl.is_client = 0;
  ctrl.is_server = 1;
  /* r = mas_pre_init( argc, argv, env ); */
  IEVAL( r, mas_pre_init( argc, argv, env ) );

  MAS_LOG( "init server" );
#  ifdef MAS_USE_CURSES
  /* if ( r >= 0 )              */
  /*   r = mas_init_curses(  ); */
#  endif
  /* if ( r >= 0 )                                           */
  /*   r = mas_init( atexit_fun, initsig, argc, argv, env ); */
  IEVAL( r, mas_init( atexit_fun, initsig, argc, argv, env ) );
  /* if ( r >= 0 )              */
  /*   r = mas_init_daemon(  ); */
  IEVAL( r, mas_init_daemon(  ) );
  /* malloc_trim( 0 ); */
  /* if ( ctrl.is_parent )       */
  /* {                           */
  /*   HMSG( "PARENT to exit" ); */
  /* }                           */
  /* else                        */
  {
    MAS_LOG( "(%d) init server: to init threads", r );
    /* if ( r >= 0 )               */
    /*   r = mas_threads_init(  ); */
    IEVAL( r, mas_threads_init(  ) );
    MAS_LOG( "(%d) init server: to load protos", r );
    /* if ( r >= 0 )                   */
    /*   r = mas_init_load_protos(  ); */
    IEVAL( r, mas_init_load_protos(  ) );
    MAS_LOG( "(%d) init server: to create lcontrols", r );
    if ( r >= 0 )
      mas_lcontrols_list_create(  );
    MAS_LOG( "init server done" );
    MAS_LOG( "(%d) init server: to post-init", r );
    /* if ( r >= 0 )            */
    /*   r = mas_post_init(  ); */
    IEVAL( r, mas_post_init(  ) );
    MAS_LOG( "(%d) end init server", r );
  }
  return r;
}
#endif
void
mas_destroy_server( void )
{
  MAS_LOG( "destroy server" );
  WMSG( "DESTROY SERVER" );
  MAS_LOG( "to save opts" );
  /* if ( !ctrl.opts_saved )                                                                                            */
  /*   mas_opts_save( NULL, ctrl.progname ? ctrl.progname : "Unknown" );                                                */
  /* if ( !ctrl.opts_saved_plus )                                                                                       */
  /*   mas_opts_save_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL ); */
  
  if ( ctrl.lcontrols_list )
  {
    MAS_LOG( "to cancel listeners" );
    WMSG( "TO CANCEL LISTENERS" );
    mas_listeners_cancel(  );
  }
  /* mas_channel_deaf( &ctrl, ctrl.pchannel ); */

#ifdef MAS_USE_CURSES
  if ( use_curses )
    sleep( 1 );
  mas_close_curses(  );
#endif
  WMSG( "1 DESTROY SERVER" );
  mas_lcontrols_delete_list(  );
  WMSG( "2 DESTROY SERVER" );
  mas_in_thread_end(  );
  WMSG( "3 DESTROY SERVER" );
  mas_threads_destroy(  );
  mas_destroy(  );
  MAS_LOG( "to cancel ticker" );
  MAS_LOG( "to cancel logger" );
  if ( ctrl.threads.n.logger.thread )
  {
    WMSG( "TO STOP LOGGER" );
    mas_logger_stop(  );
  }
  if ( ctrl.threads.n.ticker.thread )
  {
    WMSG( "TO STOP TICKER" );
    mas_ticker_stop(  );
  }
  WMSG( "TO DESTROY MODULES" );
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
  WMSG( "DESTROY SERVER DONE" );
}
