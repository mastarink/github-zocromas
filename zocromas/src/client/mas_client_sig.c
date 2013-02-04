#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <signal.h>

#include <mastar/wrap/mas_memory.h>

#include "mas_common.h"
#include "zoctools/inc/mas_lib_thread.h"

#include "mas_init_client.h"

#include "mas_client.h"


/*
this:
  mas_client_sig.c
related:
  mas_client_readline.c
  mas_client.c
  mas_client_main.c

  mas_sig.c
  mas_server_sig.c
*/



/* pthread_t *listening_threads = NULL; */

/* typedef void ( *sighandler_t ) ( int ); */
/* sighandler_t signal( int signum, sighandler_t handler ); */

void
sigint_han( int s )
{
  /* extern int rl_done; */
  static unsigned long start_time = 0;
  unsigned long cur_time = ( unsigned long ) time( NULL );

  if ( !start_time || cur_time - start_time > 5 )
  {
    start_time = ( unsigned long ) time( NULL );
    ctrl.int_cnt = 0;
  }
  fprintf( stderr, "INT %d of %d; i/c:%d; i/p:%d k/l:%u\n", ctrl.int_cnt, MAS_MAX_INT_2, ctrl.in_client, ctrl.in_pipe,
           ctrl.keep_listening );
/*
  if (pthread_equal( pthread_self(  ),ctrl.pth ))
  {
    pthread_t *pt;

    pt = listening_threads;
    while ( pt && *pt )
    {
      GMSG( "INT to %lx @ %lx", ( unsigned long ) *pt, pthread_self(  ) );
      pthread_kill( *pt, SIGINT );
      pt++;
    }
  }
  */
  ctrl.int_cnt++;
  if ( ctrl.int_cnt > MAS_MAX_INT_1 )
  {
    ctrl.in_client = 0;
    ctrl.keep_listening = 0;
    ctrl.in_pipe--;
  }
  /* RL_SETSTATE( RL_STATE_DONE ); */
  if ( ctrl.int_cnt > MAS_MAX_INT_2 )
  {
    exit( 3 );
  }
  /* rl_done = 1;             */
  /* rl_reset_after_signal(); */
}

void
sigquit_han( int s )
{
  ctrl.in_client = 0;
  ctrl.keep_listening = 0;
  ctrl.in_pipe--;
  EMSG( "QUIT%s", "." );
}

void
sigabrt_han( int s )
{
  ctrl.in_client = 0;
  ctrl.keep_listening = 0;
  ctrl.in_pipe--;
  EMSG( "ABRT%s", "." );
}

void
sigterm_han( int s )
{
  EMSG( "TERM %d", ctrl.term_cnt );
  ctrl.term_cnt++;
  ctrl.keep_listening = 0;
  ctrl.in_client = 0;
  ctrl.in_pipe--;
}

void
sighup_han( int s )
{
  EMSG( "HUP" );
  ctrl.in_client = 0;
}

void
sigpipe_han( int s )
{
  EMSG( "PIPE" );
  /* ctrl.in_client = 1; */
  /* ctrl.in_pipe--; */
}

void
mas_atexit( void )
{
  HMSG( "at exit" );
  /* mas_destroy(  ); */

  mas_destroy_client(  );
  FMSG( "\n\n\nAt exit, memory_balance:%lu", memory_balance );
#ifdef MAS_TRACEMEM
  print_memlist( FL );
#endif
  if ( ctrl.saved_stderr_file )
  {
    fclose( ctrl.saved_stderr_file );
    ctrl.saved_stderr_file = NULL;
  }
}
