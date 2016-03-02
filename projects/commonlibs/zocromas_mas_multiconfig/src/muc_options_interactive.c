/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli)
#include "muc_tracen_defs_preset.h"
#include "muc_errorn_defs_preset.h"

#include <string.h>
#include <stdlib.h>                                                  /* free */
#include <stdio.h>
#include <errno.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

/* #include "muc_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "muc_errorn_defs.h"                                         (* DUF_NOERROR; MUC_CLEAR_ERROR; DUF_E_(LOWER|UPPER); MUC_TEST_R ... ♠ *) */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */
#include "muc_option_clarify_batch.h"

/* ###################################################################### */
#include "muc_options_interactive.h"
/* ###################################################################### */

muc_SR( OPTIONS, source_interactive_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive
    __attribute__ ( ( unused ) ), muc_cpchar_void_func_t cb_prompt_interactive
    __attribute__ ( ( unused ) ), muc_option_adata_t * paod, muc_option_source_code_t sourcecode MAS_UNUSED )
{

#if 0
  static char rl_prompt[256 * 10] = "";
#endif
  const char *prompt = NULL;

  muc_QT( "@   source:%s", muc_optsourcecode_name( cli, sourcecode ) );
  if ( istage == MUC_OPTION_STAGE_INTERACTIVE /* XXX ???? XXX */  )
  {
    MAST_TRACE( options, 0, "@@@@stage:%s source: interactive", muc_optstage_name( cli, istage ) );
#if 0
    if ( !*rl_prompt )
      snprintf( rl_prompt, sizeof( rl_prompt ), "%s> ", "CMD" );
#endif

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
#if 0
      if ( DUF_CONFIGG( cli.history_filename ) )
        read_history( DUF_CONFIGG( cli.history_filename ) );
#else
      if ( muc_cli_options_get_history_filename( cli ) )
        read_history( muc_cli_options_get_history_filename( cli ) );
#endif

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
              MUC_PRINTF( 0, "%s:%s", he->timestamp, he->line );
          }
        }
        MAST_TRACE( temp, 0, "@@history length:%d; offset:%d; file:%s", phstate->length, phstate->offset, DUF_CONFIGG( cli.history_filename ) );
      }
#endif
      while ( QNOERR /* && DUF_ACTG_FLAG( interactive ) */  && cb_do_interactive(  ) /* don't remove: this is for quit */
              && isatty( STDIN_FILENO ) /* only when stdin is tty */  )
      {
        char *rl_buffer = NULL, *s = NULL;

/* TODO : via callback of some kind */

        prompt = cb_prompt_interactive(  );

        while ( !rl_buffer )
          rl_buffer = readline( prompt );
        s = rl_buffer;
        if ( s && *s )
        {
          s = mas_chomp( s );
          add_history( s );
          MAST_TRACE( options, 0, "@@@@read cmd '%s'", s );
          if ( s && ( ( *s == '#' ) || !*s ) )
            continue;
        /* MAST_TRACE( any, 0, "buffer:[%s]", buffer ); */
          MAST_TRACE( explain, 0, "read config line %s", s );
          {
            {
              muc_CR( boption_xclarify_at_stdx, cli, 0 /* value_separator */ , istage, MUC_OPTION_SOURCE( INTERACTIVE ), s, 0, paod );
            }
            MAST_TRACE( options, 0, "@@@@executed cmd; QERRIND=%d; s=%s", QERRIND, s );
          }
          free( rl_buffer );
          rl_buffer = NULL;
        }
      /* MUC_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
        ERRCLEAR( OPTION_NOT_FOUND );
        muc_QTR;
      }
#if 0
      if ( DUF_CONFIGG( cli.history_filename ) )
        write_history( DUF_CONFIGG( cli.history_filename ) );
#else
      if ( muc_cli_options_get_history_filename( cli ) )
        write_history( muc_cli_options_get_history_filename( cli ) );
#endif
    }
  }

  muc_ER( OPTIONS, source_interactive_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive
      __attribute__ ( ( unused ) ), muc_cpchar_void_func_t cb_prompt_interactive
      __attribute__ ( ( unused ) ), muc_option_adata_t * paod, muc_option_source_code_t sourcecode );
}
