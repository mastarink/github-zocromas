#include <string.h>
#include <stddef.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_config_trace.h"

#include "duf_option_config.h"  /* duf_cli_options_get_longopts_table */

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
duf_loption_xfind_at_xtable( int longindex, const duf_longval_extended_vtable_t * xvtable, int *ptbcount, unsigned noo )
{
  const duf_longval_extended_t *extended = NULL;

  if ( xvtable )
    extended = duf_loption_xfind_at_xarr( longindex, xvtable->xlist, ptbcount, noo );
  return extended;
}

static const duf_longval_extended_t *
duf_loption_xfind_at_multix( int longindex, const duf_longval_extended_vtable_t * const *xvtables, int *ptbcount, unsigned noo,
                             const duf_longval_extended_vtable_t ** result_pxvtable )
{
  const duf_longval_extended_t *extended = NULL;

  for ( ; *ptbcount <= longindex && *xvtables; xvtables++ )
  {
    const duf_longval_extended_vtable_t *xvtable = *xvtables;

    extended = duf_loption_xfind_at_xtable( longindex, xvtable, ptbcount, noo );
    if ( extended )
    {
      if ( result_pxvtable )
        *result_pxvtable = xvtable;
      break;
    }
  }
  return extended;
}

/* longindex + stdx => extended */
const duf_longval_extended_t *
duf_loption_xfind_at_stdx( int longindex, const duf_longval_extended_vtable_t ** result_pxvtable, unsigned *pnoo )
{
  const duf_longval_extended_t *extended = NULL;
  int tbcount = 0;

#if 0
  for ( const duf_longval_extended_vtable_t * const *xvtables = duf_extended_vtable_multi(  ); tbcount <= longindex && *xvtables; xvtables++ )
  {
    const duf_longval_extended_vtable_t *xvtable = *xvtables;

    extended = duf_loption_xfind_at_xtable( longindex, xvtable, &tbcount, 0 /* noo */  );
    if ( extended )
    {
      if ( result_pxvtable )
        *result_pxvtable = xvtable;
      break;
    }
  }
#else
  extended = duf_loption_xfind_at_multix( longindex, duf_extended_vtable_multi(  ), &tbcount, 0 /* noo */ , result_pxvtable );
#endif
  if ( !extended )
  {
    /* continue with tbcount */
#if 0
    for ( const duf_longval_extended_vtable_t * const *xvtables = duf_extended_vtable_multi(  ); tbcount <= longindex && *xvtables; xvtables++ )
    {
      const duf_longval_extended_vtable_t *xvtable = *xvtables;

      extended = duf_loption_xfind_at_xtable( longindex, xvtable, &tbcount, 1 /* noo */  );
      if ( extended )
      {
        if ( result_pxvtable )
          *result_pxvtable = xvtable;
        if ( pnoo )
          *pnoo = !*pnoo;
        break;
      }
    }
#else
    extended = duf_loption_xfind_at_multix( longindex, duf_extended_vtable_multi(  ), &tbcount, 1 /* noo */ , result_pxvtable );
    if ( extended && pnoo )
      *pnoo = !*pnoo;
#endif
  }

  return extended;
}
