#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_control_types.h>
extern mas_control_t ctrl;

#include <mastar/types/mas_opts_types.h>

#include <mastar/fileinfo/mas_unidata.h>
#include <mastar/modules/mas_modules_commands_eval.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/channel/mas_channel_buffer.h>
#include <mastar/channel/mas_channel.h>

#include <mastar/messageio/mas_message_io.h>



#include <mastar/types/mas_message_types.h>


__attribute__ ( ( constructor ) )
     static void http_constructor( void )
{
  HMSG( "CONSTRUCTOR proto xcromas" );
}

/*
this:
  mas_transaction_xcromas.c
related:
  mas_transaction.c
  mas_transaction_http.c

  mas_tools.c
  mas_msg_tools.c
*/

static void
do_exit_server( mas_rcontrol_t * prcontrol )
{
  if ( prcontrol )
  {
    if ( ctrl.restart )
      prcontrol->qbin = MSG_BIN_RESTART;
    else if ( ctrl.exit )
      prcontrol->qbin = MSG_BIN_QUIT;
    else
      prcontrol->qbin = MSG_BIN_DISCONNECT;
    prcontrol->connection_keep_alive = 0;
  }
  ctrl.stop_listeners = 1;
  /* ctrl.watcher_stop = 1; */
}

static int
mas_proto_xcromas_evaluate_and_answer( mas_rcontrol_t * prcontrol, const char *question, mas_header_t * pheader )
{
  int r = -1;
  mas_evaluated_t *answer = NULL;

  prcontrol->qbin = MSG_BIN_NONE;
  answer = mas_evaluate_transaction_command( prcontrol, question );
  tMSG( "B(%d) Q(%d) SL(%d)", prcontrol->qbin, ctrl.do_exit, ctrl.stop_listeners );
  if ( ctrl.do_exit )
  {
    HMSG( "DO_EXIT" );
    MAS_LOG( "qbin => %u", prcontrol->qbin );
    tMSG( "qbin => %u; ctrl.stop_listeners:%u", prcontrol->qbin, ctrl.stop_listeners );
    do_exit_server( prcontrol );
    tMSG( "qbin => %u; ctrl.stop_listeners:%u", prcontrol->qbin, ctrl.stop_listeners );
  }
  pheader->binary = prcontrol->qbin;
  HMSG( "ANSWERING [do_exit:%d; ctrl.stop_listeners:%d]", ctrl.do_exit, ctrl.stop_listeners );
  /* if ( ( answer = mas_evaluate_cmd( 0 (* only_level *) , question, question (* args *) , 1 (*level *) , pheader, NULL, */
  /*                                   prcontrol, NULL ) ) )                                                              */
  if ( MAS_VALID_ANSWER( answer ) )
  {
    HMSG( "ANSWER: %lx:%s", ( unsigned long ) answer, answer ? ( char * ) answer->data : NULL );
    tMSG( "%s%ssign %s;answer is %s (%s)", pheader->binary ? "bin;" : "", pheader->new_opts ? "new opts;" : "",
          pheader->bad ? "BAD;" : "", pheader->sign == MSG_SIGNATURE ? "ok" : "bad", answer && answer->data ? "+" : "-" );
    if ( pheader->bad )
    {
      r = -2;
      EMSG( "r:%d", r );
    }
    else
    {
      if ( pheader && MAS_VALID_ANSWER( answer ) && !pheader->binary )
        pheader->len = strlen( ( char * ) answer->data ) + 1;
      r = mas_channel_write_message( prcontrol->h.pchannel, answer->data, pheader );
      /* EMSG( "ANSWER: %d => %s", r, answer ); */
      if ( r < 0 )
      {
        P_ERR;
        EMSG( "r:%d", r );
      }
    }
    /* mas_free( answer ); */
    mas_evaluated_delete( answer );
    answer = NULL;
  }
  else
  {
    if ( answer && answer->data && pheader )
      pheader->binary = MSG_BIN_UNKNOWN_COMMAND;
    r = mas_channel_write_message( prcontrol->h.pchannel, NULL, pheader );
    HMSG( "WRITTEN %d", r );
    if ( r < 0 )
    {
      P_ERR;
      EMSG( "r:%d", r );
    }
  }
  return r;
}

