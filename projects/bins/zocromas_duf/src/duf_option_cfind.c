#undef MAS_TRACING
#include <string.h>

/* #include "duf_maintenance_z.h" */
#include "duf_maintenance_tracen.h"
/* #include "duf_maintenance_errors.h" */

#include "duf_config_util.h"

#include "duf_xtended_table.h"
#include "duf_option_stage.h"
#include "duf_option_config.h"

#include "duf_option_lfind.h"

/* ###################################################################### */
#include "duf_option_cfind.h"
/* ###################################################################### */

static const duf_option_t *
duf_coption_find_at_arr( duf_option_code_t codeval, const duf_option_t * arr, int *plongindex, int *pr )
{
  const duf_option_t *roption = NULL;
  int rpr = 0;
  int longindex = 0;

  for ( ; !roption && arr->name; arr++, longindex++ )
  {
    if ( arr )
    {
      DUF_TRACE( findopt, +1, "@li2ex %d [%s] %d:%d", longindex, arr->name, arr->val, codeval );
    /* assert( 0 ); */
      if ( arr->val == codeval )
      {
        roption = arr;
        DUF_TRACE( findopt, +1, "@li2ex FOUND %d [%s]", longindex, arr->name );
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
duf_coption_find_at_std( duf_option_code_t codeval, int *plongindex, int *pr )
{
  return duf_coption_find_at_arr( codeval, duf_cli_options_get_longopts_table(duf_get_config_cli()), plongindex, pr );
}

static const duf_option_t *
duf_lcoption_find_at_std( duf_option_code_t codeval, int *plongindex, int *pr )
{
  const duf_option_t *roption = NULL;

  if ( *plongindex >= 0 && codeval > DUF_OPTION_VAL_LONG )
#if 0
    roption = &( duf_cli_options_get_longopts_table(duf_get_config_cli())[longindex] );
#else
    roption = duf_loption_find_at_std( *plongindex );
#endif
  else if ( codeval != '?' )
    roption = duf_coption_find_at_std( codeval, plongindex, pr );
  return roption;
}

#if 0
const char *
duf_coption_find_name_at_std( duf_option_code_t codeval, int *pr )
{
  const duf_option_t *longoption = NULL;

  longoption = duf_coption_find_at_std( codeval, pr );
  return longoption ? longoption->name : NULL;
}
#endif

const char *
duf_lcoption_find_name_at_std( duf_option_code_t codeval, int *plongindex, int *pr )
{
  const duf_option_t *longoption = NULL;

  longoption = duf_lcoption_find_at_std( codeval, plongindex, pr );
  return longoption ? longoption->name : NULL;
}

static const duf_longval_extended_t *
duf_coption_xfind_at_xarr( duf_option_code_t codeval, const duf_longval_extended_t * xarr, int *pr )
{
  const duf_longval_extended_t *rxtended = NULL;
  int rpr = 0;
  int tbcount = 0;

  for ( ; !rxtended && xarr->o.name; xarr++, tbcount++ )
  {
    DUF_TRACE( findopt, +1, "@li2ex %d [%s] %d:%d", tbcount, xarr->o.name, xarr->o.val, codeval );
  /* assert( 0 ); */
    if ( xarr->o.val == codeval )
    {
      rxtended = xarr;
      DUF_TRACE( findopt, +1, "@li2ex FOUND %d [%s]", tbcount, xarr->o.name );
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
duf_coption_xfind_at_xtable( duf_option_code_t codeval, const duf_longval_extended_vtable_t * xvtable,
                             const duf_longval_extended_vtable_t ** result_pxvtable, int *pr )
{
  const duf_longval_extended_t *rxtended = NULL;

  rxtended = duf_coption_xfind_at_xarr( codeval, xvtable->xlist, pr );
  if ( result_pxvtable )
    *result_pxvtable = xvtable;
  return rxtended;
}

/* codeval => extended, by standard multi-table */
const duf_longval_extended_t *
duf_coption_xfind_at_stdx( duf_option_code_t codeval, const duf_longval_extended_vtable_t ** result_pxvtable, int *pr )
{
  const duf_longval_extended_t *rxtended = NULL;

#if 0
  int ok;
#endif
  int rpr = 0;
  int ntable = 0;

  if ( codeval && codeval != '?' )
  {
    for ( duf_longval_extended_vtable_t **multix = duf_cli_options_xvtable_multi(duf_get_config_cli()); !rxtended && *multix; multix++, ntable++ )
    {
      const duf_longval_extended_vtable_t *xvtable = *multix;

#if 0
      {
        int tbcount = 0;

        for ( const duf_longval_extended_t * xarr = xvtable->xlist; !rxtended && xarr->o.name; xarr++, tbcount++ )
        {
          if ( xarr )
          {
            DUF_TRACE( findopt, +1, "@li2ex %d:%d [%s] %d:%d", ntable, tbcount, xarr->o.name, xarr->o.val, codeval );
            if ( xarr->o.val == codeval )
            {
              rxtended = xarr;
              if ( result_pxvtable )
                *result_pxvtable = xvtable;
              DUF_TRACE( findopt, +1, "@li2ex FOUND %d:%d [%s]", ntable, tbcount, xarr->o.name );
              break;                                                 /* ? */
            }
          }
        }
        DUF_TRACE( findopt, +1, "@li2ex ? %d:%d [%s]", ntable, tbcount, rxtended ? rxtended->o.name : NULL );
      }
#else
      rxtended = duf_coption_xfind_at_xtable( codeval, xvtable, result_pxvtable, pr );
      DUF_TRACE( findopt, +1, "@li2ex ? %d [%s]", ntable, rxtended ? rxtended->o.name : NULL );
#endif
    }
  }
#if 0
  if ( !ok )
    DUF_MAKE_ERROR( rpr, DUF_ERROR_OPTION_NOT_FOUND );
#endif
  if ( pr )
    *pr = rpr;
  return rxtended;
}
