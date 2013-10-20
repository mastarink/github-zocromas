#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <grp.h>
#include <errno.h>
#include <fcntl.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "mas_runonce_config_types.h"

#include "mas_runonce_config.h"
#include "mas_runonce_pid.h"
#include "mas_runonce_test.h"
#include "mas_runonce_base.h"
#include "mas_runonce.h"

int
runonce_waiton( config_section_t * sect, runonce_flags_t flags )
{
  int npids;

  do
  {
    printf( "Wait Instances %d\n", sect->instances );
    runonce_pids_reset(  );
    npids = runonce_config_section_get_pids( sect, flags );
  }
  while ( !sect->instances );

  return npids;
}

int
runonce_waitoff( config_section_t * sect, runonce_flags_t flags, pid_t pid )
{
  int npids;

  do
  {
    int status = 0;

    waitpid( pid, &status, WNOHANG );
    /* mas_nanosleep( 2 ); */
    /* printf( "Wait Instances %d : %u\n", sect->instances, sect->pids ? sect->pids[0] : 0 ); */
    runonce_pids_reset(  );
    npids = runonce_config_section_get_pids( sect, flags );
  }
  while ( sect->instances );

  return npids;
}

int
runonce_waitoff_1( config_section_t * sect, runonce_flags_t flags )
{
  int npids;
  int haspid = 0;

  do
  {
    for ( int ipid = 0; ipid < sect->instances; ipid++ )
    {
      char psdir[64];
      struct stat st;

      snprintf( psdir, sizeof( psdir ), "/proc/%u", sect->pids[ipid] );
      haspid = ( 0 == stat( psdir, &st ) && S_ISDIR( st.st_mode ) );
      /* printf( "DIR %d - %s\n", haspid, psdir ); */
    }
  }
  while ( haspid );

  runonce_pids_reset(  );
  npids = runonce_config_section_get_pids( sect, flags );

  return npids;
}

