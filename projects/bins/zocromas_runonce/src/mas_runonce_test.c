#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "mas_runonce_config_types.h"

/* #include "mas_runonce_config.h" */
/* #include "mas_runonce_test.h"   */
/* #include "mas_runonce_base.h"   */
#include "mas_runonce_pid.h"
#include "mas_runonce.h"


int
runonce_test( int argc, char *argv[] )
{

  /* for ( unsigned i = 0; i < pidarray_size; i++ )                                                             */
  /* {                                                                                                  */
  /*   if ( pidarray[i].progname )                                                                      */
  /*   {                                                                                                */
  /*     printf( "%u@%u: [%s(%s)]\n", i, pidarray[i].ppid, pidarray[i].progname, pidarray[i].cmdline ); */
  /*   }                                                                                                */
  /* }                                                                                                  */

  printf( "--------------------------------------------------------------------------------\n" );
  {
    pid_t pids[512];
    int n;


    {
      char *a[] = { "wmifinfo", "-i", "enp2s0" };
      n = runonce_pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "wmifinfo", NULL, NULL, 3, a );
      for ( int i = 0; i < n; i++ )
        printf( "%d: PID=%u\n", i, pids[i] );
      printf( "-wmifinfo -i enp2s0 %d pids -------------------------------------------------------------------------------\n", n );
    }
    {
      char *a[] = { "wmifinfo", "-i", "enp3s0" };
      n = runonce_pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "wmifinfo", NULL, NULL, 3, a );
      for ( int i = 0; i < n; i++ )
        printf( "%d: PID=%u\n", i, pids[i] );
      printf( "-wmifinfo -i enp3s0 %d pids -------------------------------------------------------------------------------\n", n );
    }
    {
      char *a[] = { "wmifinfo", "-i", "enp4s0" };
      n = runonce_pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "wmifinfo", NULL, NULL, 3, a );
      for ( int i = 0; i < n; i++ )
        printf( "%d: PID=%u\n", i, pids[i] );
      printf( "-wmifinfo -i enp4s0 %d pids -------------------------------------------------------------------------------\n", n );
    }
    return 0;
    n = runonce_pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "python2.7", "pamusb-agent", "/usr/bin/", 0, NULL );
    for ( int i = 0; i < n; i++ )
      printf( "%d: PID=%u\n", i, pids[i] );
    printf( "-speech-dispatcher %d pids -------------------------------------------------------------------------------\n", n );

    n = runonce_pidof( pids, sizeof( pids ) / sizeof( pids[0] ), NULL, "speech-dispatcher", "/usr/bin/", 0, NULL );
    for ( int i = 0; i < n; i++ )
      printf( "%d: PID=%u\n", i, pids[i] );
    printf( "-speech-dispatcher %d pids -------------------------------------------------------------------------------\n", n );

    n = runonce_pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "speech-dispatcher", NULL, "/usr/bin/", 0, NULL );
    for ( int i = 0; i < n; i++ )
      printf( "%d: PID=%u\n", i, pids[i] );
    printf( "-speech-dispatcher %d pids -------------------------------------------------------------------------------\n", n );

    n = runonce_pidof( pids, sizeof( pids ) / sizeof( pids[0] ), "python2.7", "rednotebook", NULL, 0, NULL );
    for ( int i = 0; i < n; i++ )
      printf( "%d: PID=%u\n", i, pids[i] );
    printf( "-rednotebook %d pids -------------------------------------------------------------------------------\n", n );
  }
  return 0;
}
