#include <stdarg.h>
#include <string.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"


/* ###################################################################### */
#include "duf_config.h"
/* ###################################################################### */

duf_config_t *duf_config = NULL;


int
duf_config_create( void )
{
  duf_config = mas_malloc( sizeof( duf_config_t ) );
  memset( duf_config, 0, sizeof( duf_config ) );
  duf_config->db_dir = mas_strdup( getenv( "MAS_SHN_PROJECTS_DIR" ) );
  duf_config->db_dir = mas_strcat_x( duf_config->db_dir, "/../duf_db" );
  duf_config->db_name = mas_strdup( "duf-photo.sqlite3" );
  return 0;
}

int
duf_config_delete( void )
{
  mas_free( duf_config->db_dir );
  duf_config->db_dir = NULL;

  mas_free( duf_config->db_name );
  duf_config->db_name = NULL;

  mas_free( duf_config->group );
  duf_config->group = NULL;

  mas_del_argv( duf_config->targc, duf_config->targv, 0 );
  duf_config->targc = 0;
  duf_config->targv = NULL;



  mas_free( duf_config );
  duf_config = NULL;
  return 0;
}

int
duf_config_show( void )
{
  if ( duf_config )
  {
    fprintf( stderr, "db_dir: %s\n", duf_config->db_dir );
  }
  for ( int ia = 0; ia < duf_config->targc; ia++ )
  {
    fprintf( stderr, "targv[%d]: %s\n", ia, duf_config->targv[ia] );
  }
  return 0;
}
