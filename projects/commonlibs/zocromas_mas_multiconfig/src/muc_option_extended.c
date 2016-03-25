/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli)
/* #include "muc_tracen_defs_preset.h" */

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/trace/mas_trace.h> */

#include "muc_config_cli_structs.h"
/* ###################################################################### */
#include "muc_option_extended.h"                                     /* muc_longindex_extended_count etc. ♠ */
/* ###################################################################### */

int
muc_longindex_extended_count( muc_longval_extended_vtable_t ** xvtables )
{
  int xcount = 0;

#if 0
  const muc_longval_extended_vtable_t *xtable;

  while ( ( xtable = *xvtables++ ) )
  {
    const muc_longval_extended_t *xtended = xtable->xlist;

    while ( xtended->o.name )
    {
      xcount++;
      xtended++;
      if ( xtended->can_no )
        xcount++;
    }
  }
#else
  for ( muc_longval_extended_vtable_t ** pxtables = xvtables; *pxtables; pxtables++ )
  {
    const muc_longval_extended_t *xtended = NULL;

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
