/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

/* #include "duf_config_util.h" */

#include "duf_utils.h"                                               /* duf_strtol_suff duf_strtoll_suff etc. ♠ */

/* #include "duf_option_defs.h" */

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */

#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */

/* ###################################################################### */
#include "duf_option_typed.h"
/* ###################################################################### */

int
duf_xoption_clarify_typed_call( duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg,
                                unsigned noo, duf_option_stage_t istage MAS_UNUSED, duf_option_source_t source MAS_UNUSED )
{
  DUF_STARTR( r );
  switch ( extended->calltype )
  {
  case DUF_OPTION_CALL_TYPE_NONE:
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED );
    break;
  case DUF_OPTION_CALL_TYPE_EIA:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.eia.func )
        DOR( r, ( extended->call.fdesc.eia.func ) ( extended->call.fdesc.eia.arg ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.eia.func, extended->call.fdesc.eia.arg ); /* fixed int arg */
#endif
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_EV:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.ev.func )
        DUF_MAKE_ERROR( r, ( extended->call.fdesc.ev.func ) (  ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.ev.func );                      /* fixed no arg */
#endif
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_A:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
  /* assert( DUF_CONFIGG( cli.carg.argv ) ); */
    assert( duf_cli_options_get_carg( cli ) && duf_cli_options_get_carg( cli )->argv );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.a.func )
        DOR( r, ( extended->call.fdesc.a.func ) ( DUF_CONFIGG( cli.carg.argc ), DUF_CONFIGG( cli.carg.argv ) ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
  /* IF_DORF( r, extended->call.fdesc.a.func, DUF_CONFIGG( cli.carg.argc ), DUF_CONFIGG( cli.carg.argv ) ); */
    IF_DORF( r, extended->call.fdesc.a.func, duf_cli_options_get_carg( cli )->argc, duf_cli_options_get_carg( cli )->argv ); /* argc and argv */
#endif
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_AA:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
  /* assert( DUF_CONFIGG( cli.carg.argc ) ); */
  /* assert( DUF_CONFIGG( cli.carg.argv ) ); */
    assert( duf_cli_options_get_carg( cli ) && duf_cli_options_get_cargc( cli ) && duf_cli_options_get_cargv( cli ) );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.aa.func )
        DOR( r, ( extended->call.fdesc.aa.func ) ( DUF_CONFIGG( cli.carg ) ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
  /* IF_DORF( r, extended->call.fdesc.aa.func, DUF_CONFIGG( cli.carg ) ); */
    IF_DORF( r, extended->call.fdesc.aa.func, duf_cli_options_get_carg( cli ) ); /* arg[cv] as mas_cargvc_t */
#endif
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_N:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.n.func )
        DOR( r, ( ( extended->call.fdesc.n.func ) ( duf_strtol_suff( optargg, &r ) ) ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.n.func, duf_strtol_suff( optargg, &r ) );
#endif
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_S:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.s.func )
        DOR( r, ( ( extended->call.fdesc.s.func ) ( optargg ) ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.s.func, optargg );              /* optarg */
#endif
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_SAS:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.sa.func )
        DOR( r, ( extended->call.fdesc.sa.func ) ( extended->call.fdesc.sa.arg ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.sas.func, extended->call.fdesc.sas.arg ); /* fixed string arg */
#endif
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_SAN:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
#if 0
    if ( DUF_NOERROR( r ) )
    {
      if ( extended->call.fdesc.sa.func )
        DOR( r, ( extended->call.fdesc.sa.func ) ( extended->call.fdesc.sa.arg ) );
      else
        DUF_MAKE_ERROR( r, DUF_ERROR_NO_FUNC );
    }
#else
    IF_DORF( r, extended->call.fdesc.san.func, optargg, extended->call.fdesc.san.arg ); /* optarg and fixed num arg */
#endif
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_TN1:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    IF_DORF( r, extended->call.fdesc.tn1.func, duf_cli_options_get_targ( cli ), duf_strtol_suff( optargg, &r ) ); /* targ as mas_argvc_t and numeric optarg */
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
#if 0
  case DUF_OPTION_CALL_TYPE_TN2:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    assert( &duf_cli_options_get_targ( cli )->argc == duf_cli_options_get_ptargc( cli ) );
    assert( &duf_cli_options_get_targ( cli )->argv == duf_cli_options_get_ptargv( cli ) );
    IF_DORF( r, extended->call.fdesc.tn2.func, duf_cli_options_get_ptargc( cli ), duf_cli_options_get_ptargv( cli ), duf_strtol_suff( optargg, &r ) );
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED );
    break;
#endif
  case DUF_OPTION_CALL_TYPE_TS1:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    assert( &duf_cli_options_get_targ( cli )->argc == duf_cli_options_get_ptargc( cli ) );
    assert( &duf_cli_options_get_targ( cli )->argv == duf_cli_options_get_ptargv( cli ) );
    IF_DORF( r, extended->call.fdesc.ts1.func, duf_cli_options_get_targ( cli ), optargg ); /* targ as mas_argvc_t and optarg */
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  case DUF_OPTION_CALL_TYPE_TS2:
    if ( noo )
      DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
    assert( &duf_cli_options_get_targ( cli )->argc == duf_cli_options_get_ptargc( cli ) );
    assert( &duf_cli_options_get_targ( cli )->argv == duf_cli_options_get_ptargv( cli ) );
    IF_DORF( r, extended->call.fdesc.ts2.func, duf_cli_options_get_ptargc( cli ), duf_cli_options_get_ptargv( cli ), optargg ); /* targc, targv and optarg */
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_CLARIFIED ); */
    break;
  }
  DUF_ENDR( r );
}
