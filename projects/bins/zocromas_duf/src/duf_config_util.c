/* #include <stdarg.h> */
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_dbg.h"
#include "duf_pdi_credel.h"

#include "duf_ufilter.h"

#include "duf_option_extended.h"
#include "duf_option_names.h"

#include "duf_utils_path.h"
#include "duf_config_ref.h"
/* ###################################################################### */
#include "duf_config_util.h"
/* ###################################################################### */


static const char *
duf_string_options_at_string_xsdb_getvar( const char *name, const char *arg DUF_UNUSED )
{
  static char buf[256];
  size_t len;
  size_t llen;
  const char *label = "TIME(";
  char *pbuf = buf;

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
      pbuf = DUF_CONFIGGSP( db.main.name );
  }
  /* T( "@@@@@@var %s => '%s'", name, pbuf ); */
  return pbuf;
}

char *
_duf_string_options_expand( const char *s, char protected_prefix, int *pexpandable_later )
{
  char *xs = NULL;

  DUF_TRACE( explain, 0, "s: \"%s\"", s );
  if ( s )
  {
    if ( protected_prefix && *s == protected_prefix )
    {
      xs = mas_strdup( s );
      if ( pexpandable_later )
        *pexpandable_later = 1; /* expand later */
    }
    else
    {
      xs = mas_expand_string_cb_arg( s, duf_string_options_at_string_xsdb_getvar, NULL );
      {
        char *xs1;

        xs1 = mas_expand_string( xs );
        mas_free( xs );
        xs = xs1;
      }
    }
  }
  DUF_TRACE( explain, 0, "xs: \"%s\"", xs );
  return xs;
}

char *
duf_string_options_expand( const char *s, char protected_prefix )
{
  return _duf_string_options_expand( s, protected_prefix, NULL );
}

char *
duf_config_string_expanded( duf_config_string_t * cs )
{
  char *p = cs->value;
  char *x = NULL;

  if ( p )
  {
    if ( *p == '?' )
      p++;
    x = duf_string_options_expand( p, 0 );
    mas_free( cs->expanded );
    cs->expanded = x;
  }
  return cs->expanded;
}

char *
duf_config_db_path_add_subdir( const char *dir, int *pr )
{
  int rpr = 0;
  char *path = NULL;

  path = mas_strdup( dir );
  if ( DUF_CONFIGGSP( db.subdir ) )
  {
    if ( strchr( DUF_CONFIGGSP( db.subdir ), '/' ) )
    {
      DUF_MAKE_ERROR( rpr, DUF_ERROR_MKDIR );
    }
    else
    {
      int ry;

      path = mas_strcat_x( path, DUF_CONFIGGSP( db.subdir ) );
      path = duf_normalize_path( path );
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
              DUF_MAKE_ERROR( rpr, DUF_ERROR_MKDIR );
              DUF_SHOW_ERROR( "(ry:%d) errno:%d mkdir :%s; path:'%s'", ry, errno, s ? s : serr, path );
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
