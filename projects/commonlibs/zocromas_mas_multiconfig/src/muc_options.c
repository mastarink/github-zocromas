/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include "muc_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>
#include <sys/stat.h>                                                /* struct stat */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */

#include "muc_option_types.h"                                        /* muc_longval_extended_t; muc_longval_extended_vtable_t ♠ */

#include "muc_options_file.h"
#include "muc_options_env.h"
#include "muc_options_cli.h"
#include "muc_options_interactive.h"
#include "muc_config_trace_enum.h"

#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

#include "muc_option_clarify_new.h"
/* ###################################################################### */
#include "muc_options_def.h"
#include "muc_options.h"
/* ###################################################################### */

static
muc_SR( OPTIONS, all_options_heterogeneous, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
        muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod )
{
#ifdef MAS_TRACING
  int er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#else
  int MAS_UNUSED er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#endif

  if ( istage <= MUC_OPTION_STAGE_SETUP )
    MUC_OPTSRC( cli, fr, incfg, istage, paod, CFG );                 /* config - only acts at stages <= MUC_OPTION_STAGE_SETUP (??) */
  MUC_OPTSRC( cli, sr, incfg_stg, istage, paod, CFGSTG );            /* per-stage config */
  MUC_OPTSRC( cli, er, env, istage, paod, ENV );
  {
    MUC_OPTSRC( cli, or, cli, istage, paod, CLI );
  }                                                                  /* if ( istage == MUC_OPTION_STAGE_FIRST (* XXX ???? XXX *)  ) */
  if ( istage >= MUC_OPTION_STAGE_BOOT )                             /* don't read stdin before MUC_OPTION_STAGE_BOOT : can't re-read! */
    MUC_OPTSRC( cli, isi, stdin, istage, paod, STDIN );
  MUC_OPTSRC( cli, ir, indirect, istage, paod, DUFFILE );
/* if ( DUF_ACTG_FLAG( interactive ) ) */
  if ( cb_do_interactive && cb_do_interactive(  ) )                  /* interactive - only at INTERACTIVE (`pseudo´) stage ´¨ˇ˘˙ ΞΞΞΞΞΞΞΞΞΞΞΞ ­ */
  {
    MUC_OPTSRCI( cli, iir, interactive, istage, cb_do_interactive, cb_prompt_interactive, paod, INTERAC );
    QTT( "after OPTSRCI interactive" );
  }
  MUC_OPTSRC( cli, lr, incfg_last, istage, paod, CFGLAST );

  MAST_TRACE( explain, 2, "or: %d; fr: %d; sr: %d; er: %d; isi: %d; ir: %d; iir: %d; lr: %d; tr: %d; r: %s", or, fr, sr, er, isi, ir, iir, lr, tr,
              QERRNAME );

  muc_ER( OPTIONS, all_options_heterogeneous, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
          muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod );
}

muc_SR( OPTIONS, all_options_paod, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
        muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod )
{
#ifdef MAS_TRACING
  int er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#else
  int MAS_UNUSED er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#endif
  if ( istage <= MUC_OPTION_STAGE_SETUP )
  {
    muc_CR( soption_xclarify_new_booted_source, cli, istage, MUC_OPTION_SOURCE( CFG ), paod );
    fr = QERRIND;
  }
  muc_CR( soption_xclarify_new_booted_source, cli, istage, MUC_OPTION_SOURCE( CFGSTG ), paod );
  sr = QERRIND;
  muc_CR( soption_xclarify_new_booted_source, cli, istage, MUC_OPTION_SOURCE( ENV ), paod );
  er = QERRIND;
  muc_CR( soption_xclarify_new_booted_source, cli, istage, MUC_OPTION_SOURCE( CLI ), paod );
  or = QERRIND;
/* if ( istage == MUC_OPTION_STAGE_FIRST (* XXX ???? XXX *)  ) */
  muc_CR( soption_xclarify_new_booted_source, cli, istage, MUC_OPTION_SOURCE( STDIN ), paod );
  isi = QERRIND;
  muc_CR( soption_xclarify_new_booted_source, cli, istage, MUC_OPTION_SOURCE( DUFFILE ), paod );
  ir = QERRIND;
  if ( cb_do_interactive && cb_do_interactive(  ) )                  /* interactive - only at INTERACTIVE (`pseudo´) stage ´¨ˇ˘˙ ΞΞΞΞΞΞΞΞΞΞΞΞ ­ */
  {
    MUC_OPTSRCI( cli, iir, interactive, istage, cb_do_interactive, cb_prompt_interactive, paod, INTERAC );
    QTT( "after OPTSRCI interactive" );
    iir = QERRIND;
  }
  muc_CR( soption_xclarify_new_booted_source, cli, istage, MUC_OPTION_SOURCE( CFGLAST ), paod );
  lr = QERRIND;

  MAST_TRACE( explain, 2, "or: %d; fr: %d; sr: %d; er: %d; isi: %d; ir: %d; iir: %d; lr: %d; tr: %d; r: %s", or, fr, sr, er, isi, ir, iir, lr, tr,
              QERRNAME );

  muc_ER( OPTIONS, all_options_paod, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
          muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod );
}

