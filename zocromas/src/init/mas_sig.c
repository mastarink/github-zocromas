#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <signal.h>

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include "mas_sig.h"


/*
this:
  mas_sig.c
related:
  mas_server_sig.c
  mas_client_sig.c
*/



void
sigwinch_han( int s )
{
  GMSG( "WINCH" );
}

void
sigusr1_han( int s )
{
  GDMSG( "USR1" );
}
void
sigusr2_han( int s )
{
  GDMSG( "USR2" );
}


/* extern pthread_t *listening_threads; */



int
mas_init_sig( void )
{
#if 0
  signal( SIGPIPE, sigpipe_han );
  signal( SIGHUP, sighup_han );
  signal( SIGWINCH, sigwinch_han );
  signal( SIGTERM, sigterm_han );
  signal( SIGINT, sigint_han );
  signal( SIGQUIT, sigquit_han );
  signal( SIGUSR1, sigusr1_han );
  signal( SIGPOLL, sigpoll_han );
#else
  /* struct sigaction                                       */
  /* {                                                      */
  /*   void ( *sa_handler ) ( int );                        */
  /*   void ( *sa_sigaction ) ( int, siginfo_t *, void * ); */
  /*   sigset_t sa_mask;                                    */
  /*   int sa_flags;                                        */
  /*   void ( *sa_restorer ) ( void );                      */
  /* };                                                     */


  static struct sigaction sa_int = {
    .sa_handler = sigint_han
  };
  static struct sigaction sa_hup = {
    .sa_handler = sighup_han
  };
  static struct sigaction sa_pipe = {
    .sa_handler = sigpipe_han
  };
  static struct sigaction sa_term = {
    .sa_handler = sigterm_han
  };
  static struct sigaction sa_abrt = {
    .sa_handler = sigabrt_han
  };
  static struct sigaction sa_quit = {
    .sa_handler = sigquit_han
  };
  static struct sigaction sa_usr1 = {
    .sa_handler = sigusr1_han
  };
  static struct sigaction sa_usr2 = {
    .sa_handler = sigusr2_han
  };
#  if 0
  static sigset_t sset;

  sigemptyset( &sset );
  sigaddset( &sset, SIGINT );
  sigaddset( &sset, SIGHUP );
  /* sigaddset( &sset, SIGWINCH ); */
  sigaddset( &sset, SIGTERM );
  sigaddset( &sset, SIGPIPE );
  sigaddset( &sset, SIGUSR1 );
  sigaddset( &sset, SIGPOLL );
#  endif

  /* sigemptyset( &oset ); */
  /* sigprocmask( SIG_BLOCK, &sset, &oset ); */
  sigaction( SIGINT, &sa_int, NULL );
  sigaction( SIGHUP, &sa_hup, NULL );
  sigaction( SIGTERM, &sa_term, NULL );
  sigaction( SIGABRT, &sa_abrt, NULL );
  sigaction( SIGPIPE, &sa_pipe, NULL );
  sigaction( SIGUSR1, &sa_usr1, NULL );
  sigaction( SIGUSR2, &sa_usr2, NULL );
  sigaction( SIGQUIT, &sa_quit, NULL );
  /* sigaction( SIGHUP, SIG_DFL, NULL ); */
  /* sigaction( SIGTERM, SIG_DFL, NULL ); */
#endif
  return 0;
}

void
mas_dfl_sig( void )
{
  /* signal( SIGPIPE, SIG_DFL );  */
  /* signal( SIGHUP, SIG_DFL );   */
  /* signal( SIGWINCH, SIG_DFL ); */
  /* signal( SIGTERM, SIG_DFL );  */
  /* signal( SIGINT, SIG_DFL );   */
  /* signal( SIGQUIT, SIG_DFL );  */
  /* signal( SIGUSR1, SIG_DFL );  */
  /* signal( SIGPOLL, SIG_DFL );  */
}
