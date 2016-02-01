#include <string.h>
#include <time.h>

#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_trace.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"

/* ###################################################################### */
#include "duf_expandable.h"
/* ###################################################################### */

void
duf_cfg_string_delete( duf_expandable_string_t * cs )
{
  DEBUG_START(  );
  mas_free( cs->value );
  cs->value = NULL;
  mas_free( cs->expanded );
  cs->expanded = NULL;
  DEBUG_END(  );
}

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
    if ( duf_config && ( 0 == strcmp( name + 1, "db_name" ) || 0 == strcmp( name + 1, "dbname" ) || 0 == strcmp( name + 1, "db-name" ) ) )
      pbuf = DUF_CONFIGGSP( db.main.name );
  }
  /* T( "@@@@@@var %s => '%s'", name, pbuf ); */
  return pbuf;
}

static char *
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

/* expands duf_expandable_string_t
 * */
char *
duf_config_string_expanded( duf_expandable_string_t * cs )
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
