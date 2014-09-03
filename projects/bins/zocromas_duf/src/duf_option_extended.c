#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_option_extended.h"
/* ###################################################################### */



extern const duf_longval_extended_t *lo_extended;
extern unsigned lo_extended_count;



const duf_longval_extended_t *
duf_longindex_extended( int longindex )
{
  const duf_longval_extended_t *extended = NULL;

  extended = &lo_extended[longindex];
  return extended;
}

const char *
duf_longindex_extended_name( int longindex )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex_extended( longindex );
  return extended ? extended->o.name : NULL;
}

duf_option_code_t
duf_longindex_extended_codeval( int longindex )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex_extended( longindex );
  return extended ? extended->o.val : 0;
}


