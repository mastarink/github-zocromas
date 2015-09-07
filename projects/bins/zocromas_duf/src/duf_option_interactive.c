#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"

#include "duf_option_defs.h"

#include "duf_config_ref.h"
#include "duf_option_cmd.h"


/* ###################################################################### */
#include "duf_option_interactive.h"
/* ###################################################################### */

int
duf_interactive_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );
  static char rl_prompt[256 * 10] = "";

  DUF_TRACE( options, 1, "@@@stream stage:%d", istage );
  if ( !*rl_prompt )
    snprintf( rl_prompt, sizeof( rl_prompt ), "%s> ", "CMD" );

#if 0
  add_history( "quit" );
  add_history( "pwd" );
  add_history( "max-depth" );
  add_history( "max-seq" );
  add_history( "selection" );
  add_history( "same-md5 /mnt/new_media/media/photo/Pictures/photos/sel/catfly.jpg" );
  add_history( "lsfiles" );
  add_history( "list-sccb" );
  add_history( "list-targ" );
  add_history( "clear-targ" );
  add_history( "add-targ /mnt/new_media/media/photo/Pictures/photos/sel/" );
  add_history( "cd /mnt/new_media/media/photo/Pictures/photos/" );
  add_history( "tree" );
  add_history( "recursive" );
  add_history( "no-recursive" );
#endif

  if ( duf_config->cli.output.history_filename )
    read_history( duf_config->cli.output.history_filename );
  {
    HISTORY_STATE *phstate;

    DUF_TRACE( temp, 0, "HISTORY:%s", duf_config->cli.output.history_filename );
    phstate = history_get_history_state(  );
    if ( phstate->length == 0 )
    {
      add_history( "quit" );
      add_history( "lsfiles" );
      add_history( "cd /" );
    }
    else
    {
      for(int i=0;i<phstate->length; i++){}
    }
    DUF_TRACE( temp, 0, "@@history length:%d; offset:%d", phstate->length, phstate->offset );
  }
  while ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( interactive ) && isatty( STDIN_FILENO ) )
  {
    char *rl_buffer = NULL, *s = NULL;

    DUF_TRACE( path, 0, "@path@pdi: %s", duf_levinfo_path( duf_config->pdi ) );
    snprintf( rl_prompt, sizeof( rl_prompt ), "%s:%s> ", "db", duf_levinfo_path( duf_config->pdi ) );



    while ( !rl_buffer )
      rl_buffer = readline( rl_prompt );


    s = rl_buffer;
    if ( s && *s )
    {
      s = mas_chomp( s );
      add_history( s );

      DUF_TRACE( options, 0, "@@@@read cmd '%s'", s );
      if ( s && ( ( *s == '#' ) || !*s ) )
        continue;
      /* DUF_TRACE( any, 0, "buffer:[%s]", buffer ); */
      DUF_TRACE( explain, 0, "read config line %s", s );
      {
        char *xs;

        xs = mas_expand_string( s );
        DUF_TRACE( explain, 0, "expanded config line %s", xs );
/* 
 * duf_execute_cmd_long_xtables_std return codeval>0 for "help" option
 *   =0 for other option
 *   errorcode<0 for error
 * */
        DOR( r, duf_execute_cmd_long_xtables_std( xs, ' ', istage ) );

        DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );

        DUF_TRACE( options, 0, "@@@@executed cmd; r=%d; xs=%s [i/a:%d] pdi:%d;", r, xs, DUF_ACTG_FLAG( interactive ), duf_config->pdi ? 1 : 0 );
        mas_free( xs );
      }
      free( rl_buffer );
      rl_buffer = NULL;
    }
    if ( duf_config->cli.output.history_filename )
      write_history( duf_config->cli.output.history_filename );
  }
  DEBUG_ENDR( r );
}
