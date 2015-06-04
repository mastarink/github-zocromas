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
  DUF_TRACE( options, 0, "parse cli options (%d)...", duf_config->longopts_table ? 1 : 0 );
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
    DUF_TRACE( options, 2, "cli options r: %d", r );
    DUF_TRACE( options, 0, "carg.argv[%d]=\"%s\"", optind, duf_config->carg.argv[optind] );

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
    /* targ.argv becomes valid here - may init pdi etc. */
  }
  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND );
}

int
duf_cli_options( int argc, char *argv[] )
{
  int r = 0;

#if 0
  /* Don't use it before all oprions got */
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
#endif
  DUF_TRACE( options, 0, "cli options..." );
  if ( duf_config )
  {
    duf_config->carg.argc = argc;
    duf_config->carg.argv = argv;
    if ( !duf_config->cli.shorts )
      duf_config->cli.shorts = duf_cli_option_shorts( lo_extended_table_multi );

    DOR( r, duf_parse_cli_options( duf_config->cli.shorts, DUF_OPTION_STAGE_DEFAULT ) );
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
