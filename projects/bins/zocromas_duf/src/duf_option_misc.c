#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "duf_maintenance.h"

#include "duf_sccb.h"


#include "duf_option_descr.h"
/* #include "duf_option_extended.h" */
#include "duf_option_names.h"
/* #include "duf_option_restore.h" */
#include "duf_option.h"


/* ###################################################################### */
#include "duf_option_misc.h"
/* ###################################################################### */

mas_error_code_t
duf_option_$_history( void )
{
  DEBUG_STARTR( r );

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
  /* DUF_TRACE( temp, 0, "@@history length:%d; offset:%d; file:%s", phstate->length, phstate->offset, DUF_CONFIGG( cli.output.history_filename ) ); */
  DUF_TRACE( temp, 0, "@@history length:%d; offset:%d;", phstate->length, phstate->offset );

  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_$_list_targ1( mas_argvc_t * targ, long n )
{
  DEBUG_STARTR( r );

  if ( targ->argc && targ->argv )
    for ( int ia = 0; ia < targ->argc; ia++ )
    {
      DUF_PRINTF( 0, "%s %d. %s", n == ia ? "*" : " ", ia, targ->argv[ia] );
    }

  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_$_list_targ2( int *ptargc, char ***ptargv, long n )
{
  DEBUG_STARTR( r );

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

  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_$_clear_targ1( mas_argvc_t * targ, long n )
{
  DEBUG_STARTR( r );
#if 0
  if ( targ->argc && targ->argv && n == 0 )
    targ->argc = mas_argv_delete( targ->argc, targ->argv );
  targ->argv = NULL;
#else
  if ( n == 0 && targ->argc && targ->argv )
    mas_argvc_delete( targ );
#endif
  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_$_clear_targ2( int *ptargc, char ***ptargv, long n )
{
  DEBUG_STARTR( r );

  if ( ptargc && ptargv && n == 0 )
    *ptargc = mas_argv_delete( *ptargc, *ptargv );
  *ptargv = NULL;
  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_$_add_targ1( mas_argvc_t * targ, const char *s )
{
  DEBUG_STARTR( r );

  mas_add_argvc_arg( targ, s );
  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_$_add_targ2( int *ptargc, char ***ptargv, const char *s )
{
  DEBUG_STARTR( r );

  if ( ptargc && ptargv )
    *ptargc = mas_add_argv_arg( *ptargc, ptargv, s );

  DEBUG_ENDR( r );
}


mas_error_code_t
duf_option_$_echo( const char *arg )
{
  DEBUG_STARTR( r );

  DUF_PRINTF( 0, "%s", arg );

  DEBUG_ENDR( r );
}
