/* #undef MAS_TRACING */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>


#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */

#include "muc_option_lfind.h"
/* ###################################################################### */
#include "muc_option_cfind.h"                                        /* muc_coption_xfind_at_stdx;  ..._coption_*  etc... ♠ */
/* ###################################################################### */

static const muc_option_t *
muc_coption_find_at_arr( muc_config_cli_t * cli, muc_option_gen_code_t codeval, const muc_option_t * arr, int *plongindex, int *pr )
{
  const muc_option_t *roption = NULL;
  int rpr = 0;
  int longindex = 0;

  for ( ; !roption && arr->name; arr++, longindex++ )
  {
    if ( arr )
    {
      MAST_TRACE( findopt, +1, "@li2ex %d [%s] %d:%d", longindex, arr->name, arr->val, codeval );
    /* assert( 0 ); */
      if ( arr->val == codeval )
      {
        roption = arr;
        MAST_TRACE( findopt, +1, "@li2ex FOUND %d [%s]", longindex, arr->name );
#if 0
        ok = 1;
#endif
        break;                                                       /* ? */
      }
    }
  }
  if ( plongindex )
    *plongindex = longindex;
  if ( pr )
    *pr = rpr;
  return roption;
}

static const muc_option_t *
muc_coption_find_at_std( muc_config_cli_t * cli, muc_option_gen_code_t codeval, int *plongindex, int *pr )
{
  return muc_coption_find_at_arr( cli, codeval, muc_cli_options_get_longopts_table( cli ), plongindex, pr );
}

static const muc_option_t *
muc_lcoption_find_at_std( muc_config_cli_t * cli, muc_option_gen_code_t codeval, muc_option_gen_code_t maxcodeval, int *plongindex, int *pr )
{
  const muc_option_t *roption = NULL;

  if ( *plongindex >= 0 && codeval > maxcodeval /* DUF_OPTION_VAL_LONG */  )
    roption = muc_loption_find_at_std( cli, *plongindex );
  else if ( codeval != '?' )
    roption = muc_coption_find_at_std( cli, codeval, plongindex, pr );
  return roption;
}

#if 0
const char *...
_coption_find_name_at_std( ... _option_gen_code_t codeval, int *pr )
{
  const ... _option_t *longoption = NULL;

  longoption = ... _coption_find_at_std( codeval, pr );
  return longoption ? longoption->name : NULL;
}
#endif

const char *
muc_lcoption_find_name_at_std( muc_config_cli_t * cli, muc_option_gen_code_t codeval, muc_option_gen_code_t maxcodeval, int *plongindex, int *pr )
{
  const muc_option_t *longoption = NULL;

  longoption = muc_lcoption_find_at_std( cli, codeval, maxcodeval, plongindex, pr );
  return longoption ? longoption->name : NULL;
}

static const muc_longval_extended_t *
muc_coption_xfind_at_xarr( muc_config_cli_t * cli, muc_option_gen_code_t codeval, const muc_longval_extended_t * xarr, int *pr )
{
  const muc_longval_extended_t *rxtended = NULL;
  int rpr = 0;
  int tbcount = 0;

  for ( ; !rxtended && xarr->o.name; xarr++, tbcount++ )
  {
    MAST_TRACE( findopt, +1, "@li2ex %d [%s] %d:%d", tbcount, xarr->o.name, xarr->o.val, codeval );
  /* assert( 0 ); */
    if ( xarr->o.val == codeval )
    {
      rxtended = xarr;
      MAST_TRACE( findopt, +1, "@li2ex FOUND %d [%s]", tbcount, xarr->o.name );
#if 0
      ok = 1;
#endif
      break;                                                         /* ? */
    }
  }
  if ( pr )
    *pr = rpr;
  return rxtended;
}

static const muc_longval_extended_t *
muc_coption_xfind_at_xtable( muc_config_cli_t * cli, muc_option_gen_code_t codeval, const muc_longval_extended_vtable_t * xvtable,
                             const muc_longval_extended_vtable_t ** result_pxvtable, int *pr )
{
  const muc_longval_extended_t *rxtended = NULL;

  rxtended = muc_coption_xfind_at_xarr( cli, codeval, xvtable->xlist, pr );
  if ( result_pxvtable )
    *result_pxvtable = xvtable;
  return rxtended;
}

/* codeval => extended, by standard multi-table */
const muc_longval_extended_t *
muc_coption_xfind_at_stdx( muc_config_cli_t * cli, muc_option_gen_code_t codeval, const muc_longval_extended_vtable_t ** result_pxvtable, int *pr )
{
  const muc_longval_extended_t *rxtended = NULL;

#if 0
  int ok;
#endif
  int rpr = 0;
  int ntable = 0;

  if ( codeval && codeval != '?' )
  {
    for ( muc_longval_extended_vtable_t ** multix = muc_cli_options_xvtable_multi( cli ); !rxtended && *multix; multix++, ntable++ )
    {
      const muc_longval_extended_vtable_t *xvtable = *multix;

      rxtended = muc_coption_xfind_at_xtable( cli, codeval, xvtable, result_pxvtable, pr );
      MAST_TRACE( findopt, +1, "@li2ex ? %d [%s]", ntable, rxtended ? rxtended->o.name : NULL );
    }
  }
#if 0
  if ( !ok )
    MASE_MAKE_ERROR( rpr, DUF_ERROR_OPTION_NOT_FOUND );
#endif
  if ( pr )
    *pr = rpr;
  return rxtended;
}
