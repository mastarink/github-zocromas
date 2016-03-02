/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include "muc_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

/* #include "muc_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "muc_errorn_defs.h"                                         (* DUF_NOERROR; MUC_CLEAR_ERROR; DUF_E_(LOWER|UPPER); MUC_TEST_R ... ♠ *) */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_config_output_weak.h"

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */
#include "muc_option_clarify.h"
#include "muc_option_cfind.h"                                        /* muc_coption_xfind_at_stdx;  ..._coption_*  etc... ♠ */

#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

#include "muc_option_clarify_batch.h"

#include "muc_option_clarify_new.h"

/* ###################################################################### */
#include "muc_options_cli.h"
/* ###################################################################### */

static char *
clrfy_cli_opts_msgs( muc_config_cli_t * cli, muc_option_gen_code_t codeval, int optindd, int optoptt )
{
  const char *arg;
  static const char *msg = "Invalid option";
  char buffer[2048] = "";

  arg = muc_cli_options_get_cargvn( cli, optindd );
  if ( mas_verbose && mas_verbose(  ) > 0 )
    snprintf( buffer, sizeof( buffer ), "%s '%s' arg[%d]=\"%s\" [%u/%c/%c]", msg, arg, optindd, arg, codeval, codeval, optoptt );
  else
    snprintf( buffer, sizeof( buffer ), " %s '%s'", msg, arg );
  return mas_strdup( buffer );
}

static
muc_SR( OPTIONS, lcoption_parse, muc_config_cli_t * cli, int longindex, muc_option_t * longitem, muc_option_gen_code_t codeval,
        muc_option_gen_code_t maxcodeval, muc_option_stage_t istage, muc_option_adata_t * paod )
{

  MAST_TRACE( options, 40, "@@@@@@getopt_long: cv:%-4d =>  ('%c')  li:%d; '%s'/'%s' oi:%d; oo:%d; oe:%d; stage:%s" /* */
              , codeval                                              /* */
              , codeval > ' ' && codeval <= 'z' ? codeval : '?'      /* */
              , longindex                                            /* */
              , longindex >= 0 ? muc_cli_options_get_longopts_table( cli )[longindex].name : "?" /* */
              , longitem ? longitem->name : ""                       /* */
              , optind                                               /* */
              , optopt                                               /* */
              , opterr                                               /* */
              , muc_optstage_name( cli, istage )                     /* */
           );
  {
    int optoptt = 0;

    optoptt = 0;
    if ( codeval == '?' )
    {
      char *msg = NULL;

      optoptt = optopt;

      msg = clrfy_cli_opts_msgs( cli, codeval, optind - 1, optoptt );

      MAST_TRACE( options, +140, "@@@@@@@%d:%s", optind - 1, muc_cli_options_get_carg( cli )->argv[optind - 1] );
      if ( istage != MUC_OPTION_STAGE_BOOT )
        ERRMAKE_M( OPTION_NOT_FOUND, msg );
      mas_free( msg );

    /* MUC_CLEAR_ERROR( QERRIND, DUF_ERROR_OPTION_NOT_FOUND ); */
    }

#if 0                                                                /* 1:old clarify; 0:new clarify; see also ..._option_clarify_batch.c.... ; 20160115.170518 */
    muc_CR( lcoption_clarify, longindex, codeval, optarg, istage, MUC_OPTION_SOURCE( CLI ) );
#else
    if ( QNOERR )
    {
      mas_cargvc_t *carg;
      const char *longoptname = NULL;
      const char *qarg = NULL;

      carg = muc_cli_options_get_carg( cli );
      if ( optind > 0 )
        qarg = carg->argv[optind - 1];
    /* longoptname = ..._coption_find_name_at_std( codeval, QPERRIND ); */
      if ( longitem )
        longoptname = longitem->name;
      if ( !longoptname )
        longoptname = muc_lcoption_find_name_at_std( cli, codeval, maxcodeval, &longindex, QPERRIND );
      assert( ( longindex >= 0 && longoptname ) || ( longindex < 0 && !longoptname ) );
      if ( longindex < 0 /* && codeval == '?' */  && !longoptname && qarg && qarg[0] == '-' && qarg[1] == '-' && qarg[2] != '-' )
        longoptname = qarg + 2;
      if ( longoptname )
      {
        muc_CR( soption_xclarify_na_new_at_stdx_default, cli, longoptname, optarg, 0 /* value_separator */ , istage, MUC_OPTION_SOURCE( CLI ),
                paod );
      }
# if 0
      else if ( codeval == '?' )
      {
        SERR( OPTION_NOT_PARSED );
        assert( 0 );
      }
# endif
      else                                                           /* ERROR */
      {
        SERR( OPTION_NOT_PARSED );
      /* assert( 0 ); */
      }
    }
#endif
  }
  muc_ER( OPTIONS, lcoption_parse, muc_config_cli_t * cli, int longindex, muc_option_t * longitem, muc_option_gen_code_t codeval,
          muc_option_gen_code_t maxcodeval, muc_option_stage_t istage, muc_option_adata_t * paod );
}

