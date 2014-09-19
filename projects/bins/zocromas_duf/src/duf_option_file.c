#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_option_cmd.h"
#  include "duf_options_enum.h"

/* ###################################################################### */
#include "duf_option_file.h"
/* ###################################################################### */



static FILE *
duf_infile( int dot, const char *at, const char *filename )
{
  FILE *f = NULL;
  char *cfgpath = NULL;

  cfgpath = mas_strdup( at );
  cfgpath = mas_strcat_x( cfgpath, "/" );
  if ( dot )
    cfgpath = mas_strcat_x( cfgpath, "." );
  cfgpath = mas_strcat_x( cfgpath, filename );
  DUF_TRACE( options, 0, "opening conf file %s", cfgpath );
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
  return f;
}

/* 
 * return codeval>0 for "help" option
 *   =0 for other option
 *   errorcode<0 for error
 * */
static int
duf_infile_options_at_dir_and_file( int argc, char *argv[], const char *cfgdir, const char *filename )
{
  int r = 0;
  FILE *f = NULL;

  if ( cfgdir )
    f = duf_infile( 0, cfgdir, filename );
  if ( !f )
  {
    cfgdir = getenv( "HOME" );
    DUF_TRACE( options, 0, "getting variable HOME value for config path (secondary) : %s", cfgdir );
    f = duf_infile( 1, cfgdir, filename );
  }
  DUF_TRACE( explain, 0, "to read config file" );
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
  if ( f )
    fclose( f );
  DUF_TRACE( explain, 0, "read config file" );
  return r;
}

/* 
 * return codeval>0 for "help" option
 *   =0 for other option
 *   errorcode<0 for error
 * */
static int
duf_infile_options_at_file( int argc, char *argv[], const char *filename )
{
  int r = 0;
  const char *cfgdir = NULL;

  cfgdir = getenv( DUF_CONFIG_PATH_FROM_ENV );
  DUF_TRACE( options, 0, "getting variable " DUF_CONFIG_PATH_FROM_ENV " value for config path : %s", cfgdir );
/* 
 * duf_infile_options_at_dir_and_file return codeval>0 for "help" option
 *   =0 for other option
 *   errorcode<0 for error
 * */
  r = duf_infile_options_at_dir_and_file( argc, argv, cfgdir, filename );

  DUF_TEST_R( r );
  return r;
}

/* 
 * return oclass>0 for "help" option
 *   =0 for other option
 *   errorcode<0 for error
 * */
int
duf_infile_options( int argc, char *argv[] )
{
  return duf_infile_options_at_file( argc, argv, DUF_CONFIG_FILE_NAME );
}
