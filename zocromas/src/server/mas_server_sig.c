#include "mas_server_def.h"
#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>
#include <signal.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/thtools/mas_thread_tools.h>
#include <mastar/modules/mas_modules_commands_eval.h>

/* mas_destroy */
#include "mas_init_server.h"

#include "listener/inc/mas_listeners.h"

#include "mas_server_sig.h"

/*
this:
  mas_server_sig.c
related:
  mas_sig.c
  mas_client_sig.c

  mas_watcher.c

*/



static int term_cnt = 0;

/* static int try_cnt = 0; */

/* typedef void ( *sighandler_t ) ( int ); */
/* sighandler_t signal( int signum, sighandler_t handler ); */

static void
to_quit( void )
{
  if ( ctrl.keep_listening )
  {
    GDMSG( "unset keep_listening" );
  }
  ctrl.keep_listening = 0;
  ctrl.watcher_stop = 1;
  /* mas_listeners_stop(  ); */
  ctrl.stop_listeners = 1;
}

void
sigterm_han( int s )
{
  GDMSG( "TERM %d", term_cnt );
  term_cnt++;
  to_quit(  );
  ctrl.in_client = 0;
  ctrl.in_pipe--;
}

/* static void                                                                                                                                */
/* show_info( FILE * ftinfo )                                                                                                                 */
/* {                                                                                                                                          */
/*   unsigned ith;                                                                                                                            */
/*   mas_lcontrol_t *plcontrol;                                                                                                               */
/*                                                                                                                                            */
/*   fprintf( ftinfo, "\n" );                                                                                                                 */
/*   fprintf( ftinfo,                                                                                                                         */
/*            "main_thread: %u [%lx]\nmaster_thread: %u [%lx]\nticker_thread: %u [%lx]\nlogger_thread: %u [%lx]\nwatcher_thread: %u [%lx]\n", */
/*            ctrl.main_tid, ctrl.main_thread, ctrl.master_tid, ctrl.master_thread, ctrl.ticker_tid, ctrl.ticker_thread,                      */
/*            ctrl.logger_tid, ctrl.logger_thread, ctrl.watcher_tid, ctrl.watcher_thread );                                                   */
/*   fprintf( ftinfo,                                                                                                                         */
/*            "Server info:\n\tclients: {%lu - %lu = %lu}\n" "\tlogdir: %s;\tlogpath: %s;\n"                                                  */
/*            "\tserver; pid:%u; \t\ttid:%5u/%4x; [%lx]\n", ctrl.clients_came, ctrl.clients_gone,                                             */
/*            ctrl.clients_came - ctrl.clients_gone, opts.logdir, ctrl.logpath, ctrl.main_pid, ctrl.main_tid, ctrl.main_tid,                  */
/*            ctrl.main_thread );                                                                                                             */
/*   ith = 0;                                                                                                                                 */
/*   (* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); *)                                                                           */
/*   pthread_rwlock_rdlock( &ctrl.thglob.lcontrols_list_rwlock );                                                                             */
/*   MAS_LIST_FOREACH( plcontrol, ctrl.lcontrols_list, next )                                                                                 */
/*   {                                                                                                                                        */
/*     fprintf( ftinfo, "\t\t%u. %20s:%-6u; \ttid:%5u/%4x; [%lx] {%lu - %lu = %lu} %s\n", ith,                                                */
/*              plcontrol->host, plcontrol->port, plcontrol->h.tid, plcontrol->h.tid, plcontrol->h.thread, plcontrol->clients_came,           */
/*              plcontrol->clients_gone, plcontrol->clients_came - plcontrol->clients_gone, plcontrol->h.pchannel ? "listening" : "closed" ); */
/*     {                                                                                                                                      */
/*       unsigned itr;                                                                                                                        */
/*       mas_rcontrol_t *prcontrol;                                                                                                           */
/*                                                                                                                                            */
/*       (* cMSG( "srv S" ); *)                                                                                                               */
/*       itr = 0;                                                                                                                             */
/*       (* pthread_mutex_lock( &plcontrol->transaction_mutex ); *)                                                                           */
/*       pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );                                                                             */
/*       MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )                                                            */
/*       {                                                                                                                                    */
/*         mas_channel_t *pchannel = NULL;                                                                                                    */
/*         char *sip = NULL;                                                                                                                  */
/*                                                                                                                                            */
/*         struct timeval now_time;                                                                                                           */
/*         struct timeval inactive_time;                                                                                                      */
/*                                                                                                                                            */
/*         gettimeofday( &now_time, NULL );                                                                                                   */
/*         inactive_time.tv_sec = now_time.tv_sec - prcontrol->h.activity_time.tv_sec;                                                        */
/*         inactive_time.tv_usec = now_time.tv_usec - prcontrol->h.activity_time.tv_usec;                                                     */
/*         if ( inactive_time.tv_usec < 0 )                                                                                                   */
/*         {                                                                                                                                  */
/*           inactive_time.tv_usec += 1000000;                                                                                                */
/*           inactive_time.tv_sec -= 1;                                                                                                       */
/*         }                                                                                                                                  */
/*         (* cMSG( "srv L %p", prcontrol ); *)                                                                                               */
/*                                                                                                                                            */
/*         pchannel = prcontrol ? prcontrol->h.pchannel : NULL;                                                                               */
/*         (* if ( pchannel )                            *)                                                                                   */
/*         (*   sip = mas_channel_ip_string( pchannel ); *)                                                                                   */
/*         if ( pchannel )                                                                                                                    */
/*           sip = mas_ip_string( &pchannel->serv.addr.sin_addr );                                                                            */
/*                                                                                                                                            */
/*         fprintf( ftinfo, "\t\t\t%u. %s:%u \ttid:%5u/%4x; [%lx] I/A %ld.%-ld #%u\n", itr, sip ? sip : "?",                                  */
/*                  pchannel ? pchannel->port : 0, prcontrol->h.tid, prcontrol->h.tid, prcontrol->h.thread, inactive_time.tv_sec,             */
/*                  inactive_time.tv_usec, prcontrol->xch_cnt );                                                                              */
/*         if ( sip )                                                                                                                         */
/*           mas_free( sip );                                                                                                                 */
/*         itr++;                                                                                                                             */
/*       }                                                                                                                                    */
/*       pthread_rwlock_unlock( &plcontrol->transaction_rwlock );                                                                             */
/*       (* pthread_mutex_unlock( &plcontrol->transaction_mutex ); *)                                                                         */
/*       (* cMSG( "srv E" ); *)                                                                                                               */
/*     }                                                                                                                                      */
/*     ith++;                                                                                                                                 */
/*   }                                                                                                                                        */
/*   fprintf( ftinfo, "ith:%d\n", ith );                                                                                                      */
/*   pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );                                                                             */
/*   (* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); *)                                                                         */
/* }                                                                                                                                          */


