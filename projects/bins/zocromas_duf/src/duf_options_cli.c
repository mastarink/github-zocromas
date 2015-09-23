#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_option.h"
#include "duf_option_extended.h"
#include "duf_option_names.h"

/* ###################################################################### */
#include "duf_options_cli.h"
/* ###################################################################### */

static int
duf_reorder_at_sign( int argc, char *argv[] )
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

static int
duf_clarify_cli_opts( const char *shorts, duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  int longindex;
  int cnt = 0;
  duf_option_code_t codeval;
  duf_cargvc_t carg;

  DEBUG_E_NO( DUF_ERROR_OPTION_NOT_FOUND );
  opterr = 0;
  optind = 1;
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
  while ( DUF_NOERROR( r )
          && ( ( int ) ( longindex = -1, codeval = getopt_long( carg.argc, carg.argv, shorts, DUF_CONFIGG( longopts_table ), &longindex ) ) >= 0 ) )
  {
    DUF_TRACE( options, +2, "@@@getopt_long codeval: %d (%c) longindex:%d", codeval, codeval > ' ' && codeval <= 'z' ? codeval : '?', longindex );
/*
 * duf_clarify_opt return
 *        oclass (>0) for "help" options
 *                =0  for normal options
 * or  errorcode (<0) for error
 * */
    DOR( r, duf_clarify_opt( codeval, longindex, optarg, istage, DUF_OPTION_SOURCE_CLI ) ); /* => duf_clarify_xcmd_full */
    /* DUF_TEST_R1( r ); */
    DUF_TRACE( options, +4, "cli options r: %d", r );
    if ( optind > 0 )
      DUF_TRACE( options, +2, "carg.argv[%d]=\"%s\"", optind - 1, DUF_CONFIGG( carg.argv )[optind - 1] );
    if ( optind >= 0 )
      DUF_TRACE( options, +2, "carg.argv[%d]=\"%s\"", optind, DUF_CONFIGG( carg.argv )[optind] );

    if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_FOUND ) || DUF_IS_ERROR_N( r, DUF_ERROR_OPTION ) )
    {
      const char *arg;
      static const char *msg = "Invalid option";

      arg = DUF_CONFIGG( carg.argv )[optind - 1];
      if ( optopt && codeval > ' ' && codeval <= 'z' )
      {
        if ( DUF_CONFIGG( cli.dbg.verbose ) == 0 )
          DUF_SHOW_ERROR( "@@@@@@@@@@@  %s '-%c'      ", msg, optopt );
        else
        {
          if ( codeval == '?' )
            DUF_SHOW_ERROR( "@@@@@@@@@@@  %s '-%c' arg[%d]=\"%s\"", msg, optopt, optind, arg );
          else
            DUF_SHOW_ERROR( "@@@@@@@@@@@  %s '-%c' arg[%d]=\"%s\" [%u/%c]", msg, optopt, optind, arg, codeval, codeval );
        }
      }
      else
      {
        if ( DUF_CONFIGG( cli.dbg.verbose ) == 0 )
          DUF_SHOW_ERROR( "@@@@@@@@@@@  %s '%s'                  ", msg, arg );
        else
          DUF_SHOW_ERROR( "@@@@@@@@@@@  %s '%s' arg[%d]=\"%s\" [%u/%c]", msg, arg, optind, arg, codeval, codeval );
      }
    }
    cnt++;
  }

  DUF_TRACE( options, +2, "optind:%d; carg.argc:%d", optind, carg.argc );
  DUF_TRACE( explain, 0, "parsed %d CLI options %s", cnt, duf_error_name( r ) );
  if ( istage == 0 && optind < DUF_CONFIGG( carg.argc ) )
  {
    mas_del_argv( DUF_CONFIGG( targ.argc ), DUF_CONFIGG( targ.argv ), 0 );
    DUF_CONFIGW( targ.argc ) = 0;
    DUF_CONFIGW( targ.argv ) = NULL;

    DUF_TRACE( options, +2, "(for targ) carg.argv[%d]=\"%s\"", optind, DUF_CONFIGG( carg.argv )[optind] );
    DUF_CONFIGW( targ.argc ) =
          mas_add_argv_argv( DUF_CONFIGG( targ.argc ), DUF_CONFIGA( targ.argv ), DUF_CONFIGG( carg.argc ), DUF_CONFIGG( carg.argv ), optind );

    DUF_CONFIGW( targ_offset ) = duf_reorder_at_sign( DUF_CONFIGG( targ.argc ), DUF_CONFIGG( targ.argv ) );



    /* targ.argv becomes valid here - may init pdi etc. */
  }

  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND );
}

int
duf_cli_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  assert( duf_config );

  DUF_TRACE( options, 0, "@@@@(%d) source: cli", istage );
#if 0
  /* Don't use it before all oprions got */
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
#endif
  DUF_TRACE( options, +2, "cli options..." );

  DOR( r, duf_clarify_cli_opts( DUF_CONFIGG( cli.shorts ), istage ) );

  DUF_TRACE( explain, 2, "cli options  %s", duf_error_name( r ) );
#if 0
  /* Don't use it before all options processed */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
#endif
  DEBUG_ENDR( r );
}

  /* if ( code==DUF_OPTION_ ## up  ) \                                                                                                          */
  /*   printf("[%d : %c] %s %d :: %d\n", code, code>' ' && code <= 'z' ? code : '-', #up, DUF_CONFIGG(pref.lo), DUF_CONFIGG(cli.trace.sample)); \ */
