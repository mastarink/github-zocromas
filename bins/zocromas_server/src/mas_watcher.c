#include <mastar/wrap/mas_std_def.h>
/* #include "mas_server_def.h" */
/* #include "mas_basic_def.h"  */

#include <stdlib.h>
#include <sys/time.h>

#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>

#include <mastar/thtools/mas_ocontrol_tools.h>
#include <mastar/thtools/mas_thread_tools.h>

#include <mastar/transaction/mas_transaction_control.h>
#include <mastar/transaction/mas_transaction.h>

#include <mastar/listener/mas_listener_control.h>
#include <mastar/listener/mas_listeners.h>

#include "mas_server_tools.h"

#include "mas_watcher.h"

/*
this:
  mas_watcher.c
related:
  mas_master.c
  mas_ticker.c
more:
  mas_cmdmod_server.c
  mas_init_server.c
  mas_init_server.h
  mas_cmdmod_client.c
  mas_cmdmod_get.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_server_main.c
  mas_server_sig.c
  mas_server_sig.h
  mas_server_tools.c
  mas_server_tools.h

*/




/* static char *tmp = NULL; */

static size_t watcher_stacksize = 0;
static void *watcher_stackaddr = NULL;

static void
mas_watcher_cleanup( void *arg )
{
/* mas_lcontrols_cleaning_transactions returns not-joined-count */
  MAS_LOG( "by the way (watcher ending): cleaning transactions" );
  mas_lcontrols_cleaning_transactions( ctrl.forget_transactions, 0 /* don't wait */  );
  FMSG( "WATCHER CLEANUP" );
  MAS_LOG( "watcher cleanup" );
}

static void
mas_watcher( void )
{
#define MUL 10
  int itick = 0;
  double zinterval = 2.;
  double interval = zinterval / MUL;
  int stop = 0;
  int nlisteners_ever_open = 0;

  while ( !ctrl.fatal && !stop )
  {
    unsigned long cur_time = ( unsigned long ) time( NULL );
    int out;

    if ( !ctrl.stamp.start_time )
      ctrl.stamp.start_time = ( unsigned long ) time( NULL );
    out = itick % MUL == 0 && !ctrl.watcher_hide;
    {
      unsigned nlistener = 0;
      unsigned ntransaction = 0;
      unsigned nlistener_open = 0;
      unsigned long elapsed_time;
      mas_lcontrol_t *plcontrol = NULL;

      elapsed_time = cur_time - ctrl.stamp.start_time;
      if ( out )
      {
        /* thMSG( "watcher %lu", elapsed_time ); */
        MAS_LOG( "WATCH ==========================================================================================" );
        MAS_LOG( "WATCH main th: %u [%lx] master th: %u [%lx]", ctrl.main_tid, ctrl.main_thread, ctrl.master_tid, ctrl.master_thread );
        MAS_LOG( "WATCH ticker th: %u [%lx] watcher th: %u [%lx] logger th: %u [%lx]", ctrl.ticker_tid, ctrl.ticker_thread,
                 ctrl.watcher_tid, ctrl.watcher_thread, ctrl.logger_tid, ctrl.logger_thread );
        MAS_LOG( "WATCH clients: {%lu - %lu = %lu};server pid:%u; tid:%5u; [%lx]", ctrl.clients_came, ctrl.clients_gone,
                 ctrl.clients_came - ctrl.clients_gone, ctrl.main_pid, ctrl.main_tid, ctrl.main_thread );
      }
      nlistener = 0;
      ntransaction = 0;
      nlistener_open = 0;
      pthread_rwlock_rdlock( &ctrl.thglob.lcontrols_list_rwlock );
      MAS_LIST_FOREACH( plcontrol, ctrl.lcontrols_list, next )
      {
        struct timeval linactive_time;
        struct timeval lnow_time;

        {
          gettimeofday( &lnow_time, NULL );
          {
            linactive_time.tv_sec = lnow_time.tv_sec - plcontrol->h.activity_time.tv_sec;
            linactive_time.tv_usec = lnow_time.tv_usec - plcontrol->h.activity_time.tv_usec;
            if ( linactive_time.tv_usec < 0 )
            {
              linactive_time.tv_usec += 1000000;
              linactive_time.tv_sec -= 1;
            }
          }
        }

        if ( out )
        {
          MAS_LOG( "WATCH > %u. %20s:%-6u;tid:%5u; [%lx] {%lu - %lu = %lu} {%lu.%06lu %lu.%06lu I/A:%lu.%06lu}  [%s]", nlistener,
                   plcontrol->host, plcontrol->port, plcontrol->h.tid, plcontrol->h.thread, plcontrol->clients_came,
                   plcontrol->clients_gone, plcontrol->clients_came - plcontrol->clients_gone, plcontrol->h.activity_time.tv_sec,
                   plcontrol->h.activity_time.tv_usec, lnow_time.tv_sec, lnow_time.tv_usec, linactive_time.tv_sec,
                   linactive_time.tv_usec, plcontrol->h.pchannel ? "L" : "C" );
        }
        {
          mas_rcontrol_t *prcontrol;
          unsigned ntransaction_in_l;

          nlistener++;
          ntransaction_in_l = 0;
          pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );
          MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )
          {
            char *sip = NULL;
            mas_channel_t *pchannel = NULL;
            struct timeval rnow_time;
            struct timeval rinactive_time;

            ntransaction_in_l++;
            ntransaction++;
            if ( plcontrol->h.pchannel )
            {
              nlisteners_ever_open++;
              nlistener_open++;
            }
            {
              gettimeofday( &rnow_time, NULL );
              {
                rinactive_time.tv_sec = rnow_time.tv_sec - prcontrol->h.activity_time.tv_sec;
                rinactive_time.tv_usec = rnow_time.tv_usec - prcontrol->h.activity_time.tv_usec;
                if ( rinactive_time.tv_usec < 0 )
                {
                  rinactive_time.tv_usec += 1000000;
                  rinactive_time.tv_sec -= 1;
                }
              }
            }
            pchannel = prcontrol ? prcontrol->h.pchannel : NULL;
            if ( out )
            {
              const char *proto_name = NULL;

              if ( pchannel )
                sip = mas_ip_string( &pchannel->serv.addr.sin_addr );

              proto_name = prcontrol->proto_desc ? prcontrol->proto_desc->name : "?";
              MAS_LOG( "WATCH  >> %u. %s %s:%u;tid:%5u; [%lx] {%lu.%06lu %lu.%06lu I/A:%lu.%06lu} #%u", ntransaction_in_l,
                       proto_name, sip ? sip : "?", pchannel ? pchannel->port : 0, prcontrol->h.tid, prcontrol->h.thread,
                       prcontrol->h.activity_time.tv_sec, prcontrol->h.activity_time.tv_usec, rnow_time.tv_sec, rnow_time.tv_usec,
                       rinactive_time.tv_sec, rinactive_time.tv_usec, prcontrol->xch_cnt );
              if ( sip )
                mas_free( sip );
            }
            if ( prcontrol->proto_desc && 0 == strcmp( prcontrol->proto_desc->name, "http" ) )
              if ( rinactive_time.tv_sec > 10 )
              {
                FMSG( "WATCHER : http tr.end!!" );
                mas_transaction_cancel( prcontrol );
              }
          }
        }
        pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
      }
      pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
      if ( out )
        MAS_LOG( "WATCH ==========================================================================================" );
      if ( ctrl.watcher_stop )
      {
        FMSG( "WATCHER STOP : %d %d %d", nlistener_open, ntransaction, nlisteners_ever_open );
        if ( nlistener_open == 0 && ntransaction == 0 /* && nlisteners_ever_open */  )
          stop = 1;
      }
      else if ( !ctrl.main_thread || ( !ctrl.master_thread && opts.make_master_thread ) || !ctrl.watcher_thread || !ctrl.logger_thread
                || !ctrl.ticker_thread )
      {
        FMSG( "WATCHER main:%d; master:%d", ctrl.main_thread ? 1 : 0, ctrl.master_thread ? 1 : 0 );
      }
      /* fprintf( stderr, "WATCHER %u %u %u\n", nlistener_open, ntransaction, nlisteners_ever_open ); */
    }
    if ( ctrl.stop_listeners )
    {
      ctrl.keep_listening = 0;
      mas_listeners_cancel(  );
      ctrl.watcher_stop = 1;
      /* ctrl.stop_listeners = 0; */
      FMSG( "WATCHER - STOP LISTENERS" );
    }
    /* FMSG( "WATCHER SL %d", ctrl.stop_listeners ); */

    mas_nanosleep( interval );

    mas_lcontrols_cleaning_transactions( ctrl.forget_transactions, 0 /* don't wait */  );
    itick++;
  }
}

