/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include "duf_tracen_defs_preset.h"
#include "duf_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/tools/mas_convert.h>                                /* mas_strtol_suff; mas_strtoll_suff; etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */

#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */

/* ###################################################################### */
#include "duf_option_typed.h"
/* ###################################################################### */

SR( OPTIONS, xoption_clarify_typed_call, duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg,
    unsigned noo, duf_option_stage_t istage MAS_UNUSED, duf_option_source_t source MAS_UNUSED )
{

  switch ( extended->calltype )
  {
  case DUF_OPTION_CALL_TYPE_NONE:
    ERRMAKE( OPTION_NOT_CLARIFIED );
    break;
  case DUF_OPTION_CALL_TYPE_EIA:
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
    IF_CRV( extended->call.fdesc.eia.func, extended->call.fdesc.eia.arg ); /* fixed int arg */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_EV:
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
    IF_CRV( extended->call.fdesc.ev.func );                          /* fixed no arg */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_A:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
  /* assert( DUF_CONFIGG( cli.carg.argv ) ); */
    assert( duf_cli_options_get_carg( cli ) && duf_cli_options_get_carg( cli )->argv );
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
    IF_CRV( extended->call.fdesc.a.func, duf_cli_options_get_carg( cli )->argc, duf_cli_options_get_carg( cli )->argv ); /* argc and argv */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_AA:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
  /* assert( DUF_CONFIGG( cli.carg.argc ) ); */
  /* assert( DUF_CONFIGG( cli.carg.argv ) ); */
    assert( duf_cli_options_get_carg( cli ) && duf_cli_options_get_cargc( cli ) && duf_cli_options_get_cargv( cli ) );
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
    IF_CRV( extended->call.fdesc.aa.func, duf_cli_options_get_carg( cli ) ); /* arg[cv] as mas_cargvc_t */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_N:
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
    IF_CRV( extended->call.fdesc.n.func, mas_strtol_suff( optargg, QPERRIND ) );
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_S:
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
    IF_CRV( extended->call.fdesc.s.func, optargg );                  /* optarg */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_SAS:
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
    IF_CRV( extended->call.fdesc.sas.func, extended->call.fdesc.sas.arg ); /* fixed string arg */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_SAN:
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
    IF_CRV( extended->call.fdesc.san.func, optargg, extended->call.fdesc.san.arg ); /* optarg and fixed num arg */
#endif
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_TN1:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
    IF_CRV( extended->call.fdesc.tn1.func, duf_cli_options_get_targ( cli ), mas_strtol_suff( optargg, QPERRIND ) ); /* targ as mas_argvc_t and numeric optarg */
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
#if 0
  case DUF_OPTION_CALL_TYPE_TN2:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
    assert( &duf_cli_options_get_targ( cli )->argc == duf_cli_options_get_ptargc( cli ) );
    assert( &duf_cli_options_get_targ( cli )->argv == duf_cli_options_get_ptargv( cli ) );
    IF_CRV( extended->call.fdesc.tn2.func, duf_cli_options_get_ptargc( cli ), duf_cli_options_get_ptargv( cli ),
            mas_strtol_suff( optargg, QPERRIND ) );
    ERRMAKE( OPTION_NOT_CLARIFIED );
    break;
#endif
  case DUF_OPTION_CALL_TYPE_TS1:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
    assert( &duf_cli_options_get_targ( cli )->argc == duf_cli_options_get_ptargc( cli ) );
    assert( &duf_cli_options_get_targ( cli )->argv == duf_cli_options_get_ptargv( cli ) );
    IF_CRV( extended->call.fdesc.ts1.func, duf_cli_options_get_targ( cli ), optargg ); /* targ as mas_argvc_t and optarg */
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_TS2:
    if ( noo )
      ERRMAKE( OPTION_NOT_PARSED );
    assert( &duf_cli_options_get_targ( cli )->argc == duf_cli_options_get_ptargc( cli ) );
    assert( &duf_cli_options_get_targ( cli )->argv == duf_cli_options_get_ptargv( cli ) );
    IF_CRV( extended->call.fdesc.ts2.func, duf_cli_options_get_ptargc( cli ), duf_cli_options_get_ptargv( cli ), optargg ); /* targc, targv and optarg */
  /* ERRMAKE(OPTION_NOT_CLARIFIED ); */
    break;
  }

  ER( OPTIONS, xoption_clarify_typed_call, duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg,
      unsigned noo, duf_option_stage_t istage MAS_UNUSED, duf_option_source_t source MAS_UNUSED );
}
