#include "mas_server_def.h"
#include "mas_basic_def.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/modules/mas_modules_commands_eval.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>


#include "mas_message_io.h"
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


int
mas_proto_xcromas_evaluate_and_answer( const char *question, mas_header_t * pheader, mas_rcontrol_t * prcontrol )
{
  int r = -1;
  char *answer = NULL;

  prcontrol->qbin = MSG_BIN_NONE;
  answer = mas_evaluate_command( 0, NULL, NULL, prcontrol, question, question /* args */ , 1 /*level */  );
  pheader->binary = prcontrol->qbin;
  mMSG( ">>> QBIN : %d", prcontrol->qbin);
  /* if ( ( answer = mas_evaluate_cmd( 0 (* only_level *) , question, question (* args *) , 1 (*level *) , pheader, NULL, */
  /*                                   prcontrol, NULL ) ) )                                                              */
  if ( MAS_VALID_ANSWER( answer ) )
  {
    /* EMSG( "ANSWER: %lx:%s", ( unsigned long ) answer, answer ); */
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
mas_proto_xcromas( mas_rcontrol_t * prcontrol, mas_header_t * pheader_data )
{
  int r = -1;

  /* rMSG( "CHECK SPC" ); */
  if ( prcontrol && pheader_data && pheader_data->sign == MSG_SIGNATURE )
  {
    mas_header_t header;
    const char *question = NULL;

    memset( &header, 0, sizeof( header ) );
    MAS_LOG( "xcromas session" );
    prcontrol->proto = MAS_TRANSACTION_PROTOCOL_XCROMAS;
    if ( pheader_data )
    {
      header = *pheader_data;
      question = ( char * ) ( pheader_data + 1 );
    }
    rMSG( "q:%s", question && *question ? question : "-" );
    tMSG( "cl. q:%s from pid %u", question && *question ? question : "-EMPTY-", header.pid );

    {

      tMSG( "got msg from pid=%u", header.pid );
      MAS_LOG( "xc (pid:%u) Q: %s", header.pid, question && *question ? question : "-" );

      header.new_opts = 0;
      if ( header.sign != MSG_SIGNATURE )
      {
        EMSG( "header sig : %lx", ( unsigned long ) header.sign );
        header.bad = 1;
      }
      {
        header.done_cmd = 0;
        if ( header.bad )
        {
          r = -2;
          EMSG( "r:%d", r );
        }
        else
        {
          r = mas_proto_xcromas_evaluate_and_answer( question, &header, prcontrol );
        }
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
