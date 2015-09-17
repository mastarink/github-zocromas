#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_status_ref.h"

#include "duf_option_find.h"
#include "duf_option_extended.h"
#include "duf_option_names.h"

/* ###################################################################### */
#include "duf_option_descr.h"
/* ###################################################################### */


static const char *
_duf_find_longval_help( duf_option_code_t codeval, int *pr )
{
  const char *ph = NULL;
  const duf_longval_extended_t *extended;

  extended = duf_find_codeval_extended_std( codeval, NULL, pr );
  /* extended = &lo_extended[ilong]; */

  if ( extended )
    ph = extended->help;
  return extended ? ( ph ? ph : "no description" ) : "not found";
}

const char *
duf_find_longval_help( duf_option_code_t codeval, int *pr )
{
  const char *ph = _duf_find_longval_help( codeval, pr );

  return ph ? ph : "-";
}

char *
duf_option_description_xd( const duf_longval_extended_t * extended, const char *delimh, const char *delim, int *pr )
{
  char *s = NULL;

  if ( extended )
  {
    duf_option_code_t codeval;

    codeval = extended->o.val;
    s = duf_option_names_d( codeval, delim );
    if ( s )
    {
      const char *h;

      /* h = _duf_find_longval_help( codeval, pr ); */
      h = extended->help;
      if ( h )
      {
        s = mas_strcat_x( s, delimh ? delimh : " :: " );
        s = mas_strcat_x( s, h );
      }
    }
  }
  return s;
}

char *
duf_option_description_d( int longindex, const char *delimh, const char *delim, int *pr )
{
  char *p = NULL;
  int r = 0;
  const duf_longval_extended_t *extended;

  extended = duf_longindex2extended( longindex, NULL, &r );
  if ( DUF_NOERROR( r ) )
    p = duf_option_description_xd( extended, delimh, delim, pr );
  if ( pr )
    *pr = r;
  return p;
}

static char *
duf_option_description_x( const duf_longval_extended_t * extended, int *pr )
{
  return duf_option_description_xd( extended, NULL, NULL, pr );
}

/* char *                                                          */
/* duf_option_description( int longindex, int *pr )                */
/* {                                                               */
/*   return duf_option_description_d( longindex, NULL, NULL, pr ); */
/* }                                                               */
/*                                                                 */

const char *
duf_option_description_x_tmp( int index, const duf_longval_extended_t * extended, int *pr )
{
  const char *x = NULL;

  if ( index < 0 )
  {
    index = global_status.tmp->explanation_index++;
    if ( global_status.tmp->explanation_index >= DUF_TMP_EXPLANATION_MAX )
      global_status.tmp->explanation_index = 0;
  }

  if ( index >= 0 && index < DUF_TMP_EXPLANATION_MAX )
  {
    mas_free( global_status.tmp->option_explanation[index] );
    global_status.tmp->option_explanation[index] = NULL;
    global_status.tmp->option_explanation[index] = duf_option_description_x( extended, pr );
    x = global_status.tmp->option_explanation[index];
  }
  return x;
}
