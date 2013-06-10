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

#ifdef MAS_OLD_VARIABLES_HTTP
#  include <mastar/variables/mas_variables.h>
#else
#  include <mastar/types/mas_varset_types.h>
#  include <mastar/varset/mas_varset_object.h>
#  include <mastar/varset/mas_varset.h>
#endif


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifndef MAS_SERVER_NOLOG
#  include <mastar/log/mas_logger.h>
#  include <mastar/log/mas_log.h>
#endif

#include <mastar/modules/mas_modules_ctrl_module.h>
#include <mastar/modules/mas_modules_load_module.h>

#include <mastar/listener/mas_listener_control_list.h>
#include <mastar/listener/mas_listeners.h>

#ifdef MAS_USE_CURSES
#  include <mastar/msg/mas_curses.h>
#endif

#include <mastar/thtools/mas_thread_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;


#include "mas_init_threads.h"


#include <mastar/init/mas_init.h>
#include <mastar/init/mas_opts.h>

#include "mas_ticker.h"

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

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  /* if ( stderr )                                                                      */
  /*   fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno ); */
}


int
mas_init_load_protos( MAS_PASS_OPTS_DECLARE1 )
{
  MAS_PASS_OPTS_DECL_PREF;
  int r = 0;
  mas_transaction_protodesc_t *proto_descs = NULL;

  MAS_LOG( "(%d) init / load protos", r );
  if ( !MAS_PASS_OPTS_PREF protosv.c )
  {
    HMSG( "NO PROTCOLOS DEFINED" );
    WMSG( "no protcolos defined" );
    IEVAL( r, -1 );
  }
  if ( !ctrl.proto_descs )
  {
    int protos_num = 0;

    proto_descs = mas_calloc( MAS_PASS_OPTS_PREF protosv.c, sizeof( mas_transaction_protodesc_t ) );
    memset( proto_descs, 0, MAS_PASS_OPTS_PREF protosv.c * sizeof( mas_transaction_protodesc_t ) );
    for ( int ipr = 0; ipr < MAS_PASS_OPTS_PREF protosv.c; ipr++ )
    {
      /* from one */
      proto_descs[ipr].proto_id = protos_num + 1;
      proto_descs[ipr].name = mas_strdup( MAS_PASS_OPTS_PREF protosv.v[ipr] );
      proto_descs[ipr].func =
            ( mas_transaction_fun_t ) mas_modules_load_func_from( MAS_PASS_OPTS_PREF protosv.v[ipr], "mas_proto_main",
                                                                  MAS_PASS_OPTS_PREF dir.proto );
      if ( !proto_descs[ipr].func )
      {
        EMSG( "PROTO LOAD %s FAIL", proto_descs[ipr].name );
        IEVAL( r, -1 );
        WMSG( "INIT PROTOS - #%d: %s", ipr, MAS_PASS_OPTS_PREF protosv.v[ipr] );
      }
      else
      {
        WMSG( "INIT PROTOS + #%d: %s", ipr, MAS_PASS_OPTS_PREF protosv.v[ipr] );
      }
      protos_num++;
      MAS_LOG( "(%d) init / load protos #%d", r, protos_num );
    }
    ctrl.protos_num = protos_num;
    ctrl.proto_descs = proto_descs;
    if ( MAS_PASS_OPTS_PREF protosv.c && !ctrl.protos_num )
    {
      IEVAL( r, -1 );
    }
    HMSG( "(%d) INIT S PROTOS %d of %d", r, protos_num, MAS_PASS_OPTS_PREF protosv.c );
  }
  else
  {
    IEVAL( r, -1 );
  }
  /* r = ctrl.proto_descs ? 0 : -1; */
  IEVAL( r, ctrl.proto_descs ? 0 : -1 );
  MAS_LOG( "(%d) init / load protos done", r );
  return r;
}

int
mas_protos_destroy(  )
{
  int r = 0;

  for ( int ipr = 0; ipr < ctrl.protos_num; ipr++ )
  {
#ifdef MAS_OLD_VARIABLES_HTTP
    mas_variables_list_head_t *vars;

    vars = ctrl.proto_descs[ipr].variables;
    ctrl.proto_descs[ipr].variables = NULL;
    if ( vars )
      mas_variables_delete( vars );
#else
    mas_varset_t *vars;

    vars = ctrl.proto_descs[ipr].variables;
    ctrl.proto_descs[ipr].variables = NULL;
    if ( vars )
      mas_varset_delete( vars );
#endif
  }
  return r;
}

