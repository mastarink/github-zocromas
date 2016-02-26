#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
 
/* ###################################################################### */
#include "mas_expandable.h"                                          /* mas_expandable_string_t; mas_string_expanded ♠ */
/* ###################################################################### */

void
mas_expandable_string_delete( mas_expandable_string_t * cs )
{
  /* DUF_START(  ); */
  mas_free( cs->value );
  cs->value = NULL;
  mas_free( cs->expanded );
  cs->expanded = NULL;
  /* DUF_END(  ); */
}

static char *
_duf_string_options_expand( const char *s, char protected_prefix, int *pexpandable_later, mas_arg_get_cb_arg_t varfunc )
{
  char *xs = NULL;

  if ( s )
  {
    if ( protected_prefix && *s == protected_prefix )
    {
      xs = mas_strdup( s );
      if ( pexpandable_later )
        *pexpandable_later = 1;                                      /* expand later */
    }
    else
    {
      assert( varfunc );
      xs = mas_expand_string_cb_arg( s, varfunc /* mas_string_options_at_string_xsdb_getvar */ , NULL );
      {
        char *xs1;

        xs1 = mas_expand_string( xs );
        mas_free( xs );
        xs = xs1;
      }
    }
  }
  return xs;
}

char *
mas_string_options_expand( const char *s, char protected_prefix, mas_arg_get_cb_arg_t varfunc )
{
  return _duf_string_options_expand( s, protected_prefix, NULL, varfunc );
}

/* expands mas_expandable_string_t
 * */
const char *
mas_string_expanded( mas_expandable_string_t * cs )
{
  const char *p = cs->value;
  char *x = NULL;

  if ( p )
  {
    if ( !cs->protected_prefix && *p == '?' )
      p++;
    assert( cs->varfunc );
    x = mas_string_options_expand( p, cs->protected_prefix, cs->varfunc );
    mas_free( cs->expanded );
    cs->expanded = x;
  }
  return cs->expanded;
}
