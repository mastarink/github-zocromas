#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_option_extended.h"
/* ###################################################################### */



#if 0
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
#endif

const duf_longval_extended_t *
duf_longindex2extended( int longindex, const duf_longval_extended_table_t ** pxtable, int *pr )
{
  const duf_longval_extended_t *extended = NULL;
  int ntable = 0;
  int tbcount = 0;

  for ( const duf_longval_extended_table_t ** xtables = lo_extended_table_multi; tbcount <= longindex && *xtables; xtables++, ntable++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

    for ( const duf_longval_extended_t * xtended = xtable->table; tbcount <= longindex && xtended->o.name; xtended++, tbcount++ )
    {
      DUF_TRACE( options, 5, "@li2ex %d:%d [%s]", ntable, tbcount, xtended->o.name );
      if ( tbcount == longindex )
      {
        extended = xtended;
        if ( pxtable )
          *pxtable = xtable;
        /* break; */
      }
    }
  }

  return extended;
}

int
duf_longindex_extended_count( const duf_longval_extended_table_t ** xtables )
{
  int xcount = 0;
  const duf_longval_extended_table_t *xtable;

  while ( ( xtable = *xtables++ ) )
  {
    const duf_longval_extended_t *xtended = xtable->table;

    while ( xtended->o.name )
    {
      /* if ( xtended->stage.min == 0 && xtended->stage.max == 0 ) */
      xcount++;
      xtended++;
    }
  }
  return xcount;
}
