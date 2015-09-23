#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_levinfo_ref.h"

#include "duf_option_defs.h"

#include "duf_config_ref.h"
#include "duf_option_cmd.h"


/* ###################################################################### */
#include "duf_options_string.h"
#include "duf_options_interactive.h"
/* ###################################################################### */

int
duf_interactive_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );
  static char rl_prompt[256 * 10] = "";

  if ( istage == DUF_OPTION_STAGE_INTERACTIVE /* XXX ???? XXX */  )
  {
    DUF_TRACE( options, 0, "@@@@(%d) source: interactive", istage );
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
    {
      if ( DUF_CONFIGG( cli.output.history_filename ) )
        read_history( DUF_CONFIGG( cli.output.history_filename ) );
#if 0
      {
        HISTORY_STATE *phstate;

        phstate = history_get_history_state(  );
        if ( phstate->length == 0 )
        {
          add_history( "quit" );
          add_history( "lsfiles" );
          add_history( "cd /" );
        }
        else
        {
          for ( int i = 0; i < phstate->length; i++ )
          {
            HIST_ENTRY *he;

            he = history_get( i );
            if ( he )
              DUF_PRINTF( 0, "%s:%s", he->timestamp, he->line );
          }
        }
        DUF_TRACE( temp, 0, "@@history length:%d; offset:%d; file:%s", phstate->length, phstate->offset, DUF_CONFIGG( cli.output.history_filename ) );
      }
#endif
      while ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( interactive ) /* don't remove: this is for quit */
              && isatty( STDIN_FILENO ) /* only when stdin is tty */  )
      {
        char *rl_buffer = NULL, *s = NULL;

        DUF_TRACE( path, 0, "@path@pdi: %s", duf_levinfo_path( DUF_CONFIGG( pdi ) ) );
        snprintf( rl_prompt, sizeof( rl_prompt ), "A-F:%d;A-D:%d; %s:%s> ", DUF_ACTG_FLAG( allow_files ), DUF_ACTG_FLAG( allow_dirs ), "db",
                  duf_levinfo_path( DUF_CONFIGG( pdi ) ) );
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
#if 0
            char *xs;

            xs = mas_expand_string( s );
            DUF_TRACE( explain, 0, "expanded config line %s", xs );
/* 
 * duf_exec_cmd_long_xtables_std return codeval>0 for "help" option
 *   =0 for other option
 *   errorcode<0 for error
 * */
            DOR( r, duf_exec_cmd_long_xtables_std( xs, ' ', istage, DUF_OPTION_SOURCE_INTERACTIVE ) );
            DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
            DUF_TRACE( options, 0, "@@@@executed cmd; r=%d; xs=%s [i/a:%d] pdi:%d;", r, xs, DUF_ACTG_FLAG( interactive ),
                       DUF_CONFIGG( pdi ) ? 1 : 0 );
            mas_free( xs );
#else
            DOR( r, duf_string_options_at_string( 0 /* vseparator */ , istage, DUF_OPTION_SOURCE_INTERACTIVE, s, 0 ) );
            DUF_TRACE( options, 0, "@@@@executed cmd; r=%d; s=%s [i/a:%d] pdi:%d;", r, s, DUF_ACTG_FLAG( interactive ),
                       DUF_CONFIGG( pdi ) ? 1 : 0 );
#endif
          }
          free( rl_buffer );
          rl_buffer = NULL;
        }
        if ( DUF_CONFIGG( cli.output.history_filename ) )
          write_history( DUF_CONFIGG( cli.output.history_filename ) );
      }
    }
  }
  DEBUG_ENDR( r );
}
