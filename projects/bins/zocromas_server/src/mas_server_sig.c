#define MAS_MAX_INT_1 2
#define MAS_MAX_INT_2 10
#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>
#include <signal.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
/* #include <mastar/types/mas_opts_types.h> */
extern mas_control_t ctrl;

/* extern mas_options_t opts; */

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/thtools/mas_thread_tools.h>
#include <mastar/modules/mas_modules_commands_eval.h>

/* mas_destroy */
#include "mas_init_server.h"

#include <mastar/listener/mas_listeners.h>

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
to_exit( void )
{
  if ( ctrl.keep_listening )
  {
    GDMSG( "unset keep_listening" );
  }
  ctrl.keep_listening = 0;
  ctrl.watcher_stop = 1;
  mas_listeners_stop(  );
  ctrl.stop_listeners = 1;
}

void
sigterm_han( int s )
{
  GDMSG( "TERM %d", term_cnt );
  term_cnt++;
  to_exit(  );
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
/*            ctrl.threads.n.main.tid, ctrl.threads.n.main.thread, ctrl.threads.n.master.tid, ctrl.threads.n.master.thread, ctrl.threads.n.ticker.tid, ctrl.threads.n.ticker.thread,                      */
/*            ctrl.threads.n.logger.tid, ctrl.threads.n.logger.thread, ctrl.threads.n.watcher.tid, ctrl.threads.n.watcher.thread );                                                   */
/*   fprintf( ftinfo,                                                                                                                         */
/*            "Server info:\n\tclients: {%lu - %lu = %lu}\n" "\tlogdir: %s;\tlogpath: %s;\n"                                                  */
/*            "\tserver; pid:%u; \t\ttid:%5u/%4x; [%lx]\n", ctrl.clients_came, ctrl.clients_gone,                                             */
/*            ctrl.clients_came - ctrl.clients_gone, opts.dir.log, ctrl.logpath, ctrl.threads.n.main.pid, ctrl.threads.n.main.tid, ctrl.threads.n.main.tid,                  */
/*            ctrl.threads.n.main.thread );                                                                                                             */
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
pinfo( void )
{
  char *infos = NULL;

  infos = mas_evaluate_command( "server info" );
  if ( infos )
  {
    if ( !ctrl.stderrfile || ( fputs( infos, ctrl.stderrfile ) < 0 ) )
      if ( !ctrl.old_stderrfile || ( fputs( infos, ctrl.old_stderrfile ) < 0 ) )
        fputs( infos, ctrl.msgfile );

    /* show_info( ctrl.old_stderrfile ); */
    mas_free( infos );
  }
}

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

    pt = ctrl.threads.n.listening.threads;
    while ( pt && *pt )
    {
      EMSG( "INT to %lx @ %lx", ( unsigned long ) *pt, mas_pthread_self(  ) );
      pthread_kill( *pt, SIGINT );
      pt++;
    }
  }
  */
  if ( ctrl.pserver_thread && ctrl.pserver_thread->pid == mas_gettid(  ) )
  {
    int_cnt++;
    /* RL_SETSTATE( RL_STATE_DONE ); */
    if ( int_cnt > MAS_MAX_INT_2 )
    {
      to_exit(  );

      thMSG( "waiting at %lx", mas_pthread_self(  ) );

      /* sleep( 10 ); */
      /* exit( 3 ); */
    }
    /* HMSG( "DAEMON -%d +%d", opts.nodaemon, ctrl.daemon ); */
    if ( ctrl.daemon && ctrl.old_stderrfile )
    {
      if ( int_cnt < 2 )
        pinfo(  );
      fprintf( ctrl.old_stderrfile, "\n\nINT %d of %d", int_cnt, MAS_MAX_INT_2 );
      /* HMSG( "(%d) DAEMON -%d +%d; fr:%d", int_cnt, opts.nodaemon, ctrl.daemon, fr ); */
    }
    else
    {
      if ( int_cnt < 2 )
        pinfo(  );
      if ( !ctrl.stderrfile
           || fprintf( ctrl.stderrfile, "INT %d of %d [%lx] [%lx]\x1b[K\r", int_cnt, MAS_MAX_INT_2, mas_pthread_self(  ),
                       ctrl.threads.n.main.thread ) < 0 )
        if ( !ctrl.old_stderrfile
             || fprintf( ctrl.old_stderrfile, "INT %d of %d [%lx] [%lx]\x1b[K\r", int_cnt, MAS_MAX_INT_2, mas_pthread_self(  ),
                         ctrl.threads.n.main.thread ) < 0 )
          fprintf( ctrl.msgfile, "INT %d of %d [%lx] [%lx]\x1b[K\r", int_cnt, MAS_MAX_INT_2, mas_pthread_self(  ),
                   ctrl.threads.n.main.thread );
      /* errno = 0; */
    }
  }
  ctrl.sigint_time = ( unsigned long ) time( NULL );
}

void
sighup_han( int s )
{
  ctrl.restart = 1;
  to_exit(  );
  thMSG( "waiting at %lx", mas_pthread_self(  ) );
  EMSG( "HUP" );
}

void
sigquit_han( int s )
{
  to_exit(  );
  thMSG( "waiting at %lx", mas_pthread_self(  ) );
  EMSG( "QUIT" );
}

/* void                                                                          */
/* sigabrt_han( int s )                                                          */
/* {                                                                             */
/*   to_exit(  );                                                                */
/*   (*                                                                          */
/*      if ( pthread_equal( mas_pthread_self(  ), ctrl.pth ) )                   */
/*      {                                                                        */
/*      pthread_t *pt;                                                           */
/*                                                                               */
/*      pt = ctrl.threads.n.listening.threads;                                   */
/*      while ( pt && *pt )                                                      */
/*      {                                                                        */
/*      GMSG( "HUP to %lx @ %lx", ( unsigned long ) *pt, mas_pthread_self(  ) ); */
/*      // pthread_kill( *pt, SIGHUP );                                          */
/*      pthread_cancel( *pt );                                                   */
/*      pt++;                                                                    */
/*      }                                                                        */
/*      }                                                                        */
/*    *)                                                                         */
/*   EMSG( "ABRT" );                                                             */
/* }                                                                             */

void
sigpipe_han( int s )
{
  /* EMSG( "PIPE" ); */
  /*
     if ( pthread_equal( mas_pthread_self(  ), ctrl.pth ) )
     {
     pthread_t *pt;

     pt = ctrl.threads.n.listening.threads;
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
  {
    extern unsigned long memory_balance;

    IMSG( "AT EXIT, memory_balance:%ld", memory_balance );
    if ( print_memlist( ctrl.stderrfile, FL ) < 0 )
      if ( print_memlist( ctrl.old_stderrfile, FL ) < 0 )
        if ( print_memlist( ctrl.msgfile, FL ) < 0 )
          print_memlist( stderr, FL );
    IMSG( "AT EXIT, memory_balance:%ld", memory_balance );
  }
#else
  IMSG( "AT EXIT" );
#endif
  IMSG( "=====[%u @ %u]=================================================================", getpid(  ), getppid(  ) );
  if ( ctrl.msgfile )
  {
    fclose( ctrl.msgfile );
    ctrl.msgfile = NULL;
  }
  _exit( 0 );
}

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno ); */
  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;
  ctrl.is_client = 0;
  ctrl.is_server = 1;
  atexit( mas_atexit );
}

__attribute__ ( ( destructor ) )
     static void master_destructor( void )
{
  /* fprintf( stderr, "******************** DESTRUCTOR %s e%d\n", __FILE__, errno ); */
}
