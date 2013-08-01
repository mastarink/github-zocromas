#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <pwd.h>
#include <grp.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_control_types.h>

#ifndef MAS_SERVER_NOLOG
#  include <mastar/log/mas_logger.h>
#  include <mastar/log/mas_log.h>
#endif


#include "mas_init_daemon.h"


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
  EVAL_PREPARE;
  CTRL_PREPARE;
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
mas_init_child_process( mas_options_t * popts, const char **message )
{
  EVAL_PREPARE;
  CTRL_PREPARE;
  int r = 0, rn = 0;

  ctrl.threads.n.daemon.pid = getpid(  );
  ctrl.threads.n.daemon.tid = mas_gettid(  );
  ctrl.threads.n.daemon.thread = mas_pthread_self(  );
  ctrl.pserver_thread = &ctrl.threads.n.daemon;

  IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaemonI" ) );
  HMSG( "INIT CHILD pid:%u(%u)@%u", getpid(  ), ctrl.threads.n.daemon.pid, getppid(  ) );

  /* sleep(200); */
  /* mas_destroy_server(  ); */
  if ( !popts->daemon.disable_setsid )
    IEVAL( r, setsid(  ) );
  if ( !popts->daemon.disable_chdir )
    IEVAL( r, chdir( "/" ) );
  if ( popts->group )
  {
    gid_t rgid, egid, sgid;
    struct group grp, *pgrp;
    char grbuf[512];

    getresgid( &rgid, &egid, &sgid );
    /* int getgrnam_r( const char *name, struct group *grp, char *buf, size_t buflen, struct group **result );   */
    getgrnam_r( popts->group, &grp, grbuf, sizeof( grbuf ), &pgrp );
    HMSG( "rgid:%u; egid:%u; sgid:%u; %u", rgid, egid, sgid, pgrp ? pgrp->gr_gid : 0 );
    if ( pgrp )
    {
      IEVAL( r, setgid( pgrp->gr_gid ) );
      getresgid( &rgid, &egid, &sgid );
      HMSG( "(%d)rgid:%u; egid:%u; sgid:%u; %u", r, rgid, egid, sgid, pgrp ? pgrp->gr_gid : 0 );
      if ( rgid )
        r = 0;
    }
  }
  if ( popts->user )
  {
    uid_t ruid, euid, suid;
    struct passwd pwd, *ppwd;
    char pwbuf[512];

    getresuid( &ruid, &euid, &suid );
    /* int getpwnam_r( const char *name, struct passwd *pwd, char *buf, size_t buflen, struct passwd **result ); */
    getpwnam_r( popts->user, &pwd, pwbuf, sizeof( pwbuf ), &ppwd );
    HMSG( "ruid:%u; euid:%u; suid:%u; %u:%u", ruid, euid, suid, ppwd ? ppwd->pw_uid : 0, ppwd ? ppwd->pw_gid : 0 );
    if ( ppwd )
    {
      IEVAL( r, setuid( ppwd->pw_uid ) );
      if ( ruid )
        r = 0;
    }
  }

  IEVAL( r, mas_init_daemon_stdio( popts, NULL ) );
  IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaemon" ) );
  ctrl.is_child = 1;

  if ( message )
    *message = __func__;
  return r;
}

int
mas_init_parent_process( mas_options_t * popts, const char **message )
{
  EVAL_PREPARE;
  CTRL_PREPARE;
  int r = 0, rn = 0;

  IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParent" ) );
  HMSG( "INIT PARENT pid:%u@%u ; child:%u ", getpid(  ), getppid(  ), ctrl.threads.n.daemon.pid );
  /* HMSG( "PARENT : daemon pid:%u ; pid:%u ; ppid:%u", ctrl.threads.n.daemon.pid, getpid(  ), getppid(  ) ); */
  ctrl.is_parent = 1;
  r = 7777;
  if ( message )
    *message = __func__;

  return r;
}

int
mas_init_daemon( mas_options_t * popts, const char **message )
{
  EVAL_PREPARE;
  CTRL_PREPARE;
  int r = -1;

  MAS_LOG( "init daemon" );
  if ( ctrl.daemon /* && !ctrl.is_child && !ctrl.is_parent */  )
  {
    r = 0;
    HMSG( "INIT DAEMON >" );
    MAS_LOG( "init daemonize" );
    if ( popts->daemon.sys )
    {
      popts->daemon.disable_setsid = 1;
      popts->daemon.disable_chdir = 1;
      IEVAL( r, daemon( 0, 0 ) );
      HMSG( "INIT DAEMON SYS" );
      IEVAL( r, mas_init_child_process( popts, NULL ) );
    }
    else
    {
      pid_t pid_daemon;

      IEVAL( r, mas_fork(  ) );
      pid_daemon = r;
      if ( r >= 0 )
        r = 0;
      MAS_LOG( "(%d) init fork", pid_daemon );
      if ( pid_daemon == 0 )
      {
        IEVAL( r, mas_init_child_process( popts, NULL ) );
      }
      else if ( pid_daemon > 0 )
      {
        ctrl.threads.n.daemon.pid = pid_daemon;
        IEVAL( r, mas_init_parent_process( popts, NULL ) );
      }
    }
  }
  else
  {
    ctrl.threads.n.daemon.pid = getpid(  );
    HMSG( "INIT NO DAEMON pid:%u", ctrl.threads.n.daemon.pid );
    r = 0;
  }
  MAS_LOG( "(%d) init daemon almost done", r );
  MAS_LOG( "(%d) init daemon done", r );
  HMSG( "INIT DAEMON" );
  if ( message )
    *message = __func__;

  return r > 0 ? 0 : r;
}

__attribute__ ( ( constructor ) )
     static void f_constructor( void )
{
  if ( stderr )
    fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno );
}
