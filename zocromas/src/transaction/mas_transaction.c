#include "mas_server_def.h"
#include "mas_basic_def.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/channel/mas_channel_object.h>
#include <mastar/channel/mas_channel_open.h>
#include <mastar/channel/mas_channel.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/thtools/mas_ocontrol_tools.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/thtools/mas_thread_tools.h>
#include <mastar/variables/mas_thread_variables.h>

/* #include "http/inc/mas_transaction_http.h" */
/* #include "xcromas/inc/mas_transaction_xcromas.h" */

#include <mastar/variables/mas_variables.h>

#include "mas_rcontrol_object.h"
#include "mas_transaction_control.h"
#include "mas_transaction.h"


/*
this:
  mas_transaction.c
related:
  mas_rcontrol_object.c
  mas_transaction_control.c
  mas_transaction_control_types.h
  mas_master.c
  mas_transaction_xcromas.c
  mas_transaction_http.c
  
  mas_channel.c
  mas_lib_thread.c
  mas_server_tools.c

  mas_variables.c
  mas_thread_variables.c
*/

/* static size_t transaction_stacksize = 0; */
/* static void *transaction_stackaddr = NULL; */

/* naming : setup + pthread_create = start */
int
#ifdef MAS_TR_PERSIST
mas_transaction_start( mas_lcontrol_t * plcontrol, unsigned persistent_transaction )
#else
mas_transaction_start( mas_lcontrol_t * plcontrol )
#endif
{
  int r = 0;

  if ( plcontrol )
  {
    /* tMSG( "opnd chn(j.bef/ fork) r:%d", r ); */

    /* ??????? not here */
    /* mas_lcontrol_cleaning_transactions( ... ); */

    /* wMSG( "cl. come in" ); */
    MAS_LOG( "client came prc:%p", ( void * ) plcontrol );
    plcontrol->h.status = MAS_STATUS_WORK;
    {
      mas_rcontrol_t *prcontrol = NULL;

#ifdef MAS_TR_PERSIST
      prcontrol = mas_rcontrol_make( plcontrol, persistent_transaction );
#else
      prcontrol = mas_rcontrol_make( plcontrol );
#endif
      if ( prcontrol )
      {
        ctrl.clients_came0++;
        if ( opts.transaction_single )
        {
          mas_transaction( prcontrol );
          mas_transaction_cleanup( prcontrol );
          mas_rcontrol_delete( prcontrol, 1 );
        }
        else
        {
          MAS_LOG( "cr'ing tr. th; prc=%p #%lu", ( void * ) prcontrol, prcontrol->h.serial );
          /* r = mas_xpthread_create( &prcontrol->h.thread, mas_transaction_th, MAS_THREAD_TRANSACTION, ( void * ) prcontrol ); */
          /* pthread_mutex_lock( &ctrl.thglob.logger_mutex );   */
          /* pthread_mutex_unlock( &ctrl.thglob.logger_mutex ); */
          r = pthread_create( &prcontrol->h.thread, &ctrl.thglob.transaction_attr, mas_transaction_th, ( void * ) prcontrol );
          if ( prcontrol->h.thread )
          {
            MAS_LOG( "cr'ed tr. th; prc=%p [%lx] #%lu", ( void * ) prcontrol, prcontrol->h.thread, prcontrol->h.serial );
          }
        }
      }
    }
  }
  else
  {
#ifdef EMSG
    EMSG( "no plcontrol" );
#endif
    ctrl.keep_listening = 0;
  }
  return r;
}

int
mas_transaction_cancel( mas_rcontrol_t * prcontrol )
{
  EMSG( "CANCEL R%lu:%u (prcontrol:%p) th:%lx", prcontrol->h.serial, prcontrol->h.status, ( void * ) prcontrol, prcontrol->h.thread );
  MAS_LOG( "cancelling R%lu:%u", prcontrol->h.serial, prcontrol->h.status );
  if ( prcontrol->h.thread )
  {
    mas_pthread_cancel( prcontrol->h.thread );
    /* prcontrol->complete = 1;    (* ????????????? *) */
  }
  return 0;
}

/* mas_transaction_protodesc_t _protos[] = {                 */
/*   {MAS_TRANSACTION_PROTOCOL_HTTP, mas_proto_http}         */
/*   , {MAS_TRANSACTION_PROTOCOL_XCROMAS, mas_proto_xcromas} */
/* };                                                        */

/* size_t protos_num = (* sizeof( _protos ) / sizeof( _protos[0] ) *) 0; */
/* mas_transaction_protodesc_t *protos = (* &_protos[0] *) NULL;      */

