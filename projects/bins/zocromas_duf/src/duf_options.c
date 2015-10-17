#include <string.h>
#include <unistd.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_options_file.h"
#include "duf_options_env.h"
#include "duf_options_cli.h"
#include "duf_options_interactive.h"

#include "duf_option_names.h"

/* ###################################################################### */
#include "duf_options.h"
/* ###################################################################### */

/*
 * get get options/settings
 *    - global variable duf_config must be created/inited
 * ************************
 * 1. call duf_env_options, i.e. get options/settings from OS environment
 * 2. call duf_infile_options, i.e. get options/settings from configuration file(s)
 * 3. call duf_cli_options, i.e. get options/settings from command line
 * */

/*
 * TODO join duf_action and duf_all_options
 *
 *
 * duf_action								| duf_all_options
 * with istage = DUF_OPTION_STAGE_FIRST					| with istage
 * 									|  
 * 									|
 * 									|  - duf_env_options
 * 									|  - duf_infile_options
 *   - duf_cli_options( istage )					|  - duf_cli_options
 *   - duf_stdin_options( istage )					|
 *   - duf_indirect_options( istage )					|  - duf_indirect_options
 *   - duf_interactive_options( DUF_OPTION_STAGE_INTERACTIVE )		|
 * */

int
duf_all_options(  /* int argc, char *argv[], */ duf_option_stage_t istage, int is_interactive )
{
  DEBUG_STARTR( r );

  /* assert( duf_config ); */
#if 0
  DUF_TRACE( temp, 0, "@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@@@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@@@@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@@@@@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@@@@@@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@@@@@@@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
  DUF_TRACE( temp, 0, "@@@@@@@@@@@@@@this is temp DUF_TRACE :%d", DUF_CONFIGG( opt.trace.temp ) );
#endif

  DUF_TRACE( options, 0, "@@@@@stage:%s(%d)", duf_optstage_name( istage ), istage );
#ifdef MAS_TRACING
  int er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#else
  int DUF_UNUSED er = 0, fr = 0, sr = 0, or = 0, isi = 0, ir = 0, iir = 0, lr = 0, tr = 0;
#endif
  DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );

#if 1
#  define DUF_OPTSRC(_r, _xr, _name, _istage )  \
  { \
    DUF_TRACE( options, 0, "@@@@@@stage:%s(%d) {" #_name "}", duf_optstage_name( _istage ), _istage ); \
    if ( DUF_NOERROR( r ) ) \
    { \
      DORF( _r, duf_ ## _name ## _options, _istage ); \
      _xr = _r; \
    } \
    DUF_TRACE( options, +4, "@got " #_name " options; " #_xr ":%d (%c)  %s", _xr, _xr > ' ' && _xr < 'z' ? _xr : '-', duf_error_name_i( r ) ); \
  }
  DUF_OPTSRC( r, er, env, istage ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
  DUF_OPTSRC( r, fr, incfg, istage );
  DUF_OPTSRC( r, sr, incfg_stg, istage );
  DUF_OPTSRC( r, or, cli, istage );
  DUF_OPTSRC( r, isi, stdin, istage );
  DUF_OPTSRC( r, ir, indirect, istage );
  /* if ( DUF_ACTG_FLAG( interactive ) ) */
  if ( is_interactive )
    DUF_OPTSRC( r, iir, interactive, istage );
  DUF_OPTSRC( r, lr, incfg_last, istage );
#else
  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_env_options( istage ) ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
    er = r;
  }
  DUF_TRACE( options, +4, "@got env options; er:%d (%c)  %s", er, er > ' ' && er < 'z' ? er : '-', duf_error_name_i( r ) );

  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_cfg_options( istage ) ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
    fr = r;
  }
  DUF_TRACE( options, +4, "@got infile options; fr:%d (%c)  %s", fr, fr > ' ' && fr < 'z' ? fr : '-', duf_error_name_i( r ) );

  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_cli_options( istage ) ); /* => duf_parse_exec_option => duf_clarify_xcmd_full => duf_clarify_xcmd_(typed|old) */
    or = r;
  }
  DUF_TRACE( options, +4, "@got cli options; or:%d (%c)  %s", or, or > ' ' && or < 'z' ? or : '-', duf_error_name_i( r ) );

#  if 1
  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_stdin_options( istage ) ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
    isi = r;
  }
  DUF_TRACE( options, +4, "@got indirect options; or:%d (%c)  %s", isi, isi > ' ' && isi < 'z' ? isi : '-', duf_error_name_i( r ) );
