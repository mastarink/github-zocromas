#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_xtended_table.h"
#include "duf_option_stage.h"

/* ###################################################################### */
#include "duf_option_cfind.h"
/* ###################################################################### */

static const duf_longval_extended_t *
duf_coption_xfind_at_xarr( duf_option_code_t codeval, const duf_longval_extended_t * xarr, int *pr )
{
  const duf_longval_extended_t *rxtended = NULL;
  int rpr = 0;
  int ntable = 0;
  int tbcount = 0;

  for ( ; !rxtended && xarr->o.name; xarr++, tbcount++ )
  {
    if ( xarr )
    {
      DUF_TRACE( findopt, +1, "@li2ex %d:%d [%s] %d:%d", ntable, tbcount, xarr->o.name, xarr->o.val, codeval );
      /* assert( 0 ); */
      if ( xarr->o.val == codeval )
      {
        rxtended = xarr;
        DUF_TRACE( findopt, +1, "@li2ex FOUND %d:%d [%s]", ntable, tbcount, xarr->o.name );
#if 0
        ok = 1;
#endif
        break;                  /* ? */
      }
    }
  }
  if ( pr )
    *pr = rpr;
  return rxtended;
}

static const duf_longval_extended_t *
duf_coption_xfind_at_xtable( duf_option_code_t codeval, const duf_longval_extended_table_t * xtable,
                             const duf_longval_extended_table_t ** result_pxtable, int *pr )
{
  const duf_longval_extended_t *rxtended = NULL;

  rxtended = duf_coption_xfind_at_xarr( codeval, xtable->table, pr );
  if ( result_pxtable )
    *result_pxtable = xtable;
  return rxtended;
}

/* codeval => extended, by standard multi-table */
const duf_longval_extended_t *
duf_coption_xfind_at_stdx( duf_option_code_t codeval, const duf_longval_extended_table_t ** result_pxtable, int *pr )
{
  const duf_longval_extended_t *rxtended = NULL;

#if 0
  int ok;
#endif
  int rpr = 0;
  int ntable = 0;
  int tbcount = 0;

  if ( codeval && codeval != '?' )
  {
    for ( const duf_longval_extended_table_t ** multix = duf_extended_table_multi(  ); !rxtended && *multix; multix++, ntable++ )
    {
      const duf_longval_extended_table_t *xtable = *multix;

#if 0
      for ( const duf_longval_extended_t * xarr = xtable->table; !rxtended && xarr->o.name; xarr++, tbcount++ )
      {
        if ( xarr )
        {
          DUF_TRACE( findopt, +1, "@li2ex %d:%d [%s] %d:%d", ntable, tbcount, xarr->o.name, xarr->o.val, codeval );
          if ( xarr->o.val == codeval )
          {
            rxtended = xarr;
            if ( result_pxtable )
              *result_pxtable = xtable;
            DUF_TRACE( findopt, +1, "@li2ex FOUND %d:%d [%s]", ntable, tbcount, xarr->o.name );
            break;              /* ? */
          }
        }
      }
#else
      rxtended = duf_coption_xfind_at_xtable( codeval, xtable, result_pxtable, pr );
#endif
    }
    DUF_TRACE( findopt, +1, "@li2ex ? %d:%d [%s]", ntable, tbcount, rxtended ? rxtended->o.name : NULL );
  }
#if 0
  if ( !ok )
    DUF_MAKE_ERROR( rpr, DUF_ERROR_OPTION_NOT_FOUND );
#endif
  if ( pr )
    *pr = rpr;
  return rxtended;
}
