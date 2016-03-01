/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include "duf_errorn_defs_preset.h" 

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
# include <mastar/error/mas_error_defs_ctrl.h>
# include <mastar/error/mas_error_defs.h>                            /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
# include <mastar/error/mas_error_defs_make.h>                       /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */


#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_option_descr.h"                                        /* duf_xoption_description_tmp */

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */
#include "duf_option_typed.h"

#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */

/* ###################################################################### */
#include "duf_option_clarify.h"
/* ###################################################################### */

/* 20160227.123618 */
SR( OPTIONS, xoption_clarify, duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg,
    const duf_longval_extended_vtable_t * xtable, unsigned noo, duf_option_stage_t istage, duf_option_source_t source )
{

  if ( extended )
  {
    DUF_TRACE( options, 55, "@@@fully clarify option: [--%s='%s'] stage:%s; `noo`:%d; source:%s",
               extended ? extended->o.name : "?", optargg, duf_optstage_name( cli, istage ), noo, duf_optsource_name( cli, source ) );
    CR( xoption_clarify_typed, cli, extended, optargg, xtable, noo, istage, source );
    DUF_TRACE( options, 55, "@clarified typed: --%s / [%s]; (%d:%s)", extended->o.name, duf_xoption_description_tmp( cli, -1, extended ), QERRIND,
               QERRNAME );

#if 0
    if ( QISERR_N( OPTION_NOT_PARSED ) && !noo )
    {
      assert( 0 );
      DUF_TRACE( explain, 1, "@old opt for %s", extended ? extended->o.name : NULL );
      CR( clarify_xcmd_old, extended, optargg, istage, xtable );
    }
#endif
    DUF_TRACE( options, 55, "@clarified cli option: %s (%d:%s)", duf_xoption_description_tmp( cli, -1, extended ), QERRIND, QERRNAME );
  }
  else
  {
    ERRMAKE( OPTION_NOT_PARSED );
    assert( 0 );
  }
  DUF_TRACE( options, +170, "@[%s]; arg:%s; istage:%d; `noo`:%d", duf_xoption_description_tmp( cli, -1, extended ), optargg, istage, noo );

  ER( OPTIONS, xoption_clarify, duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg,
      const duf_longval_extended_vtable_t * xtable, unsigned noo, duf_option_stage_t istage, duf_option_source_t source );
}

/* 20150924.144102 */
SR( OPTIONS, argv_clarify, duf_config_cli_t * cli, mas_argvc_t * ptarg, mas_cargvc_t * pcarg, int pos )
{

  mas_del_argv( ptarg->argc, ptarg->argv, 0 );
  ptarg->argc = 0;
  ptarg->argv = NULL;

  ptarg->argc = mas_add_argv_argv( ptarg->argc, &ptarg->argv, pcarg->argc, pcarg->argv, pos );
  DUF_TRACE( options, 150, "@@@@@argc:%d", ptarg->argc );

/* targ.argv becomes valid here - may init pdi etc. */

  ER( OPTIONS, argv_clarify, duf_config_cli_t * cli, mas_argvc_t * ptarg, mas_cargvc_t * pcarg, int pos );
}
