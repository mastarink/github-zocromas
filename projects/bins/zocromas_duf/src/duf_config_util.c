#include <assert.h>
#include <string.h>
#include <errno.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_time.h>                                   /* mas_xlocaltime; mas_double_time; mas_xgmtime; mas_xvstrftime; mas_xvstrftime_time; mas_tstrflocaltime; mas_strtime2long; mas_strflocaltime; mas_strfgmtime; etc. ♣ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ♣ */

#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

/* #include "duf_expandable.h"                                          (* duf_expandable_string_t; duf_string_expanded ♠ *) */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */
#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
/* ###################################################################### */

#if 0
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
#endif

duf_config_cli_t *
duf_get_config_cli( void )
{
  assert( duf_config );
#if 0
  return duf_config ? &duf_config->cli : NULL;
#else
  return duf_config ? ( duf_config_cli_t * ) duf_config->pcli : NULL;
#endif
}

duf_config_opt_t *
duf_get_config_opt( void )
{
  return duf_config ? &duf_config->opt : NULL;
}

duf_ufilter_t *
duf_get_config_ufilter( void )
{
  return duf_config ? duf_config->vars.puz : NULL;
}

#ifdef MAS_TRACING
mas_config_trace_t *
duf_get_trace_config( void )
{
  assert( duf_config );
  return duf_config->opt.ptracecfg;
}
#endif

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
duf_string_options_at_string_xsdb_getvar( const char *name, const char *arg MAS_UNUSED )
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
