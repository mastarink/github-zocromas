#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <grp.h>
#include <errno.h>
#include <fcntl.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "mas_runonce_config_types.h"

#include "mas_runonce_pid.h"
#include "mas_runonce_section_pid.h"
#include "mas_runonce_window.h"



#include "mas_runonce_action.h"


static int
runonce_waiton( config_section_t * sect, int nsec, runonce_flags_t flags, pid_t pid )
{
  int npids;

  /* int status = 0; */
  int w = 0;

  do
  {
    char *scode = "?";
    siginfo_t info = {.si_code = 0,.si_pid = 0,.si_uid = 0,.si_signo = 0,.si_status = 0 };
    int exit_status = 0;
    int exit_signal = 0;

    info.si_code = 0;
    w = waitid( P_PID, pid, &info, WNOHANG | WEXITED | WSTOPPED | WCONTINUED );
    /* w = waitpid( pid, &status, WNOHANG ); */
    switch ( info.si_code )
    {
    case CLD_EXITED:
      exit_status = info.si_status;
      scode = "EXITED";
      break;
    case CLD_KILLED:
      exit_signal = info.si_status;
      scode = "KILLED";
      break;
    case CLD_DUMPED:
      exit_signal = info.si_status;
      scode = "DUMPED";
      break;
    case CLD_STOPPED:
      exit_signal = info.si_status;
      scode = "STOPPED";
      break;
    case CLD_TRAPPED:
      scode = "TRAPPED";
      break;
    case CLD_CONTINUED:
      exit_signal = info.si_status;
      scode = "CONTINUED";
      break;
    default:
      exit_status = info.si_status;
      scode = "DEFAULT";
      break;
    }
    if ( flags.verbose > 1 /* && w >= 0 */  )
      printf( "(%d) Wait ON Instances %d(pid=%u) status:[exit=%d:si=%d:sig=%d:%s(%u)]\n", w, sect->instances, pid, exit_status,
              info.si_status, exit_signal, scode, info.si_code );
    /* printf( "(%d) Wait ON Instances %d status:[%d:%d:%d]\n", w, sect->instances, WIFEXITED( status ), WEXITSTATUS( status ), */
    /*         WIFEXITED( status ) );                                                                                           */
    runonce_pids_reset(  );
    npids = runonce_section_get_pids( sect, flags );
  }
  while ( !sect->instances && w >= 0 );

  return npids;
}

static int
runonce_waitoff( config_section_t * sect, int nsec, runonce_flags_t flags, pid_t pid )
{
  int npids;

  /* int status = 0; */
  int w = 0;
  int done = 0;

  if ( flags.verbose )
    printf( "function %s\n", __func__ );
  do
  {
    char *scode = "?";
    siginfo_t info = {.si_code = 0,.si_pid = 0,.si_uid = 0,.si_signo = 0,.si_status = 0 };
    int exit_status = 0;
    int exit_signal = 0;

    w = waitid( P_PID, pid, &info, WNOHANG | WEXITED | WSTOPPED | WCONTINUED );
    /* w = waitpid( pid, &status, WNOHANG ); */
    switch ( info.si_code )
    {
    case CLD_EXITED:
      exit_status = info.si_status;
      scode = "EXITED";
      done = 1;
      break;
    case CLD_KILLED:
      exit_signal = info.si_status;
      scode = "KILLED";
      done = 1;
      break;
    case CLD_DUMPED:
      exit_signal = info.si_status;
      scode = "DUMPED";
      break;
    case CLD_STOPPED:
      exit_signal = info.si_status;
      scode = "STOPPED";
      break;
    case CLD_TRAPPED:
      scode = "TRAPPED";
      break;
    case CLD_CONTINUED:
      exit_signal = info.si_status;
      scode = "CONTINUED";
      break;
    }
    if ( flags.verbose > 1 /* && w >= 0 */  )
      printf( "(%d) Wait OFF Instances %d(pid=%u) status:[%d:%d:%s(%u)]\n", w, sect->instances, pid, exit_status, exit_signal, scode,
              info.si_code );
    /* w = waitpid( pid, &status, WNOHANG ); */
    /* mas_nanosleep( 2 ); */
    /* if ( w )                                                                                             */
    /*   printf( "(%d) Wait OFF Instances %d : %u\n", w, sect->instances, sect->pids ? sect->pids[0] : 0 ); */
    runonce_pids_reset(  );
    npids = runonce_section_get_pids( sect, flags );
  }
  while ( sect->instances && w >= 0 && !done );

  return npids;
}

