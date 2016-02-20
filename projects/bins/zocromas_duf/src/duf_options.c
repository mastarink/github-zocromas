/* #undef MAS_TRACING */
/* #include <stddef.h> */
#include <string.h>

#include "duf_maintenance_z.h"
#include "duf_maintenance_tracen.h"
#include "duf_maintenance_errors.h"

/* #include "duf_config.h" */
/* #include "duf_config_util.h" */

#include "duf_option_types.h"

#include "duf_options_file.h"
#include "duf_options_env.h"
#include "duf_options_cli.h"
#include "duf_options_interactive.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"

#include "duf_option_clarify_new.h"
/* ###################################################################### */
#include "duf_options_def.h"
#include "duf_options.h"
/* ###################################################################### */

SR( OPTIONS, all_options_heterogeneous, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive,
    duf_cpchar_void_func_t cb_prompt_interactive, duf_option_adata_t * paod )
{
#ifdef MAS_TRACING
  int er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#else
  int DUF_UNUSED er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#endif

  if ( istage <= DUF_OPTION_STAGE_SETUP )
    DUF_OPTSRC( fr, incfg, istage, paod, CFG );                      /* config - only acts at stages <= DUF_OPTION_STAGE_SETUP (??) */
  DUF_OPTSRC( sr, incfg_stg, istage, paod, CFGSTG );                 /* per-stage config */
  DUF_OPTSRC( er, env, istage, paod, ENV );                          /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
  {
    DUF_OPTSRC( or, cli, istage, paod, CLI );
  }                                                                  /* if ( istage == DUF_OPTION_STAGE_FIRST (* XXX ???? XXX *)  ) */
  if ( istage >= DUF_OPTION_STAGE_BOOT )                             /* don't read stdin before DUF_OPTION_STAGE_BOOT : can't re-read! */
    DUF_OPTSRC( isi, stdin, istage, paod, STDIN );
  DUF_OPTSRC( ir, indirect, istage, paod, DUFFILE );
/* if ( DUF_ACTG_FLAG( interactive ) ) */
  if ( cb_do_interactive && cb_do_interactive(  ) )                  /* interactive - only at INTERACTIVE (`pseudo´) stage ´¨ˇ˘˙ ΞΞΞΞΞΞΞΞΞΞΞΞ ­ */
    DUF_OPTSRCI( iir, interactive, istage, cb_do_interactive, cb_prompt_interactive, paod, INTERAC );
  DUF_OPTSRC( lr, incfg_last, istage, paod, CFGLAST );

  DUF_TRACE( explain, 2, "or: %d; fr: %d; sr: %d; er: %d; isi: %d; ir: %d; iir: %d; lr: %d; tr: %d; r: %s", or, fr, sr, er, isi, ir, iir, lr, tr,
             QERRNAME );

  ER( OPTIONS, all_options_heterogeneous, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive,
      duf_cpchar_void_func_t cb_prompt_interactive, duf_option_adata_t * paod );
}

SR( OPTIONS, all_options_paod, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive,
    duf_cpchar_void_func_t cb_prompt_interactive, duf_option_adata_t * paod )
{
#ifdef MAS_TRACING
  int er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#else
  int DUF_UNUSED er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#endif
  if ( istage <= DUF_OPTION_STAGE_SETUP )
  {
    CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( CFG ), paod );
    fr = QERRIND;
  }
  CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( CFGSTG ), paod );
  sr = QERRIND;
  CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( ENV ), paod );
  er = QERRIND;
  CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( CLI ), paod );
  or = QERRIND;
/* if ( istage == DUF_OPTION_STAGE_FIRST (* XXX ???? XXX *)  ) */
  CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( STDIN ), paod );
  isi = QERRIND;
  CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( DUFFILE ), paod );
  ir = QERRIND;
  if ( cb_do_interactive && cb_do_interactive(  ) )                  /* interactive - only at INTERACTIVE (`pseudo´) stage ´¨ˇ˘˙ ΞΞΞΞΞΞΞΞΞΞΞΞ ­ */
  {
    DUF_OPTSRCI( iir, interactive, istage, cb_do_interactive, cb_prompt_interactive, paod, INTERAC );
    iir = QERRIND;
  }
  CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( CFGLAST ), paod );
  lr = QERRIND;

  DUF_TRACE( explain, 2, "or: %d; fr: %d; sr: %d; er: %d; isi: %d; ir: %d; iir: %d; lr: %d; tr: %d; r: %s", or, fr, sr, er, isi, ir, iir, lr, tr,
             QERRNAME );

  ER( OPTIONS, all_options_paod, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive,
      duf_cpchar_void_func_t cb_prompt_interactive, duf_option_adata_t * paod );
}

