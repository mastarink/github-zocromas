#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"


#include "duf_option_stage.h"

#include "duf_option_config.h"
#include "duf_option_clarify.h"
#include "duf_option_lcclarify.h"
#include "duf_option_cfind.h"

#include "duf_option_clarify_batch.h"

#include "duf_option_clarify_new.h"


/* ###################################################################### */
#include "duf_options_cli.h"
/* ###################################################################### */

DUF_UNUSED static char *
duf_clrfy_cli_opts_msgs( duf_option_code_t codeval, int optindd, int optoptt )
{
  const char *arg;
  static const char *msg = "Invalid option";
  char buffer[2048] = "";

#if 0
  arg = duf_cli_options_config(  )->carg.argv[optindd];
#else
  arg = duf_cli_options_get_cargvn( optindd );
#endif
  if ( duf_verbose(  ) )
    snprintf( buffer, sizeof( buffer ), "%s '%s' arg[%d]=\"%s\" [%u/%c/%c]", msg, arg, optindd, arg, codeval, codeval, optoptt );
  else
    snprintf( buffer, sizeof( buffer ), " %s '%s'", msg, arg );
  return mas_strdup( buffer );
}

#if 0
static void
duf_clrfy_cli_opts_msg( duf_option_code_t codeval, int optindd, int optoptt, const char *shorts_unused DUF_UNUSED )
{
  /* const char *arg; */
  /* static const char *msg = "Invalid option"; */

  /* arg = DUF_CONFIGG( cli.carg.argv )[optindd]; */
#  if 0
  if ( optoptt && codeval > ' ' && codeval <= 'z' )
  {
    if ( DUF_CONFIGG( opt.dbg.verbose ) == 0 )
      _DUF_SHOW_ERROR( "@@@@@@@@@@@ V %s '-%c'      ", msg, optoptt );
    else
    {
      if ( codeval == '?' )
        _DUF_SHOW_ERROR( "@@@@@@@@@@@ A %s '-%c' arg[%d]=\"%s\"", msg, optoptt, optindd, arg );
      else
        _DUF_SHOW_ERROR( "@@@@@@@@@@@ B %s '-%c' arg[%d]=\"%s\" [%u/%c]", msg, optoptt, optindd, arg, codeval, codeval );
    }
  }
  else
#  endif
  {
#  if 0
    if ( DUF_CONFIGG( opt.dbg.verbose ) )
      _DUF_SHOW_ERROR( "@@@@@@@@@@@ %s '%s' arg[%d]=\"%s\" [%u/%c/%c]", msg, arg, optindd, arg, codeval, codeval, optoptt );
    else
      _DUF_SHOW_ERROR( "@@@@@@@@@@@ %s '%s'", msg, arg );
#  else
    char *s;

    s = duf_clrfy_cli_opts_msgs( codeval, optindd, optoptt );
    _DUF_SHOW_ERROR( "@@@@@@@@@@@%s", s );
    mas_free( s );
#  endif
#  if 0
    for ( int i = 0; i < DUF_CONFIGG( cli.carg.argc ); i++ )
    {
      _DUF_SHOW_ERROR( "@@@ (%d) %c%d '%s'", optindd, i == optindd ? '*' : ' ', i, DUF_CONFIGG( cli.carg.argv )[i] );
    }
#  endif
  }
}
#endif

static
SR( OPTIONS, lcoption_parse, int longindex, duf_option_code_t codeval, duf_option_stage_t istage, duf_option_adata_t * paod )
{

  DUF_TRACE( options, 40, "@@@@@@getopt_long: cv:%-4d =>  ('%c') '%s' li:%d; oi:%d; oo:%d; oe:%d; stage:%s", codeval, codeval > ' '
             && codeval <= 'z' ? codeval : '?', longindex >= 0 ? duf_cli_options_get_longopts_table(  )[longindex].name : "?", longindex, optind,
             optopt, opterr, duf_optstage_name( istage ) );
  {
    int optoptt = 0;

    optoptt = 0;
    if ( codeval == '?' )
    {
      char *msg = NULL;

      optoptt = optopt;

      msg = duf_clrfy_cli_opts_msgs( codeval, optind - 1, optoptt );

      DUF_TRACE( options, +140, "@@@@@@@%d:%s", optind - 1, duf_cli_options_get_carg(  )->argv[optind - 1] );
      DUF_MAKE_ERRORM( QERRIND, DUF_ERROR_OPTION_NOT_FOUND, msg );
      mas_free( msg );
    }

#if 0                           /* 1:old clarify; 0:new clarify; see also duf_option_clarify_batch.c.... ; 20160115.170518 */
    CR( lcoption_clarify, longindex, codeval, optarg, istage, DUF_OPTION_SOURCE( CLI ) );
#else
    mas_cargvc_t *carg;
    const char *longoptname = NULL;
    const char *qarg = NULL;

    carg = duf_cli_options_get_carg(  );
    if ( optind > 0 )
      qarg = carg->argv[optind - 1];
    /* longoptname = duf_coption_find_name_at_std( codeval, QPERRIND ); */
    longoptname = duf_lcoption_find_name_at_std( codeval, longindex, QPERRIND );
    if ( longindex < 0 && codeval == '?' && !longoptname && qarg && qarg[0] == '-' && qarg[1] == '-' && qarg[2] != '-' )
      longoptname = qarg + 2;

    if ( longoptname )
      CR( soption_xclarify_new_at_stdx_default, ( const char * ) NULL, longoptname, optarg, 0 /* vseparator */ , istage, DUF_OPTION_SOURCE( CLI ),
          paod );
    else if ( codeval == '?' )
    {
      SERR( OPTION_NOT_PARSED );
      assert( 0 );
    }
    else                        /* ERROR */
    {
      SERR( OPTION_NOT_PARSED );
      assert( 0 );
    }
#endif
  }
  ER( OPTIONS, lcoption_parse, int longindex, duf_option_code_t codeval, duf_option_stage_t istage, duf_option_adata_t * paod );
}

