#include "mas_server_def.h"
#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <pthread.h>
#include <signal.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/thtools/mas_thread_tools.h>


/* mas_destroy */
#include "mas_init_server.h"

#include "listener/inc/mas_listeners.h"

#include "mas_sig.h"
#include "mas_server_sig.h"

/*
this:
  mas_server_sig.c
related:
  mas_sig.c
  mas_client_sig.c

  mas_watcher.c

*/



int term_cnt = 0;
int try_cnt = 0;

/* typedef void ( *sighandler_t ) ( int ); */
/* sighandler_t signal( int signum, sighandler_t handler ); */


void
sigterm_han( int s )
{
  GDMSG( "TERM %d", term_cnt );
  term_cnt++;
  ctrl.keep_listening = 0;
  ctrl.watcher_stop = 1;
  ctrl.in_client = 0;
  ctrl.in_pipe--;
}

static void
info( FILE * ftinfo )
{
  unsigned ith;
  mas_lcontrol_t *plcontrol;

  fprintf( ftinfo, "\n" );
  fprintf( ftinfo,
           "main_thread: %u [%lx]\nmaster_thread: %u [%lx]\nticker_thread: %u [%lx]\nlogger_thread: %u [%lx]\nwatcher_thread: %u [%lx]\n",
           ctrl.main_tid, ctrl.main_thread, ctrl.master_tid, ctrl.master_thread, ctrl.ticker_tid, ctrl.ticker_thread,
           ctrl.logger_tid, ctrl.logger_thread, ctrl.watcher_tid, ctrl.watcher_thread );
  fprintf( ftinfo,
           "Server info:\n\tclients: {%lu - %lu = %lu}\n" "\tlogdir: %s;\tlogpath: %s;\n"
           "\tserver; pid:%u; \t\ttid:%5u/%4x; [%lx]\n", ctrl.clients_came, ctrl.clients_gone,
           ctrl.clients_came - ctrl.clients_gone, opts.logdir, ctrl.logpath, ctrl.main_pid, ctrl.main_tid, ctrl.main_tid,
           ctrl.main_thread );
  ith = 0;
  /* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); */
  pthread_rwlock_rdlock( &ctrl.thglob.lcontrols_list_rwlock );
  MAS_LIST_FOREACH( plcontrol, ctrl.lcontrols_list, next )
  {
    fprintf( ftinfo, "\t\t%u. %20s:%-6u; \ttid:%5u/%4x; [%lx] {%lu - %lu = %lu} %s\n", ith,
             plcontrol->host, plcontrol->port, plcontrol->h.tid, plcontrol->h.tid, plcontrol->h.thread, plcontrol->clients_came,
             plcontrol->clients_gone, plcontrol->clients_came - plcontrol->clients_gone,
             plcontrol->h.pchannel ? "listening" : "closed" );
    {
      unsigned itr;
      mas_rcontrol_t *prcontrol;

      /* cMSG( "srv S" ); */
      itr = 0;
      /* pthread_mutex_lock( &plcontrol->transaction_mutex ); */
      pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );
      MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )
      {
        mas_channel_t *pchannel = NULL;
        char *sip = NULL;

        struct timeval now_time;
        struct timeval inactive_time;

        gettimeofday( &now_time, NULL );
        inactive_time.tv_sec = now_time.tv_sec - prcontrol->h.activity_time.tv_sec;
        inactive_time.tv_usec = now_time.tv_usec - prcontrol->h.activity_time.tv_usec;
        if ( inactive_time.tv_usec < 0 )
        {
          inactive_time.tv_usec += 1000000;
          inactive_time.tv_sec -= 1;
        }
        /* cMSG( "srv L %p", prcontrol ); */

        pchannel = prcontrol ? prcontrol->h.pchannel : NULL;
        /* if ( pchannel )                            */
        /*   sip = mas_channel_ip_string( pchannel ); */
        if ( pchannel )
          sip = mas_ip_string( &pchannel->serv.addr.sin_addr );

        fprintf( ftinfo, "\t\t\t%u. %s:%u \ttid:%5u/%4x; [%lx] I/A %ld.%-ld #%u\n", itr, sip ? sip : "?",
                 pchannel ? pchannel->port : 0, prcontrol->h.tid, prcontrol->h.tid, prcontrol->h.thread, inactive_time.tv_sec,
                 inactive_time.tv_usec, prcontrol->xch_cnt );
        if ( sip )
          mas_free( sip );
        itr++;
      }
      pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
      /* pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
      /* cMSG( "srv E" ); */
    }
    ith++;
  }
  fprintf( ftinfo, "ith:%d\n", ith );
  pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
  /* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); */
}


