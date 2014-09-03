#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_option_extended.h"

/* ###################################################################### */
#include "duf_option_find.h"
/* ###################################################################### */



extern const duf_longval_extended_t *lo_extended;
extern unsigned lo_extended_count;

int
duf_find_long( duf_option_code_t codeval, int *pr )
{
  int r = DUF_ERROR_OPTION;
  int longindex = 0;

  assert( lo_extended_count );
  DUF_TRACE( explain, 2, "to find codeval : %d (%c)", codeval, codeval > ' ' && codeval <= 'z' ? codeval : '?' );
  for ( int ilong = 0; codeval && ilong < lo_extended_count; ilong++ )
  {
    if ( lo_extended[ilong].o.val == codeval )
    {
      longindex = ilong;
      DUF_TRACE( explain, 2, "%4d. find codeval codeval: %d ? %d", ilong, codeval, lo_extended[ilong].o.val );
      r = 0;
      break;
    }
  }
  if ( pr )
    *pr = r;
  return longindex;
}
const duf_longval_extended_t *
duf_find_extended( duf_option_code_t codeval, int *pr )
{
  int ix;
  const duf_longval_extended_t *extended = NULL;

  ix = duf_find_long( codeval, pr );
  extended = duf_longindex_extended( ix );

  return extended;
}