static
SR( OPTIONS, cli_parse_targ, int optindd, duf_option_stage_t istage )
{
  int numxargv = 0;
  mas_cargvc_t *carg;

  carg = duf_cli_options_get_carg(  );
  numxargv = carg->argc - optind;
  DUF_TRACE( options, 40, "@cli opts: stage:%s; carg->argc:%d; numxargv:%d; optind:%d;", duf_optstage_name( istage ), carg->argc, numxargv, optind );

  if ( QNOERR && istage == DUF_OPTION_STAGE_SETUP && /* optind < carg.argc && */ numxargv > 0 )
  {
    DUF_TRACE( options, +150, "(for targ) carg.argv[%d]=\"%s\"", optind, carg->argv[optind] );
    CR( argv_clarify, duf_cli_options_get_targ(  ), carg, optind );

    if ( QNOERR )
      /* DUF_CONFIGWN( cli.targ_offset, duf_reorder_argvc_at_sign( DUF_CONFIGA( cli.targ ) ) ); */
      duf_cli_options_reset_targ_offset(  );
  }
  else
  {
    DUF_TRACE( options, +150, "(no targ) optind:%d; optindd:%d; numxargv:%d", optind, optindd, numxargv );
  }
  ER( OPTIONS, cli_parse_targ, int optindd, duf_option_stage_t istage );
}

/* 20160113.124316 */
static
SR( OPTIONS, cli_parse, const char *shorts, duf_option_stage_t istage, duf_option_adata_t * paod )
{
  /* DEBUG_STARTR( r ); */

  int longindex;
  duf_option_code_t codeval;
  mas_cargvc_t *carg;

  /* int optoptt = 0; */
  int optindd = 0, optindp DUF_UNUSED = 0;

  DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );
  optopt = 0;
  opterr = 0;
  optind = 1;
  optindp = optind;

  carg = duf_cli_options_get_carg(  );


  DUF_TRACE( options, 40, "@to clarify cli opts: stage:%s; carg->argc:%d", duf_optstage_name( istage ), carg->argc );


  /* optind
   *   inside loop means next arg to process in non-permuted list
   *   outside loop means first not processed arg in permuted list
   * */
#if 0
  while ( QNOERR
          &&
          ( ( int )
            ( optopt = 0, longindex = -1, codeval =
              getopt_long( carg->argc, carg->argv, shorts, duf_cli_options_get_longopts_table(  ), &longindex ) ) >= 0 ) )
  {
    optindd = optind;
    CR( lcoption_parse, longindex, codeval, istage );
    optindp = optind;
  }
#else
  do
  {
    optopt = 0, longindex = -1;

    codeval = getopt_long( carg->argc, carg->argv, shorts, duf_cli_options_get_longopts_table(  ), &longindex );
    if ( codeval >= 0 )
    {
      optindd = optind;
#  if 1
      CR( lcoption_parse, longindex, codeval, istage, paod );
/* TODO */
#  else
      const duf_longval_extended_t *extended = NULL;

      extended = duf_noption_xfind_no_at_stdx( name, arg ? 1 : 0, 1 /* soft */ , pno, pcnt, QPERRIND );
      CR( xoption_parse, extended, istage );
#  endif
      optindp = optind;
    }
  }
  while ( QNOERR && codeval >= 0 );
#endif
  CR( cli_parse_targ, optindd, istage );
  /* DEBUG_ENDR_UPPER( r, DUF_ERROR_OPTION_NOT_FOUND ); */
  DEBUG_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );
  /* DEBUG_ENDR( r ); */
  ER( OPTIONS, cli_parse, const char *shorts, duf_option_stage_t istage, duf_option_adata_t * paod );
}

SR( OPTIONS, source_cli_parse, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive DUF_UNUSED,
    duf_cpchar_void_func_t cb_prompt_interactive DUF_UNUSED,  duf_option_adata_t *  paod )
{
  /* DEBUG_STARTR( r ); */

  DUF_TRACE( options, 20, "@@@@cli options; stage:%s", duf_optstage_name( istage ) );

  CR( cli_parse, duf_cli_options_get_shorts(  ), istage , paod);

  if ( istage < DUF_OPTION_STAGE_SETUP )
    DUF_CLEAR_ERROR( QERRIND, DUF_ERROR_OPTION_NOT_FOUND );

  DUF_TRACE( options, 20, "@@@@cli options done; stage:%s (%d:%s)", duf_optstage_name( istage ), QERRIND, QERRNAME );
  /* DEBUG_ENDR( r ); */

  ER( OPTIONS, source_cli_parse, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive DUF_UNUSED,
      duf_cpchar_void_func_t cb_prompt_interactive DUF_UNUSED,  duf_option_adata_t *  paod );
}
