#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


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

  extended = duf_find_extended( codeval, pr );
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

      h = _duf_find_longval_help( codeval, pr );
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
  const duf_longval_extended_t *extended;

  extended = duf_longindex_extended( longindex );
  return duf_option_description_xd( extended, delimh, delim, pr );
}

char *
duf_option_description_x( const duf_longval_extended_t * extended, int *pr )
{
  return duf_option_description_xd( extended, NULL, NULL, pr );
}

char *
duf_option_description( int longindex, int *pr )
{
  return duf_option_description_d( longindex, NULL, NULL, pr );
}

