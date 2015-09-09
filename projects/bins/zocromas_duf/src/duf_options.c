#include <assert.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h"
/* #include "duf_maindb.h" */

#include "duf_option_extended.h"

#include "duf_options_file.h"
#include "duf_options_env.h"
#include "duf_options_cli.h"
#include "duf_options_interactive.h"

#include "duf_option_defs.h"

#include "duf_option_names.h"
/* #include "duf_option_restore.h" */
/* #include "duf_option.h" */

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
duf_all_options(  /* int argc, char *argv[], */ duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  assert( duf_config );
  DUF_TRACE( temp, 0, "@this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
#if 0
  DUF_TRACE( temp, 0, "@@this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
  DUF_TRACE( temp, 0, "@@@this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
  DUF_TRACE( temp, 0, "@@@@this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
  DUF_TRACE( temp, 0, "@@@@@this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
  DUF_TRACE( temp, 0, "@@@@@@this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
  DUF_TRACE( temp, 0, "@@@@@@@this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
  DUF_TRACE( temp, 0, "@@@@@@@@this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
  DUF_TRACE( temp, 0, "@@@@@@@@@this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
  DUF_TRACE( temp, 0, "@@@@@@@@@@this is temp DUF_TRACE :%d", duf_config->cli.trace.temp );
#endif


#ifdef MAS_TRACING
  int er = 0, fr = 0, or = 0, isi = 0, ir = 0, iir = 0, tr = 0;
#else
  int DUF_UNUSED er = 0, fr = 0, or = 0, isi = 0, ir = 0, iir = 0, tr = 0;
#endif
  DEBUG_E_NO( DUF_ERROR_OPTION_NOT_FOUND );

#if 1
#  define DUF_OPTSRC(_r, _xr, _name, _istage)  \
  { \
    if ( DUF_NOERROR( r ) ) \
    { \
      DOR( _r, duf_ ## _name ## _options( _istage ) ); \
      _xr = _r; \
    } \
    DUF_TRACE( options, +2, "@got " #_name " options; " #_xr ":%d (%c)  %s", _xr, _xr > ' ' && _xr < 'z' ? _xr : '-', duf_error_name( r ) ); \
  }
  DUF_OPTSRC( r, er, env, istage ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
  DUF_OPTSRC( r, fr, infile, istage );
  DUF_OPTSRC( r, or, cli, istage );
  DUF_OPTSRC( r, isi, stdin, istage );
  DUF_OPTSRC( r, ir, indirect, istage );
  if ( DUF_ACTG_FLAG( interactive ) )
    DUF_OPTSRC( r, iir, interactive, istage );
#else
  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_env_options( istage ) ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
    er = r;
  }
  DUF_TRACE( options, +2, "@got env options; er:%d (%c)  %s", er, er > ' ' && er < 'z' ? er : '-', duf_error_name( r ) );

  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_infile_options( istage ) ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
    fr = r;
  }
  DUF_TRACE( options, +2, "@got infile options; fr:%d (%c)  %s", fr, fr > ' ' && fr < 'z' ? fr : '-', duf_error_name( r ) );

  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_cli_options( istage ) ); /* => duf_parse_exec_option => duf_clarify_xcmd_full => duf_clarify_xcmd_(typed|old) */
    or = r;
  }
  DUF_TRACE( options, +2, "@got cli options; or:%d (%c)  %s", or, or > ' ' && or < 'z' ? or : '-', duf_error_name( r ) );

#  if 1
  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_stdin_options( istage ) ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
    isi = r;
  }
  DUF_TRACE( options, +2, "@got indirect options; or:%d (%c)  %s", isi, isi > ' ' && isi < 'z' ? isi : '-', duf_error_name( r ) );
#  endif

#  if 1
/* duf_indirect_options - only for next stage  */
  if ( DUF_NOERROR( r ) )
  {
    DOR( r, duf_indirect_options( istage ) ); /* => duf_exec_cmd_long_xtables_std => duf_exec_cmd_xtable => duf_clarify_xcmd_full */
    ir = r;
  }
  DUF_TRACE( options, +2, "@got indirect options; or:%d (%c)  %s", ir, ir > ' ' && ir < 'z' ? ir : '-', duf_error_name( r ) );
#  endif

  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( interactive ) )
  {
    DOR( r, duf_interactive_options( istage ) );
    iir = r;
  }
  DUF_TRACE( options, +2, "@got indirect options; or:%d (%c)  %s", iir, iir > ' ' && iir < 'z' ? iir : '-', duf_error_name( r ) );
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
  DUF_TRACE( options, +2, "@got stdin options; or:%d (%c)  %s", ir, ir > ' ' && ir < 'z' ? ir : '-', duf_error_name( r ) );
#endif

  DUF_TRACE( explain, 2, "or: %d; fr: %d; er: %d; isi: %d; ir: %d; iir: %d; tr: %d; r: %s", or, fr, er, isi, ir, iir, tr, duf_error_name( r ) );
  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND );
}


int
duf_show_options( const char *a0 )
{
  DEBUG_STARTR( r );
#if 0
  int oseq = 0;

  DUF_TRACE( options, +2, "%s", a0 );
  for ( duf_option_code_t codeval = DUF_OPTION_VAL_NONE; codeval < DUF_OPTION_VAL_MAX_LONG; codeval++ )
  {
#define BUFSZ 1024 * 4
    char buffer[BUFSZ] = "";

    duf_restore_some_option( buffer, codeval, BUFSZ );
    if ( *buffer )
    {
      oseq++;
      DUF_TRACE( options, +2, "%2d. %s", oseq, buffer );
    }
  }
  DUF_TRACE( options, +2, " --" );
#endif
  DEBUG_ENDR( r );
}
