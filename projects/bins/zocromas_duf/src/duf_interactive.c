#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_utils_print.h"

#include "duf_config_ref.h"

#include "duf_option_defs.h"
#include "duf_option_cmd.h"

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"

/* ###################################################################### */
#include "duf_interactive.h"
/* ###################################################################### */


int
duf_interactive( void )
{
  int r = 0;

  /* DUF_UNUSED duf_depthinfo_t di = {.depth = -1, */
  /*   .seq = 0,                                   */
  /*   .levinfo = NULL,                            */
  /*   .u = duf_config->u,                         */
  /* };                                            */
  /* r = duf_pdi_init_wrap( &di, real_path, 0 );   */

  /* rl_generic_bind( ISMACR, "\x1bq", ( char * ) "server exit\n", rl_get_keymap(  ) );     */
  /* rl_generic_bind( ISMACR, "\x1b[21~", ( char * ) "server exit\n", rl_get_keymap(  ) );  */
  /* rl_generic_bind( ISMACR, "\x1bi", ( char * ) "get server info\n", rl_get_keymap(  ) ); */
  /* rl_generic_bind( ISMACR, "\x1b" "b", ( char * ) "bye\n", rl_get_keymap(  ) );          */

  /* snprintf( rl_prompt, sizeof( rl_prompt ), "(bye to force exit) (%u) %% ", ctrl.restart_cnt ); */
  /* rh = read_history( fpath ); */

  /* duf_pdi_close( &duf_config->di ); */
  assert( duf_config->pdi );
  assert( duf_config->pdi->levinfo );
  /* if ( r >= 0 ) */
  {
    char *duf_rl_buffer = NULL;
    static char rl_prompt[256 * 10] = "";

    DUF_E_MAX( 5, DUF_ERROR_TOO_DEEP, DUF_ERROR_LEVINFO_SIZE );

    if ( !*rl_prompt )
      snprintf( rl_prompt, sizeof( rl_prompt ), "%s> ", "CMD" );

    add_history( "cd /mnt/new_media/media/photo/Pictures/photos/" );
    add_history( "quit" );
    add_history( "pwd" );
    add_history( "max-depth" );
    add_history( "max-seq" );

    while ( r >= 0 && DUF_ACT_FLAG( interactive ) )
    {
      r = 0;
      DOR( r, duf_pdi_reinit_oldpath( duf_config->pdi, DUF_U_FLAG( recursive ) ) );

      if ( duf_config && duf_config->pdi )
        snprintf( rl_prompt, sizeof( rl_prompt ), "%s:%s> ", "db", duf_levinfo_path( duf_config->pdi ) );
      while ( !duf_rl_buffer )
        duf_rl_buffer = readline( rl_prompt );
      mas_chomp( duf_rl_buffer );
      if ( *duf_rl_buffer )
      {
        if ( duf_rl_buffer && *duf_rl_buffer )
          add_history( duf_rl_buffer );
        /* DOR( r, duf_execute_cmd_long( duf_rl_buffer, lo_extended_i, lo_extended_i_count, ' ', 1 ) ); */
        DOR( r, duf_execute_cmd_long_std( duf_rl_buffer, ' ', 1 ) );
      }
      /* DUF_TEST_R( r ); */
      DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_MULTIPLE, DUF_ERROR_OPTION, DUF_ERROR_OPTION_VALUE, DUF_ERROR_PATH, DUF_ERROR_MAX_SEQ_REACHED );
      free( duf_rl_buffer );
      duf_rl_buffer = NULL;
      fprintf( stderr, "\n" );
    }
  }
  return r;
}
