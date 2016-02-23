/* #undef MAS_TRACING */
#include <string.h>

/* #include "duf_maintenance_z.h" */
#include "duf_maintenance_tracen.h"
/* #include "duf_maintenance_errors.h" */

/* #include "duf_config_util.h" */

#include "duf_option_config.h"                                       /* duf_cli_options_get_longopts_table */

#include "duf_xtended_table.h"

/* ###################################################################### */
#include "duf_option_lfind.h"
/* ###################################################################### */

const duf_option_t *
duf_loption_find_at_std( duf_config_cli_t * cli,int longindex )
{
  const duf_option_t *roption = NULL;

  if ( longindex >= 0 )
    roption = &( duf_cli_options_get_longopts_table( cli )[longindex] );
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
duf_loption_xfind_at_multix( int longindex, duf_longval_extended_vtable_t ** xvtables, int *ptbcount, unsigned noo,
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
duf_loption_xfind_at_stdx( duf_config_cli_t * cli,int longindex, const duf_longval_extended_vtable_t ** result_pxvtable, unsigned *pnoo )
{
  const duf_longval_extended_t *extended = NULL;
  int tbcount = 0;

#if 0
  for ( const duf_longval_extended_vtable_t * const *xvtables = duf_cli_options_xvtable_multi( cli ); tbcount <= longindex && *xvtables; xvtables++ )
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
  extended = duf_loption_xfind_at_multix( longindex, duf_cli_options_xvtable_multi( cli ), &tbcount, 0 /* noo */ , result_pxvtable );
#endif
  if ( !extended )
  {
  /* continue with tbcount */
#if 0
    for ( const duf_longval_extended_vtable_t * const *xvtables = duf_cli_options_xvtable_multi( cli ); tbcount <= longindex && *xvtables;
          xvtables++ )
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
    extended = duf_loption_xfind_at_multix( longindex, duf_cli_options_xvtable_multi( cli ), &tbcount, 1 /* noo */ , result_pxvtable );
    if ( extended && pnoo )
      *pnoo = !*pnoo;
#endif
  }

  return extended;
}
