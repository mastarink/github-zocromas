#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_options_config.h"
#include "duf_option.h"

/* #include "duf_option_names.h" */
#include "duf_option_stage.h"

/* ###################################################################### */
#include "duf_options_string.h"
#include "duf_options_cli.h"
/* ###################################################################### */


static char *
duf_clrfy_cli_opts_msgs( duf_option_code_t codeval, int optindd, int optoptt, const char *shorts_unused DUF_UNUSED )
{
  const char *arg;
  static const char *msg = "Invalid option";
  char buffer[2048] = "";

  arg = duf_cli_options_config(  )->carg.argv[optindd];
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

    s = duf_clrfy_cli_opts_msgs( codeval, optindd, optoptt, shorts_unused );
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

/* 20150924.144037 */
static int
duf_clarify_cli_opts( const char *shorts, duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  int longindex;
  duf_option_code_t codeval;
  mas_cargvc_t *carg;
  int optoptt = 0, optindd = 0, optindp DUF_UNUSED = 0;
  int numxargv = 0;


  DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );
  optopt = 0;
  opterr = 0;
  optind = 1;
  optindp = optind;
#if 0
  carg.argc = DUF_CONFIGG( cli.carg.argc );
  carg.argv = DUF_CONFIGG( cli.carg.argv );
#else
  /* carg = DUF_CONFIGG( cli.carg ); */
  carg = duf_cli_options_get_carg(  );
#endif


  DUF_TRACE( options, 40, "@to clarify cli opts: stage:%s; carg->argc:%d", duf_optstage_name( istage ), carg->argc );


#if 0
  {
    for ( const duf_option_t * po = DUF_CONFIGG( cli.longopts_table ); po && po->name; po++ )
    {
      DUF_TRACE( options, +140, "L:%3ld. %s", ( po - DUF_CONFIGG( cli.longopts_table ) ), po->name );
    }
  }
#endif
  /* optind
   *   inside loop means next arg to process in non-permuted list
   *   outside loop means first not processed arg in permuted list
   * */

  while ( DUF_NOERROR( r )
          &&
          ( ( int )
            ( optopt = 0, longindex = -1, codeval =
              getopt_long( carg->argc, carg->argv, shorts, duf_cli_options_get_longopts_table(  ), &longindex ) ) >= 0 ) )
  {
    DUF_TRACE( options, 40, "@@@@@@getopt_long: cv:%-4d =>  ('%c') '%s' li:%d; oi:%d; oo:%d; oe:%d", codeval, codeval > ' '
               && codeval <= 'z' ? codeval : '?', longindex >= 0 ? duf_cli_options_get_longopts_table(  )[longindex].name : "?", longindex, optind,
               optopt, opterr );
    optoptt = 0;
    optindd = optind;
    if ( codeval == '?' )
    {
      char *msg = NULL;

      optoptt = optopt;
#if 0
      msg = duf_clrfy_cli_opts_msgs( codeval, optindp, optoptt, shorts );
#else
      msg = duf_clrfy_cli_opts_msgs( codeval, optind - 1, optoptt, shorts );
#endif
      DUF_TRACE( options, +140, "@@@@@@@%d:%s", optind - 1, carg->argv[optind - 1] );
      DUF_MAKE_ERRORM( r, DUF_ERROR_OPTION_NOT_FOUND, msg );
      mas_free( msg );
    }
    /* {                                                                                                                       */
    /*   int rt = 0;                                                                                                           */
    /*   rt = duf_find_name_long_no( name, int witharg, const duf_longval_extended_t * xtended, int soft, int *pno, int *pr ); */
    /* }                                                                                                                       */

/*
 * duf_clarify_opt(_x) return
 *        oclass (>0) for "help" options
 *                =0  for normal options
 * or  errorcode (<0) for error
 * */
    DOR( r, duf_clarify_opt_x( codeval, longindex, optarg, istage, DUF_OPTION_SOURCE_CLI ) ); /* => duf_clarify_xcmd_full */
    /* DUF_TEST_R1( r ); */
#if 0
    if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_FOUND ) || DUF_IS_ERROR_N( r, DUF_ERROR_OPTION ) )
      duf_clrfy_cli_opts_msg( codeval, optindp, optoptt, shorts );
#endif
    optindp = optind;
  }
  numxargv = carg->argc - optind;
  /* for ( int i = 1; i < optind; i++ )                                                                                           */
  /* {                                                                                                                            */
  /*   T( "@@oooooo (optind:%d:%d:%d:%d)  %d of %d :: '%s'", optind, optindd, numxargv, carg.argc, i, carg.argc, carg.argv[i] );  */
  /* }                                                                                                                            */
  /* for ( int i = optind; i < carg.argc; i++ )                                                                                   */
  /* {                                                                                                                            */
  /*   T( "@@@CCCCCC (optind:%d:%d:%d:%d)  %d of %d :: '%s'", optind, optindd, numxargv, carg.argc, i, carg.argc, carg.argv[i] ); */
  /* }                                                                                                                            */
  if ( DUF_NOERROR( r ) && istage == DUF_OPTION_STAGE_SETUP && /* optind < carg.argc && */ numxargv > 0 )
  {
    DUF_TRACE( options, +150, "(for targ) carg.argv[%d]=\"%s\"", optind, carg->argv[optind] );
#if 0
    mas_del_argv( DUF_CONFIGG( cli.targ.argc ), DUF_CONFIGG( cli.targ.argv ), 0 );
    DUF_CONFIGWN( cli.targ.argc, 0 );
    DUF_CONFIGWP( cli.targ.argv, NULL );

    DUF_CONFIGWN( cli.targ.argc,
                  mas_add_argv_argv( DUF_CONFIGG( cli.targ.argc ), DUF_CONFIGA( cli.targ.argv ), DUF_CONFIGG( carg.argc ), DUF_CONFIGG( carg.argv ),
                                     optind ) );




    /* targ.argv becomes valid here - may init pdi etc. */
#else
    DOR( r, duf_clarify_argv( duf_cli_options_get_targ(  ), carg, optind ) );
#endif

#if 0
    DUF_CONFIGWN( cli.targ_offset, duf_reorder_argv_at_sign( DUF_CONFIGG( cli.targ.argc ), DUF_CONFIGG( cli.targ.argv ) ) );
#else
    if ( DUF_NOERROR( r ) )
      /* DUF_CONFIGWN( cli.targ_offset, duf_reorder_argvc_at_sign( DUF_CONFIGA( cli.targ ) ) ); */
      duf_cli_options_reset_targ_offset(  );

#endif
  }
  else
  {
    DUF_TRACE( options, +150, "(no targ) optind:%d; optindd:%d; numxargv:%d", optind, optindd, numxargv );
  }

  DEBUG_ENDR_UPPER( r, DUF_ERROR_OPTION_NOT_FOUND );
}

int
duf_source_cli_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  /* assert( duf_config ); */

  DUF_TRACE( options, 20, "@@@@cli options; stage:%s", duf_optstage_name( istage ) );

#if 0
  DOR( r, duf_clarify_cli_opts( DUF_CONFIGG( cli.shorts ), istage ) );
#else
  DOR( r, duf_clarify_cli_opts( duf_cli_options_get_shorts(  ), istage ) );
#endif
  if ( istage == DUF_OPTION_STAGE_PRESETUP )
    DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );

  DUF_TRACE( options, 20, "@@@@cli options done; stage:%s (%d:%s)", duf_optstage_name( istage ), r, mas_error_name_i( r ) );
  DEBUG_ENDR( r );
}
