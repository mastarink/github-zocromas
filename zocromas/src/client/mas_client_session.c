#include "mas_client_def.h"
#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/channel/mas_channel.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#ifdef MAS_CLIENT_LOG
#include <mastar/log/mas_log.h>
#endif

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/msg/mas_curses.h>
/* #include "mas_common.h" */

#include "xcromas/inc/mas_message_io.h"

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
_mas_client_exchange( mas_channel_t * pchannel, const char *question, mas_header_t * header, const char *answer_format )
{
  int r = 0;
  char *answer_buffer = NULL;
  WINDOW *w_win;

  w_win = w_main;

  /* HMSG( "to write data string %s", question ); */
  r = mas_channel_write_message( pchannel, question, NULL );
  /* HMSG( "(%d) written data string %s", r, question ); */
  if ( r <= 0 )
  {
#ifdef EMSG
    EMSG( "(%d) client write err", r );
#endif
  }
  else
  {
    /* HMSG( "to read message" ); */
    r = mas_channel_read_message( pchannel, &answer_buffer, header );
    /* EMSG( "READ %d", r ); */
    if ( r <= 0 )
    {
      /* MSG( "(%d) client back none", r ); */
#ifdef EMSG
      EMSG( "(%d) none", r );
#endif
    }
    else if ( header && header->binary )
    {
      tMSG( "(%d) BINARY from %x : %lx", r, header->pid, header->pth );
      switch ( header->binary )
      {
      case MSG_BIN_OPTS:
        HMSG( "it's OPTS" );
        {
          mas_options_t *new_opts;

          new_opts = ( mas_options_t * ) answer_buffer;
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
        HMSG( "Empty command" );
        break;
      case MSG_BIN_UNKNOWN_COMMAND:
#ifdef EMSG
        EMSG( "Unknown command : %s", answer_buffer && *answer_buffer ? answer_buffer : "?" );
#endif
        break;
      case MSG_BIN_ERROR_IN_COMMAND:
#ifdef EMSG
        EMSG( "Error in command : %s", answer_buffer && *answer_buffer ? answer_buffer : "?" );
#endif
        break;
      case MSG_BIN_QUIT:
        HMSG( "it's QUIT" );
        ctrl.in_client = 0;
        ctrl.in_pipe--;
        break;
      case MSG_BIN_RESTART:
        HMSG( "it's RESTART" );
        ctrl.restart = 1;
        ctrl.in_client = 0;
        ctrl.in_pipe--;
        break;
      case MSG_BIN_DISCONNECT:
        ctrl.in_pipe--;
        break;
      default:
        break;
      }
    }
    else if ( answer_buffer && *answer_buffer && 0 == strcmp( answer_buffer, "[QUIT]" ) )
    {
      ctrl.in_client = 0;
      ctrl.in_pipe--;
    }
    else if ( answer_buffer && *answer_buffer )
    {
      if ( answer_format )
      {
        fprintf( stdout, answer_format, answer_buffer );
      }
    }
    else
    {
      tMSG( "nothing (r:%d) from %x - %lx\n", r, header ? header->pid : 0, header ? header->pth : 0 );
    }
    mas_free( answer_buffer );
  }
  return r;
}

int
mas_client_exchange( mas_channel_t * pchannel, const char *question, const char *answer_format )
{
  int r = 0;
  mas_header_t header;

  memset( &header, 0, sizeof( header ) );
  r = _mas_client_exchange( pchannel, question, &header, answer_format );
  if ( header.new_opts )
  {
    HMSG( "before _mas_client_exchange" );
    /* MSG( "NEW OPTS, get opts : %d", header.new_opts ); */
    r = _mas_client_exchange( pchannel, "get opts", &header, NULL );
    HMSG( "after exchange_cs" );
  }

  return r;
}
