#include "mas_client_def.h"
#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;


#include "mas_client_session.h"
#include "mas_client.h"

#include "mas_client_readline.h"

/*
this:
  mas_client_readline.c
related:
  mas_client.c
  mas_client_main.c
  mas_client_sig.c
more:
  mas_cmdmod_client.c
  mas_client_session.c
  mas_init_client.c
  mas_client_session.h
  mas_init_client.h
  mas_client_readline.h
  mas_client.h
  mas_listener_wait_client.h
  mas_listener_wait_client.c

*/



int
mas_exchange_with_readline( mas_channel_t * pchannel )
{
  int r = 0;
  char *mas_readline_buffer = NULL;

  while ( !mas_readline_buffer )
  {
    char prompt[256];

    ctrl.status = MAS_STATUS_WAIT;
    /* mas_readline_buffer = readline( " % \x1b[K" ); */
    /* rl_catch_signals = 0; */
    snprintf( prompt, sizeof( prompt ), "(bye to force exit) (%u) %% ", ctrl.restart_cnt );
    mas_readline_buffer = readline( prompt );
    ctrl.status = MAS_STATUS_WORK;
    {
      HIST_ENTRY *he;
      int hp;

      hp = where_history(  );
      /* if ( hp >= 0 )          */
      /* {                       */
      /*   remove_history( hp ); */
      /* }                       */
      he = history_get( history_length );
      /* mMSG( "HISTORY:%s", he->line ); */
#if 0
      mMSG( "HISTORY ADD:%s {%s} hp:%d", mas_readline_buffer, he ? he->line : "-", hp ? 1 : 0 );
      add_history( mas_readline_buffer );
#else
      if ( mas_readline_buffer && *mas_readline_buffer )
      {
        if ( he && 0 != strcmp( he->line, mas_readline_buffer ) )
        {
          /* mMSG( "HISTORY ADD:%s {%s} hp:%d", mas_readline_buffer, he ? he->line : "-", hp ? 1 : 0 ); */
          add_history( mas_readline_buffer );
        }
        else
        {
          HMSG( "HISTORY NOT ADDED:%s {%s}", he ? he->line : NULL, mas_readline_buffer );
        }
      }
#endif
    }
    ctrl.int_cnt = ctrl.term_cnt = ctrl.try_cnt = 0;

    if ( mas_readline_buffer )
    {
      /* HMSG( "readline:%s", mas_readline_buffer ); */
      if ( 0 == strcmp( mas_readline_buffer, "bye" ) )
      {
        ctrl.in_pipe--;
        ctrl.in_client = 0;
      }
      else if ( 0 == strcmp( mas_readline_buffer, "reconnect" ) )
      {
        ctrl.in_pipe--;
      }
      else
      {
//      r = mas_client_exchange( pchannel, mas_readline_buffer, "answer\n>\x1b[1;44;33m%s\x1b[0m<\n" );
        r = mas_client_exchange( pchannel, mas_readline_buffer, "%s\n" );
        mas_other_free( mas_readline_buffer );
        mas_readline_buffer = NULL;
      }
      break;
    }
    else
    {
      MSG( "NIL\x1b[K" );
      usleep( 300 );
      /* sleep( 1 ); */
      MSG( "\r\x1b[K" );
    }
  }
  return r;
}

int
mas_client_init_readline( void )
{
  int rh = 0;

  HMSG( "HISTORY to LOAD" );
  if ( opts.configdir )
  {
    char *fpath = NULL;

    fpath = mas_strdup( opts.configdir );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, "history" );

    rh = read_history( fpath );
    HMSG( "(%d) HISTORY LOAD: %s", rh, fpath );
    mas_free( fpath );
  }
  else
  {
    EMSG( "configdir not set" );
  }
  if ( rh != 0 )
  {
    /* HMSG( "init readline" ); */
    add_history( "pwd" );
    add_history( "system date" );
    add_history( "system cal" );
    add_history( "set msg on" );
    add_history( "set msg off" );
    add_history( "get mem info" );
    add_history( "get mim info" );
    add_history( "get mim anfo" );
    add_history( "quit" );
    add_history( "exit" );
    add_history( "bye" );
    add_history( "reconnect" );
    add_history( "restart" );
    add_history( "get client info" );
    add_history( "get server info" );
    add_history( "server listener remove zocromas.mastar.lan 5003" );
    /* HMSG( "/init readline" ); */
  }
  return 0;
}

void
mas_client_destroy_readline( void )
{
  HMSG( "HISTORY to SAVE" );
  if ( opts.configdir )
  {
    int rh = 0;
    char *fpath = NULL;

    fpath = mas_strdup( opts.configdir );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, "history" );

    rh = write_history( fpath );
    HMSG( "(%d) HISTORY SAVE: %s", rh, fpath );
    mas_free( fpath );
  }
}
