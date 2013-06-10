#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <signal.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>



/*
this:
  mas_direct_sig.c
related:
  mas_direct.c
  mas_client_sig.c
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
  exit( 3 );
}

void
sigquit_han( int s )
{
#ifdef EMSG
  EMSG( "QUIT%s", "." );
#endif
}

void
sigterm_han( int s )
{
#ifdef EMSG
  EMSG( "TERM" );
#endif
}

void
sighup_han( int s )
{
#ifdef EMSG
  EMSG( "HUP" );
#endif
}

void
sigpipe_han( int s )
{
#ifdef EMSG
  EMSG( "PIPE" );
#endif
}

void
mas_atexit( void )
{
#ifdef MAS_TRACEMEM
  extern unsigned long memory_balance;

  FMSG( "AT EXIT, memory_balance:%ld", memory_balance );
#else
  FMSG( "AT EXIT" );
#endif
  FMSG( "======================================================================" );
}

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  atexit( mas_atexit );
  fprintf( stderr, "******************** CONSTRUCTOR %s\n", __FILE__ );
}

__attribute__ ( ( destructor ) )
     static void master_destructor( void )
{
  fprintf( stderr, "******************** DESTRUCTOR %s\n", __FILE__ );
}