static
muc_SR( OPTIONS, cli_parse_targ, muc_config_cli_t * cli, int optindd, muc_option_stage_t istage )
{
  int numxargv = 0;
  mas_cargvc_t *carg;

  carg = muc_cli_options_get_carg( cli );
  numxargv = carg->argc - optindd;
  MAST_TRACE( options, 40, "@cli opts: stage:%s; carg->argc:%d; numxargv:%d; optindd:%d;", muc_optstage_name( cli, istage ), carg->argc, numxargv,
              optindd );

/* 20160129.121211 MUC_OPTION_STAGE_SETUP Changed to  MUC_OPTION_STAGE_BOOT */
  if ( QNOERR && istage == MUC_OPTION_STAGE_BOOT && /* optind < carg.argc && */ numxargv > 0 )
  {
  /* to do once, at stage SETUP (?), for next stages */
    MAST_TRACE( options, +150, "(for targ) carg.argv[%d]=\"%s\"", optindd, carg->argv[optindd] );
    muc_CR( argv_clarify, cli, muc_cli_options_get_targ( cli ), carg, optindd );

    if ( QNOERR )
      muc_cli_options_reset_targ_offset( cli );
  }
  else
  {
    MAST_TRACE( options, +150, "(no targ) optind:%d; optindd:%d; numxargv:%d", optind, optindd, numxargv );
  }
  muc_ER( OPTIONS, cli_parse_targ, muc_config_cli_t * cli, int optindd, muc_option_stage_t istage );
}

/* 20160113.124316 */
static
muc_SR( OPTIONS, cli_parse, muc_config_cli_t * cli, const char *shorts, muc_option_gen_code_t maxcodeval, muc_option_stage_t istage,
        muc_option_adata_t * paod )
{

  mas_cargvc_t *carg;

/* int optoptt = 0; */
  int optindd = 0, optindp MAS_UNUSED = 0;

  MASE_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );
  optopt = 0;
  opterr = 0;
  optind = 1;
  optindp = optind;

  carg = muc_cli_options_get_carg( cli );

  MAST_TRACE( options, 40, "@to clarify cli opts: stage:%s; carg->argc:%d", muc_optstage_name( cli, istage ), carg->argc );

  {
    muc_option_gen_code_t codeval;
    muc_option_t *longtable;

    longtable = muc_cli_options_get_longopts_table( cli );

  /* optind
   *   inside loop means next arg to process in non-permuted list
   *   outside loop means first not processed arg in permuted list
   * */
    do
    {
      int longindex;

      optopt = 0, longindex = -1;

      codeval = getopt_long( carg->argc, carg->argv, shorts, longtable, &longindex );
      if ( codeval >= 0 )
      {
        optindd = optind;
      /* codeval >=0 && codeval!='?' && longindex<0 means short => in this case longindex=...  */
#if 1
        muc_CR( lcoption_parse, cli, longindex, longindex >= 0 ? &longtable[longindex] : NULL, codeval, maxcodeval, istage, paod );
/* TODO */
#else
        const muc_longval_extended_t *extended = NULL;

        extended = ... _noption_xfind_no_at_stdx( name, arg ? 1 : 0, 1 /* soft */ , pno, pcnt, QPERRIND );
        muc_CR( xoption_parse, extended, istage );
#endif
        optindp = optind;
      }
    }
    while ( QNOERR && codeval >= 0 );
  }
  muc_CR( cli_parse_targ, cli, optindd, istage );
/* MUC_ENDR_UPPER( r, DUF_ERROR_OPTION_NOT_FOUND ); */
  MASE_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );

  muc_ER( OPTIONS, cli_parse, muc_config_cli_t * cli, const char *shorts, muc_option_gen_code_t maxcodeval, muc_option_stage_t istage,
          muc_option_adata_t * paod );
}

muc_SR( OPTIONS, source_cli_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive MAS_UNUSED,
        muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod, muc_option_source_code_t sourcecode MAS_UNUSED )
{

  MAST_TRACE( optsource, 0, "@   source:%s", muc_optsourcecode_name( cli, sourcecode ) );
  MAST_TRACE( options, 20, "@@@@cli options; stage:%s", muc_optstage_name( cli, istage ) );

  muc_CR( cli_parse, cli, muc_cli_options_get_shorts( cli ), muc_cli_options_get_maxcodeval( cli ), istage, paod );

  if ( istage < MUC_OPTION_STAGE_SETUP )
    ERRCLEAR( OPTION_NOT_FOUND );

  MAST_TRACE( options, 20, "@@@@cli options done; stage:%s (%d:%s)", muc_optstage_name( cli, istage ), QERRIND, QERRNAME );

  muc_ER( OPTIONS, source_cli_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive MAS_UNUSED,
          muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod, muc_option_source_code_t sourcecode MAS_UNUSED );
}
