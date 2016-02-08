#include <string.h>
#include <stddef.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_config_trace.h"

#include "duf_option_config.h" /* duf_cli_options_get_longopts_table */

#include "duf_xtended_table.h"

/* ###################################################################### */
#include "duf_option_lfind.h"
/* ###################################################################### */


const duf_option_t *
duf_loption_find_at_std( int longindex )
{
  const duf_option_t *roption = NULL;

  if ( longindex >= 0 )
    roption = &( duf_cli_options_get_longopts_table(  )[longindex] );
  return roption;
}

/* longindex + extended_arrat => extended */
static const duf_longval_extended_t *
duf_loption_xfind_at_xarr( int longindex, const duf_longval_extended_t * xarr, int *ptbcount, unsigned noo )
{
  const duf_longval_extended_t *extended = NULL;

  for ( const duf_longval_extended_t * xtended = xarr; *ptbcount <= longindex && xtended->o.name; xtended++ )
  {
    DUF_TRACE( options, +500, "li2ex %d [%s]", *ptbcount, xtended->o.name );
    if ( noo )
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
duf_loption_xfind_at_xtable( int longindex, const duf_longval_extended_vtable_t * xtable, int *ptbcount, unsigned noo )
{
  const duf_longval_extended_t *extended = NULL;

  if ( xtable )
    extended = duf_loption_xfind_at_xarr( longindex, xtable->xlist, ptbcount, noo );
  return extended;
}

static const duf_longval_extended_t *
duf_loption_xfind_at_multix( int longindex, const duf_longval_extended_vtable_t ** xvtables, int *ptbcount, unsigned noo,
                             const duf_longval_extended_vtable_t ** result_pxtable )
{
  const duf_longval_extended_t *extended = NULL;

  for ( ; *ptbcount <= longindex && *xvtables; xvtables++ )
  {
    const duf_longval_extended_vtable_t *xtable = *xvtables;

    extended = duf_loption_xfind_at_xtable( longindex, xtable, ptbcount, noo );
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
duf_loption_xfind_at_stdx( int longindex, const duf_longval_extended_vtable_t ** result_pxtable, unsigned *pnoo )
{
  const duf_longval_extended_t *extended = NULL;
  int tbcount = 0;

#if 0
  for ( const duf_longval_extended_vtable_t ** xvtables = duf_extended_vtable_multi(  ); tbcount <= longindex && *xvtables; xvtables++ )
  {
    const duf_longval_extended_vtable_t *xtable = *xvtables;

    extended = duf_loption_xfind_at_xtable( longindex, xtable, &tbcount, 0 /* noo */  );
    if ( extended )
    {
      if ( result_pxtable )
        *result_pxtable = xtable;
      break;
    }
  }
#else
  extended = duf_loption_xfind_at_multix( longindex, duf_extended_vtable_multi(  ), &tbcount, 0 /* noo */ , result_pxtable );
#endif
  if ( !extended )
  {
    /* continue with tbcount */
#if 0
    for ( const duf_longval_extended_vtable_t ** xvtables = duf_extended_vtable_multi(  ); tbcount <= longindex && *xvtables; xvtables++ )
    {
      const duf_longval_extended_vtable_t *xtable = *xvtables;

      extended = duf_loption_xfind_at_xtable( longindex, xtable, &tbcount, 1 /* noo */  );
      if ( extended )
      {
        if ( result_pxtable )
          *result_pxtable = xtable;
        if ( pnoo )
          *pnoo = !*pnoo;
        break;
      }
    }
#else
    extended = duf_loption_xfind_at_multix( longindex, duf_extended_vtable_multi(  ), &tbcount, 1 /* noo */ , result_pxtable );
    if ( extended && pnoo )
      *pnoo = !*pnoo;
#endif
  }

  return extended;
}
