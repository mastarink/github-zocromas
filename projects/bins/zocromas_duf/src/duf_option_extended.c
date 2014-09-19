#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_option_extended.h"
/* ###################################################################### */




const char *
duf_longindex_extended_name( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex2extended( longindex, pr );
  return extended ? extended->o.name : NULL;
}

duf_option_code_t
duf_longindex_extended_codeval( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex2extended( longindex, pr );
  return extended ? extended->o.val : 0;
}

const duf_longval_extended_t *
duf_longindex2extended( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;
  int ntable = 0;
  int tbcount = 0;

  for ( const duf_longval_extended_t ** xtables = lo_extended_multi; tbcount <= longindex && *xtables; xtables++, ntable++ )
  {
    for ( const duf_longval_extended_t * xtable = *xtables; tbcount <= longindex && xtable->o.name; xtable++, tbcount++ )
    {
      DUF_TRACE( options, 5, "@li2ex %d:%d [%s]", ntable, tbcount, xtable->o.name );
      if ( tbcount == longindex )
        extended = xtable;
    }
  }

  return extended;
}

int
duf_longindex_extended_count( const duf_longval_extended_t ** xtables )
{
  int tbcount = 0;
  const duf_longval_extended_t *xtable;

  while ( ( xtable = *xtables++ ) )
  {
    while ( xtable->o.name )
    {
      tbcount++;
      xtable++;
    }
  }
  return tbcount;
}
