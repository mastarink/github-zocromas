/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include "duf_tracen_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */

#include "duf_option_lfind.h"
/* ###################################################################### */
#include "duf_option_cfind.h"                                        /* duf_coption_xfind_at_stdx;  ..._coption_*  etc... ♠ */
/* ###################################################################### */

static const duf_option_t *
duf_coption_find_at_arr( duf_config_cli_t * cli, duf_option_gen_code_t codeval, const duf_option_t * arr, int *plongindex, int *pr )
{
  const duf_option_t *roption = NULL;
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

static const duf_option_t *
duf_coption_find_at_std( duf_config_cli_t * cli, duf_option_gen_code_t codeval, int *plongindex, int *pr )
{
  return duf_coption_find_at_arr( cli, codeval, duf_cli_options_get_longopts_table( cli ), plongindex, pr );
}

static const duf_option_t *
duf_lcoption_find_at_std( duf_config_cli_t * cli, duf_option_gen_code_t codeval, duf_option_gen_code_t maxcodeval, int *plongindex, int *pr )
{
  const duf_option_t *roption = NULL;

  if ( *plongindex >= 0 && codeval > maxcodeval /* DUF_OPTION_VAL_LONG */  )
    roption = duf_loption_find_at_std( cli, *plongindex );
  else if ( codeval != '?' )
    roption = duf_coption_find_at_std( cli, codeval, plongindex, pr );
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
duf_lcoption_find_name_at_std( duf_config_cli_t * cli, duf_option_gen_code_t codeval, duf_option_gen_code_t maxcodeval, int *plongindex, int *pr )
{
  const duf_option_t *longoption = NULL;

  longoption = duf_lcoption_find_at_std( cli, codeval, maxcodeval, plongindex, pr );
  return longoption ? longoption->name : NULL;
}

static const duf_longval_extended_t *
duf_coption_xfind_at_xarr( duf_config_cli_t * cli, duf_option_gen_code_t codeval, const duf_longval_extended_t * xarr, int *pr )
{
  const duf_longval_extended_t *rxtended = NULL;
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

static const duf_longval_extended_t *
duf_coption_xfind_at_xtable( duf_config_cli_t * cli, duf_option_gen_code_t codeval, const duf_longval_extended_vtable_t * xvtable,
                             const duf_longval_extended_vtable_t ** result_pxvtable, int *pr )
{
  const duf_longval_extended_t *rxtended = NULL;

  rxtended = duf_coption_xfind_at_xarr( cli, codeval, xvtable->xlist, pr );
  if ( result_pxvtable )
    *result_pxvtable = xvtable;
  return rxtended;
}

/* codeval => extended, by standard multi-table */
const duf_longval_extended_t *
duf_coption_xfind_at_stdx( duf_config_cli_t * cli, duf_option_gen_code_t codeval, const duf_longval_extended_vtable_t ** result_pxvtable, int *pr )
{
  const duf_longval_extended_t *rxtended = NULL;

#if 0
  int ok;
#endif
  int rpr = 0;
  int ntable = 0;

  if ( codeval && codeval != '?' )
  {
    for ( duf_longval_extended_vtable_t ** multix = duf_cli_options_xvtable_multi( cli ); !rxtended && *multix; multix++, ntable++ )
    {
      const duf_longval_extended_vtable_t *xvtable = *multix;

      rxtended = duf_coption_xfind_at_xtable( cli, codeval, xvtable, result_pxvtable, pr );
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