#define XSTR(s) STR(s)
#define STR(s) #s

static int
mas_init_pid( MAS_PASS_OPTS_DECLARE int indx, const char *shash_name )
{
  int r = 0;

  if ( shash_name && *shash_name && indx < MAS_MAX_PIDFD )
  {
    MAS_PASS_OPTS_DECL_PREF;
    char *pidpath;

    pidpath = mas_strdup( MAS_PASS_OPTS_PREF dir.pids );
    pidpath = mas_strcat_x( pidpath, shash_name );
    HMSG( "PIDPATH: %s", pidpath );
    YEVALM( r, mas_open( pidpath, O_CREAT | O_WRONLY | O_TRUNC /* | O_EXCL */ , S_IWUSR | S_IRUSR ), "(%d) file:%s", pidpath );
    if ( r < 0 )
    {
      const char *pp = XSTR( MAS_SYSCONFDIR ) "/../run";

      mas_free( pidpath );

      pidpath = mas_strdup( pp );
      pidpath = mas_strcat_x( pidpath, shash_name );
      r = 0;
      YEVALM( r, mas_open( pidpath, O_CREAT | O_WRONLY | O_TRUNC /* | O_EXCL */ , S_IWUSR | S_IRUSR ), "(%d) file:%s", pidpath );
      HMSG( "(%d)PIDPATH 2a : [%s] %s", r, shash_name, pidpath );
      if ( r < 0 )
      {
        mkdir( pp, S_IRUSR | S_IWUSR | S_IXUSR );
        r = 0;
        YEVALM( r, mas_open( pidpath, O_CREAT | O_WRONLY | O_TRUNC /* | O_EXCL */ , S_IWUSR | S_IRUSR ), "(%d) file:%s", pidpath );
        HMSG( "(%d)PIDPATH 2b : %s", r, pidpath );
      }
      MAS_LOG( "Test 1" );
      MAS_LOG( "Test 2" );
      MAS_LOG( "Test 3" );
      MAS_LOG( "Test 4" );
      MAS_LOG( "Test 5" );
      MAS_LOG( "Test 6" );
      MAS_LOG( "Test 7" );
      HMSG( "(%d)PIDPATH 2c : %s", r, pidpath );
    }
    if ( r > 0 )
    {
      ctrl.pidfd[indx] = r;
      IEVAL( r, lockf( ctrl.pidfd[indx], F_TLOCK, 0 ) );
      HMSG( "PIDLCK: %d (%d) %s", r, ctrl.pidfd[indx], pidpath );
      IEVAL( r, write( ctrl.pidfd[indx], &ctrl.threads.n.main.pid, sizeof( ctrl.threads.n.main.pid ) ) );
      WMSG( "PIDW: %d", r );
      if ( r < 0 )
      {
        mas_close( ctrl.pidfd[indx] );
        ctrl.pidfd[indx] = -1;
      }
      else
        ctrl.pidfilesv.c = mas_add_argv_arg( ctrl.pidfilesv.c, &ctrl.pidfilesv.v, pidpath );
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
mas_init_pids( MAS_PASS_OPTS_DECLARE1 )
{
  int r = 0;
  char *shash_namebuf = NULL;

  ctrl.threads.n.main.tid = mas_gettid(  );
  ctrl.threads.n.main.pid = getpid(  );
  ctrl.threads.n.main.thread = mas_pthread_self(  );
  ctrl.pserver_thread = &ctrl.threads.n.main;

  shash_namebuf = mas_malloc( 512 );
  MAS_LOG( "(%d) init pids", r );
  if ( shash_namebuf )
  {
    MAS_PASS_OPTS_DECL_PREF;
    int indx = -1;

    *shash_namebuf = 0;
    WMSG( "PIDSDIR: %s", MAS_PASS_OPTS_PREF dir.pids );
    if ( MAS_PASS_OPTS_PREF single_instance && MAS_PASS_OPTS_PREF dir.pids )
    {
      snprintf( shash_namebuf, sizeof( shash_namebuf ), "/%s.pid", ctrl.is_client ? "client" : "server" );
      indx = 0;
    }
    else if ( MAS_PASS_OPTS_PREF single_child && MAS_PASS_OPTS_PREF dir.pids )
    {
      snprintf( shash_namebuf, sizeof( shash_namebuf ), "/%s.%u.pid", ctrl.is_client ? "client" : "server", getppid(  ) );
      indx = 1;
    }
    if ( indx >= 0 )
    {
      IEVAL( r, *shash_namebuf ? mas_init_pid( MAS_PASS_OPTS_PASS indx, shash_namebuf ) : -1 );
    }
    mas_free( shash_namebuf );
  }
  MAS_LOG( "(%d) init pids done", r );
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
   8 Change the working directory of the daemon process to root 
   9 close stdin, stdout and stderr file descriptors.
   = Let the main logic of daemon process run.
*/
int
mas_init_daemon( MAS_PASS_OPTS_DECLARE1 )
{
  int r = 0, rn = 0;
  pid_t pid_daemon;

  MAS_LOG( "init daemon" );
  if ( ctrl.daemon )
  {
    HMSG( "INIT DAEMON @ %u", getpid(  ) );
    WMSG( "DAEMONIZE" );
    MAS_LOG( "init daemonize" );
    IEVAL( r, mas_fork(  ) );
    pid_daemon = r;
    MAS_LOG( "(%d) init fork", r );
    if ( pid_daemon == 0 )
    {
      MAS_PASS_OPTS_DECL_PREF;
      ctrl.threads.n.daemon.pid = getpid(  );
      ctrl.threads.n.daemon.tid = mas_gettid(  );
      ctrl.threads.n.daemon.thread = mas_pthread_self(  );
      ctrl.pserver_thread = &ctrl.threads.n.daemon;

      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaemonI" ) );
      HMSG( "INIT DAEMON:%u rn:%u", ctrl.threads.n.daemon.pid, rn );

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
      HMSG( "CHILD : %u @ %u @ %u - %s : %d", pid_daemon, getpid(  ), getppid(  ), MAS_PASS_OPTS_PREF msgfilename, ctrl.msgfile ? 1 : 0 );
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
      chdir( "/" );
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaemon" ) );
    }
    else if ( pid_daemon > 0 )
    {
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParent" ) );
      ctrl.threads.n.daemon.pid = pid_daemon;
      HMSG( "PARENT : %u @ %u @ %u", pid_daemon, getpid(  ), getppid(  ) );
      ctrl.is_parent = 1;
    }
  }
  else
  {
    HMSG( "INIT NO DAEMON" );
  }
  MAS_LOG( "(%d) init daemon almost done", r );
  MAS_LOG( "(%d) init daemon done", r );
  return r;
}

/* #ifdef MAS_INIT_SEPARATE                                                                          */
/* int                                                                                               */
/* mas_init_server( void ( *atexit_fun ) ( void ), int initsig, int argc, char **argv, char **env )  */
/* {                                                                                                 */
/*   int r = 0;                                                                                      */
/*                                                                                                   */
/*   ctrl.status = MAS_STATUS_START;                                                                 */
/*   ctrl.start_time = mas_double_time(  );                                                          */
/*                                                                                                   */
/*                                                                                                   */
/* #  ifdef MAS_SERVER_NOLOG                                                                         */
/*   ctrl.log_disabled = 1;                                                                          */
/* #  endif                                                                                          */
/*   (* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server *) */
/*   ctrl.is_client = 0;                                                                             */
/*   ctrl.is_server = 1;                                                                             */
/*   (* r = mas_pre_init( argc, argv, env ); *)                                                      */
/*   IEVAL( r, mas_pre_init( argc, argv, env ) );                                                    */
/*                                                                                                   */
/*   MAS_LOG( "init server" );                                                                       */
/* #  ifdef MAS_USE_CURSES                                                                           */
/*   (* if ( r >= 0 )              *)                                                                */
/*   (*   r = mas_init_curses(  ); *)                                                                */
/* #  endif                                                                                          */
/*   (* if ( r >= 0 )                                           *)                                   */
/*   (*   r = mas_init( atexit_fun, initsig, argc, argv, env ); *)                                   */
/*   IEVAL( r, mas_init( atexit_fun, initsig, argc, argv, env ) );                                   */
/*   (* if ( r >= 0 )              *)                                                                */
/*   (*   r = mas_init_daemon(  ); *)                                                                */
/*   IEVAL( r, mas_init_daemon(  ) );                                                                */
/*   (* malloc_trim( 0 ); *)                                                                         */
/*   (* if ( ctrl.is_parent )       *)                                                               */
/*   (* {                           *)                                                               */
/*   (*   HMSG( "PARENT to exit" ); *)                                                               */
/*   (* }                           *)                                                               */
/*   (* else                        *)                                                               */
/*   {                                                                                               */
/*     MAS_LOG( "(%d) init server: to init threads", r );                                            */
/*     (* if ( r >= 0 )               *)                                                             */
/*     (*   r = mas_threads_init(  ); *)                                                             */
/*     IEVAL( r, mas_threads_init(  ) );                                                             */
/*     MAS_LOG( "(%d) init server: to load protos", r );                                             */
/*     (* if ( r >= 0 )                   *)                                                         */
/*     (*   r = mas_init_load_protos( MAS_PASS_OPTS_DECLARE1 ); *)                                   */
/*     IEVAL( r, mas_init_load_protos( MAS_PASS_OPTS_DECLARE1 ) );                                   */
/*     MAS_LOG( "(%d) init server: to create lcontrols", r );                                        */
/*     if ( r >= 0 )                                                                                 */
/*       mas_lcontrols_list_create(  );                                                              */
/*     MAS_LOG( "init server done" );                                                                */
/*     MAS_LOG( "(%d) init server: to post-init", r );                                               */
/*     (* if ( r >= 0 )            *)                                                                */
/*     (*   r = mas_post_init(  ); *)                                                                */
/*     IEVAL( r, mas_post_init(  ) );                                                                */
/*     MAS_LOG( "(%d) end init server", r );                                                         */
/*   }                                                                                               */
/*   return r;                                                                                       */
/* }                                                                                                 */
/* #endif                                                                                            */
void
mas_destroy_server( MAS_PASS_OPTS_DECLARE1 )
{
  EMSG( "D" );
  {
    int rn = 0;
    char name_buffer[32] = "?";

    IEVAL( rn, prctl( PR_GET_NAME, ( unsigned long ) name_buffer ) );
    if ( ctrl.is_parent )
    {
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParServerD" ) );
    }
    else
    {
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaeServerD" ) );
    }
    MAS_LOG( "destroy server" );
    EMSG( "DESTROY SERVER %s", name_buffer );
  }
  MAS_LOG( "to save opts" );
  /* if ( !ctrl.opts_saved )                                                                                            */
  /*   mas_opts_save( NULL, ctrl.progname ? ctrl.progname : "Unknown" );                                                */
  /* if ( !ctrl.opts_saved_plus )                                                                                       */
  /*   mas_opts_save_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL ); */

  EMSG( "L" );
  mas_listeners_stop(  );
  EMSG( "/L" );
  /* mas_channel_deaf( &ctrl, ctrl.pchannel ); */

#ifdef MAS_USE_CURSES
  if ( use_curses )
    sleep( 1 );
  mas_close_curses(  );
#endif
  mas_lcontrols_delete_list(  );
  mas_in_thread_end(  );
  mas_threads_destroy( MAS_PASS_OPTS_PASS1 );
  mas_protos_destroy(  );


  mas_destroy( MAS_PASS_OPTS_PASS1 );
  MAS_LOG( "to cancel ticker" );
  MAS_LOG( "to cancel logger" );
  if ( ctrl.threads.n.logger.thread )
  {
    EMSG( "TO STOP LOGGER" );
    mas_logger_stop(  );
  }
  if ( ctrl.threads.n.ticker.thread )
  {
    EMSG( "TO STOP TICKER" );
    mas_ticker_stop(  );
  }
  WMSG( "TO DESTROY MODULES" );
  mas_modules_unregister(  );
  {
    for ( int i = 0; i < MAS_MAX_PIDFD; i++ )
    {
      if ( ctrl.pidfd[i] > 0 )
      {
        /* char *pidpath;                           */
        /*                                          */
        /* pidpath = mas_strdup( MAS_PASS_OPTS_PREF dir.pids );    */
        /* pidpath = mas_strcat_x( pidpath, name ); */

        mas_close( ctrl.pidfd[i] );
        /* unlink( pidpath ); */
      }
      ctrl.pidfd[i] = 0;
    }
  }

  MAS_LOG( "destroy server done" );
  WMSG( "DESTROY SERVER DONE" );
  EMSG( "/D logQsize:%lu - %lu = %lu", ctrl.log_q_came, ctrl.log_q_gone, ctrl.log_q_came - ctrl.log_q_gone );
}