#  endif

#  if 1
/* duf_indirect_options - only for next stage  */
  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_indirect_options( istage ) ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
    ir = r;
  }
  DUF_TRACE( options, +4, "@got indirect options; or:%d (%c)  %s", ir, ir > ' ' && ir < 'z' ? ir : '-', duf_error_name_i( r ) );
#  endif

  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( interactive ) )
  {
    DOR( r, duf_interactive_options( istage ) );
    iir = r;
  }
  DUF_TRACE( options, +4, "@got indirect options; or:%d (%c)  %s", iir, iir > ' ' && iir < 'z' ? iir : '-', duf_error_name_i( r ) );
#endif

#ifdef MAS_TRACING
  {
    struct stat st_stdin;

    if ( fstat( STDIN_FILENO, &st_stdin ) >= 0 )
    {
      DUF_TRACE( options, 20, "isatty:%d, st_dev:%04llx, st_rdev:%04llx, st_ino:%04llx, chr:%d, isfifo:%d", isatty( STDIN_FILENO ),
                 ( unsigned long long ) st_stdin.st_dev, ( unsigned long long ) st_stdin.st_rdev, ( unsigned long long ) st_stdin.st_ino,
                 S_ISCHR( st_stdin.st_mode ), S_ISFIFO( st_stdin.st_mode ) );

      switch ( st_stdin.st_mode & S_IFMT )
      {
      case S_IFBLK:
        DUF_TRACE( options, 20, "block device" );
        break;
      case S_IFCHR:
        DUF_TRACE( options, 20, "character device" );
        break;
      case S_IFDIR:
        DUF_TRACE( options, 20, "directory" );
        break;
      case S_IFIFO:
        DUF_TRACE( options, 20, "FIFO/pipe" );
        break;
      case S_IFLNK:
        DUF_TRACE( options, 20, "symlink" );
        break;
      case S_IFREG:
        DUF_TRACE( options, 20, "regular file" );
        break;
      case S_IFSOCK:
        DUF_TRACE( options, 20, "socket" );
        break;
      default:
        DUF_TRACE( options, 20, "unknown?" );
        break;
      }
    }
  }
#endif



#if 0
/* duf_stdin_options - only for next stage, can be executed only once (direct stdin reading!)  */
  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_stdin_options( istage ) );
    ir = r;
  }
  DUF_TRACE( options, +4, "@got stdin options; or:%d (%c)  %s", ir, ir > ' ' && ir < 'z' ? ir : '-', duf_error_name_i( r ) );
#endif

  DUF_TRACE( explain, 2, "or: %d; fr: %d; sr: %d; er: %d; isi: %d; ir: %d; iir: %d; lr: %d; tr: %d; r: %s", or, fr, sr, er, isi, ir, iir, lr, tr,
             duf_error_name_i( r ) );
  DEBUG_ENDR_UPPER( r, DUF_ERROR_OPTION_NOT_FOUND );
}


int
duf_show_options( const char *a0 DUF_UNUSED )
{
  DEBUG_STARTR( r );
#if 0
  int oseq = 0;

  DUF_TRACE( options, +4, "%s", a0 );
  for ( duf_option_code_t codeval = DUF_OPTION_VAL_NONE; codeval < DUF_OPTION_VAL_MAX_LONG; codeval++ )
  {
#  define BUFSZ 1024 * 4
    char buffer[BUFSZ] = "";

    duf_restore_some_option( buffer, codeval, BUFSZ );
    if ( *buffer )
    {
      oseq++;
      DUF_TRACE( options, +4, "%2d. %s", oseq, buffer );
    }
  }
  DUF_TRACE( options, +4, " --" );
#endif
  DEBUG_ENDR( r );
}
