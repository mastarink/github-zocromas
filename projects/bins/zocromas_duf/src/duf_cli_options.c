/* #include <stdarg.h> */
#include <string.h>
#include <stddef.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_error_types.h"
#include "duf_trace_defs.h"

#include "duf_types.h"
#include "duf_utils.h"
#include "duf_service.h"

/* #include "duf_config.h" */
#include "duf_config_ref.h"

#include "duf_option.h"
#include "duf_options.h"

/* ###################################################################### */
#include "duf_cli_options.h"
/* ###################################################################### */


/* extern const duf_option_t *duf_longopts; */
/* extern const int duf_longopts_count; */


int
duf_cli_options( int argc, char *argv[] )
{
  int r = 0;

  /* DUF_PRINTF(0, ">>>>>>>>>> %lu", offsetof( duf_config_t, u)); */

#if 0
  /* Don't use it before all oprions got */
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
#endif
  if ( duf_config )
  {
    duf_option_code_t codeval;
    int longindex = 0;

    opterr = 0;
    {
      char *shorts = NULL;

      shorts = duf_cli_option_shorts(  );
      while ( r == 0 && ( ( int ) ( codeval = getopt_long( argc, argv, shorts, duf_config->longopts_table, &longindex ) ) >= 0 ) )
      {
        /* DUF_PRINTF( 0, "@@@@@@@@@@ >>>>> %d : %d ", codeval, longindex ); */
        r = duf_parse_option( codeval, longindex, optarg );
        DUF_TRACE( explain, 2, "parse options r: %d", r );
        if ( r == DUF_ERROR_OPTION )
        {
          DUF_ERROR( "Invalid option -- '%c' optind=%d/%s opt=%u/%c", optopt, optind, argv[optind - 1], codeval, codeval );
          /* r = optopt ? optopt : opt; */
        }
      }
      mas_free( shorts );
    }
    if ( optind < argc )
    {
      mas_del_argv( duf_config->targc, duf_config->targv, 0 );
      duf_config->targc = 0;
      duf_config->targv = NULL;

      duf_config->targc = mas_add_argv_argv( duf_config->targc, &duf_config->targv, argc, argv, optind );
    }
  }
  DUF_TRACE( explain, 2, "cli options r: %d", r );
#if 0
  /* Don't use it before all options processed */
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
#endif
  return r;
}

  /* if ( code==DUF_OPTION_ ## up  ) \                                                                                                          */
  /*   printf("[%d : %c] %s %d :: %d\n", code, code>' ' && code <= 'z' ? code : '-', #up, duf_config->pref.lo, duf_config->cli.trace.sample); \ */
