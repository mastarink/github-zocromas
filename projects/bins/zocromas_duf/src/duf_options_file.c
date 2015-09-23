#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_option_cmd.h"

#include "duf_utils_path.h"

/* ###################################################################### */
#include "duf_options.h"
#include "duf_options_string.h"
#include "duf_options_file.h"
/* ###################################################################### */


/*
 * open configuration file
 *         with path combined from dir and filename, possibly prefixed by dot
 * ***********************
 * ?! replaces DUF_CONFIGG( config_path )
 * return FILE *, optionally indirectly return error code (errno)
 * */
static FILE *
duf_infilepath( const char *filepath, int *pr )
{
  FILE *f = NULL;

  f = fopen( filepath, "r" );

  mas_free( DUF_CONFIGG( config_path ) );
  DUF_CONFIGWS( config_path , mas_strdup( filepath ));
  DUF_TRACE( options, 0, "opened conf file %s %s", DUF_CONFIGG( config_path ), f ? "Ok" : "FAIL" );
  if ( !f && pr )
    *pr = errno;
  return f;
}

static FILE *
duf_infile( int dot, const char *at, const char *filename, int *pr )
{
  FILE *f = NULL;
  char *cfgpath = NULL;

  assert( duf_config );
  cfgpath = mas_strdup( at );
  assert( cfgpath );
#if 0
  cfgpath = mas_strcat_x( cfgpath, "/" );
#else
  cfgpath = duf_normalize_path( cfgpath );
#endif
  assert( cfgpath );
  if ( dot )
    cfgpath = mas_strcat_x( cfgpath, "." );
  assert( cfgpath );
  cfgpath = mas_strcat_x( cfgpath, filename );
  assert( cfgpath );
  f = duf_infilepath( cfgpath, pr );
  mas_free( cfgpath );
  return f;
}

/* 20150122
 * get options/settings from passed configuration dir and filename
 *    - global variable duf_config must be created/inited
 * ***********************************************
 * 1. call duf_infile with dot=0
 * 2. if none found (no success) : call duf_infile with dot=1 and HOME as dir (home-based configuration file)
 * 3. if none found (no success) : call duf_infile with dot=0 and current dir (cwd, current directory, work directory)
 * 4. if any file opened at 1, 2 or 3 :
 *            4.1. read it line by line,
 *            4.2. chomp each string (line),
 *            4.3. drop comments (lines started with '#' or '#' prefixed with spaces)
 *            4.4. call mas_expand_string to expand string (my zocromas_mas_tools library)
 *            4.5. call duf_exec_cmd_long_xtables_std to parse/process/execute option
 *            4.6. close configuration file
 * */


int
duf_infile_options_at_stream( duf_option_stage_t istage, FILE * f, duf_option_source_t source )
{
  DEBUG_STARTR( r );

  DUF_TRACE( options, 1, "@@@stream stage:%d", istage );
  while ( DUF_NOERROR( r ) && f && !feof( f ) )
  {
    char buffer[1024];
    char *s;

    s = fgets( buffer, sizeof( buffer ), f );
    if ( s )
    {
      s = mas_chomp( s );
      DUF_TRACE( options, 1, "@@@@read cmd '%s'", s );
      if ( s && ( ( *s == '#' ) || !*s ) )
        continue;
      /* DUF_TRACE( any, 0, "buffer:[%s]", buffer ); */
      DUF_TRACE( explain, 0, "read config line %s", s );
      {
#if 0
        char *xs;

        xs = mas_expand_string( s );
        DUF_TRACE( explain, 0, "expanded config line %s", xs );
/* 
 * duf_exec_cmd_long_xtables_std return codeval>0 for "help" option
 *   =0 for other option
 *   errorcode<0 for error
 * */
#  if 0
        DOR( r, duf_exec_cmd_long_xtables_std( xs, '=', istage, source ? source : DUF_OPTION_SOURCE_STREAM ) );
#  else
        DOR( r, duf_string_options_at_string( 0 /* vseparator */ , istage, source ? source : DUF_OPTION_SOURCE_STREAM, xs, 0 ) );
#  endif

        DUF_TRACE( options, 5, "executed cmd; r=%d; xs=%s", r, xs );
        mas_free( xs );
#else
        DOR( r, duf_string_options_at_string( 0 /* vseparator */ , istage, source ? source : DUF_OPTION_SOURCE_STREAM, s, 0 ) );
#endif
      }
    }
  }
  DEBUG_ENDR( r );
}

