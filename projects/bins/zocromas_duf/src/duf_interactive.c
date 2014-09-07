#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "duf_trace_defs.h"
#include "duf_optable_def.h"

#include "duf_utils_print.h"

#include "duf_config_ref.h"
#include "duf_options.h"
#include "duf_option_cmd.h"
#include "duf_option_help.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"


/* ###################################################################### */
#include "duf_interactive.h"
/* ###################################################################### */



static const duf_longval_extended_t _lo_extended_i[] = {
  {.o = {DO_Q( "version" ), /*        */ DO_A_N, /* */ DO_V( VERSION )} /*        */ , DO_CL( REFERENCE ) /* */ ,
   /*      */ DO_T( AFUN ), DO_AFUN( duf_option_version ) /*                                                 */ , DO_H( version info ) /*       */ },
  {.o = {DO_Q( "quit" ), /*   */ DO_A_N, /* */ DO_VF( INTERACTIVE )} /* */ , DO_CL( CONTROL ) /* */ ,
   /*      */ DO_O( NOFLAG, cli.act.v ), DO_FL( act, interactive ) /*                                       */ , DO_H( get dir info ) /*       */ },

  {0}
};

const duf_longval_extended_t *lo_extended_i = _lo_extended_i;
unsigned lo_extended_i_count = sizeof( _lo_extended_i ) / sizeof( _lo_extended_i[0] );

int
duf_interactive( const char *real_path )
{
  int r = 0;

  DUF_UNUSED duf_depthinfo_t di = {.depth = -1,
    .seq = 0,
    .levinfo = NULL,
    .u = duf_config->u,
  };
  r = duf_pdi_init_wrap( &di, real_path, 0 );

  /* rl_generic_bind( ISMACR, "\x1bq", ( char * ) "server exit\n", rl_get_keymap(  ) );     */
  /* rl_generic_bind( ISMACR, "\x1b[21~", ( char * ) "server exit\n", rl_get_keymap(  ) );  */
  /* rl_generic_bind( ISMACR, "\x1bi", ( char * ) "get server info\n", rl_get_keymap(  ) ); */
  /* rl_generic_bind( ISMACR, "\x1b" "b", ( char * ) "bye\n", rl_get_keymap(  ) );          */

  /* snprintf( rl_prompt, sizeof( rl_prompt ), "(bye to force exit) (%u) %% ", ctrl.restart_cnt ); */
  /* rh = read_history( fpath ); */

  {
    char *duf_rl_buffer = NULL;
    static char rl_prompt[256] = "";

    if ( !*rl_prompt )
      snprintf( rl_prompt, sizeof( rl_prompt ), "%s> ", real_path );

    while ( DUF_ACT_FLAG( interactive ) )
    {
      while ( !duf_rl_buffer )
        duf_rl_buffer = readline( rl_prompt );
      if ( duf_rl_buffer && *duf_rl_buffer )
        add_history( duf_rl_buffer );
      r = _duf_cli_getcmd_long( duf_rl_buffer, lo_extended_i, lo_extended_i_count );
      free( duf_rl_buffer );
      duf_rl_buffer = NULL;
    }
  }
  return r;
}