SR( OPTIONS, all_options, duf_option_stage_t istage /*, int is_interactive */ , duf_int_void_func_t cb_do_interactive,
    duf_cpchar_void_func_t cb_prompt_interactive, duf_option_adata_t * paod, int from_paod )
{
/* DUF_STARTR( r ); */

  DUF_TRACE( options, 10, "@@@@@to do all options for stage %s; is_interactive:%d", duf_optstage_name( istage ),
             cb_do_interactive ? cb_do_interactive(  ) : 0 );

#if 1
  DUF_TRACE( temp, 0, "@@@this is 0 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 1, "@@@@this is 1 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 2, "@@@@@this is 2 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 3, "@@@@@@this is 3 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 4, "@@@@@@@this is 4 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 5, "@@@@@@@@this is 5 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 6, "@@@@@@@@@this is 6 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 7, "@@@@@@@@@@this is 7 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 8, "@@@@@@@@@@@this is 8 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 9, "@@@@@@@@@@@@this is 9 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 10, "@@@@@@@@@@@@@this is 10 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 11, "@@@@@@@@@@@@@@this is 11 temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
#endif

  DUF_TRACE( optstage, 0, "@@stage:%s(%d)", duf_optstage_name( istage ), istage );
#ifdef MAS_TRACING
  int er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#else
  int DUF_UNUSED er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#endif

/* if ( istage >= DUF_OPTION_STAGE_MIN && istage <= DUF_OPTION_STAGE_MAX ) */
  {
    DUF_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );
    {
      if ( from_paod )
      {
#if 0
        if ( istage <= DUF_OPTION_STAGE_SETUP )
        {
          CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( CFG ), paod );
          fr = QERRIND;
        }
        CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( CFGSTG ), paod );
        sr = QERRIND;
        CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( ENV ), paod );
        er = QERRIND;
        CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( CLI ), paod );
        or = QERRIND;
      /* if ( istage == DUF_OPTION_STAGE_FIRST (* XXX ???? XXX *)  ) */
        CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( STDIN ), paod );
        isi = QERRIND;
        CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( DUFFILE ), paod );
        ir = QERRIND;
        if ( cb_do_interactive && cb_do_interactive(  ) )            /* interactive - only at INTERACTIVE (`pseudo´) stage ´¨ˇ˘˙ ΞΞΞΞΞΞΞΞΞΞΞΞ ­ */
        {
          DUF_OPTSRCI( iir, interactive, istage, cb_do_interactive, cb_prompt_interactive, paod, INTERAC );
          iir = QERRIND;
        }
        CR( soption_xclarify_new_booted_source, istage, DUF_OPTION_SOURCE( CFGLAST ), paod );
        lr = QERRIND;
#else
        CR( all_options_paod, istage, cb_do_interactive, cb_prompt_interactive, paod );
#endif
      }
      else
      {
#if 0
      /* heterogeneous */
        if ( istage <= DUF_OPTION_STAGE_SETUP )
          DUF_OPTSRC( fr, incfg, istage, paod, CFG );                /* config - only acts at stages <= DUF_OPTION_STAGE_SETUP (??) */
        DUF_OPTSRC( sr, incfg_stg, istage, paod, CFGSTG );           /* per-stage config */
        DUF_OPTSRC( er, env, istage, paod, ENV );                    /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
        DUF_OPTSRC( or, cli, istage, paod, CLI );
      /* if ( istage == DUF_OPTION_STAGE_FIRST (* XXX ???? XXX *)  ) */
        DUF_OPTSRC( isi, stdin, istage, paod, STDIN );
        DUF_OPTSRC( ir, indirect, istage, paod, DUFFILE );
      /* if ( DUF_ACTG_FLAG( interactive ) ) */
        if ( cb_do_interactive && cb_do_interactive(  ) )            /* interactive - only at INTERACTIVE (`pseudo´) stage ´¨ˇ˘˙ ΞΞΞΞΞΞΞΞΞΞΞΞ ­ */
          DUF_OPTSRCI( iir, interactive, istage, cb_do_interactive, cb_prompt_interactive, paod, INTERAC );
        DUF_OPTSRC( lr, incfg_last, istage, paod, CFGLAST );
#else
        CR( all_options_heterogeneous, istage, cb_do_interactive, cb_prompt_interactive, paod );
#endif
      }

#ifdef MAS_TRACING
      {
        struct stat st_stdin;

        if ( fstat( STDIN_FILENO, &st_stdin ) >= 0 )
        {
          DUF_TRACE( options, 120, "isatty:%d, st_dev:%04llx, st_rdev:%04llx, st_ino:%04llx, chr:%d, isfifo:%d", isatty( STDIN_FILENO ),
                     ( unsigned long long ) st_stdin.st_dev, ( unsigned long long ) st_stdin.st_rdev, ( unsigned long long ) st_stdin.st_ino,
                     S_ISCHR( st_stdin.st_mode ), S_ISFIFO( st_stdin.st_mode ) );

          switch ( st_stdin.st_mode & S_IFMT )
          {
          case S_IFBLK:
            DUF_TRACE( options, 120, "block device" );
            break;
          case S_IFCHR:
            DUF_TRACE( options, 120, "character device" );
            break;
          case S_IFDIR:
            DUF_TRACE( options, 120, "directory" );
            break;
          case S_IFIFO:
            DUF_TRACE( options, 120, "FIFO/pipe" );
            break;
          case S_IFLNK:
            DUF_TRACE( options, 120, "symlink" );
            break;
          case S_IFREG:
            DUF_TRACE( options, 120, "regular file" );
            break;
          case S_IFSOCK:
            DUF_TRACE( options, 120, "socket" );
            break;
          default:
            DUF_TRACE( options, 120, "unknown?" );
            break;
          }
        }
      }
#endif

      DUF_TRACE( explain, 2, "or: %d; fr: %d; sr: %d; er: %d; isi: %d; ir: %d; iir: %d; lr: %d; tr: %d; r: %s", or, fr, sr, er, isi, ir, iir, lr, tr,
                 QERRNAME );
      DUF_TRACE( options, 10, "@@@@@done all options for stage %s (%d:%s)", duf_optstage_name( istage ), QERRIND, QERRNAME );
    }
    DUF_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );
  }
/* DUF_ENDR_UPPER( QERRIND, DUF_ERROR_OPTION_NOT_FOUND ); */
  ER( OPTIONS, duf_all_options, duf_option_stage_t istage, int is_interactive, duf_option_adata_t * paod );
}
