#include <string.h>
#include <errno.h>

#include <mastar/tools/mas_tools.h>                                  /* mas_tstrflocaltime */
#include <mastar/tools/mas_utils_path.h>                             /* mas_concat_path */

#include "duf_maintenance.h"

#include "duf_expandable.h"

#include "duf_config.h"
#include "duf_config_defs.h"
#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_config_util_z.h"
#include "duf_config_util.h"
/* ###################################################################### */

static void *
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

void *
duf_get_pointer_ufilter( void )
{
  return duf_config ? duf_config->vars.puz : NULL;
}

duf_config_cli_t *
duf_get_config_cli( void )
{
  assert( duf_config );
#if 0
  return duf_config ? &duf_config->cli : NULL;
#else
  return duf_config ? duf_config->pcli : NULL;
#endif
}

void *
duf_get_pointer_config_cli( void )
{
  return duf_get_config_cli(  );
}

void *
duf_get_pointer_config_trace( void )
{
  return duf_get_config_opt(  )->ptracecfg;
}
void *
duf_get_pointer_config_trace_enum( void )
{
  return duf_get_config_opt(  )->ptracecfg->class_levels;
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

#if 0
char
duf_option_delimiter( void )
{
  return duf_config ? duf_config->cli.option_delimiter : ':';
}
#endif

#if 0
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
#endif

char *
duf_config_db_path_add_subdir( const char *dir, int *pr )
{
  int rpr = 0;
  char *path = NULL;

  if ( DUF_CONFIGGSP( db.subdir_x ) )
  {
    if ( strchr( DUF_CONFIGGSP( db.subdir_x ), '/' ) )
    {
      DUF_MAKE_ERROR( rpr, DUF_ERROR_MKDIR );
    }
    else
    {
      int ry;

      path = mas_concat_path( dir, DUF_CONFIGGSP( db.subdir_x ) );
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

const char *
duf_string_options_at_string_xsdb_getvar( const char *name, const char *arg DUF_UNUSED )
{
  static char buf[256];
  size_t len;
  size_t llen;
  const char *label = "TIME(";
  const char *pbuf = buf;

  llen = strlen( label );
  *buf = 0;
  len = strlen( name );
  if ( len > llen && 0 == strncmp( name, "TIME(", llen ) && name[len - 1] == ')' )
  {
  /* strftime */
    char *fmt;

    fmt = mas_strndup( name + llen, len - llen - 1 );
    mas_tstrflocaltime( buf, sizeof( buf ), fmt, time( NULL ) );
    mas_free( fmt );
    pbuf = buf;
  }
  else if ( *name == '+' )
  {
    if ( 0 == strcmp( name + 1, "db_name" ) || 0 == strcmp( name + 1, "dbname" ) || 0 == strcmp( name + 1, "db-name" ) )
      pbuf = DUF_CONFIGGSP( db.main.name_x );
  }
/* T( "@@@@@@var %s => '%s'", name, pbuf ); */
  return pbuf;
}

int
duf_output_force_color( void )
{
  return duf_output_force_color_c( duf_config );
}

int
duf_output_nocolor( void )
{
  return duf_output_nocolor_c( duf_config );
}

int
duf_output_progress( void )
{
  return duf_output_progress_c( duf_config );
}

int
duf_output_use_binformat( void )
{
  return duf_output_use_binformat_c( duf_config );
}
