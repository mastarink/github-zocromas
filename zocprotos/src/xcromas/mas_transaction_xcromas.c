#include <mastar/wrap/mas_std_def.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/modules/mas_modules_commands_eval.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>


#include <mastar/message_io/mas_message_io.h>
#include "mas_transaction_xcromas.h"


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
do_quit_server( mas_rcontrol_t * prcontrol )
{
  ctrl.keep_listening = 0;
  prcontrol->keep_alive = 0;
  MAS_LOG( "KA => %u", prcontrol->keep_alive );
  if ( prcontrol )
  {
    if ( ctrl.restart )
      prcontrol->qbin = MSG_BIN_RESTART;
    else if ( ctrl.quit )
      prcontrol->qbin = MSG_BIN_QUIT;
    else
      prcontrol->qbin = MSG_BIN_DISCONNECT;
  }
  ctrl.stop_listeners = 1;
}

static int
mas_proto_xcromas_evaluate_and_answer( mas_rcontrol_t * prcontrol, const char *question, mas_header_t * pheader )
{
  int r = -1;
  char *answer = NULL;

  prcontrol->qbin = MSG_BIN_NONE;
  answer = mas_evaluate_command( 0, NULL, NULL, prcontrol, question, question /* args */ , 1 /*level */  );
  cMSG( "B(%d) Q(%d) SL(%d)", prcontrol->qbin, ctrl.do_quit, ctrl.stop_listeners );
  if ( ctrl.do_quit )
  {
    MAS_LOG( "qbin => %u", prcontrol->qbin );
    cMSG( "qbin => %u; ctrl.stop_listeners:%u", prcontrol->qbin, ctrl.stop_listeners );
    do_quit_server( prcontrol );
    cMSG( "qbin => %u; ctrl.stop_listeners:%u", prcontrol->qbin, ctrl.stop_listeners );
  }
  pheader->binary = prcontrol->qbin;
  HMSG( ">>> QBIN : %d", prcontrol->qbin );
  /* if ( ( answer = mas_evaluate_cmd( 0 (* only_level *) , question, question (* args *) , 1 (*level *) , pheader, NULL, */
  /*                                   prcontrol, NULL ) ) )                                                              */
  if ( MAS_VALID_ANSWER( answer ) )
  {
    HMSG( "ANSWER: %lx:%s", ( unsigned long ) answer, answer );
    tMSG( "%s%ssign %s;answer is %s (%s)", pheader->binary ? "bin;" : "", pheader->new_opts ? "new opts;" : "",
          pheader->bad ? "BAD;" : "", pheader->sign == MSG_SIGNATURE ? "ok" : "bad", answer ? "+" : "-" );
    if ( pheader->bad )
    {
      r = -2;
      EMSG( "r:%d", r );
    }
    else
    {
      if ( pheader && MAS_VALID_ANSWER( answer ) && !pheader->binary )
        pheader->len = strlen( answer ) + 1;
      r = mas_channel_write_message( prcontrol->h.pchannel, answer, pheader );
      /* EMSG( "ANSWER: %d => %s", r, answer ); */
      if ( r < 0 )
      {
        P_ERR;
        EMSG( "r:%d", r );
      }
    }
    mas_free( answer );
    answer = NULL;
  }
  else
  {
    if ( answer && pheader )
      pheader->binary = MSG_BIN_UNKNOWN_COMMAND;
    r = mas_channel_write_message( prcontrol->h.pchannel, NULL, pheader );
    tMSG( "written %d", r );
    if ( r < 0 )
    {
      P_ERR;
      EMSG( "r:%d", r );
    }
  }
  return r;
}

int
mas_proto_main( mas_rcontrol_t * prcontrol, mas_transaction_protodesc_t *proto_desc, const void *pheader_void )
{
  int r = -1;
  const mas_header_t *pheader_data = ( mas_header_t * ) pheader_void;

  if ( prcontrol && pheader_data && pheader_data->sign == MSG_SIGNATURE )
  {
    mas_header_t header_copy;
    const char *question = NULL;

    memset( &header_copy, 0, sizeof( header_copy ) );
    MAS_LOG( "xcromas session" );
    /* prcontrol->proto = MAS_TRANSACTION_PROTOCOL_XCROMAS; */
    if ( pheader_data )
    {
      header_copy = *pheader_data;
      question = ( char * ) ( pheader_data + 1 );
    }
    rMSG( "Q:%s", question && *question ? question : "-" );
    tMSG( "cl. q:%s from pid %u", question && *question ? question : "-EMPTY-", header_copy.pid );


    rMSG( "got msg from pid=%u", header_copy.pid );
    MAS_LOG( "xc (pid:%u) Q: %s", header_copy.pid, question && *question ? question : "-" );

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
        rMSG( "q for e & a :%s", question );
        r = mas_proto_xcromas_evaluate_and_answer( prcontrol, question, &header_copy );
        rMSG( "e & a :%d", r );
      }
    }

    question = NULL;
  }
  else
  {
    r = 0;
  }
  return r;
}