int
duf_infile_options_at_filepath( duf_option_stage_t istage, const char *filepath )
{
  DEBUG_STARTR( r );
  FILE *f = NULL;

  f = duf_infilepath( filepath, NULL );

  DUF_TRACE( options, 0, "to read config file %s", filepath );
  if ( f )
  {
    DOR( r, duf_infile_options_at_stream( istage, f, DUF_OPTION_SOURCE_FILE ) );

    fclose( f );
    DUF_TRACE( options, 0, "read config file %s", filepath );
  }
  else
  {
    DUF_TRACE( options, 0, "fail to read config file %s", filepath );
    DUF_MAKE_ERROR( r, DUF_ERROR_OPEN );
  }
  DEBUG_ENDR( r );
}

static int
duf_infile_options_at_dir_and_file( duf_option_stage_t istage, const char *cfgdir, const char *filename, int optional )
{
  DEBUG_STARTR( r );
  FILE *f = NULL;

  if ( cfgdir )
    f = duf_infile( 0, cfgdir, filename, NULL );
  if ( !f )
  {
    cfgdir = getenv( "HOME" );
    DUF_TRACE( options, 0, "getting variable HOME value for config path (secondary) : %s", cfgdir );
    f = duf_infile( 1, cfgdir, filename, NULL );
  }
  if ( !f )
  {
    cfgdir = ".";
    f = duf_infile( 0, cfgdir, filename, NULL );
  }

  DUF_TRACE( explain, 0, "to read config file" );
  if ( f )
  {
    DOR( r, duf_infile_options_at_stream( istage, f, DUF_OPTION_SOURCE_CFG ) );

    fclose( f );
    DUF_TRACE( explain, 0, "read config file" );
  }
  else if ( !optional )
  {
    DUF_TRACE( explain, 0, "read config file" );
    DUF_MAKE_ERROR( r, DUF_ERROR_OPEN );
  }
  DEBUG_ENDR( r );
}

/* 20150122
 * get options/settings from passed configuration filename
 *    - global variable duf_config must be created/inited
 * ***********************************************
 * 1. set configuration directory from OS envirionment
 * 2. call duf_infile_options_at_dir_and_file
 * */
int
duf_infile_options_at_file( duf_option_stage_t istage, const char *filename, int optional )
{
  DEBUG_STARTR( r );
  const char *cfgdir = NULL;

  cfgdir = getenv( DUF_CONFIG_PATH_FROM_ENV );
  DUF_TRACE( options, 0, "getting variable " DUF_CONFIG_PATH_FROM_ENV " value for config path : %s", cfgdir );
/* 
 * duf_infile_options_at_dir_and_file return codeval>0 for "help" option
 *   =0 for other option
 *   errorcode<0 for error
 * */
  DOR( r, duf_infile_options_at_dir_and_file( istage, cfgdir, filename, optional ) );

  DEBUG_ENDR( r );
}

/*
 * get options/settings from standard configuration file(s)
 *    - global variable duf_config must be created/inited
 * ***********************************************
 * 1. set configuration file name (constant)
 * 2. call duf_infile_options_at_file
 * */
int
duf_incfgf_options( duf_option_stage_t istage, const char *bfilename, int optional )
{
  DEBUG_STARTR( r );
  char *filename;

  filename = mas_strdup( bfilename );
  filename = mas_strcat_x( filename, ".conf" );
  DUF_TRACE( options, 0, "@@@@(%d) source: infile(%s)", istage, filename );

  DOR( r, duf_infile_options_at_file( istage, filename, optional ) );

  mas_free( filename );
  DEBUG_ENDR( r );
}

int
duf_incfg_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  if ( istage == DUF_OPTION_STAGE_SETUP )
    DOR( r, duf_incfgf_options( istage, DUF_CONFIG_FILE_NAME, 0 ) );

  DEBUG_ENDR( r );
}

