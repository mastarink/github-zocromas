/* #include <stdlib.h> */
#include <dirent.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "mas_runonce_config_types.h"

#include "mas_runonce_config_group.h"



#include "mas_runonce_config.h"



config_t configuration = {.directory = NULL };

static int
cmpcfg( const void *a, const void *b )
{
  config_group_t *acfg, *bcfg;

  acfg = ( config_group_t * ) a;
  bcfg = ( config_group_t * ) b;

  return acfg->pos - bcfg->pos;
}

void
runonce_config_delete( void )
{
  for ( int id = 0; id < configuration.numgroups; id++ )
  {
    runonce_config_group_delete( &configuration.groups[id] );
  }
  mas_free( configuration.groups );
  configuration.groups = NULL;
  configuration.numgroups = 0;

  mas_free( configuration.directory );
  configuration.directory = NULL;

  mas_free( configuration.group_pattern );
  configuration.group_pattern = NULL;
}

int
runonce_config_create( void )
{
  const char *dir;

  dir = getenv( "MSHDIRS_DIR" );
  /* printf( "dir=%s\n", dir ); */
  dir = getenv( "MSHDIRS_CONF_DIR" );
  /* printf( "dir=%s\n", dir ); */
  if ( !dir || !*dir )
  {
    printf( "(%s) MSHDIRS_CONF_DIR not defined\n", __func__ );
  }
  else
  {
    DIR *d;

    configuration.directory = mas_strdup( dir );
    configuration.directory = mas_strcat_x( configuration.directory, "/runonce" );
    d = opendir( configuration.directory );
    if ( d )
    {
      /* printf( "dir %s opened\n", configuration.directory ); */
      {
        struct dirent de, *pde = NULL;

        while ( 0 == readdir_r( d, &de, &pde ) && pde )
        {
          size_t ln;

          ln = strlen( pde->d_name );
          if ( pde->d_type == DT_REG && 0 == strncmp( pde->d_name + ln - 5, ".conf", 5 ) )
          {
            runonce_config_group_create( pde->d_name, ln - 5 );
          }
        }
      }
      closedir( d );
      /* printf( "dir %s closed\n", configuration.directory ); */
    }
    else
    {
      printf( "(%s) can't open %s\n", __func__, configuration.directory );
    }
  }
  qsort( configuration.groups, configuration.numgroups, sizeof( config_group_t ), cmpcfg );
  return 0;
}

int
runonce_config_load( void )
{
  for ( int id = 0; id < configuration.numgroups; id++ )
  {
    if ( configuration.flags.verbose > 4 )
      printf( "+ load config group id=%d\n", id );
    runonce_config_group_load( configuration.directory, &configuration.groups[id] );
  }
  return 0;
}
