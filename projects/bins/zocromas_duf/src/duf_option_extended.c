/* #include <stddef.h> */
#include <string.h>

/* ###################################################################### */
#include "duf_option_extended.h"
/* ###################################################################### */

int
duf_longindex_extended_count( const duf_longval_extended_vtable_t * const *xvtables )
{
  int xcount = 0;

#if 0
  const duf_longval_extended_vtable_t *xtable;

  while ( ( xtable = *xvtables++ ) )
  {
    const duf_longval_extended_t *xtended = xtable->xlist;

    while ( xtended->o.name )
    {
      xcount++;
      xtended++;
      if ( xtended->can_no )
        xcount++;
    }
  }
#else
  for ( const duf_longval_extended_vtable_t * const *pxtables = xvtables; *pxtables; pxtables++ )
  {
    const duf_longval_extended_t *xtended = NULL;

    xtended = ( *pxtables )->xlist;
    while ( xtended->o.name )
    {
      xcount++;
      xtended++;
      if ( xtended->can_no )
        xcount++;
    }
  }
#endif
  return xcount;
}
