/* #undef MAS_TRACING */
#include <string.h>

#include "duf_context.h"

#include "duf_pathinfo_ref.h"

/* ###################################################################### */
#include "duf_levinfo_context.h"
/* ###################################################################### */

#if 0
void
duf_levinfo_clear_context( duf_levinfo_t * pli )
{
  assert( pli );
  duf_clear_context( &pli->context );
}
#endif

#if 0                                                                /* 20160120.184935 : obsolete (see mod_collect_mime_uni.c) */
void
duf_levinfo_set_context_up_destructor( duf_depthinfo_t * pdi, duf_void_voidp_func_t destr )
{
  assert( pdi );
  {
    int d;

# if 0
    d = pdi->pathinfo.depth - 1;
# else
    d = duf_pi_depth( &pdi->pathinfo ) - 1;
# endif
    assert( d >= 0 );
# if 0
    duf_set_context_destructor( &pdi->pathinfo.levinfo[d].context, destr );
# else
    duf_set_context_destructor( duf_pi_context_d( &pdi->pathinfo, d ), destr );
# endif
  }
}
#endif
