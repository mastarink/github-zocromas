#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/channel/mas_channel.h>
#include <mastar/channel/mas_channel_buffer.h>
#include <mastar/channel/mas_channel_open.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#ifdef MAS_CLIENT_LOG
#  include <mastar/log/mas_log.h>
#endif

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/msg/mas_curses.h>

#include <mastar/messageio/mas_message_io.h>

#include "mas_client_session.h"


/*
this:
  mas_client_session.c
related:
  mas_client.c
  mas_client_main.c
  mas_client_readline.c
  mas_client_sig.c
*/


static int
_mas_client_exchange( mas_channel_t * pchannel, const char *question, mas_header_t * pheader, const char *answer_format )
{
  int r = 0;
  char *answer = NULL;

#ifdef MAS_USE_CURSES
  WINDOW *w_win;

  w_win = w_main;
#endif
  tMSG( "to write data string %s", question );
  /* r = mas_channel_write_message( pchannel, question, NULL ); */
  IEVAL( r, mas_channel_write_message( pchannel, question, NULL ) );
  HMSG( "(%d) written data string %s", r, question );
/*   if ( r <= 0 )            */
/*   {                        */
/* #ifdef EMSG                */
/*     EMSG( "WRITE error" ); */
/* #endif                     */
/*     P_ERR;                 */
/*   }                        */
/*   else                     */
  {
    HMSG( "TO READ MESSAGE" );
    /* r = mas_channel_read_message( pchannel, &answer, pheader ); */

#if 0
    {
      const mas_header_t *pheader_data;

      pheader_data = ( mas_header_t * ) mas_channel_buffer( pchannel, NULL );
      if ( pheader_data && pheader_data->sign == MSG_SIGNATURE )
      {
        while ( sizeof( mas_header_t ) + pheader_data->len < pchannel->buffer.length )
        {
          HMSG( "pheader_data->len:%u; buflen:%lu", pheader_data->len, pchannel->buffer.length );
          mas_channel_read_some( pchannel );
        }
        *pheader = *pheader_data;
      }
    }
#else
    pchannel->buffer.maxread = 1028 * 4;
    IEVAL( r, mas_channel_read_message( pchannel, &answer, pheader ) );
#endif





    HMSG( "GOT MESSAGE(%d:%u) [hdr:%lu]", r, pheader->len, ( unsigned long ) sizeof( mas_header_t ) );
    if ( r <= 0 )
    {
/* #ifdef EMSG                 */
/*       EMSG( "READ error" ); */
/* #endif                      */
/*       P_ERR;                */
      IEVAL( r, -1 );
      mas_channel_close( pchannel );
    }
    else if ( pheader && pheader->binary )
    {
      tMSG( "(%d) BINARY from %lx : %lx", r, ( unsigned long ) pheader->pid, ( unsigned long ) pheader->pth );
      switch ( pheader->binary )
      {
      case MSG_BIN_OPTS:
        MSG( "it's OPTS" );
        {
          mas_options_t *new_opts;

          new_opts = ( mas_options_t * ) answer;
          /* MSG( "opts:%x", ( *( unsigned int * ) new_opts ) ); */
          /* MSG( "msg:%d / %d", new_opts->msg_c, new_opts->msg_s ); */
          /* MSG( "msg:%d / %d", opts.f.bit.msg_c, opts.f.bit.msg_s ); */
          opts = *new_opts;
          /* HMSG( "opts msg_c:%d", opts.f.bit.msg_c );     */
          /* HMSG( "opts msg_s:%d", opts.f.bit.msg_s );     */
          /* HMSG( "opts msg_tr:%d", opts.f.bit.msg_tr );   */
          /* HMSG( "opts msg_m:%d", opts.f.bit.msg_m );     */
          /* HMSG( "opts msg_r:%d", opts.f.bit.msg_r );     */
          /* HMSG( "opts msg_l:%d", opts.f.bit.msg_l );     */
          /* HMSG( "opts msg_w:%d", opts.f.bit.msg_w );     */
          /* HMSG( "opts msg_th:%d", opts.f.bit.msg_th );   */
          /* HMSG( "opts msg_sg:%d", opts.f.bit.msg_sg );   */
          /* HMSG( "opts msg_mem:%d", opts.f.bit.msg_mem ); */
          /* MSG( "msg:%d / %d", opts.f.bit.msg_c, opts.f.bit.msg_s ); */
        }
        break;
      case MSG_BIN_EMPTY_COMMAND:
        MSG( "Empty command" );
        break;
      case MSG_BIN_UNKNOWN_COMMAND:
#ifdef EMSG
        EMSG( "Unknown command : %s", answer && *answer ? answer : "?" );
#endif
        break;
      case MSG_BIN_ERROR_IN_COMMAND:
#ifdef EMSG
        EMSG( "Command : %s", answer && *answer ? answer : "?" );
#endif
        break;
      case MSG_BIN_QUIT:
        MSG( "it's QUIT" );
        ctrl.in_client = 0;
        ctrl.in_pipe--;
        break;
      case MSG_BIN_RESTART:
        MSG( "it's RESTART" );
        ctrl.restart = 1;
        ctrl.in_client = 0;
        ctrl.in_pipe--;
        break;
      case MSG_BIN_DISCONNECT:
        ctrl.in_pipe--;
        break;
      default:
        tMSG( "it's unknown BIN%d", pheader->binary );
        break;
      }
    }
    else if ( answer && *answer && 0 == strcmp( answer, "[QUIT]" ) )
    {
      tMSG( "it's NOBIN; %s", answer );
      ctrl.in_client = 0;
      ctrl.in_pipe--;
    }
    else if ( answer && *answer )
    {
      tMSG( "it's NOBIN; %s", answer );
      if ( answer_format )
      {
        fprintf( stdout, answer_format, answer );
      }
      else
      {
        fprintf( stdout, "%s", answer );
      }
    }
    else
    {
      HMSG( "nothing (r:%d) from %lx - %lx\n", r, ( unsigned long ) ( pheader ? pheader->pid : 0 ),
            ( unsigned long ) ( pheader ? pheader->pth : 0 ) );
    }
    mas_free( answer );
  }
  return r;
}

int
mas_client_exchange( mas_channel_t * pchannel, const char *question, const char *answer_format )
{
  int r = 0;
  mas_header_t header;

  HMSG( "CLIENT EXCHANGE" );
  memset( &header, 0, sizeof( header ) );
  IEVAL( r, _mas_client_exchange( pchannel, question, &header, answer_format ) );
  if ( ( !( r < 0 ) ) && header.new_opts )
  {
    /* MSG( "NEW OPTS, get opts : %d", header.new_opts ); */
    /* r = _mas_client_exchange( pchannel, "get opts", &header, NULL ); */
    IEVAL( r, _mas_client_exchange( pchannel, "get opts", &header, NULL ) );
  }
  return r;
}
