#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


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

static char prompt[256];


int
mas_exchange_with_readline( mas_channel_t * pchannel )
{
  int r = 0;
  char *mas_readline_buffer = NULL;

  while ( !mas_readline_buffer )
  {

    ctrl.status = MAS_STATUS_WAIT;
    /* mas_readline_buffer = readline( " % \x1b[K" ); */
    /* rl_catch_signals = 0; */
    mas_readline_buffer = readline( prompt );
    ctrl.status = MAS_STATUS_WORK;
    {
      HIST_ENTRY *he;

      /* int hp; */

      ( void ) /* hp = */ where_history(  );
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
          WMSG( "HISTORY NOT ADDED:%s {%s}", he ? he->line : NULL, mas_readline_buffer );
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
        /* r = mas_client_exchange( pchannel, mas_readline_buffer, "%s\n" ); */
        IEVAL( r, mas_client_exchange( pchannel, mas_readline_buffer, "%s\n" ) );
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
mas_client_readline_event( void )
{
  /* HMSG( "HOOK" ); */
  return 0;
}

/*                                            */
/* "\e[3;3~":  "# Alt-Delete\n"               */
/* "\eO1;3P":  "# Alt-F1\n"                   */
/* "\e[21;3~": "# Alt-F10\n"                  */
/* "\e[23;3~": "# Alt-F11\n"                  */
/* "\e[24;3~": "# Alt-F12\n"                  */
/* "\eO1;3Q":  "# Alt-F2\n"                   */
/* "\eO1;3R":  "# Alt-F3\n"                   */
/* "\eO1;3S":  "# Alt-F4\n"                   */
/* "\e[15;3~": "# Alt-F5\n"                   */
/* "\e[17;3~": "# Alt-F6\n"                   */
/* "\e[18;3~": "# Alt-F7\n"                   */
/* "\e[19;3~": "# Alt-F8\n"                   */
/* "\e[20;3~": "# Alt-F9\n"                   */
/* "\e[3;5~":  "# Control-Delete\n"           */
/* "\e[1;5B":  "# Control-Down\n"             */
/* "\e[21;5~": "# Control-F10\n"              */
/* "\e[23;5~": "# Control-F11\n"              */
/* "\e[24;5~": "# Control-F12\n"              */
/* "\eO1;5P":  "# Control-F1\n" # not working */
/* "\eO1;5Q":  "# Control-F2\n"               */
/* "\eO1;5R":  "# Control-F3\n"               */
/* "\eO1;5S":  "# Control-F4\n"               */
/* "\e[15;5~": "# Control-F5\n"               */
/* "\e[17;5~": "# Control-F6\n"               */
/* "\e[18;5~": "# Control-F7\n"               */
/* "\e[19;5~": "# Control-F8\n"               */
/* "\e[20;5~": "# Control-F9\n"               */
/* "\e[16;5~": "# Control-Fx\n"               */
/* "\e[1;5D":  "# Control-Left\n"             */
/* "\e[1;5C":  "# Control-Right\n"            */
/* "\e[1;5A":  "# Control-Up\n"               */
/* "\e[3~":    "# Delete\n"                   */
/* "\eOF":     "# End\n"                      */
/* "\eOP":     "# F01\n"                      */
/* "\e[21~":   "# F10\n"                      */
/* "\e[23~":   "# F11\n"                      */
/* "\e[24~":   "# F12\n"                      */
/* "\eOQ":     "# F2\n"                       */
/* "\eOR":     "# F3\n"                       */
/* "\eOS":     "# F4\n"                       */
/* "\e[15~":   "# F5\n"                       */
/* "\e[17~":   "# F6\n"                       */
/* "\e[18~":   "# F7\n"                       */
/* "\e[19~":   "# F8\n"                       */
/* "\e[20~":   "# F9\n"                       */
/* "\e[22~":   "# Fx\n"                       */
/* "\e[16~":   "# Fx1\n"                      */
/* "\eOH":     "# Home\n"                     */
/* "\e[2~":    "# Insert\n"                   */
/* "\e[6~":    "# PageDown"                   */
/* "\e[5~":    "# PageUp\n"                   */
/* "\e[3;2~":  "# Shift-Delete\n"             */
/* "\eO1;2P":  "# Shift-F1\n"                 */
/* #"\e[22;2~": "# Shift-F10\n" # not working */
/* "\e[23;2~": "# Shift-F11\n"                */
/* "\e[24;2~": "# Shift-F12\n"                */
/* "\eO1;2Q":  "# Shift-F2\n"                 */
/* "\eO1;2R":  "# Shift-F3\n"                 */
/* "\eO1;2S":  "# Shift-F4\n"                 */
/* "\e[15;2~": "# Shift-F5\n"                 */
/* "\e[17;2~": "# Shift-F6\n"                 */
/* "\e[18;2~": "# Shift-F7\n"                 */
/* "\e[19;2~": "# Shift-F8\n"                 */
/* "\e[20;2~": "# Shift-F9\n"                 */
/* "\e[21;2~": "# Shift-F10\n"                */
/* "\e[16;2~": "# Shift-Fx1\n"                */
/*                                            */
/* "\eq": "# Alt-q\n"                         */
/* "\ew": "# Alt-w\n"                         */
/* "\ee": "# Alt-e\n"                         */
/* "\er": "# Alt-r\n"                         */
/* "\et": "# Alt-t\n"                         */
/* "\ey": "# Alt-y\n"                         */
/* "\eu": "# Alt-u\n"                         */
/* "\ei": "# Alt-i\n"                         */
/* "\eo": "# Alt-o\n"                         */
/* "\ep": "# Alt-p\n"                         */
/* "\e[": "# Alt-[\n"                         */
/* "\e]": "# Alt-]\n"                         */
/* "\ea": "# Alt-a\n"                         */
/* "\es": "# Alt-s\n"                         */
/* #"\ed": "# Alt-d\n"                        */
/* "\ef": "# Alt-f\n"                         */
/* "\eg": "# Alt-g\n"                         */
/* "\eh": "# Alt-h\n"                         */
/* "\ej": "# Alt-j\n"                         */
/* "\ek": "# Alt-k\n"                         */
/* "\el": "# Alt-l\n"                         */
/* "\e;": "# Alt-;\n"                         */
/* "\e'": "# Alt-'\n"                         */
/*                                            */
/* "\ez": "# Alt-z\n"                         */
/* "\ex": "# Alt-x\n"                         */
/* "\ec": "# Alt-c\n"                         */
/* "\ev": "# Alt-v\n"                         */
/* "\eb": "# Alt-b\n"                         */
/* "\en": "# Alt-n\n"                         */
/* "\em": "# Alt-m\n"                         */
/* "\e,": "# Alt-,\n"                         */
/* "\e.": "# Alt-.\n"                         */
/* "\e/": "# Alt-/\n"                         */
/* "\e\\": "# Alt-\\\n"                       */
/*                                            */
/* #Meta-u: "# Hu\n"                          */
/* #Meta-Control-u: "# MCU\n"                 */
/*                                            */
/*                                            */
int
mas_client_init_readline( void )
{
  int rh = 0;

  /* rl_add_defun( "quit", mas_client_readline_quit, CTRL( 'q' ) ); */
  /* rl_parse_and_bind( "\"\\C-q\": \"server_quit\"\n" ); */
  rl_generic_bind( ISMACR, "\x1bq", ( char * ) "server exit\n", rl_get_keymap(  ) );
  rl_generic_bind( ISMACR, "\x1b[21~", ( char * ) "server exit\n", rl_get_keymap(  ) );
  rl_generic_bind( ISMACR, "\x1bi", ( char * ) "get server info\n", rl_get_keymap(  ) );
  rl_generic_bind( ISMACR, "\x1b" "b", ( char * ) "bye\n", rl_get_keymap(  ) );

  snprintf( prompt, sizeof( prompt ), "(bye to force exit) (%u) %% ", ctrl.restart_cnt );
  /* rl_event_hook = mas_client_readline_event; */
  WMSG( "HISTORY to LOAD from %s", opts.dir.history );
  if ( opts.dir.history )
  {
    char *fpath = NULL;

    fpath = mas_strdup( opts.dir.history );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, "history" );

    rh = read_history( fpath );
    WMSG( "(%d) HISTORY LOAD: %s", rh, fpath );
    mas_free( fpath );
  }
  else
  {
    EMSG( "dir.history not set" );
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
  WMSG( "HISTORY to SAVE" );
  if ( opts.dir.history )
  {
    int rh = 0;
    char *fpath = NULL;

    fpath = mas_strdup( opts.dir.history );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, "history" );

    rh = write_history( fpath );
    WMSG( "(%d) HISTORY SAVE: %s", rh, fpath );
    mas_free( fpath );
  }
}
