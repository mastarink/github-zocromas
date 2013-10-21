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



#include "mas_runonce_action.h"


static int
runonce_waiton( config_section_t * sect, runonce_flags_t flags, pid_t pid )
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
      printf( "(%d) Wait ON Instances %d(%u) status:[%d:%d:%d:%s/%u]\n", w, sect->instances, pid, exit_status, info.si_status, exit_signal,
              scode, info.si_code );
    /* printf( "(%d) Wait ON Instances %d status:[%d:%d:%d]\n", w, sect->instances, WIFEXITED( status ), WEXITSTATUS( status ), */
    /*         WIFEXITED( status ) );                                                                                           */
    runonce_pids_reset(  );
    npids = runonce_section_get_pids( sect, flags );
  }
  while ( !sect->instances && w >= 0 );

  return npids;
}

static int
runonce_waitoff( config_section_t * sect, runonce_flags_t flags, pid_t pid )
{
  int npids;

  /* int status = 0; */
  int w = 0;
  int done = 0;

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
      printf( "(%d) Wait OFF Instances %d(%u) status:[%d:%d:%s/%u]\n", w, sect->instances, pid, exit_status, exit_signal, scode,
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
runonce_launch( config_group_t * grp, config_section_t * sect, runonce_flags_t flags )
{
  int done = 0;

  /* const char *command = sect->values[RUNONCE_COMMAND]; */
  int canlaunch = sect->values[RUNONCE_NOLAUNCH] ? 0 : 1;

  done = __LINE__;
  if ( !sect->instances && canlaunch )
  {
    {
      char *command = mas_argv_string( sect->largc, sect->largv, 0 );

      if ( flags.verbose )
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

        runonce_waiton( sect, flags, child );
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

          printf( "DRY {%s} execvp(%s, %s)\n", env, sect->name, command );
          mas_free( env );
          mas_free( command );
        }
        else if ( bad < 0 )
        {
          char *command = mas_argv_string( sect->largc, sect->largv, 0 );

          printf( "ERROR execvp(%s, %s)\n", sect->name, command );
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

          /* execvpe( sect->name, sect->largv, NULL ); */
          bad = execvpe( sect->largv[0], sect->largv, sect->lenvp );
          if ( bad < 0 )
          {
            char *command = mas_argv_string( sect->largc, sect->largv, 0 );
            char buf[256];

            printf( "ERROR 'execvp(%s, %s)' - %s\n", sect->largv[0], command, strerror_r( errno, buf, sizeof( buf ) ) );
            mas_free( command );
          }
        }
        printf( "EXEC FAIL\n" );
        exit( 17 );
      }
    }
  }
  return done;
}

int
runonce_terminate( config_group_t * grp, config_section_t * sect, runonce_flags_t flags )
{
  int done = 0;
  int signal = 0;
  const char *signame = sect->values[RUNONCE_PFIN];

  if ( flags.verbose )
    printf( "terminate %s:%s? (%d inst)\n", grp->name, sect->name, sect->instances );
  if ( signame && !sect->values[RUNONCE_NOTERMINATE] && !sect->values[RUNONCE_NOSTOP] && sect->values[RUNONCE_PFIN] )
  {
    done = __LINE__;
    if ( 0 == strcmp( signame, "TERM" ) )
      signal = SIGTERM;
    else if ( 0 == strcmp( signame, "QUIT" ) )
      signal = SIGQUIT;
    else if ( 0 == strcmp( signame, "HUP" ) )
      signal = SIGHUP;
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
          kill( sect->pids[ipid], signal );
          runonce_waitoff( sect, flags, sect->pids[ipid] );
        }
    }
    else
    {
    }
  }
  return done;
}

int
runonce_exit( config_group_t * grp, config_section_t * sect, runonce_flags_t flags )
{
  int done = 0;

  if ( !sect->values[RUNONCE_NOEXIT] && !sect->values[RUNONCE_NOSTOP] && sect->values[RUNONCE_QUITTER] )
  {
    done = __LINE__;
    if ( sect->instances )
    {
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
          runonce_waitoff( sect, flags, child );
          /* waitpid( child, &status, WNOHANG ); */
          waitpid( child, &status, 0 );
        }
        else
        {
          execvp( sect->qargv[0], sect->qargv );
          exit( 0 );
        }
      }
    }
    else
    {
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
