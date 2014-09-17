#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_option_extended.h"
/* ###################################################################### */




const duf_longval_extended_t *
duf_longindex_extended( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  if ( longindex >= 0 && longindex < lo_extended_count )
    extended = &lo_extended[longindex];
  else if ( pr )
    *pr = DUF_ERROR_OPTION;
  return extended;
}

const char *
duf_longindex_extended_name( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex_extended( longindex, pr );
  return extended ? extended->o.name : NULL;
}

duf_option_code_t
duf_longindex_extended_codeval( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex_extended( longindex, pr );
  return extended ? extended->o.val : 0;
}
