#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_option.h"
#include "duf_option_extended.h"
#include "duf_option_names.h"

#include "duf_config.h"
#include "duf_options.h"
/* ###################################################################### */
#include "duf_options_string.h"
#include "duf_options_cli.h"
/* ###################################################################### */

static int
duf_reorder_argv_at_sign( int argc, char *argv[] )
{
  int ra = 0;
  int nkra = -1;
  int kra = -1;


  for ( int ia = 0; ia < argc; ia++ )
  {
    if ( *( argv[ia] ) == '@' )
      kra = ia;
    else
      nkra = ia;
    if ( kra >= 0 && nkra >= 0 )
    {
      if ( kra > nkra )
      {
        char *t;

        t = argv[kra];
        argv[kra] = argv[nkra];
        argv[nkra] = t;
        ia = 0;
        kra = nkra = -1;
      }
    }
  }
  ra = argc;
#if 1
  for ( int ia = 0; ia < argc; ia++ )
  {
    if ( argv[ia][0] != '@' )
    {
      ra = ia;
      break;
    }
  }
#else
  for ( int ia = argc - 1; ia >= 0; ia-- )
  {
    if ( argv[ia] && argv[ia][0] != '@' )
      ra = ia;
  }
#endif


  return ra;
}

/* 20150924.144047 */
static int
duf_reorder_argvc_at_sign( duf_argvc_t * ptarg )
{
  return duf_reorder_argv_at_sign( ptarg->argc, ptarg->argv );
}

