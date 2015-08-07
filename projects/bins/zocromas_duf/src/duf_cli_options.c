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

#ifdef MAS_TRACING
  for ( int ia = 0; ia < argc; ia++ )
  {
    DUF_TRACE( temp, 2, "Before argv[%d]='%s'", ia, argv[ia] );
  }
#endif

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
    DUF_TRACE( temp, 2, "1> ia:%d ra:%d %s", ia, ra, argv[ia] );
    if ( argv[ia][0] != '@' )
    {
      ra = ia;
      break;
    }
    DUF_TRACE( temp, 2, "2> ia:%d ra:%d %s", ia, ra, argv[ia] );
  }
#else
  for ( int ia = argc - 1; ia >= 0; ia-- )
  {
    DUF_TRACE( temp, 2, "1> ia:%d ra:%d %s", ia, ra, argv[ia] );
    if ( argv[ia] && argv[ia][0] != '@' )
      ra = ia;
    DUF_TRACE( temp, 2, "2> ia:%d ra:%d %s", ia, ra, argv[ia] );
  }
#endif

#ifdef MAS_TRACING
  for ( int ia = 0; ia < argc; ia++ )
  {
    DUF_TRACE( temp, 2, "After argv[%d]='%s'", ia, argv[ia] );
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
  DUF_TRACE( options, 1, "parse cli options (%d)...", duf_config->longopts_table ? 1 : 0 );
#if 0
  {
    for ( const duf_option_t * po = duf_config->longopts_table; po && po->name; po++ )
    {
      DUF_TRACE( options, 3, "L:%3ld. %s", ( po - duf_config->longopts_table ), po->name );
    }
  }
#endif
  while ( ( r == 0 )
          && ( ( int ) ( longindex = -1, codeval = getopt_long( carg.argc, carg.argv, shorts, duf_config->longopts_table, &longindex ) ) >= 0 ) )
  {
    DUF_TRACE( options, 1, "getopt_long codeval: %d (%c) longindex:%d", codeval, codeval > ' ' && codeval <= 'z' ? codeval : '?', longindex );
/*
 * duf_parse_option return
 *        oclass (>0) for "help" options
 *                =0  for normal options
 * or  errorcode (<0) for error
 * */
    r = duf_parse_option( codeval, longindex, optarg, istage );
    DUF_TRACE( options, 3, "cli options r: %d", r );
    DUF_TRACE( options, 1, "carg.argv[%d]=\"%s\"", optind, duf_config->carg.argv[optind] );

    if ( r == DUF_ERROR_OPTION_NOT_FOUND || r == DUF_ERROR_OPTION )
    {
      DUF_SHOW_ERROR( "Invalid option -- '%c' optind=%d/%s opt=%u/%c", optopt, optind, duf_config->carg.argv[optind - 1], codeval, codeval );
    }
    cnt++;
  }

  DUF_TRACE( options, 0, "optind:%d; carg.argc:%d", optind, carg.argc );
  DUF_TRACE( explain, 0, "parsed %d CLI options %s", cnt, duf_error_name( r ) );
  if ( istage == 0 && optind < duf_config->carg.argc )
  {
    mas_del_argv( duf_config->targ.argc, duf_config->targ.argv, 0 );
    duf_config->targ.argc = 0;
    duf_config->targ.argv = NULL;

    DUF_TRACE( options, 0, "(for targ) carg.argv[%d]=\"%s\"", optind, duf_config->carg.argv[optind] );
    duf_config->targ.argc = mas_add_argv_argv( duf_config->targ.argc, &duf_config->targ.argv, duf_config->carg.argc, duf_config->carg.argv, optind );

    duf_config->targ_offset = duf_reorder_at_sign( duf_config->targ.argc, duf_config->targ.argv );

    DUF_TRACE( temp, 2, ">> duf_config->targ_offset:%d", duf_config->targ_offset );
    for ( int ia = 0; ia < duf_config->targ.argc; ia++ )
    {
      DUF_TRACE( temp, 2, "After duf_config->targv[%d]='%s'", ia, duf_config->targ.argv[ia] );
    }

    DUF_TRACE( temp, 2, ">> targ_offset:%d", duf_config->targ_offset );

    DUF_TRACE( temp, 2, ">> optind=%d; targc=%d", optind, duf_config->targ.argc );


    /* targ.argv becomes valid here - may init pdi etc. */
  }
  DUF_TRACE( temp, 2, ">> targ_offset:%d", duf_config->targ_offset );
  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND );
}

int
duf_cli_options( duf_option_stage_t istage )
{
  int r = 0;

#if 0
  /* Don't use it before all oprions got */
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
#endif
  DUF_TRACE( options, 0, "cli options..." );
  if ( duf_config )
  {
    DOR( r, duf_parse_cli_options( duf_config->cli.shorts, istage ) );
  }
  DUF_TRACE( explain, 2, "cli options  %s", duf_error_name( r ) );
#if 0
  /* Don't use it before all options processed */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
#endif
  return r;
}

  /* if ( code==DUF_OPTION_ ## up  ) \                                                                                                          */
  /*   printf("[%d : %c] %s %d :: %d\n", code, code>' ' && code <= 'z' ? code : '-', #up, duf_config->pref.lo, duf_config->cli.trace.sample); \ */