/* transaction should be cancelled?
 * FIXME all transactions remains active (keep-alive?) when one 1 of 2 quit
 * */
static int
mas_transaction_xch( mas_rcontrol_t * prcontrol )
{
  int r = -1;

  MAS_LOG( "starting transaction xch (%lu protos)", ctrl.protos_num );
  tMSG( "starting transaction xch (%lu protos)", ctrl.protos_num );
  HMSG("+ TRANS EXCHANGE");
  if ( prcontrol && prcontrol->h.pchannel )
  {
    char *data = NULL;
    size_t sz = 0;

    r = 0;

    MAS_LOG( "to read rq (read all)" );
    r = mas_channel_read_all( prcontrol->h.pchannel, &data, &sz );
    MAS_LOG( "read rq: %d", r );
    if ( r == 0 )
    {
      mas_channel_close( prcontrol->h.pchannel );
      MAS_LOG( "read none" );
    }

    {
      struct timeval td;

      gettimeofday( &td, NULL );
      prcontrol->h.activity_time = td;
    }
    if ( r >= 0 && data )
    {
      prcontrol->h.status = MAS_STATUS_WORK;
      r = 0;
      if ( !ctrl.protos_num )
      {
        EMSG( "No proto's loaded; data[%lu]", sz );
        MAS_LOG( "No proto's loaded; data[%lu]", sz );
      }
      for ( int np = 0; np < ctrl.protos_num; np++ )
      {
        if ( r == 0 || prcontrol->proto_desc == &ctrl.proto_descs[np] )
        {
          /* EMSG( "@~~~~~~~~~(%u) proto:%u : %p", prcontrol->proto_desc ? prcontrol->proto_desc->proto_id : -1, ctrl.proto_descs[np].proto_id, */
          /*       ( void * ) ( unsigned long long ) ctrl.proto_descs[np].function );                                                           */
          r = ( ctrl.proto_descs[np].function ) ( prcontrol, &ctrl.proto_descs[np], data );
          if ( r > 0 )
            prcontrol->proto_desc = &ctrl.proto_descs[np];
        }
      }

      /* if ( r == 0 && ( prcontrol->proto == MAS_TRANSACTION_PROTOCOL_NONE || prcontrol->proto == MAS_TRANSACTION_PROTOCOL_HTTP ) )    */
      /*   r = mas_proto_http( prcontrol, data );                                                                                       */
      /* if ( r == 0 && ( prcontrol->proto == MAS_TRANSACTION_PROTOCOL_NONE || prcontrol->proto == MAS_TRANSACTION_PROTOCOL_XCROMAS ) ) */
      /*   r = mas_proto_xcromas( prcontrol, data );                                                                                    */
    }
    if ( r < 0 )
    {
      rMSG( "no data - cl.gone (r:%d)", r );
      EMSG( "r:%d; i/s:%d; i/c:%d", r, ctrl.keep_listening, ctrl.in_client );
      MAS_LOG( "error r:%d; i/s:%d; i/c:%d", r, ctrl.keep_listening, ctrl.in_client );
    }
    if ( data )
      mas_free( data );
  }
  if ( prcontrol && ( !prcontrol->proto_desc || prcontrol->proto_desc->proto_id == 0 ) )
  {
    prcontrol->keep_alive = 0;
    MAS_LOG( "KA => %u", prcontrol->keep_alive );
    r = -1;
  }
  MAS_LOG( "end transaction xch" );
  tMSG( "end transaction xch" );
  HMSG("- TRANS EXCHANGE");
  return r;
}

