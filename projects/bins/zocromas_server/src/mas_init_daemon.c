#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/prctl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifndef MAS_SERVER_NOLOG
#  include <mastar/log/mas_logger.h>
#  include <mastar/log/mas_log.h>
#endif




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
mas_init_daemon_stdio( mas_options_t * popts, const char **message )
{
  extern mas_control_t ctrl;
  int r = 0;

  if ( ctrl.redirect_std )
  {
    {
      {
        ctrl.old_stdout = dup( STDOUT_FILENO );
        MAS_LOG( "(%d) init daemon; dup", r );
      }
      {
        int foutd = -1;

        if ( popts->stdout_filename )
        {
          foutd = mas_open( popts->stdout_filename, O_CREAT | O_WRONLY | O_TRUNC, 0777 );
          MAS_LOG( "(%d) init daemon; open foutd", r );
          dup2( foutd, STDOUT_FILENO );
          mas_close( foutd );
          MAS_LOG( "(%d) init daemon; close foutd", r );
        }
        MAS_LOG( "(%d) init daemon; dup2 / out", r );
      }
    }
    {
      {
        ctrl.old_stderr = dup( STDERR_FILENO );
        ctrl.old_stderrfile = fdopen( ctrl.old_stderr, "w" );
        setvbuf( ctrl.old_stderrfile, NULL, _IONBF, 0 );
        MAS_LOG( "(%d) init daemon; fdopen", r );
      }
      {
        int ferrd = -1;

        if ( popts->stderr_filename )
        {
          ferrd = mas_open( popts->stderr_filename, O_CREAT | O_WRONLY | O_TRUNC, 0777 );
          MAS_LOG( "(%d) init daemon; open ferrd", r );
          dup2( ferrd, STDERR_FILENO );
          MAS_LOG( "(%d) init daemon; dup2 / err", r );
          mas_close( ferrd );
          MAS_LOG( "(%d) init daemon; close ferrd", r );
        }
        setvbuf( stderr, NULL, _IONBF, 0 );
        /* MFP( "MFP to redirected stderr\n" ); */
        fprintf( stderr, "fprintf to redirected stderr\n" );
      }
    }
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
  if ( message )
    *message = __func__;
  return r;
}

int
mas_init_daemon( mas_options_t * popts, const char **message )
{
  extern mas_control_t ctrl;
  int r = 0, rn = 0;
  pid_t pid_daemon;

  MAS_LOG( "init daemon" );
  if ( ctrl.daemon )
  {
    HMSG( "INIT DAEMON pid:%u", getpid(  ) );
    WMSG( "DAEMONIZE" );
    MAS_LOG( "init daemonize" );
    IEVAL( r, mas_fork(  ) );
    pid_daemon = r;
    if ( r >= 0 )
      r = 0;
    MAS_LOG( "(%d) init fork", pid_daemon );
    if ( pid_daemon == 0 )
    {
      ctrl.threads.n.daemon.pid = getpid(  );
      ctrl.threads.n.daemon.tid = mas_gettid(  );
      ctrl.threads.n.daemon.thread = mas_pthread_self(  );
      ctrl.pserver_thread = &ctrl.threads.n.daemon;

      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaemonI" ) );
      HMSG( "INIT DAEMON:%u rn:%u", ctrl.threads.n.daemon.pid, rn );

      HMSG( "CHILD : %u @ %u @ %u - %s : %d", pid_daemon, getpid(  ), getppid(  ), popts->msgfilename, ctrl.msgfile ? 1 : 0 );
      /* sleep(200); */
      /* mas_destroy_server(  ); */
      IEVAL( r, setsid(  ) );
      IEVAL( r, chdir( "/" ) );
      IEVAL( r, mas_init_daemon_stdio( popts, NULL ) );
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaemon" ) );
    }
    else if ( pid_daemon > 0 )
    {
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParent" ) );
      ctrl.threads.n.daemon.pid = pid_daemon;
      HMSG( "PARENT : daemon pid:%u ; pid:%u ; ppid:%u", pid_daemon, getpid(  ), getppid(  ) );
      ctrl.is_parent = 1;
      r = 7777;
    }
  }
  else
  {
    HMSG( "INIT NO DAEMON pid:%u", getpid(  ) );
  }
  MAS_LOG( "(%d) init daemon almost done", r );
  MAS_LOG( "(%d) init daemon done", r );
  if ( message )
    *message = __func__;
  return r;
}
