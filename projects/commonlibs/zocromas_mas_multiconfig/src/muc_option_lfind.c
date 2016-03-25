/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */

#include "muc_config_cli_structs.h"
/* ###################################################################### */
#include "muc_option_lfind.h"
/* ###################################################################### */

const muc_option_t *
muc_loption_find_at_std( muc_config_cli_t * cli, int longindex )
{
  const muc_option_t *roption = NULL;

  if ( longindex >= 0 )
    roption = &( muc_cli_options_get_longopts_table( cli )[longindex] );
  return roption;
}

/* longindex + extended_arrat => extended */
static const muc_longval_extended_t *
muc_loption_xfind_at_xarr( muc_config_cli_t * cli, int longindex, const muc_longval_extended_t * xarr, int *ptbcount, unsigned noo )
{
  const muc_longval_extended_t *extended = NULL;

  for ( const muc_longval_extended_t * xtended = xarr; *ptbcount <= longindex && xtended->o.name; xtended++ )
  {
    MAST_TRACE( options, +500, "li2ex %d [%s]", *ptbcount, xtended->o.name );
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
static const muc_longval_extended_t *
muc_loption_xfind_at_xtable( muc_config_cli_t * cli, int longindex, const muc_longval_extended_vtable_t * xvtable, int *ptbcount, unsigned noo )
{
  const muc_longval_extended_t *extended = NULL;

  if ( xvtable )
    extended = muc_loption_xfind_at_xarr( cli, longindex, xvtable->xlist, ptbcount, noo );
  return extended;
}

static const muc_longval_extended_t *
muc_loption_xfind_at_multix( muc_config_cli_t * cli, int longindex, muc_longval_extended_vtable_t ** xvtables, int *ptbcount, unsigned noo,
                             const muc_longval_extended_vtable_t ** result_pxvtable )
{
  const muc_longval_extended_t *extended = NULL;

  for ( ; *ptbcount <= longindex && *xvtables; xvtables++ )
  {
    const muc_longval_extended_vtable_t *xvtable = *xvtables;

    extended = muc_loption_xfind_at_xtable( cli, longindex, xvtable, ptbcount, noo );
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
const muc_longval_extended_t *
muc_loption_xfind_at_stdx( muc_config_cli_t * cli, int longindex, const muc_longval_extended_vtable_t ** result_pxvtable, unsigned *pnoo )
{
  const muc_longval_extended_t *extended = NULL;
  int tbcount = 0;

  extended = muc_loption_xfind_at_multix( cli, longindex, muc_cli_options_xvtable_multi( cli ), &tbcount, 0 /* noo */ , result_pxvtable );
  if ( !extended )
  {
  /* continue with tbcount */
    extended = muc_loption_xfind_at_multix( cli, longindex, muc_cli_options_xvtable_multi( cli ), &tbcount, 1 /* noo */ , result_pxvtable );
    if ( extended && pnoo )
      *pnoo = !*pnoo;
  }

  return extended;
}
