#include <assert.h>
#include <string.h>
#include <errno.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_option_cmd.h"
#include "duf_options_enum.h"

/* ###################################################################### */
#include "duf_option_file.h"
/* ###################################################################### */


/*
 * open configuration file
 *         with path combined from dir and filename, possibly prefixed by dot
 * ***********************
 * ?! replaces duf_config->config_path
 * return FILE *, optionally indirectly return error code (errno)
 * */
static FILE *
duf_infile( int dot, const char *at, const char *filename, int *pr )
{
  FILE *f = NULL;
  char *cfgpath = NULL;

  cfgpath = mas_strdup( at );
  cfgpath = mas_strcat_x( cfgpath, "/" );
  if ( dot )
    cfgpath = mas_strcat_x( cfgpath, "." );
  cfgpath = mas_strcat_x( cfgpath, filename );
  if ( cfgpath )
  {
    f = fopen( cfgpath, "r" );
  }
  if ( duf_config )
  {
    mas_free( duf_config->config_path );
    duf_config->config_path = cfgpath;
  }
  else
    mas_free( cfgpath );
  DUF_TRACE( options, 0, "opening conf file %s %s", cfgpath, f ? "Ok" : "FAIL" );
  if ( !f && pr )
    *pr = errno;
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
 *            4.5. call duf_execute_cmd_long_xtables_std to parse/process/execute option
 *            4.6. close configuration file
 * */
static int
duf_infile_options_at_dir_and_file( int argc, char *argv[], const char *cfgdir, const char *filename )
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
    while ( r >= 0 && f && !feof( f ) )
    {
      char buffer[1024];
      char *s;

      s = fgets( buffer, sizeof( buffer ), f );
      if ( s )
      {
        s = mas_chomp( s );
        if ( s && ( ( *s == '#' ) || !*s ) )
          continue;
        /* DUF_TRACE( any, 0, "buffer:[%s]", buffer ); */
        DUF_TRACE( explain, 0, "read config line %s", s );
        {
          char *xs;

          xs = mas_expand_string( s );
          DUF_TRACE( explain, 0, "expanded config line %s", xs );
/* 
 * duf_execute_cmd_long_xtables_std return codeval>0 for "help" option
 *   =0 for other option
 *   errorcode<0 for error
 * */
          r = duf_execute_cmd_long_xtables_std( xs, '=', 0 );
          mas_free( xs );
        }
      }
    }
    fclose( f );
    DUF_TRACE( explain, 0, "read config file" );
  }
  else
  {
    DUF_TRACE( explain, 0, "read config file" );
    r = DUF_ERROR_OPEN;
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
static int
duf_infile_options_at_file( int argc, char *argv[], const char *filename )
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
  DOR( r, duf_infile_options_at_dir_and_file( argc, argv, cfgdir, filename ));

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
duf_infile_options( int argc, char *argv[] )
{
  return duf_infile_options_at_file( argc, argv, DUF_CONFIG_FILE_NAME );
}
