/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>

/* #include "duf_config_util.h" */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */

#include "duf_xtended_table.h"

/* ###################################################################### */
#include "duf_option_lfind.h"
/* ###################################################################### */

const duf_option_t *
duf_loption_find_at_std( duf_config_cli_t * cli, int longindex )
{
  const duf_option_t *roption = NULL;

  if ( longindex >= 0 )
    roption = &( duf_cli_options_get_longopts_table( cli )[longindex] );
  return roption;
}

/* longindex + extended_arrat => extended */
static const duf_longval_extended_t *
duf_loption_xfind_at_xarr( duf_config_cli_t * cli, int longindex, const duf_longval_extended_t * xarr, int *ptbcount, unsigned noo )
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
duf_loption_xfind_at_xtable( duf_config_cli_t * cli, int longindex, const duf_longval_extended_vtable_t * xvtable, int *ptbcount, unsigned noo )
{
  const duf_longval_extended_t *extended = NULL;

  if ( xvtable )
    extended = duf_loption_xfind_at_xarr( cli, longindex, xvtable->xlist, ptbcount, noo );
  return extended;
}

static const duf_longval_extended_t *
duf_loption_xfind_at_multix( duf_config_cli_t * cli, int longindex, duf_longval_extended_vtable_t ** xvtables, int *ptbcount, unsigned noo,
                             const duf_longval_extended_vtable_t ** result_pxvtable )
{
  const duf_longval_extended_t *extended = NULL;

  for ( ; *ptbcount <= longindex && *xvtables; xvtables++ )
  {
    const duf_longval_extended_vtable_t *xvtable = *xvtables;

    extended = duf_loption_xfind_at_xtable( cli, longindex, xvtable, ptbcount, noo );
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
duf_loption_xfind_at_stdx( duf_config_cli_t * cli, int longindex, const duf_longval_extended_vtable_t ** result_pxvtable, unsigned *pnoo )
{
  const duf_longval_extended_t *extended = NULL;
  int tbcount = 0;

  extended = duf_loption_xfind_at_multix( cli, longindex, duf_cli_options_xvtable_multi( cli ), &tbcount, 0 /* noo */ , result_pxvtable );
  if ( !extended )
  {
  /* continue with tbcount */
    extended = duf_loption_xfind_at_multix( cli, longindex, duf_cli_options_xvtable_multi( cli ), &tbcount, 1 /* noo */ , result_pxvtable );
    if ( extended && pnoo )
      *pnoo = !*pnoo;
  }

  return extended;
}