/* naming : pthread_create argument = th */
static void *
mas_watcher_th( void *arg )
{
  ctrl.watcher_tid = mas_gettid(  );

  MAS_LOG( "watcher start" );
  mas_in_thread( MAS_THREAD_WATCHER, NULL, NULL );

  pthread_cleanup_push( mas_watcher_cleanup, NULL );
  mas_watcher(  );
  pthread_cleanup_pop( 1 );

  MAS_LOG( "watcher stop" );
  FMSG( "WATCHER STOP" );
  mas_pthread_exit( NULL );
  return ( NULL );
}

/* naming : pthread_create = start */
int
mas_watcher_start( void )
{
  int r = 0;

  if ( !ctrl.watcher_thread )
  {
    {
      int r;

      r = pthread_attr_getstack( &ctrl.thglob.watcher_attr, &watcher_stackaddr, &watcher_stacksize );
      tMSG( "creating watcher thread stack:%lu @ %p", watcher_stacksize, watcher_stackaddr );
      HMSG( "+ WATCHER" );
    }

    MAS_LOG( "starting watcher th." );

    /* r = mas_xpthread_create( &ctrl.watcher_thread, mas_watcher_th, MAS_THREAD_WATCHER, NULL ); */
    r = pthread_create( &ctrl.watcher_thread, &ctrl.thglob.watcher_attr, mas_watcher_th, NULL );
    MAS_LOG( "(%d) created(?) watcher thread [%lx]", r, ctrl.watcher_thread );
  }
  else
  {
    MAS_LOG( "running w/o watcher th." );
  }
  return r;
}

/* naming : cancel + join = stop */
int
mas_watcher_stop( void )
{
  int r = 0;

  if ( ctrl.watcher_thread )
  {
    MAS_LOG( "stopping (cancelling) watcher [%lx]", ctrl.watcher_thread );

    mas_pthread_cancel( ctrl.watcher_thread );

    mas_xpthread_join( ctrl.watcher_thread );
    MAS_LOG( "stopped watcher" );
    ctrl.watcher_thread = ( pthread_t ) 0;
    HMSG( "- WATCHER" );
    FMSG( "WATCHER STOPPED" );
  }
  else
  {
    MAS_LOG( "watcher already cancelled" );
  }
  return r;
}