/* return 0 to end transaction */
/* return >0 to continue       */
void *
mas_transaction( mas_rcontrol_t * prcontrol )
{
  int r = 1;

  MAS_LOG( "starting transaction" );
  tMSG( "starting transaction" );
  HMSG( "+ TRANS" );

  if ( prcontrol )
  {
    if ( !prcontrol->h.pchannel )
    {
      /* rMSG( "no open channel in tr." ); */
      MAS_LOG( "no open channel in tr." );
    }
    if ( prcontrol->h.pchannel )
    {
      mas_lcontrol_t *plcontrol;

      plcontrol = prcontrol ? prcontrol->plcontrol : NULL;

      MAS_LOG( "starting transaction 1" );
      /* rMSG( "start transaction" ); */
      MAS_LOG( "starting transaction 2" );
      ctrl.in_pipe++;
      /* rMSG( MAS_SEPARATION_LINE ); */
      prcontrol->h.status = MAS_STATUS_INIT;
      prcontrol->keep_alive = 1;
      MAS_LOG( "KA => %u", prcontrol->keep_alive );
      while ( r >= 0 && prcontrol->keep_alive && !prcontrol->stop && prcontrol->h.pchannel && prcontrol->h.pchannel->opened )
      {
        MAS_LOG( "starting transaction keep-alive block" );
        prcontrol->h.status = MAS_STATUS_WAIT;
        /* rMSG( "waiting cl.data; i/s:%d; i/c:%d", ctrl.keep_listening, ctrl.in_client ); */
        /* rMSG( "keep_alive %d", prcontrol->keep_alive ); */
        prcontrol->h.status = MAS_STATUS_OPEN;
        prcontrol->qbin = MSG_BIN_NONE;
        r = mas_transaction_xch( prcontrol );
        /* if ( prcontrol->stop_listeners ) */
        /* {                                */
        /*   mas_listeners_cancel(  );      */
        /* }                                */
        prcontrol->xch_cnt++;
        mas_pthread_mutex_lock( &ctrl.thglob.cnttr3_mutex );
        ctrl.xch_cnt++;
        mas_pthread_mutex_unlock( &ctrl.thglob.cnttr3_mutex );
        prcontrol->h.status = MAS_STATUS_CLOSE;
        /* rMSG( "end handling (r:%d) i/s:%d; i/c:%d", r, ctrl.keep_listening, ctrl.in_client ); */
        MAS_LOG( "end tr. keep-alive block, %s", prcontrol->proto_desc ? prcontrol->proto_desc->name : "?" );
      }
      prcontrol->h.status = MAS_STATUS_STOP;
    }
  }
  MAS_LOG( "end transaction" );
  tMSG( "end transaction" );
  HMSG( "- TRANS" );
  return NULL;
}

void
mas_transaction_cleanup( void *arg )
{
  mas_rcontrol_t *prcontrol = NULL;

  prcontrol = ( mas_rcontrol_t * ) arg;
  if ( prcontrol )
  {
    MAS_LOG( "tr. th cleanup prc:%p #%lu", ( void * ) prcontrol, prcontrol->h.serial );
    /* usleep( 1000000 ); */
    if ( prcontrol->h.pchannel )
    {
      mas_channel_t *tch;

      tch = prcontrol->h.pchannel;
      prcontrol->h.pchannel = NULL;

      /* mas_channel_close( prcontrol->h.pchannel ); */
      mas_channel_delete( tch, 1, 0 );
    }

    ctrl.in_pipe--;
    prcontrol->h.status = MAS_STATUS_END;
    /* rMSG( "end transaction; i/s:%d; i/c:%d", ctrl.keep_listening, ctrl.in_client ); */

    mas_thread_variables_delete(  );

    {
      mas_lcontrol_t *plcontrol;

      plcontrol = prcontrol ? prcontrol->plcontrol : NULL;
      mas_pthread_mutex_lock( &ctrl.thglob.cnttr1_mutex );
      plcontrol->clients_gone++;
      ctrl.clients_gone++;
      ctrl.transactions_time += mas_double_time(  ) - prcontrol->start_time;
      mas_pthread_mutex_unlock( &ctrl.thglob.cnttr1_mutex );
    }
    prcontrol->complete = 1;
    /* mas_rcontrol_delete( prcontrol, 0 ); */
  }
}

