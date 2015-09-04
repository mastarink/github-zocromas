#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_option.h"
#include "duf_option_extended.h"
#include "duf_option_names.h"

/* ###################################################################### */
#include "duf_cli_options.h"
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

int
duf_parse_cli_options( const char *shorts, duf_option_stage_t istage )
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
  carg.argc = duf_config->carg.argc;
  carg.argv = duf_config->carg.argv;
#else
  carg = duf_config->carg;
#endif
  DUF_TRACE( options, +2, "parse cli options (%d)...", duf_config->longopts_table ? 1 : 0 );
#if 0
  {
    for ( const duf_option_t * po = duf_config->longopts_table; po && po->name; po++ )
    {
      DUF_TRACE( options, +4, "L:%3ld. %s", ( po - duf_config->longopts_table ), po->name );
    }
  }
#endif
  while ( DUF_NOERROR( r )
          && ( ( int ) ( longindex = -1, codeval = getopt_long( carg.argc, carg.argv, shorts, duf_config->longopts_table, &longindex ) ) >= 0 ) )
  {
    DUF_TRACE( options, +2, "@@@getopt_long codeval: %d (%c) longindex:%d", codeval, codeval > ' ' && codeval <= 'z' ? codeval : '?', longindex );
/*
 * duf_parse_option return
 *        oclass (>0) for "help" options
 *                =0  for normal options
 * or  errorcode (<0) for error
 * */
    DOR( r, duf_parse_option( codeval, longindex, optarg, istage ) );
    /* DUF_TEST_R1( r ); */
    DUF_TRACE( options, +4, "cli options r: %d", r );
    if ( optind > 0 )
      DUF_TRACE( options, +2, "carg.argv[%d]=\"%s\"", optind - 1, duf_config->carg.argv[optind - 1] );
    if ( optind >= 0 )
      DUF_TRACE( options, +2, "carg.argv[%d]=\"%s\"", optind, duf_config->carg.argv[optind] );

    if ( DUF_IS_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ) || DUF_IS_ERROR( r, DUF_ERROR_OPTION ) )
    {
      const char *arg;
      static const char *msg = "Invalid option";

      arg = duf_config->carg.argv[optind - 1];
      if ( optopt && codeval > ' ' && codeval <= 'z' )
      {
        if ( duf_config->cli.dbg.verbose == 0 )
          DUF_SHOW_ERROR( "@@@@@%s '-%c'", msg, optopt );
        else
        {
          if ( codeval == '?' )
            DUF_SHOW_ERROR( "@@@@@%s '-%c' arg[%d]=\"%s\"", msg, optopt, optind, arg );
          else
            DUF_SHOW_ERROR( "@@@@@%s '-%c' arg[%d]=\"%s\" [%u/%c]", msg, optopt, optind, arg, codeval, codeval );
        }
      }
      else
      {
        if ( duf_config->cli.dbg.verbose == 0 )
          DUF_SHOW_ERROR( "@@@@@%s '%s' ", msg, arg );
        else
          DUF_SHOW_ERROR( "@@@@@%s '%s' arg[%d]=\"%s\" [%u/%c]", msg, arg, optind, arg, codeval, codeval );
      }
    }
    cnt++;
  }

  DUF_TRACE( options, +2, "optind:%d; carg.argc:%d", optind, carg.argc );
  DUF_TRACE( explain, 0, "parsed %d CLI options %s", cnt, duf_error_name( r ) );
  if ( istage == 0 && optind < duf_config->carg.argc )
  {
    mas_del_argv( duf_config->targ.argc, duf_config->targ.argv, 0 );
    duf_config->targ.argc = 0;
    duf_config->targ.argv = NULL;

    DUF_TRACE( options, +2, "(for targ) carg.argv[%d]=\"%s\"", optind, duf_config->carg.argv[optind] );
    duf_config->targ.argc = mas_add_argv_argv( duf_config->targ.argc, &duf_config->targ.argv, duf_config->carg.argc, duf_config->carg.argv, optind );

    duf_config->targ_offset = duf_reorder_at_sign( duf_config->targ.argc, duf_config->targ.argv );



    /* targ.argv becomes valid here - may init pdi etc. */
  }

  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND );
}

int
duf_cli_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

#if 0
  /* Don't use it before all oprions got */
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
#endif
  DUF_TRACE( options, +2, "cli options..." );
  if ( duf_config )
  {
    DOR( r, duf_parse_cli_options( duf_config->cli.shorts, istage ) );
  }
  DUF_TRACE( explain, 2, "cli options  %s", duf_error_name( r ) );
#if 0
  /* Don't use it before all options processed */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
#endif
  DEBUG_ENDR( r );
}

  /* if ( code==DUF_OPTION_ ## up  ) \                                                                                                          */
  /*   printf("[%d : %c] %s %d :: %d\n", code, code>' ' && code <= 'z' ? code : '-', #up, duf_config->pref.lo, duf_config->cli.trace.sample); \ */