int
runonce_scan( const char *grppatt, const char *sectpatt, runonce_flags_t flags, char *msg, ... )
{
  va_list args;

  /* char *action = NULL; */

  va_start( args, msg );
  for ( int ngr = 0; ngr < configuration.numgroups; ngr++ )
  {
    config_group_t *grp = NULL;

    grp = configuration.groups + ngr;
    /* printf( "ngr:%d\n", ngr ); */
    if ( !grppatt || strstr( grp->name, grppatt ) )
    {
      for ( int nsec = 0; nsec < grp->num_sections; nsec++ )
      {
        config_section_t *sect = grp->sections + nsec;

        if ( sect->name && sect->name[0] != '@' )
        {
          if ( !sectpatt || ( flags.strict && 0 == strcmp( sectpatt, sect->name ) ) || ( !flags.strict && strstr( sect->name, sectpatt ) ) )
          {
            int cnt = 0;
            va_list ids;
            int done = 0;

            /* printf( "(strict:%d) nsec:%d %s ? %s\n", flags.strict, nsec, sectpatt, sect->name ); */

            va_copy( ids, args );
            while ( !done )
            {
              roaction_t id;

              id = va_arg( ids, roaction_t );

              /* printf( "id:%d\n", id ); */

              if ( id == ROACTION_NONE )
                break;
              /* if ( grp->sections && *sect->name != '@' ) */
              {
                switch ( id )
                {
                case ROACTION_LAUNCH:
                  {
                    const char *command = sect->values[RUNONCE_COMMAND];
                    int canlaunch = 1;

                    done = __LINE__;
                    if ( !sect->instances && !sect->values[RUNONCE_NOLAUNCH] )
                    {
                      printf( "============LAUNCH== %d : %15s : %18s - command:[%s] {%d}\n", id, grp->name, sect->name, command,
                              sect->largc );
                      if ( canlaunch )
                      {
                        pid_t child;

                        child = fork(  );
                        if ( child )
                        {
                          int status = 0;

                          /* runonce_waiton( sect, flags ); */
                          waitpid( child, &status, WNOHANG );
                          printf( "A waitpid status: %d\n", status );
                          /* sleep( 3 );                                 */
                          /* waitpid( child, &status, WNOHANG );         */
                          /* printf( "B waitpid status: %d\n", status ); */
                          /* waitpid( child, &status, 0 ); */
                        }
                        else
                        {
                          int bad = 0;

                          if ( sect->values[RUNONCE_NICE] && !sect->values[RUNONCE_NONICE] )
                          {
                            char buf[256];
                            int inice = 0;

                            sscanf( sect->values[RUNONCE_NICE], "%d", &inice );
                            bad = nice( inice );
                            if ( bad < 0 )
                              printf( "NICE error %s : %s\n", sect->values[RUNONCE_NICE], strerror_r( errno, buf, sizeof( buf ) ) );
                            /* else if ( bad )                                                                                                    */
                            /*   printf( "(%d) can't NICE %s : %s\n", bad, sect->values[RUNONCE_NICE], strerror_r( errno, buf, sizeof( buf ) ) ); */
                          }
                          if ( sect->values[RUNONCE_XGROUP] && !sect->values[RUNONCE_NOGROUP] )
                          {
                            struct group *gr;

                            gr = getgrnam( sect->values[RUNONCE_XGROUP] );
                            if ( gr )
                              bad = setgid( gr->gr_gid );
                            else
                              bad = -1;
                            if ( bad < 0 )
                              printf( "can't SETGID %s\n", sect->values[RUNONCE_XGROUP] );
                          }
                          {
                            int foutd = -1;
                            char *stdout_filename = "/tmp/runonce_stdout.tmp";

                            if ( stdout_filename )
                            {
                              foutd = mas_open( stdout_filename, O_CREAT | O_WRONLY | O_APPEND, 0777 );
                              dup2( foutd, STDOUT_FILENO );
                              mas_close( foutd );
                            }
                          }
                          {
                            int ferrd = -1;
                            char *stderr_filename = "/tmp/runonce_stderr.tmp";

                            if ( stderr_filename )
                            {
                              ferrd = mas_open( stderr_filename, O_CREAT | O_WRONLY | O_APPEND, 0777 );
                              dup2( ferrd, STDERR_FILENO );
                              mas_close( ferrd );
                            }
                            setvbuf( stderr, NULL, _IONBF, 0 );
                          }

                          setsid(  );
                          /* chdir( "/" ); */
                          /* execvpe( sect->name, sect->largv, NULL ); */
                          if ( flags.dry )
                          {
                            char *command = mas_argv_string( sect->largc, sect->largv, 0 );

                            printf( "DRY 'execvp(%s, %s)'\n", sect->name, command );
                            mas_free( command );
                          }
                          else if ( bad < 0 )
                          {
                            char *command = mas_argv_string( sect->largc, sect->largv, 0 );

                            printf( "ERROR 'execvp(%s, %s)'\n", sect->name, command );
                            mas_free( command );
                          }
                          else
                            execvp( sect->name, sect->largv );
                          printf( "EXEC FAIL\n" );
                        }
                      }
                    }
                  }
                  break;
                case ROACTION_TERMINATE:
                  {
                    int signal = 0;
                    const char *signame = sect->values[RUNONCE_PFIN];

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
                        printf( "============TERM==== %d : %15s : %18s (%s:%d)\n", id, grp->name, sect->name,
                                sect->values[RUNONCE_PFIN], signal );
                        for ( int ipid = 0; ipid < sect->instances; ipid++ )
                          if ( flags.dry )
                          {
                            printf( "DRY 'kill -%d %d'\n", signal, sect->pids[ipid] );
                          }
                          else
                          {
                            printf( "'kill -%d %d'\n", signal, sect->pids[ipid] );
                            kill( sect->pids[ipid], signal );
                          }
                      }
                      else
                      {
                      }
                    }
                  }
                  break;
                case ROACTION_EXIT:
                  {
                    if ( !sect->values[RUNONCE_NOEXIT] && !sect->values[RUNONCE_NOSTOP] && sect->values[RUNONCE_QUITTER] )
                    {
                      done = __LINE__;
                      printf( "============EXIT==== %d : %15s : %18s {%d instances}\n", id, grp->name, sect->name, sect->instances );
                      if ( sect->instances )
                      {
                        if ( flags.dry )
                        {
                          printf( "%d DRY %s [%s:%s] ::\n", done, sect->name, sect->values[RUNONCE_NOEXIT], sect->values[RUNONCE_NOSTOP] );
                          for ( int ia = 0; ia < sect->qargc; ia++ )
                            printf( "%d: %s\n", ia, sect->qargv[ia] );
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

                            runonce_waitoff( sect, flags, child );
                            /* waitpid( child, &status, WNOHANG ); */
                            waitpid( child, &status, 0 );
                          }
                          else
                          {
                            execvp( sect->qargv[0], sect->qargv );
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
                  }
                  break;
                case ROACTION_STOP:
                case ROACTION_NONE:
                  printf( "============NONE==== %d : %15s : %18s\n", id, grp->name, sect->name );
                  break;
                }
              }
              printf( "-------------------------------id:%d done:%d; cnt:%d\n", id, done, cnt );
              cnt++;
            }
            if ( !done )
            {
              printf( "nothing can be done for %s\n", sect->name );
            }
            va_end( ids );
          }
          else
          {
            /* printf( "didn't match section %s (strict:%d; sectpatt:%s)\n", sect->name, flags.strict, sectpatt ); */
          }
        }
      }
    }
    else
    {
      /* printf( "didn't match group %s (strict:%d; grppatt:%s)\n", grp->name, flags.strict, grppatt ); */
    }
  }
  va_end( args );
  /* return action; */
  return 0;
}

int
runonce_config_section_get_pids( config_section_t * sect, runonce_flags_t flags )
{
  int npids = 0;
  int targc = 0;
  char **targv = NULL;

  if ( sect->values[RUNONCE_OPTS4PID] && sect->values[RUNONCE_LAUNCHER] && sect->values[RUNONCE_OPTIONS] )
  {
    targc = mas_add_argv_args( targc, &targv, sect->values[RUNONCE_LAUNCHER], 0 );
    targc = mas_add_argv_args( targc, &targv, sect->values[RUNONCE_OPTIONS], 0 );
  }
  {
    pid_t pids[512];

    if ( !npids )
    {
      char *pref = sect->values[RUNONCE_PREFIX];
      char *prog = sect->values[RUNONCE_LAUNCHER];
      char *path = sect->values[RUNONCE_PATH];

      if ( !pref )
      {
        pref = prog;
        prog = NULL;
      }
      npids = runonce_pidof( pids, sizeof( pids ) / sizeof( pids[0] ), pref, prog, path, targc, targv );
    }
    sect->instances = npids;
    mas_free( sect->pids );
    sect->pids = NULL;
    sect->pids = mas_malloc( npids * sizeof( pid_t ) );
    memcpy( sect->pids, pids, npids * sizeof( pid_t ) );
    mas_del_argv( targc, targv, 0 );
  }
  return npids;
}

int
runonce_get_pids( const char *grppatt, const char *sectpatt, runonce_flags_t flags )
{
  for ( int ngr = 0; ngr < configuration.numgroups; ngr++ )
  {
    config_group_t *grp = configuration.groups + ngr;

    /* printf( "? ngr:%d [%s:%s]\n", ngr, grp->name, grppatt ); */
    if ( !grppatt || strstr( grp->name, grppatt ) )
    {
      /* printf( "+ ngr:%d [%s:%s] %d\n", ngr, grp->name, grppatt, grp->num_sections ); */
      for ( int nsec = 0; nsec < grp->num_sections; nsec++ )
      {
        config_section_t *sect = grp->sections + nsec;

        /* printf( "nsec:%d %s ? %s\n", nsec, sectpatt, sect->name ); */
        if ( !sectpatt || ( flags.strict && 0 == strncmp( sectpatt, sect->name, strlen( sectpatt ) ) )
             || ( !flags.strict && strstr( sect->name, sectpatt ) ) )
        {
          if ( grp->sections && *sect->name != '@' )
          {
            int npids;

            npids = runonce_config_section_get_pids( sect, flags );
            if ( npids == 1 )
            {
              if ( flags.list_one )
                printf( "Running one: %s : %s\n", grp->name, sect->name );
            }
            else if ( npids )
            {
              if ( flags.list_multiple )
                printf( "Running %d instances: %s : %s\n", npids, grp->name, sect->name );
            }
            else
            {
              if ( flags.list_zero )
                printf( "Not running: %s : %s\n", grp->name, sect->name );
            }
          }
        }
      }
    }
  }
  return 0;
}

int
runonce_roaction( char *group_patt, char *sect_patt, roaction_t roaction, runonce_flags_t flags )
{
  printf( "group_patt:%s; sect_patt:%s (strict:%d)\n", group_patt, sect_patt, flags.strict );
  switch ( roaction )
  {
  case ROACTION_STOP:
    runonce_scan( group_patt, sect_patt, flags, NULL, ROACTION_EXIT, ROACTION_TERMINATE, ROACTION_NONE );
    break;
  default:
    if ( roaction )
      runonce_scan( group_patt, sect_patt, flags, NULL, roaction, ROACTION_NONE );
    break;
  }
  return 0;
}

int
main( int argc, char *argv[] )
{
  int errorcnt = 0;
  char *group_patt = NULL;
  roaction_t roaction;
  runonce_flags_t flags;

  runonce_create(  );
  runonce_config_load(  );

  {
    int opt;

    opterr = 0;
    while ( ( opt = getopt_long( argc, argv, "DLZM1Sslf:", NULL, NULL ) ) >= 0 )
    {
      /* printf( "OPT %c %s\n", opt, optarg ); */
      switch ( opt )
      {
      case 'l':
        roaction = ROACTION_LAUNCH;
        break;
      case 's':
        roaction = ROACTION_STOP;
        break;
      case 'S':
        flags.strict = 1;
        break;
      case 'L':
        flags.list_zero = 1;
        flags.list_multiple = 1;
        flags.list_one = 1;
        break;
      case '1':
        flags.list_one = 1;
        break;
      case 'Z':
        flags.list_zero = 1;
        break;
      case 'M':
        flags.list_multiple = 1;
        break;
      case 'D':
        flags.dry = 1;
        break;
      case 'f':
        if ( optarg )
          group_patt = mas_strdup( optarg );
        break;
      case '?':
        printf( "Invalid option -- '%c'\n", optopt );
        errorcnt++;
        break;
      }
    }
    /* printf( "ZERO:%d ONE:%d MULT:%d\n", flags.list_zero, flags.list_one, flags.list_multiple ); */
    if ( !errorcnt )
    {
      /* runonce_test( argc, argv ); */
      /* printf( "[[ %s ]] %d:%d\n", argv[0], argc, optind ); */
      runonce_get_pids( NULL, NULL, flags );
      if ( optind >= argc )
      {
        runonce_roaction( group_patt, NULL, roaction, flags );
      }
      else
      {
        for ( int ia = optind; ia < argc; ia++ )
        {
          runonce_roaction( group_patt, argv[ia], roaction, flags );
        }
      }
    }
    else
    {
      printf( "ERROR\n" );
    }
  }
  mas_free( group_patt );
  runonce_delete(  );
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif

  /* sleep( 600 ); */
  return 0;
}