/* working with client */
/* naming : pthread_create argument = th */
void *
mas_transaction_th( void *trcontrol )
{
  void *r = NULL;
  mas_rcontrol_t *prcontrol = NULL;

  MAS_LOG( "tr. th. started [%lx]", mas_pthread_self(  ) );
  /* struct sched_param sched;                                               */
  /*                                                                         */
  /* sched.sched_priority = 10;                                              */
  /* rs = pthread_setschedparam( mas_pthread_self(  ), SCHED_IDLE, &sched ); */

  /* {                                                                       */
  /*   int rs;                                                               */
  /*   struct sched_param sched;                                             */
  /*                                                                         */
  /*   rs = pthread_setschedparam( mas_pthread_self(  ), SCHED_RR, &sched ); */
  /* }                                                                       */
  prcontrol = ( mas_rcontrol_t * ) trcontrol;
  prcontrol->start_time = mas_double_time(  );
  MAS_LOG( "tr. th. started (%20.5f)", prcontrol->start_time );
  if ( prcontrol && prcontrol->plcontrol )
  {
    prcontrol->h.tid = mas_gettid(  );
    mas_in_thread( MAS_THREAD_TRANSACTION, prcontrol->plcontrol, prcontrol );
#ifdef MAS_TR_PERSIST
    do
#endif
    {
      MAS_LOG( "start tr.th" );
      /* rMSG( "start tr.th R%lu:%u @ L%lu:%u", prcontrol->h.serial, prcontrol->h.status, prcontrol->plcontrol->h.serial, */
      /*       prcontrol->plcontrol->h.status );                                                                          */
      prcontrol->h.status = MAS_STATUS_START;
      /* Wait for prcontrol->h.pchannel if pre-launched thread */
      MAS_LOG( "start tr.th 1" );
      if ( !( prcontrol->h.pchannel && prcontrol->h.pchannel->opened ) )
      {
        int r = -1;

        /* rMSG( "COND 1" ); */
        MAS_LOG( "cond to lock" );
        pthread_mutex_lock( &prcontrol->waitchan_mutex );
        /* rMSG( "COND 2" ); */
        while ( !( prcontrol->h.pchannel && prcontrol->h.pchannel->opened ) && r != ETIMEDOUT && !prcontrol->complete )
        {
          /* rMSG( "COND 3" ); */
          prcontrol->waitchan_waiting = 1;
          r = pthread_cond_wait( &prcontrol->waitchan_cond, &prcontrol->waitchan_mutex );
          prcontrol->waitchan_waiting = 0;
          MAS_LOG( "cond came" );
          /* rMSG( "COND 4" ); */
        }
        if ( r == ETIMEDOUT )
        {
          /* timeout occurred */
        }
        else
        {
          /* rMSG( "COND 5" ); */
          /* ........ */
        }
        pthread_mutex_unlock( &prcontrol->waitchan_mutex );
        MAS_LOG( "cond came - processing" );
      }
      {
        unsigned long nclients;

        MAS_LOG( "start tr.th 2" );
        mas_pthread_mutex_lock( &ctrl.thglob.cnttr2_mutex );
        ctrl.clients_came++;

        nclients = ctrl.clients_came - ctrl.clients_gone;
        if ( nclients > ctrl.maxclients )
          ctrl.maxclients = nclients;

        prcontrol->plcontrol->clients_came++;
        mas_pthread_mutex_unlock( &ctrl.thglob.cnttr2_mutex );
        MAS_LOG( "start tr.th 3" );
      }
      {
        /* double tstart, tend, tdiff;   */
        /*                               */
        /* tstart = mas_double_time(  ); */
        MAS_LOG( "tr. push" );
        pthread_cleanup_push( mas_transaction_cleanup, prcontrol );
        MAS_LOG( "tr. itself" );
        r = mas_transaction( prcontrol );
        pthread_cleanup_pop( 1 );
        /* tend = mas_double_time(  );   */
        /* tdiff = tend - tstart;        */
        /* if ( ctrl.maxtrtime < tdiff ) */
        /*   ctrl.maxtrtime = tdiff;     */
      }
      ctrl.clients_gone2++;

      /* rMSG( "end r/th; i/s:%d; i/c:%d", ctrl.keep_listening, ctrl.in_client ); */
      MAS_LOG( "end r/th; i/s:%d; i/c:%d", ctrl.keep_listening, ctrl.in_client );
    }
#ifdef MAS_TR_PERSIST
    while ( prcontrol->persistent_transaction );
#endif
    /* {                                       */
    /*   mas_channel_t *pchannel = NULL;       */
    /*                                         */
    /*   pchannel = prcontrol->h.pchannel;       */
    /*   prcontrol->h.pchannel = NULL;           */
    /*   mas_channel_delete( pchannel, 0, 0 ); */
    /* }                                       */
    prcontrol->complete = 1;
    /* NO?! : mas_rcontrol_delete( prcontrol, prcontrol->plcontrol ); */


    /* rMSG( MAS_SEPARATION_LINE ); */
    /* ??? mas_lcontrol_cleaning_transactions( ... ); */
  }
  MAS_LOG( "tr. th. end" );
  mas_pthread_exit( NULL );
  return r;
}

int
mas_transaction_write( const mas_rcontrol_t * prcontrol, char *cbuf, size_t sz )
{
  int w = -1;

  if ( prcontrol )
    w = mas_channel_write( prcontrol->h.pchannel, cbuf, sz );
  MAS_LOG( "written transaction %u of %lu", w, sz );
  return w;
}

int
mas_transaction_vwritef( const mas_rcontrol_t * prcontrol, const char *fmt, va_list args )
{
  int w = -1;

  if ( prcontrol )
    w = mas_channel_vwritef( prcontrol->h.pchannel, fmt, args );
  return w;
}

int
mas_transaction_write_values( mas_variables_list_head_t * variables, const char *vclass, const mas_rcontrol_t * prcontrol )
{
  int w = -1;

  /* mas_variables.c */
  if ( prcontrol )
    w = mas_variables_chwrite_bin( variables, vclass, 0, 1, prcontrol->h.pchannel );
  return w;
}
