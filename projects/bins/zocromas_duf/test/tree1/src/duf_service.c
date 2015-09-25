#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_service.h"
/* ###################################################################### */

int
duf_check_field( const char *name, int have )
{
  if ( !have )
  {
    DUF_SHOW_ERROR( "No such field: %s", name );
    /* assert(have); */
    return DUF_ERROR_NO_FIELD;
  }
  return 0;
}

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
  };
  for ( int i = 0; i < sizeof( table ) / sizeof( table[9] ); i++ )
  {
    if ( p == table[i].fun )
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