static char *
duf_clrfy_cli_opts_msgs( duf_option_code_t codeval, int optindd, int optoptt, const char *shorts_unused DUF_UNUSED )
{
  const char *arg;
  static const char *msg = "Invalid option";
  char buffer[2048] = "";

  arg = DUF_CONFIGG( carg.argv )[optindd];
  if ( DUF_CONFIGG( cli.dbg.verbose ) )
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

  /* arg = DUF_CONFIGG( carg.argv )[optindd]; */
#  if 0
  if ( optoptt && codeval > ' ' && codeval <= 'z' )
  {
    if ( DUF_CONFIGG( cli.dbg.verbose ) == 0 )
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
    if ( DUF_CONFIGG( cli.dbg.verbose ) )
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
    for ( int i = 0; i < DUF_CONFIGG( carg.argc ); i++ )
    {
      _DUF_SHOW_ERROR( "@@@ (%d) %c%d '%s'", optindd, i == optindd ? '*' : ' ', i, DUF_CONFIGG( carg.argv )[i] );
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
  duf_cargvc_t carg;
  int optoptt = 0, optindd = 0, optindp = 0;
  int numxargv = 0;

  DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );
  optopt = 0;
  opterr = 0;
  optind = 1;
  optindp = optind;
#if 0
  carg.argc = DUF_CONFIGG( carg.argc );
  carg.argv = DUF_CONFIGG( carg.argv );
#else
  carg = DUF_CONFIGG( carg );
#endif
  DUF_TRACE( options, +2, "parse cli options (%d)...", DUF_CONFIGG( longopts_table ) ? 1 : 0 );
#if 0
  {
    for ( const duf_option_t * po = DUF_CONFIGG( longopts_table ); po && po->name; po++ )
    {
      DUF_TRACE( options, +4, "L:%3ld. %s", ( po - DUF_CONFIGG( longopts_table ) ), po->name );
    }
  }
#endif
  /* optind
   *   inside loop means next arg to process in non-permuted list
   *   outside loop means first not processed arg in permuted list
   * */
  while ( DUF_NOERROR( r )
          &&
          ( ( int ) ( optopt = 0, longindex = -1, codeval = getopt_long( carg.argc, carg.argv, shorts, DUF_CONFIGG( longopts_table ), &longindex ) )
            >= 0 ) )
  {
    DUF_TRACE( options, +2, "@@@getopt_long codeval: %d (%c) longindex:%d", codeval, codeval > ' ' && codeval <= 'z' ? codeval : '?', longindex );
    optoptt = 0;
    optindd = optind;
    if ( codeval == '?' )
    {
      char *msg = NULL;

      optoptt = optopt;
      msg = duf_clrfy_cli_opts_msgs( codeval, optindp, optoptt, shorts );
      DUF_MAKE_ERRORM( r, DUF_ERROR_OPTION_NOT_FOUND, msg );
      mas_free( msg );
    }
/*
 * duf_clarify_opt(_x) return
 *        oclass (>0) for "help" options
 *                =0  for normal options
 * or  errorcode (<0) for error
 * */
    DOR( r, duf_clarify_opt_x( codeval, longindex, optarg, istage, DUF_OPTION_SOURCE_CLI ) ); /* => duf_clarify_xcmd_full */
    /* DUF_TEST_R1( r ); */
    DUF_TRACE( options, +4, "cli options r: %d", r );
#if 0
    if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_FOUND ) || DUF_IS_ERROR_N( r, DUF_ERROR_OPTION ) )
      duf_clrfy_cli_opts_msg( codeval, optindp, optoptt, shorts );
#endif
    optindp = optind;
  }
  numxargv = carg.argc - optind;
  /* for ( int i = 1; i < optind; i++ )                                                                                           */
  /* {                                                                                                                            */
  /*   T( "@@oooooo (optind:%d:%d:%d:%d)  %d of %d :: '%s'", optind, optindd, numxargv, carg.argc, i, carg.argc, carg.argv[i] );  */
  /* }                                                                                                                            */
  /* for ( int i = optind; i < carg.argc; i++ )                                                                                   */
  /* {                                                                                                                            */
  /*   T( "@@@CCCCCC (optind:%d:%d:%d:%d)  %d of %d :: '%s'", optind, optindd, numxargv, carg.argc, i, carg.argc, carg.argv[i] ); */
  /* }                                                                                                                            */
  DUF_TRACE( options, +2, "(%d:%s) istage:%d; optind:%d; carg.argc:%d", r, duf_error_name_i( r ), istage, optind, carg.argc );
  DUF_TRACE( explain, 0, "parsed %d CLI options %s", optind, duf_error_name_i( r ) );
  if ( DUF_NOERROR( r ) && istage == DUF_OPTION_STAGE_SETUP && /* optind < carg.argc && */ numxargv > 0 )
  {
    DUF_TRACE( options, +2, "(for targ) carg.argv[%d]=\"%s\"", optind, carg.argv[optind] );
#if 0
    mas_del_argv( DUF_CONFIGG( targ.argc ), DUF_CONFIGG( targ.argv ), 0 );
    DUF_CONFIGWN( targ.argc, 0 );
    DUF_CONFIGWP( targ.argv, NULL );

    DUF_CONFIGWN( targ.argc,
                  mas_add_argv_argv( DUF_CONFIGG( targ.argc ), DUF_CONFIGA( targ.argv ), DUF_CONFIGG( carg.argc ), DUF_CONFIGG( carg.argv ),
                                     optind ) );




    /* targ.argv becomes valid here - may init pdi etc. */
#else
    DOR( r, duf_clarify_argv( DUF_CONFIGA( targ ), DUF_CONFIGA( carg ), optind ) );
#endif

#if 0
    DUF_CONFIGWN( targ_offset, duf_reorder_argv_at_sign( DUF_CONFIGG( targ.argc ), DUF_CONFIGG( targ.argv ) ) );
#else
    if ( DUF_NOERROR( r ) )
      DUF_CONFIGWN( targ_offset, duf_reorder_argvc_at_sign( DUF_CONFIGA( targ ) ) );


#endif
  }
  else
  {
    DUF_TRACE( options, +2, "(no targ) optind:%d; optindd:%d; numxargv:%d", optind, optindd, numxargv );
  }

  DEBUG_ENDR_UPPER( r, DUF_ERROR_OPTION_NOT_FOUND );
}

int
duf_cli_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  assert( duf_config );

  DUF_TRACE( options, 0, "@@@@(%d) source: cli", istage );
  DUF_TRACE( options, +2, "cli options..." );

  DOR( r, duf_clarify_cli_opts( DUF_CONFIGG( cli.shorts ), istage ) );
  if ( istage == DUF_OPTION_STAGE_PRESETUP )
    DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );

  DUF_TRACE( explain, 2, "cli options  %s", duf_error_name_i( r ) );
  DEBUG_ENDR( r );
}

  /* if ( code==DUF_OPTION_ ## up  ) \                                                                                                          */
  /*   printf("[%d : %c] %s %d :: %d\n", code, code>' ' && code <= 'z' ? code : '-', #up, DUF_CONFIGG(pref.lo), DUF_CONFIGG(cli.trace.sample)); \ */
