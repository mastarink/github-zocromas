/* #undef MAS_TRACING */
#include <stdio.h>
#include <string.h>

/* ###################################################################### */
#include "duf_service.h"
/* ###################################################################### */

#if 0
int
duf_check_field( const char *name, int have )
{
  if ( !have )
  {
    _DUF_SHOW_ERROR( "No such field: %s", name );
  /* assert(have); */
    return DUF_ERROR_NO_FIELD;
  }
  return 0;
}
#endif
typedef struct
{
  const char *name;
  duf_anyhook_t fun;
} duf_fundesc_t;

#define DUF_FUN(name) { #name, (duf_anyhook_t) name}
const char *
duf_dbg_funname( duf_anyhook_t p )
{
  static char buf[512];
  int found = 0;

  static duf_fundesc_t table[] = {
  /* DUF_FUN( duf_str_cb2_uni_scan_dir ), */
  /* DUF_FUN( duf_str_cb1_leaf_scan ), */
#ifdef DUF_COMPILE_EXPIRED
    DUF_FUN( duf_sel_cb_name_parid ),
#endif
  /* DUF_FUN( duf_sel_cb_items ), */
  /* DUF_FUN( duf_sel_cb_node ), */
  /* DUF_FUN( duf_sel_cb_leaf ), */
  /* DUF_FUN( duf_sel_cb_levinfo ), */
    {NULL},
  };
  for ( unsigned i = 0; i < sizeof( table ) / sizeof( table[0] ); i++ )
  {
    if ( p && p == table[i].fun )
    {
      snprintf( buf, sizeof( buf ), "%p:%s()", ( void * ) ( unsigned long long ) p, table[i].name );
      found = 1;
    }
  }
  if ( !found )
  {
    snprintf( buf, sizeof( buf ), "[%p]", ( void * ) ( unsigned long long ) p );
  }

  return buf;
}
