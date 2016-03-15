#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/tools/mas_argvc_tools.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ✗ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ✗ *) */
/* #include "duf_dodefs.h"                                              (* DOR ✗ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_output_util.h" */

/* #include "duf_option_descr.h" */
/* #include "duf_option_names.h" */

/* ###################################################################### */
#include "duf_optimpl_misc.h"
/* ###################################################################### */

SR( SNIPPET_OPTION, optimpl_O_history, void )
{
/* DUF_STARTR( r ); */

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
/* MAST_TRACE( temp, 0, "@@history length:%d; offset:%d; file:%s", phstate->length, phstate->offset, DUF_CONFIGG( cli.output.history_filename ) ); */
  MAST_TRACE( temp, 0, "@@history length:%d; offset:%d;", phstate->length, phstate->offset );

/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_history, void );
}

SR( SNIPPET_OPTION, optimpl_O_list_targ1, mas_argvc_t * targ, long n )
{
/* DUF_STARTR( r ); */

  if ( targ->argc && targ->argv )
    for ( int ia = 0; ia < targ->argc; ia++ )
    {
      DUF_PRINTF( 0, "%s %d. %s", n == ia ? "*" : " ", ia, targ->argv[ia] );
    }

/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_list_targ1, mas_argvc_t * targ, long n );
}

SR( SNIPPET_OPTION, optimpl_O_list_targ2, int *ptargc, char ***ptargv, long n )
{
/* DUF_STARTR( r ); */

  if ( ptargc && ptargv )
  {
    int targc = *ptargc;
    char **targv = *ptargv;

    if ( targc && targv )
      for ( int ia = 0; ia < targc; ia++ )
      {
        DUF_PRINTF( 0, "%s %d. %s", n == ia ? "*" : " ", ia, targv[ia] );
      }
  }

/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_list_targ2, int *ptargc, char ***ptargv, long n );
}

SR( SNIPPET_OPTION, optimpl_O_clear_targ1, mas_argvc_t * targ, long n )
{
/* DUF_STARTR( r ); */
#if 0
  if ( targ->argc && targ->argv && n == 0 )
    targ->argc = mas_argv_delete( targ->argc, targ->argv );
  targ->argv = NULL;
#else
  if ( n == 0 && targ->argc && targ->argv )
    mas_argvc_delete( targ );
#endif
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_clear_targ1, mas_argvc_t * targ, long n );
}

SR( SNIPPET_OPTION, optimpl_O_clear_targ2, int *ptargc, char ***ptargv, long n )
{
/* DUF_STARTR( r ); */

  if ( ptargc && ptargv && n == 0 )
    *ptargc = mas_argv_delete( *ptargc, *ptargv );
  *ptargv = NULL;
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_clear_targ2, int *ptargc, char ***ptargv, long n );
}

SR( SNIPPET_OPTION, optimpl_O_add_targ1, mas_argvc_t * targ, const char *s )
{
/* DUF_STARTR( r ); */

  mas_add_argvc_arg( targ, s );
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_add_targ1, mas_argvc_t * targ, const char *s );
}

SR( SNIPPET_OPTION, optimpl_O_add_targ2, int *ptargc, char ***ptargv, const char *s )
{
/* DUF_STARTR( r ); */

  if ( ptargc && ptargv )
    *ptargc = mas_add_argv_arg( *ptargc, ptargv, s );

/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_add_targ2, int *ptargc, char ***ptargv, const char *s );
}

SR( SNIPPET_OPTION, optimpl_O_echo, const char *arg )
{
/* DUF_STARTR( r ); */

  DUF_PRINTF( 0, "%s", arg );

/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_echo, const char *arg );
}