/* int                                                                                                              */
/* mas_proto_test( mas_rcontrol_t * prcontrol, mas_transaction_protodesc_t * proto_desc, const void *buffer_void ) */
/* {                                                                                                                */
/*   int r = -1;                                                                                                    */
/*   const mas_header_t *pheader_data = ( mas_header_t * ) buffer_void;                                            */
/*                                                                                                                  */
/*   if ( prcontrol && pheader_data && pheader_data->sign == MSG_SIGNATURE )                                        */
/*     r = 0;                                                                                                       */
/*   return r;                                                                                                      */
/* }                                                                                                                */

int
mas_proto_main( mas_rcontrol_t * prcontrol, mas_transaction_protodesc_t * proto_desc, const void *buffer_void )
{
  int r = -1;

/* short: mas_channel_read_message */

  /* prcontrol->h.pchannel->buffer.maxread = 1028 * 4; */
  buffer_void = mas_channel_buffer( prcontrol->h.pchannel, NULL );


  {
    char *dump;

    dump = mas_dump2( ( void * ) buffer_void, prcontrol->h.pchannel->buffer.length, 64 );
    if ( dump )
    {
      HMSG( "got:%lu; [%s]", ( unsigned long ) prcontrol->h.pchannel->buffer.length, dump );
      mas_free( dump );
    }
  }

  {
    const mas_header_t *pheader_data = ( mas_header_t * ) buffer_void;

    if ( prcontrol && pheader_data && pheader_data->sign == MSG_SIGNATURE )
    {
      mas_header_t header_copy;
      const char *question = NULL;

      HMSG( "XCROMAS (%d) %lu", buffer_void ? 1 : 0, ( unsigned long ) prcontrol->h.pchannel->buffer.length );
      pheader_data = ( mas_header_t * ) buffer_void;
      HMSG( "h:(%lu) got:%lu; h.len:%u", ( unsigned long ) sizeof( mas_header_t ),
            ( unsigned long ) prcontrol->h.pchannel->buffer.length, pheader_data ? pheader_data->len : 0 );
      /* if ( sizeof( mas_header_t ) + pheader_data->len < prcontrol->h.pchannel->buffer.length ) */
      /*   mas_channel_read_remainder( prcontrol->h.pchannel );                                   */


      while ( sizeof( mas_header_t ) + pheader_data->len < prcontrol->h.pchannel->buffer.length )
      {
        HMSG( "pheader_data->len:%u; buflen:%lu", pheader_data->len, ( unsigned long ) prcontrol->h.pchannel->buffer.length );
        mas_channel_read_some( prcontrol->h.pchannel );
      }



      /* prcontrol->proto_desc = proto_desc; */

      memset( &header_copy, 0, sizeof( header_copy ) );
      MAS_LOG( "xcromas session" );
      /* prcontrol->proto = MAS_TRANSACTION_PROTOCOL_XCROMAS; */
      if ( pheader_data )
      {
        header_copy = *pheader_data;
        question = ( char * ) ( pheader_data + 1 );
      }
      tMSG( "Q:%s", question && *question ? question : "-" );
      tMSG( "cl. q:%s from pid %lu", question && *question ? question : "-EMPTY-", ( unsigned long ) header_copy.pid );


      tMSG( "got msg from pid=%lu", ( unsigned long ) header_copy.pid );
      MAS_LOG( "xc (pid:%lu) Q: %s", ( unsigned long ) header_copy.pid, question && *question ? question : "-" );

      header_copy.new_opts = 0;
      if ( header_copy.sign != MSG_SIGNATURE )
      {
        EMSG( "header_copy sig : %lx", ( unsigned long ) header_copy.sign );
        header_copy.bad = 1;
      }
      {
        header_copy.done_cmd = 0;
        if ( header_copy.bad )
        {
          r = -2;
          EMSG( "r:%d", r );
        }
        else
        {
          tMSG( "q for e & a :%s", question );
          r = mas_proto_xcromas_evaluate_and_answer( prcontrol, question, &header_copy );
          tMSG( "e & a :%d", r );
        }
      }

      question = NULL;
    }
    else
    {
      r = 0;
    }
  }
  return r;
}
