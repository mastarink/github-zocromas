#include <string.h>
#include <unistd.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_option_types.h"
#include "duf_maintenance_options.h"

#include "duf_options_file.h"
#include "duf_options_env.h"
#include "duf_options_cli.h"
#include "duf_options_interactive.h"

#include "duf_option_stage.h"

/* ###################################################################### */
#include "duf_options.h"
/* ###################################################################### */

SR( OPTIONS, all_options, duf_option_stage_t istage /*, int is_interactive */ , duf_int_void_func_t cb_do_interactive,
    duf_cpchar_void_func_t cb_prompt_interactive )
{
  /* DEBUG_STARTR( r ); */

  DUF_TRACE( options, 10, "@@@@@to do all options for stage %s; is_interactive:%d", duf_optstage_name( istage ),
             cb_do_interactive ? cb_do_interactive(  ) : 0 );


  /* assert( duf_config ); */
#if 1
  DUF_TRACE( temp, 0, "@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 1, "@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 2, "@@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 3, "@@@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 4, "@@@@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 5, "@@@@@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 6, "@@@@@@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 7, "@@@@@@@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 8, "@@@@@@@@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 9, "@@@@@@@@@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 10, "@@@@@@@@@@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
  DUF_TRACE( temp, 11, "@@@@@@@@@@@@@@this is temp DUF_TRACE (stage:%s)", duf_optstage_name( istage ) );
#endif
 extern duf_config_t *duf_config;

        T( "trace out:%p (%p) %d", duf_config->opt.trace.output.out, stdout , ((unsigned long)duf_config->opt.trace.output.out== (unsigned long)stdout));

  DUF_TRACE( temp, 0, "@@stage:%s(%d)", duf_optstage_name( istage ), istage );
  DUF_TRACE( options, 0, "@@stage:%s(%d)", duf_optstage_name( istage ), istage );
#ifdef MAS_TRACING
  int er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#else
  int DUF_UNUSED er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#endif
  DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );
  {
#define DUF_OPTSRCI( _xr, _name, _istage, _cb_do_interactive, _cb_prompt_interactive )  \
  { \
    DUF_TRACE( options, 10, "@@@to do %s options; stage:%s(%d)", #_name, duf_optstage_name( _istage ), _istage ); \
    if ( QNOERR ) \
    { \
      CR( source_ ## _name ## _parse, _istage, _cb_do_interactive, _cb_prompt_interactive ); \
      _xr = QERRIND; \
    } \
    DUF_TRACE( options, 10, "@@@@@done %s options; %s:%d [%c]  (%d:%s)", \
	#_name, #_xr, QERRIND, QERRIND > ' ' && QERRIND < 'z' ? QERRIND : '-', QERRIND, QERRNAME ); \
  }
#define DUF_OPTSRC( _xr, _name, _istage )  \
  DUF_OPTSRCI( _xr, _name, _istage, NULL, NULL )

    DUF_OPTSRC( fr, incfg, istage );
    DUF_OPTSRC( sr, incfg_stg, istage );
    DUF_OPTSRC( er, env, istage ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
    DUF_OPTSRC( or, cli, istage );
    DUF_OPTSRC( isi, stdin, istage );
    DUF_OPTSRC( ir, indirect, istage );
    /* if ( DUF_ACTG_FLAG( interactive ) ) */
    if ( cb_do_interactive && cb_do_interactive(  ) )
      DUF_OPTSRCI( iir, interactive, istage, cb_do_interactive, cb_prompt_interactive );
    DUF_OPTSRC( lr, incfg_last, istage );


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
    DEBUG_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );
  }
  /* DEBUG_ENDR_UPPER( QERRIND, DUF_ERROR_OPTION_NOT_FOUND ); */
  ER( OPTIONS, duf_all_options, duf_option_stage_t istage, int is_interactive );
}