muc_SR( OPTIONS, all_options, muc_config_cli_t * cli, muc_option_stage_t istage /*, int is_interactive */ , muc_int_void_func_t cb_do_interactive,
        muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod, int from_paod )
{

  assert( cli );
  MAST_TRACE( options, 10, "@@@@@to do all options for stage %s; is_interactive:%d", muc_optstage_name( cli, istage ),
              cb_do_interactive ? cb_do_interactive(  ) : 0 );

#if 1
  MAST_TRACE( temp, +0, "@@@this is 0 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, +1, "@@@@this is 1 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, +2, "@@@@@this is 2 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, +3, "@@@@@@this is 3 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, +4, "@@@@@@@this is 4 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, +5, "@@@@@@@@this is 5 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, +6, "@@@@@@@@@this is 6 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, +7, "@@@@@@@@@@this is 7 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, +8, "@@@@@@@@@@@this is 8 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, +9, "@@@@@@@@@@@@this is 9 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, 10, "@@@@@@@@@@@@@this is 10 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
  MAST_TRACE( temp, 11, "@@@@@@@@@@@@@@this is 11 temp MUC_TRACE (stage:%s)", muc_optstage_name( cli, istage ) );
#endif

  MAST_TRACE( optstage, 0, "@@stage:%s(%d)", muc_optstage_name( cli, istage ), istage );
#ifdef MAS_TRACING
  int er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#else
  int MAS_UNUSED er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#endif

/* if ( istage >= MUC_OPTION_STAGE_MIN && istage <= MUC_OPTION_STAGE_MAX ) */
  {
    MASE_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );
    {
      if ( from_paod )
      {
#if 0
        if ( istage <= MUC_OPTION_STAGE_SETUP )
        {
          muc_CR( soption_xclarify_new_booted_source, istage, MUC_OPTION_SOURCE( CFG ), paod );
          fr = QERRIND;
        }
        muc_CR( soption_xclarify_new_booted_source, istage, MUC_OPTION_SOURCE( CFGSTG ), paod );
        sr = QERRIND;
        muc_CR( soption_xclarify_new_booted_source, istage, MUC_OPTION_SOURCE( ENV ), paod );
        er = QERRIND;
        muc_CR( soption_xclarify_new_booted_source, istage, MUC_OPTION_SOURCE( CLI ), paod );
        or = QERRIND;
      /* if ( istage == MUC_OPTION_STAGE_FIRST (* XXX ???? XXX *)  ) */
        muc_CR( soption_xclarify_new_booted_source, istage, MUC_OPTION_SOURCE( STDIN ), paod );
        isi = QERRIND;
        muc_CR( soption_xclarify_new_booted_source, istage, MUC_OPTION_SOURCE( DUFFILE ), paod );
        ir = QERRIND;
        if ( cb_do_interactive && cb_do_interactive(  ) )            /* interactive - only at INTERACTIVE (`pseudo´) stage ´¨ˇ˘˙ ΞΞΞΞΞΞΞΞΞΞΞΞ ­ */
        {
          MUC_OPTSRCI( iir, interactive, istage, cb_do_interactive, cb_prompt_interactive, paod, INTERAC );
          iir = QERRIND;
        }
        muc_CR( soption_xclarify_new_booted_source, istage, MUC_OPTION_SOURCE( CFGLAST ), paod );
        lr = QERRIND;
#else
        muc_CR( all_options_paod, cli, istage, cb_do_interactive, cb_prompt_interactive, paod );
#endif
      }
      else
      {
#if 0
      /* heterogeneous */
        if ( istage <= MUC_OPTION_STAGE_SETUP )
          MUC_OPTSRC( fr, incfg, istage, paod, CFG );                /* config - only acts at stages <= MUC_OPTION_STAGE_SETUP (??) */
        MUC_OPTSRC( sr, incfg_stg, istage, paod, CFGSTG );           /* per-stage config */
        MUC_OPTSRC( er, env, istage, paod, ENV );                    /* => ..._exec_cmd_long_xtables_std => ..._exec_cmd_xtable => ..._clarify_xcmd_full */
        MUC_OPTSRC( or, cli, istage, paod, CLI );
      /* if ( istage == MUC_OPTION_STAGE_FIRST (* XXX ???? XXX *)  ) */
        MUC_OPTSRC( isi, stdin, istage, paod, STDIN );
        MUC_OPTSRC( ir, indirect, istage, paod, DUFFILE );
      /* if ( DUF_ACTG_FLAG( interactive ) ) */
        if ( cb_do_interactive && cb_do_interactive(  ) )            /* interactive - only at INTERACTIVE (`pseudo´) stage ´¨ˇ˘˙ ΞΞΞΞΞΞΞΞΞΞΞΞ ­ */
          MUC_OPTSRCI( iir, interactive, istage, cb_do_interactive, cb_prompt_interactive, paod, INTERAC );
        MUC_OPTSRC( lr, incfg_last, istage, paod, CFGLAST );
#else
        muc_CR( all_options_heterogeneous, cli, istage, cb_do_interactive, cb_prompt_interactive, paod );
#endif
      }

#ifdef MAS_TRACING
      {
        struct stat st_stdin;

        if ( fstat( STDIN_FILENO, &st_stdin ) >= 0 )
        {
          MAST_TRACE( options, 120, "isatty:%d, st_dev:%04llx, st_rdev:%04llx, st_ino:%04llx, chr:%d, isfifo:%d", isatty( STDIN_FILENO ),
                      ( unsigned long long ) st_stdin.st_dev, ( unsigned long long ) st_stdin.st_rdev, ( unsigned long long ) st_stdin.st_ino,
                      S_ISCHR( st_stdin.st_mode ), S_ISFIFO( st_stdin.st_mode ) );

          switch ( st_stdin.st_mode & S_IFMT )
          {
          case S_IFBLK:
            MAST_TRACE( options, 120, "block device" );
            break;
          case S_IFCHR:
            MAST_TRACE( options, 120, "character device" );
            break;
          case S_IFDIR:
            MAST_TRACE( options, 120, "directory" );
            break;
          case S_IFIFO:
            MAST_TRACE( options, 120, "FIFO/pipe" );
            break;
          case S_IFLNK:
            MAST_TRACE( options, 120, "symlink" );
            break;
          case S_IFREG:
            MAST_TRACE( options, 120, "regular file" );
            break;
          case S_IFSOCK:
            MAST_TRACE( options, 120, "socket" );
            break;
          default:
            MAST_TRACE( options, 120, "unknown?" );
            break;
          }
        }
      }
#endif

      MAST_TRACE( explain, 2, "or: %d; fr: %d; sr: %d; er: %d; isi: %d; ir: %d; iir: %d; lr: %d; tr: %d; r: %s", or, fr, sr, er, isi, ir, iir, lr, tr,
                  QERRNAME );
      MAST_TRACE( options, 10, "@@@@@done all options for stage %s (%d:%s)", muc_optstage_name( cli, istage ), QERRIND, QERRNAME );
    }
    MASE_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );
  }
/* MUC_ENDR_UPPER( QERRIND, DUF_ERROR_OPTION_NOT_FOUND ); */
  muc_ER( OPTIONS, muc_all_options, muc_config_cli_t * cli, muc_option_stage_t istage, int is_interactive, muc_option_adata_t * paod );
}