int
duf_incfg_last_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  DOR( r, duf_incfgf_options( istage, DUF_CONFIG_FILE_NAME "_last", 0 ) );

  DEBUG_ENDR( r );
}


#if 0
int
duf_incfg_src_options( duf_option_stage_t istage, duf_option_source_t src )
{
  DEBUG_STARTR( r );
  char *bfilename;

  static const char *tail[] = {
    [DUF_OPTION_SOURCE_NONE] = "none",
    [DUF_OPTION_SOURCE_ENV] = "env",
    [DUF_OPTION_SOURCE_STDIN] = "stdin",
    [DUF_OPTION_SOURCE_STREAM] = "stream",
    [DUF_OPTION_SOURCE_FILE] = "file",
    [DUF_OPTION_SOURCE_CFG] = "cfg",
    [DUF_OPTION_SOURCE_CLI] = "cli",
    [DUF_OPTION_SOURCE_INTERACTIVE] = "interactive",
  };
  if ( istage > DUF_OPTION_SOURCE_NONE && istage < DUF_OPTION_SOURCE_MAX )
  {
    bfilename = mas_strdup( DUF_CONFIG_FILE_NAME );
    bfilename = mas_strcat_x( bfilename, "_" );
    bfilename = mas_strcat_x( bfilename, tail[src] );
    DOR( r, duf_incfgf_options( istage, bfilename, 1 /* optional */  ) );
    mas_free( bfilename );
  }

  DEBUG_ENDR( r );
}
#endif
int
duf_incfg_stg_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  if ( istage > DUF_OPTION_STAGE_ANY && istage < DUF_OPTION_STAGE_MAX )
  {
    char *bfilename;
    const char *sn;

    sn = duf_stage_name( istage );
    if ( sn )
    {
      bfilename = mas_strdup( DUF_CONFIG_FILE_NAME );
      bfilename = mas_strcat_x( bfilename, "_" );
      bfilename = mas_strcat_x( bfilename, sn );
      DOR( r, duf_incfgf_options( istage, bfilename, 1 /* optional */  ) );
      mas_free( bfilename );
    }
  }

  DEBUG_ENDR( r );
}



/* duf_stdin_options - can be executed only once (direct stdin reading!)  */
int
duf_stdin_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );
  static int done = 0;

  DUF_TRACE( options, 0, "@@@@(%d) source: stdin", istage );
  if ( istage == DUF_OPTION_STAGE_FIRST /* XXX ???? XXX */  )
  {
    if ( !done )
    {
      DUF_TRACE( options, 4, "@@@ isatty: %d:%d:%d", isatty( STDIN_FILENO ), isatty( STDOUT_FILENO ), isatty( STDERR_FILENO ) );
      if ( !isatty( STDIN_FILENO ) /* only when stdin is NOT tty */  )
      {
        DOR( r, duf_infile_options_at_stream( istage, stdin, DUF_OPTION_SOURCE_STDIN ) );
        done = 1;
      }
    }
    else
    {
      /* DUF_MAKE_ERROR( r, DUF_ERROR_OPEN ); */
      /* assert(0); */
    }
  }
  DEBUG_ENDR( r );
}

int
duf_indirect_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  DUF_TRACE( options, 0, "@@@@(%d) source: indirect", istage );
  DUF_TRACE( temp, 2, ">> targc:%d targ_offset:%d", DUF_CONFIGG( targ.argc ), DUF_CONFIGG( targ_offset ) );
  for ( int ia = 0; ia < DUF_CONFIGG( targ_offset ); ia++ )
  {
    const char *cf;

    cf = DUF_CONFIGG( targ.argv[ia] );
    DUF_TRACE( temp, 2, ">> targv[%d]='%s'", ia, cf );
    if ( cf && *cf == '@' )
    {
      DOR( r, duf_infile_options_at_filepath( istage, cf + 1 ) );
      DUF_TRACE( temp, 2, ">> (%d) done targv[%d]='%s'", r, ia, cf );
    }
  }

  DEBUG_ENDR( r );
}