/* static int                                                              */
/* runonce_waitoff_1( config_section_t * sect, runonce_flags_t flags )     */
/* {                                                                       */
/*   int npids;                                                            */
/*   int haspid = 0;                                                       */
/*                                                                         */
/*   do                                                                    */
/*   {                                                                     */
/*     for ( int ipid = 0; ipid < sect->instances; ipid++ )                */
/*     {                                                                   */
/*       char psdir[64];                                                   */
/*       struct stat st;                                                   */
/*                                                                         */
/*       snprintf( psdir, sizeof( psdir ), "/proc/%u", sect->pids[ipid] ); */
/*       haspid = ( 0 == stat( psdir, &st ) && S_ISDIR( st.st_mode ) );    */
/*       (* printf( "DIR %d - %s\n", haspid, psdir ); *)                   */
/*     }                                                                   */
/*   }                                                                     */
/*   while ( haspid );                                                     */
/*                                                                         */
/*   runonce_pids_reset(  );                                               */
/*   npids = runonce_config_section_get_pids( sect, flags );               */
/*                                                                         */
/*   return npids;                                                         */
/* }                                                                       */

int
runonce_close( config_group_t * grp, config_section_t * sect, int nsec, runonce_flags_t flags )
{
  int done = 0;
  prec_t *pidinfo = runonce_pids_precs(  );

  runonce_wids_set(  );

  if ( sect->values[RUNONCE_CLOSE] )
  {
    for ( int ipid = 0; ipid < sect->instances; ipid++ )
    {
      pid_t pid;

      pid = sect->pids[ipid];
      if ( pid )
      {
        unsigned long wid;

        wid = pidinfo[pid].wid;
        if ( wid )
        {
          if ( flags.dry )
            printf( "DRY close %s:%s\n", grp->name, sect->name );
          mas_runonce_close_window( wid, flags );
        }
      }
    }
  }
  done = __LINE__;
  return done;
}

int
runonce_show( config_group_t * grp, config_section_t * sect, int nsec, runonce_flags_t flags )
{
  int done = 0;
  char *command = mas_argv_string( sect->largc, sect->largv, 0 );
  char *spids = NULL;
  char *swids = NULL;
  char *sdtids = NULL;
  int maxcmdlen = 90;
  prec_t *pidinfo = runonce_pids_precs(  );

  runonce_wids_set(  );
  if ( !nsec )
    printf( "%2s %1s %5s %10s %3s  %14s  %18s    %s\n", "N", "I", "PID", "WID", "D", "GROUP", "SECT", "COMMAND" );

  for ( int ipid = 0; ipid < sect->instances; ipid++ )
  {
    pid_t pid;

    pid = sect->pids[ipid];
    if ( pid )
    {
      {
        char spid[16];

        snprintf( spid, sizeof( spid ), "%u", pid );
        if ( spids )
          spids = mas_strcat_x( spids, "," );
        spids = mas_strcat_x( spids, spid );
      }
      {
        unsigned long wid;
        int dtid;

        wid = pidinfo[pid].wid;
        if ( wid )
        {
          char swid[16];
          char sdtid[16];

          snprintf( swid, sizeof( swid ), "0x%lx", wid );
          if ( swids )
            swids = mas_strcat_x( swids, "," );
          swids = mas_strcat_x( swids, swid );

          dtid = get_window_desktop_num( wid );
          if ( dtid >= 0 )
          {
            snprintf( sdtid, sizeof( sdtid ), "%d", dtid );
            if ( sdtids )
              sdtids = mas_strcat_x( sdtids, "," );
            sdtids = mas_strcat_x( sdtids, sdtid );
          }
        }
      }
    }
  }
  if ( strlen( command ) > maxcmdlen && maxcmdlen > 5 )
  {
    char *cmd;

    cmd = mas_strndup( command, maxcmdlen - 5 );
    mas_free( command );
    command = cmd;
    command = mas_strcat_x( command, "..." );
  }
  {
    char sinst[16];

    snprintf( sinst, sizeof( sinst ), "%d", sect->instances );
    done = __LINE__;
    printf( "%2d:%1s:%5s:%10s:D%2s: %14s: %18s:   [%s]\n", nsec, sect->instances ? sinst : "-", spids ? spids : "-", swids ? swids : "-",
            sdtids ? sdtids : "-", grp->name, sect->name, command );
  }
  mas_free( swids );
  mas_free( sdtids );
  mas_free( spids );
  mas_free( command );
  return done;
}

