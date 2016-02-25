/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */

/* ###################################################################### */
#include "duf_expandable.h"                                          /* duf_expandable_string_t; duf_string_expanded ♠ */
/* ###################################################################### */

void
duf_cfg_string_delete( duf_expandable_string_t * cs )
{
  DUF_START(  );
  mas_free( cs->value );
  cs->value = NULL;
  mas_free( cs->expanded );
  cs->expanded = NULL;
  DUF_END(  );
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
      xs = mas_expand_string_cb_arg( s, varfunc /* duf_string_options_at_string_xsdb_getvar */ , NULL );
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
duf_string_options_expand( const char *s, char protected_prefix, mas_arg_get_cb_arg_t varfunc )
{
  return _duf_string_options_expand( s, protected_prefix, NULL, varfunc );
}

/* expands duf_expandable_string_t
 * */
const char *
duf_string_expanded( duf_expandable_string_t * cs )
{
  const char *p = cs->value;
  char *x = NULL;

  if ( p )
  {
    if ( !cs->protected_prefix && *p == '?' )
      p++;
    assert( cs->varfunc );
    x = duf_string_options_expand( p, cs->protected_prefix, cs->varfunc );
    mas_free( cs->expanded );
    cs->expanded = x;
  }
  return cs->expanded;
}