void
sigint_han( int s )
{
  static int int_cnt = 0;
  static unsigned long prev_time = 0;
  unsigned long cur_time;

  cur_time = ( unsigned long ) time( NULL );
  if ( !prev_time || cur_time - prev_time > 2 )
    int_cnt = 0;
/*
  if (pthread_equal( mas_pthread_self(  ),ctrl.pth ))
  {
    pthread_t *pt;

    pt = ctrl.listening_threads;
    while ( pt && *pt )
    {
      EMSG( "INT to %lx @ %lx", ( unsigned long ) *pt, mas_pthread_self(  ) );
      pthread_kill( *pt, SIGINT );
      pt++;
    }
  }
  */
  int_cnt++;
  /* RL_SETSTATE( RL_STATE_DONE ); */
  if ( int_cnt > MAS_MAX_INT_2 )
  {
    if ( ctrl.keep_listening )
    {
      GDMSG( "unset keep_listening" );
    }
    ctrl.keep_listening = 0;
    ctrl.watcher_stop = 1;
    mas_listeners_stop(  );
    thMSG( "waiting at %lx", mas_pthread_self(  ) );

    /* sleep( 10 ); */
    /* exit( 3 ); */
  }
  if ( ctrl.daemon && ctrl.saved_stderr_file )
  {
    fprintf( ctrl.saved_stderr_file, "INT %d of %d", int_cnt, MAS_MAX_INT_2 );
    info( ctrl.saved_stderr_file );
  }
  else
  {
    fprintf( stderr, "INT %d of %d [%lx] [%lx]", int_cnt, MAS_MAX_INT_2, mas_pthread_self(  ), ctrl.main_thread );
    info( stderr );
  }
  prev_time = ( unsigned long ) time( NULL );
}

void
sighup_han( int s )
{
  if ( ctrl.keep_listening )
  {
    GDMSG( "unset keep_listening" );
  }
  ctrl.keep_listening = 0;
  ctrl.watcher_stop = 1;
  mas_listeners_cancel(  );
  thMSG( "waiting at %lx", mas_pthread_self(  ) );
  EMSG( "HUP" );
}

void
sigquit_han( int s )
{
  if ( ctrl.keep_listening )
  {
    GDMSG( "unset keep_listening" );
  }
  ctrl.keep_listening = 0;
  ctrl.watcher_stop = 1;
  mas_listeners_cancel(  );
  thMSG( "waiting at %lx", mas_pthread_self(  ) );
  EMSG( "QUIT%s", "." );
}

void
sigabrt_han( int s )
{
  EMSG( "ABRT" );
  ctrl.keep_listening = 0;
  ctrl.watcher_stop = 1;
  /*
     if ( pthread_equal( mas_pthread_self(  ), ctrl.pth ) )
     {
     pthread_t *pt;

     pt = ctrl.listening_threads;
     while ( pt && *pt )
     {
     GMSG( "HUP to %lx @ %lx", ( unsigned long ) *pt, mas_pthread_self(  ) );
     // pthread_kill( *pt, SIGHUP );
     pthread_cancel( *pt );
     pt++;
     }
     }
   */
}

void
sigpipe_han( int s )
{
  /* EMSG( "PIPE" ); */
  /*
     if ( pthread_equal( mas_pthread_self(  ), ctrl.pth ) )
     {
     pthread_t *pt;

     pt = ctrl.listening_threads;
     while ( pt && *pt )
     {
     GMSG( "PIPE to %lx @ %lx", ( unsigned long ) *pt, mas_pthread_self(  ) );
     pthread_kill( *pt, SIGPIPE );
     pt++;
     }
     }
   */
}

void
mas_atexit( void )
{
  extern unsigned long memory_balance;

  FMSG( "\nAT EXIT\n" );
  mas_destroy_server(  );
  FMSG( "\n\n\nAT EXIT, memory_balance:%ld", memory_balance );
#ifdef MAS_TRACEMEM
  print_memlist( FL );
#endif
}