int
runonce_launch( config_group_t * grp, config_section_t * sect, int nsec, runonce_flags_t flags )
{
  int done = 0;

  if ( flags.verbose )
    printf( "function %s\n", __func__ );
  /* const char *command = sect->values[RUNONCE_COMMAND]; */
  int canlaunch = ( sect->values[RUNONCE_NOLAUNCH] || flags.force ) ? 1 : 0;
  int stdenv = sect->values[RUNONCE_STDENV] ? 1 : 0;

  done = __LINE__;
  if ( !sect->instances && canlaunch )
  {
    if ( flags.verbose )
    {
      char *command = mas_argv_string( sect->largc, sect->largv, 0 );

      printf( "LAUNCH %14s (%d) %18s - [%s] (%d)\n", grp->name, sect->instances, sect->name, command, sect->largc );
      mas_free( command );
    }
    if ( flags.noop )
    {
    }
    else
    {
      pid_t child;

      child = fork(  );
      if ( child )
      {
        /* int status = 0; */

        runonce_waiton( sect, nsec, flags, child );
        /* printf( "A waitpid status: %d\n", status ); */
        /* sleep( 3 );                                 */
        /* waitpid( child, &status, WNOHANG );         */
        /* printf( "B waitpid status: %d\n", status ); */
        /* waitpid( child, &status, 0 ); */
      }
      else
      {
        int bad = 0;
        int gid = getgid(  );
        int uid = getuid(  );

        if ( sect->values[RUNONCE_NICE] && !sect->values[RUNONCE_NONICE] )
        {
          int inice = 0;

          sscanf( sect->values[RUNONCE_NICE], "%d", &inice );
          bad = nice( inice );
          if ( bad < 0 )
          {
            char buf[256];

            printf( "NICE error %s : %s\n", sect->values[RUNONCE_NICE], strerror_r( errno, buf, sizeof( buf ) ) );
            /* else if ( bad )                                                                                                    */
            /*   printf( "(%d) can't NICE %s : %s\n", bad, sect->values[RUNONCE_NICE], strerror_r( errno, buf, sizeof( buf ) ) ); */
          }
        }
        if ( sect->values[RUNONCE_XGROUP] && !sect->values[RUNONCE_NOGROUP] )
        {
          struct group *gr;

          gr = getgrnam( sect->values[RUNONCE_XGROUP] );
          if ( gr )
            gid = gr->gr_gid;
        }
        if ( gid && !flags.nosetgid )
        {
          bad = setgid( gid );
          if ( bad < 0 )
          {
            char buf[256];

            printf( "can't SETGID %s : %s\n", sect->values[RUNONCE_XGROUP], strerror_r( errno, buf, sizeof( buf ) ) );
          }
          if ( bad >= 0 )
          {
            bad = setuid( uid );
            if ( bad < 0 )
            {
              char buf[256];

              printf( "can't SETUID %s\n", strerror_r( errno, buf, sizeof( buf ) ) );
            }
          }
        }
        if ( flags.dry )
        {
          char *command = mas_argv_string( sect->largc, sect->largv, 0 );
          char *env = mas_argv_string( sect->lenvc, sect->lenvp, 0 );

          printf( "DRY `%s`\n", env );
          printf( "DRY %s(%s, %s)\n", stdenv ? "execvp" : "execvpe", sect->name, command );
          for ( int i = 0; i < sect->lenvc; i++ )
          {
            printf( "DRY #%d. %s\n", i, sect->lenvp[i] );
          }
          mas_free( env );
          mas_free( command );
        }
        else if ( bad < 0 )
        {
          char *command = mas_argv_string( sect->largc, sect->largv, 0 );

          printf( "ERROR %s(%s, %s)\n", stdenv ? "execvp" : "execvpe", sect->name, command );
          mas_free( command );
        }
        else
        {
          setsid(  );
          /* chdir( "/" ); */
          {
            int foutd = -1;
            char stdout_filename[64];

            snprintf( stdout_filename, sizeof( stdout_filename ), "/tmp/runonce_stdout.%d.%d.tmp", gid, uid );

            foutd = mas_open( stdout_filename, O_CREAT | O_WRONLY | O_APPEND, 0777 );
            dup2( foutd, STDOUT_FILENO );
            mas_close( foutd );
          }
          {
            int ferrd = -1;
            char stderr_filename[64];

            snprintf( stderr_filename, sizeof( stderr_filename ), "/tmp/runonce_stderr.%d.%d.tmp", gid, uid );

            ferrd = mas_open( stderr_filename, O_CREAT | O_WRONLY | O_APPEND, 0777 );
            dup2( ferrd, STDERR_FILENO );
            mas_close( ferrd );
            setvbuf( stderr, NULL, _IONBF, 0 );
          }
          /* if ( sect->values[RUNONCE_GLOBENV] )           */
          /*   bad = execvp( sect->largv[0], sect->largv ); */
          /* else                                           */
          {
            char *fpath = NULL;

            fpath = mas_strdup( sect->values[RUNONCE_PATH] );
            fpath = mas_strcat_x( fpath, "/" );
            fpath = mas_strcat_x( fpath, sect->largv[0] );
            if ( stdenv )
              bad = execvp( fpath, sect->largv );
            else
              bad = execvpe( fpath, sect->largv, sect->lenvp );
            mas_free( fpath );
          }
          if ( bad < 0 )
          {
            char *command = mas_argv_string( sect->largc, sect->largv, 0 );
            char buf[256];

            printf( "ERROR '%s(%s, %s)' - %s\n", stdenv ? "execvp" : "execvpe", sect->largv[0], command,
                    strerror_r( errno, buf, sizeof( buf ) ) );
            mas_free( command );
          }
        }
        if ( !flags.dry )
          printf( "EXEC FAIL\n" );
        exit( 17 );
      }
    }
  }
  else
  {
    if ( flags.verbose )
    {
      char *command = mas_argv_string( sect->largc, sect->largv, 0 );

      printf( "Can't LAUNCH %14s (%d) %18s - [%s] (%d)\n", grp->name, sect->instances, sect->name, command, sect->largc );
      mas_free( command );
    }
  }
  return done;
}

