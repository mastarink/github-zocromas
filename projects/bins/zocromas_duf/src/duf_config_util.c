#include <string.h>
#include <errno.h>

#include <mastar/tools/mas_utils_path.h>

#include "duf_maintenance.h"

#include "duf_expandable.h"

#include "duf_config.h"
#include "duf_config_trace.h"
#include "duf_config_defs.h"
#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_config_util_z.h"
#include "duf_config_util.h"
/* ###################################################################### */

void *
duf_get_offset( void *ptr, unsigned long off )
{
  assert( ptr );
  return ptr ? ( ( ( char * ) ptr ) + off ) : NULL;
}

void *
duf_get_config_offset( unsigned long off )
{
  return duf_get_offset( duf_config, off );
}

void *
duf_get_config_puz_offset( unsigned long off )
{
  return duf_config ? duf_get_offset( duf_config->vars.puz, off ) : NULL;
}

duf_config_cli_t *
duf_get_config_cli( void )
{
  assert( duf_config );
  return duf_config ? &duf_config->cli : NULL;
}

duf_config_opt_t *
duf_get_config_opt( void )
{
  return duf_config ? &duf_config->opt : NULL;
}

int
duf_output_level( void )
{
  return duf_config ? duf_config->opt.output.level : 0;
}

FILE *
duf_output_file( void )
{
  return duf_output_file_c( duf_config );
}

char
duf_option_delimiter( void )
{
  return duf_config ? duf_config->cli.option_delimiter : ':';
}

char *
duf_path_add_subdir( const char *dir, const char *subdir, int *pr )
{
  int rpr = 0;
  char *path = NULL;

  if ( subdir )
  {
    if ( strchr( subdir, '/' ) )
    {
      DUF_MAKE_ERROR( rpr, DUF_ERROR_MKDIR );
    }
    else
    {
      int ry;

      path = mas_concat_path( dir, subdir );
      {
        struct stat st;

        ry = stat( path, &st );
        if ( ry < 0 )
        {
          if ( errno == ENOENT )
          {
            ry = mkdir( path, S_IRWXU );

            if ( ry < 0 )
            {
              char serr[1024] = "";
              char *s;

              s = strerror_r( errno, serr, sizeof( serr ) );
              DUF_MAKE_ERRORM( rpr, DUF_ERROR_MKDIR, "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path );
              /* DUF_SHOW_ERROR( "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path ); */
            }
          }
        }
        else if ( !S_ISDIR( st.st_mode ) )
        {
          DUF_MAKE_ERROR( rpr, DUF_ERROR_STAT );
        }
      }
    }
  }
  if ( pr )
    *pr = rpr;
  return path;
}

char *
duf_config_db_path_add_subdir( const char *dir, int *pr )
{
  int rpr = 0;
  char *path = NULL;

  if ( DUF_CONFIGGSP( db.subdir ) )
  {
    if ( strchr( DUF_CONFIGGSP( db.subdir ), '/' ) )
    {
      DUF_MAKE_ERROR( rpr, DUF_ERROR_MKDIR );
    }
    else
    {
      int ry;

      path = mas_concat_path( dir, DUF_CONFIGGSP( db.subdir ) );
      {
        struct stat st;

        ry = stat( path, &st );
        if ( ry < 0 )
        {
          if ( errno == ENOENT )
          {
            ry = mkdir( path, S_IRWXU );

            if ( ry < 0 )
            {
              char serr[1024] = "";
              char *s;

              s = strerror_r( errno, serr, sizeof( serr ) );
              DUF_MAKE_ERRORM( rpr, DUF_ERROR_MKDIR, "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path );
              /* DUF_SHOW_ERROR( "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path ); */
            }
          }
        }
        else if ( !S_ISDIR( st.st_mode ) )
        {
          DUF_MAKE_ERROR( rpr, DUF_ERROR_STAT );
        }
      }
    }
  }
  if ( pr )
    *pr = rpr;
  return path;
}
