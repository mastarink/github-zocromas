#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "mas_runonce_config_types.h"

#include "mas_runonce_config_section.h"
#include "mas_runonce_config_group.h"


static void
runonce_config_group_sections_delete( config_group_t * group )
{
  for ( int isect = 0; isect < group->num_sections; isect++ )
  {
    runonce_config_section_delete( &group->sections[isect] );
  }
  mas_free( group->sections );
  group->sections = NULL;
}

config_group_t *
runonce_config_group_create( const char *name, size_t namelen )
{
  int id = configuration.numgroups;

  /* printf( "%s\n", name ); */
  if ( configuration.groups )
  {
    configuration.groups = mas_realloc( configuration.groups, ++configuration.numgroups * sizeof( config_group_t ) );
  }
  else
  {
    configuration.groups = mas_malloc( ++configuration.numgroups * sizeof( config_group_t ) );
  }
  memset( configuration.groups + id, 0, sizeof( config_group_t ) );
  configuration.groups[id].name = mas_strndup( name, namelen ? namelen : strlen( name ) );
  configuration.groups[id].filename = mas_strdup( name );
  sscanf( configuration.groups[id].name, "%u", ( unsigned * ) &configuration.groups[id].pos );
  return &configuration.groups[id];
}

void
runonce_config_group_delete( config_group_t * group )
{
  runonce_config_group_sections_delete( group );

  mas_free( group->name );
  group->name = NULL;

  mas_free( group->filename );
  group->filename = NULL;
}

static int
runonce_config_group_load_stream( FILE * stream, config_group_t * group )
{
  int nl = 0;
  size_t l;
  char *string;
  char buf[512];
  config_section_t *section = NULL;

  for ( nl = 0;
        stream && !feof( stream )
        && ( string = ( char * ) ( mas_skip_space_nz( mas_chomp( fgets( buf, sizeof( buf ), stream ) ) ) ) ); nl++ )
  {
    if ( ( l = strlen( string ) ) && *string != '#' )
    {
      if ( string[0] == '[' && string[l - 1] == ']' )
      {
        string[0] = string[l - 1] = 0;
        if ( section )
          runonce_config_section_fill( group, section );
        section = runonce_config_section_create( ++string, group );
        /* if ( section && !section->values )                                */
        /* {                                                                 */
        /*   section->values = mas_malloc( RUNONCE_MAX * sizeof( char * ) ); */
        /*   memset( section->values, 0, RUNONCE_MAX * sizeof( char * ) );   */
        /* }                                                                 */
        continue;
      }
      else if ( section )
      {
        runonce_config_section_item_create( section, string, nl );

        /* printf( "section %d\n", section->id ); */
      }
    }
    /* printf( "----------------- %d. %s\n", nl, string ); */
  }
  if ( section )
    runonce_config_section_fill( group, section );
  return 0;
}

int
runonce_config_group_load( const char *dir, config_group_t * group )
{
  char *fpath;
  size_t ln;

  ln = strlen( group->name );
  if ( ln > 5 /* && 0 == strcmp( group->name + ln - 5, ".conf" ) */  )
  {
    FILE *f;

    fpath = mas_strdup( dir );
    fpath = mas_strcat_x( fpath, "/" );
    fpath = mas_strcat_x( fpath, group->filename );
    if ( ( f = fopen( fpath, "r" ) ) )
    {
      /* printf( "opened %s\n", fpath ); */
      runonce_config_group_load_stream( f, group );
      fclose( f );
    }
    else
    {
      printf( "can't open %s\n", fpath );
    }
    mas_free( fpath );
  }
  return 0;
}