int
runonce_terminate( config_group_t * grp, config_section_t * sect, int nsec, runonce_flags_t flags )
{
  int done = 0;
  int signal = 0;
  const char *signame = sect->values[RUNONCE_PFIN];

  if ( flags.verbose )
    printf( "function %s\n", __func__ );
  if ( sect->instances )
  {
    if ( flags.verbose )
      printf( "terminate %s:%s? (%d inst) with %s\n", grp->name, sect->name, sect->instances, signame );
    if ( signame && !sect->values[RUNONCE_NOTERMINATE] && !sect->values[RUNONCE_NOSTOP] && sect->values[RUNONCE_PFIN] )
    {
      int pgrp = 0;

      done = __LINE__;
      if ( *signame == '-' )
      {
        signame++;
        pgrp = 1;
      }
      if ( 0 == strcmp( signame, "TERM" ) )
        signal = SIGTERM;
      else if ( 0 == strcmp( signame, "QUIT" ) )
        signal = SIGQUIT;
      else if ( 0 == strcmp( signame, "HUP" ) )
        signal = SIGHUP;
      if ( flags.verbose )
        printf( "terminate %s:%s? (%d inst) with %d\n", grp->name, sect->name, sect->instances, signal );
      if ( signal && sect->instances )
      {
        if ( flags.verbose )
          printf( "TERM %14s (%d) %18s (%s:%d)\n", grp->name, sect->instances, sect->name, sect->values[RUNONCE_PFIN], signal );
        for ( int ipid = 0; ipid < sect->instances; ipid++ )
          if ( flags.dry )
          {
            printf( "DRY kill -%d %d\n", signal, sect->pids[ipid] );
          }
          else
          {
            pid_t pid = sect->pids[ipid];

            kill( pgrp ? -pid : pid, signal );
            runonce_waitoff( sect, nsec, flags, sect->pids[ipid] );
          }
      }
      else
      {
      }
    }
  }
  return done;
}

int
runonce_toggle( config_group_t * grp, config_section_t * sect, int nsec, runonce_flags_t flags )
{
  int done = 0;

  if ( flags.verbose )
    printf( "function %s\n", __func__ );

  if ( sect->instances )
    done = runonce_terminate( grp, sect, nsec, flags );
  else
    done = runonce_launch( grp, sect, nsec, flags );
  return done;
}

int
runonce_exit( config_group_t * grp, config_section_t * sect, int nsec, runonce_flags_t flags )
{
  int done = 0;

  if ( !sect->values[RUNONCE_NOEXIT] && !sect->values[RUNONCE_NOSTOP] && sect->values[RUNONCE_QUITTER] && sect->instances )
  {
    done = __LINE__;
    if ( flags.verbose )
      printf( "EXIT %14s (%d) %18s\n", grp->name, sect->instances, sect->name );
    if ( flags.dry || flags.noop )
    {
      char *command = mas_argv_string( sect->qargc, sect->qargv, 0 );

      printf( "DRY %s %s\n", sect->name, command );
      /* for ( int ia = 0; ia < sect->qargc; ia++ )   */
      /*   printf( "%d: %s\n", ia, sect->qargv[ia] ); */
      mas_free( command );
    }
    else
    {
      pid_t child;

      printf( "%d %s [%s:%s] ::\n", done, sect->name, sect->values[RUNONCE_NOEXIT], sect->values[RUNONCE_NOSTOP] );
      for ( int ia = 0; ia < sect->qargc; ia++ )
        printf( "%d: %s\n", ia, sect->qargv[ia] );
      child = fork(  );
      if ( child )
      {
        int status = 0;

        printf( "To wait OFF Instances %d : %u\n", sect->instances, sect->pids ? sect->pids[0] : 0 );
        runonce_waitoff( sect, nsec, flags, child );
        /* waitpid( child, &status, WNOHANG ); */
        waitpid( child, &status, 0 );
      }
      else
      {
        execvp( sect->qargv[0], sect->qargv );
        exit( 0 );
      }
    }
    /* if (  ) */
    /* {       */
    /* }       */
    /* else    */
    /* {       */
    /* }       */
  }
  return done;
}
