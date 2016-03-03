/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include "muc_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/tools/mas_convert.h>                                /* mas_strtol_suff; mas_strtoll_suff; etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */

#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

/* ###################################################################### */
#include "muc_option_typed.h"
/* ###################################################################### */

muc_SR( OPTIONS, xoption_clarify_typed_call, muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *optargg,
        unsigned noo, muc_option_stage_t istage MAS_UNUSED, muc_option_source_t source MAS_UNUSED )
{

  switch ( extended->calltype )
  {
  case MUC_OPTION_CALL_TYPE_NONE:
    ERRMAKE( OPTION_NOT_CLARIFIED );
    break;
  case MUC_OPTION_CALL_TYPE_EIA:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.eia.func )
        DOR( r, ( extended->call.fdesc.eia.func ) ( extended->call.fdesc.eia.arg ) );
      else
        ERRMAKE( NO_FUNC );
    }
#else
  /* IF_DORF( r, extended->call.fdesc.eia.func, extended->call.fdesc.eia.arg ); (* fixed int arg *) */
    muc_IF_CRV( extended->call.fdesc.eia.func, extended->call.fdesc.eia.arg ); /* fixed int arg */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case MUC_OPTION_CALL_TYPE_EV:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.ev.func )
        DUF_MAKE_ERROR( r, ( extended->call.fdesc.ev.func ) (  ) );
      else
        ERRMAKE( NO_FUNC );
    }
#else
    muc_IF_CRV( extended->call.fdesc.ev.func );                      /* fixed no arg */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case MUC_OPTION_CALL_TYPE_A:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
  /* assert( DUF_CONFIGG( cli.carg.argv ) ); */
    assert( muc_cli_options_get_carg( cli ) && muc_cli_options_get_carg( cli )->argv );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.a.func )
        DOR( r, ( extended->call.fdesc.a.func ) ( DUF_CONFIGG( cli.carg.argc ), DUF_CONFIGG( cli.carg.argv ) ) );
      else
        ERRMAKE( NO_FUNC );
    }
#else
  /* IF_DORF( r, extended->call.fdesc.a.func, DUF_CONFIGG( cli.carg.argc ), DUF_CONFIGG( cli.carg.argv ) ); */
    muc_IF_CRV( extended->call.fdesc.a.func, muc_cli_options_get_carg( cli )->argc, muc_cli_options_get_carg( cli )->argv ); /* argc and argv */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case MUC_OPTION_CALL_TYPE_AA:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
  /* assert( DUF_CONFIGG( cli.carg.argc ) ); */
  /* assert( DUF_CONFIGG( cli.carg.argv ) ); */
    assert( muc_cli_options_get_carg( cli ) && muc_cli_options_get_cargc( cli ) && muc_cli_options_get_cargv( cli ) );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.aa.func )
        DOR( r, ( extended->call.fdesc.aa.func ) ( DUF_CONFIGG( cli.carg ) ) );
      else
        ERRMAKE( NO_FUNC );
    }
#else
  /* IF_DORF( r, extended->call.fdesc.aa.func, DUF_CONFIGG( cli.carg ) ); */
    muc_IF_CRV( extended->call.fdesc.aa.func, muc_cli_options_get_carg( cli ) ); /* arg[cv] as mas_cargvc_t */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case MUC_OPTION_CALL_TYPE_N:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.n.func )
        DOR( r, ( ( extended->call.fdesc.n.func ) ( mas_strtol_suff( optargg, QPERRIND ) ) ) );
      else
        ERRMAKE( NO_FUNC );
    }
#else
    muc_IF_CRV( extended->call.fdesc.n.func, mas_strtol_suff( optargg, QPERRIND ) );
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case MUC_OPTION_CALL_TYPE_S:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.s.func )
        DOR( r, ( ( extended->call.fdesc.s.func ) ( optargg ) ) );
      else
        ERRMAKE( NO_FUNC );
    }
#else
    muc_IF_CRV( extended->call.fdesc.s.func, optargg );              /* optarg */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case MUC_OPTION_CALL_TYPE_SAS:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.sa.func )
        DOR( r, ( extended->call.fdesc.sa.func ) ( extended->call.fdesc.sa.arg ) );
      else
        ERRMAKE( NO_FUNC );
    }
#else
    muc_IF_CRV( extended->call.fdesc.sas.func, extended->call.fdesc.sas.arg ); /* fixed string arg */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case MUC_OPTION_CALL_TYPE_SAN:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.sa.func )
        DOR( r, ( extended->call.fdesc.sa.func ) ( extended->call.fdesc.sa.arg ) );
      else
        ERRMAKE( NO_FUNC );
    }
#else
    muc_IF_CRV( extended->call.fdesc.san.func, optargg, extended->call.fdesc.san.arg ); /* optarg and fixed num arg */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case MUC_OPTION_CALL_TYPE_TN1:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
    muc_IF_CRV( extended->call.fdesc.tn1.func, muc_cli_options_get_targ( cli ), mas_strtol_suff( optargg, QPERRIND ) ); /* targ as mas_argvc_t and numeric optarg */
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
#if 0
  case MUC_OPTION_CALL_TYPE_TN2:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
    assert( &muc_cli_options_get_targ( cli )->argc == muc_cli_options_get_ptargc( cli ) );
    assert( &muc_cli_options_get_targ( cli )->argv == muc_cli_options_get_ptargv( cli ) );
    muc_IF_CRV( extended->call.fdesc.tn2.func, muc_cli_options_get_ptargc( cli ), muc_cli_options_get_ptargv( cli ),
                mas_strtol_suff( optargg, QPERRIND ) );
    ERRMAKE( OPTION_NOT_CLARIFIED );
    break;
#endif
  case MUC_OPTION_CALL_TYPE_TS1:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
    assert( &muc_cli_options_get_targ( cli )->argc == muc_cli_options_get_ptargc( cli ) );
    assert( &muc_cli_options_get_targ( cli )->argv == muc_cli_options_get_ptargv( cli ) );
    muc_IF_CRV( extended->call.fdesc.ts1.func, muc_cli_options_get_targ( cli ), optargg ); /* targ as mas_argvc_t and optarg */
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case MUC_OPTION_CALL_TYPE_TS2:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
    assert( &muc_cli_options_get_targ( cli )->argc == muc_cli_options_get_ptargc( cli ) );
    assert( &muc_cli_options_get_targ( cli )->argv == muc_cli_options_get_ptargv( cli ) );
    muc_IF_CRV( extended->call.fdesc.ts2.func, muc_cli_options_get_ptargc( cli ), muc_cli_options_get_ptargv( cli ), optargg ); /* targc, targv and optarg */
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  }

  muc_ER( OPTIONS, xoption_clarify_typed_call, muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *optargg,
          unsigned noo, muc_option_stage_t istage MAS_UNUSED, muc_option_source_t source MAS_UNUSED );
}