void
sigint_han( int s )
{
  static int int_cnt = 0;

  if ( !ctrl.sigint_time || ( unsigned long ) time( NULL ) - ctrl.sigint_time > 2 )
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
    to_quit(  );

    thMSG( "waiting at %lx", mas_pthread_self(  ) );

    /* sleep( 10 ); */
    /* exit( 3 ); */
  }
  /* HMSG( "DAEMON -%d +%d", opts.nodaemon, ctrl.daemon ); */
  if ( ctrl.daemon && ctrl.saved_stderr_file )
  {
    int fr = -1;

    if ( int_cnt < 2 )
    {
      char *infos = NULL;

      infos = mas_evaluate_command( "server info" );
      fputs( infos, stderr );
      /* show_info( ctrl.saved_stderr_file ); */
      mas_free( infos );
    }
    fr = fprintf( ctrl.saved_stderr_file, "\n\nINT %d of %d", int_cnt, MAS_MAX_INT_2 );
    fflush(ctrl.saved_stderr_file);
    /* HMSG( "(%d) DAEMON -%d +%d; fr:%d", int_cnt, opts.nodaemon, ctrl.daemon, fr ); */
  }
  else
  {
    if ( int_cnt < 2 )
    {
      char *infos = NULL;

      infos = mas_evaluate_command( "server info" );
      fputs( infos, stderr );
      /* show_info( ctrl.saved_stderr_file ); */
      mas_free( infos );
    }
    fprintf( stderr, "INT %d of %d [%lx] [%lx]\x1b[K\r", int_cnt, MAS_MAX_INT_2, mas_pthread_self(  ), ctrl.main_thread );
  }
  ctrl.sigint_time = ( unsigned long ) time( NULL );
}

void
sighup_han( int s )
{
  ctrl.restart = 1;
  to_quit(  );
  thMSG( "waiting at %lx", mas_pthread_self(  ) );
  EMSG( "HUP" );
}

void
sigquit_han( int s )
{
  to_quit(  );
  thMSG( "waiting at %lx", mas_pthread_self(  ) );
  EMSG( "QUIT" );
}

void
sigabrt_han( int s )
{
  to_quit(  );
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
  EMSG( "ABRT" );
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
  mas_destroy_server(  );
#ifdef MAS_TRACEMEM
  extern unsigned long memory_balance;

  FMSG( "AT EXIT, memory_balance:%ld", memory_balance );
  print_memlist( FL );
#else
  FMSG( "AT EXIT" );
#endif
  FMSG( "=====[%u @ %u]=================================================================", getpid(  ), getppid(  ) );
  if ( ctrl.msgfile )
  {
    fclose( ctrl.msgfile );
    ctrl.msgfile = NULL;
  }
  _exit( 0 );
}
