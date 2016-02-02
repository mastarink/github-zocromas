#include <string.h>
#include <stddef.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_config_trace.h"

#include "duf_xtended_table.h"

/* ###################################################################### */
#include "duf_option_lfind.h"
/* ###################################################################### */

/* longindex + extended_arrat => extended */
static const duf_longval_extended_t *
duf_loption_xfind_at_xarr( int longindex, const duf_longval_extended_t * xarr, int *ptbcount, int no )
{
  const duf_longval_extended_t *extended = NULL;

  for ( const duf_longval_extended_t * xtended = xarr; *ptbcount <= longindex && xtended->o.name; xtended++ )
  {
    DUF_TRACE( options, +500, "li2ex %d [%s]", *ptbcount, xtended->o.name );
    if ( no )
    {
      if ( xtended->can_no )
      {
        if ( *ptbcount == longindex )
        {
          extended = xtended;
          break;
        }
        ( *ptbcount )++;
      }
    }
    else
    {
      if ( *ptbcount == longindex )
      {
        extended = xtended;
        /* break; */
      }
      ( *ptbcount )++;
    }
  }

  return extended;
}

/* longindex + extended_table => extended */
static const duf_longval_extended_t *
duf_loption_xfind_at_xtable( int longindex, const duf_longval_extended_table_t * xtable, int *ptbcount, int no )
{
  const duf_longval_extended_t *extended = NULL;

  if ( xtable )
    extended = duf_loption_xfind_at_xarr( longindex, xtable->table, ptbcount, no );
  return extended;
}

const duf_longval_extended_t *
duf_loption_xfind_at_multix( int longindex, const duf_longval_extended_table_t ** xtables, int *ptbcount, int no,
                             const duf_longval_extended_table_t ** result_pxtable )
{
  const duf_longval_extended_t *extended = NULL;

  for ( ; *ptbcount <= longindex && *xtables; xtables++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

    extended = duf_loption_xfind_at_xtable( longindex, xtable, ptbcount, no );
    if ( extended )
    {
      if ( result_pxtable )
        *result_pxtable = xtable;
      break;
    }
  }
  return extended;
}

/* longindex + stdx => extended */
const duf_longval_extended_t *
duf_loption_xfind_at_stdx( int longindex, const duf_longval_extended_table_t ** result_pxtable, int *pno )
{
  const duf_longval_extended_t *extended = NULL;
  int tbcount = 0;

#if 0
  for ( const duf_longval_extended_table_t ** xtables = duf_extended_table_multi(  ); tbcount <= longindex && *xtables; xtables++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

    extended = duf_loption_xfind_at_xtable( longindex, xtable, &tbcount, 0 /* no */  );
    if ( extended )
    {
      if ( result_pxtable )
        *result_pxtable = xtable;
      break;
    }
  }
#else
  extended = duf_loption_xfind_at_multix( longindex, duf_extended_table_multi(  ), &tbcount, 0 /* no */ , result_pxtable );
#endif
  if ( !extended )
  {
    /* continue with tbcount */
#if 0
    for ( const duf_longval_extended_table_t ** xtables = duf_extended_table_multi(  ); tbcount <= longindex && *xtables; xtables++ )
    {
      const duf_longval_extended_table_t *xtable = *xtables;

      extended = duf_loption_xfind_at_xtable( longindex, xtable, &tbcount, 1 /* no */  );
      if ( extended )
      {
        if ( result_pxtable )
          *result_pxtable = xtable;
        if ( pno )
          *pno = !*pno;
        break;
      }
    }
#else
    extended = duf_loption_xfind_at_multix( longindex, duf_extended_table_multi(  ), &tbcount, 1 /* no */ , result_pxtable );
    if ( extended && pno )
      *pno = !*pno;
#endif
  }

  return extended;
}
