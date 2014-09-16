#include <string.h>
#include <stddef.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_option.h"
#include "duf_option_names.h"

/* ###################################################################### */
#include "duf_cli_options.h"
/* ###################################################################### */


int
duf_parse_cli_options( const char *shorts, int stage )
{
  DEBUG_STARTR( r );
  int longindex;
  int cnt = 0;
  duf_option_code_t codeval;

  int cargc;
  char *const *cargv;
  const duf_option_t *opttable;

  DEBUG_E_NO( DUF_ERROR_OPTION );
  opterr = 0;
  optind = 1;
  cargc = duf_config->cargc;
  cargv = duf_config->cargv;
  opttable = duf_config->longopts_table;
  while ( ( r == 0 ) && ( ( int ) ( longindex = -1, codeval = getopt_long( cargc, cargv, shorts, opttable, &longindex ) ) >= 0 ) )
  {
    DUF_TRACE( explain, 1, "getopt_long codeval: %d (%c) longindex:%d", codeval, codeval > ' ' && codeval <= 'z' ? codeval : '?', longindex );
/*
 * duf_parse_option return
 *        oclass (>0) for "help" options
 *                =0  for normal options
 * or  errorcode (<0) for error
 * */
    r = duf_parse_option( codeval, longindex, optarg, stage );
    DUF_TRACE( explain, 2, "cli options r: %d", r );

    if ( r == DUF_ERROR_OPTION )
    {
      DUF_SHOW_ERROR( "Invalid option -- '%c' optind=%d/%s opt=%u/%c", optopt, optind, duf_config->cargv[optind - 1], codeval, codeval );
    }
    cnt++;
  }
  DUF_TRACE( explain, 0, "parsed %d CLI options %s", cnt, duf_error_name( r ) );
  if ( stage == 0 && optind < duf_config->cargc )
  {
    mas_del_argv( duf_config->targc, duf_config->targv, 0 );
    duf_config->targc = 0;
    duf_config->targv = NULL;

    duf_config->targc = mas_add_argv_argv( duf_config->targc, &duf_config->targv, duf_config->cargc, duf_config->cargv, optind );
  }
  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION );
}

int
duf_cli_options( int argc, char *argv[] )
{
  int r = 0;

#if 0
  /* Don't use it before all oprions got */
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
#endif
  if ( duf_config )
  {
    duf_config->cargc = argc;
    duf_config->cargv = argv;
    if ( !duf_config->cli.shorts )
      duf_config->cli.shorts = duf_cli_option_shorts(  );

    DOR( r, duf_parse_cli_options( duf_config->cli.shorts, 0 ) );
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
