#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

/* #include "mulconfnt_error.h" */

#include "option.h"
#include "option_base.h"

#include "option_tablist.h"

/*
 *
 * mucs_config_option_tablist_...
 *
 * */

const mucs_option_han_t *
mucs_config_option_tablist_lookup( const mucs_option_table_list_t * tablist, mucs_variant_t variantid,
                                   const char *arg, const char *nextarg, const char *eq, const char *force_value, int *phas_value,
                                   const char **pstring_value )
{
  /* mucs_option_han_t *opt = NULL; */
  const mucs_option_han_t *found_topt = NULL;
  int has_value = 0;
  const char *string_value = NULL;

  while ( !found_topt && tablist )
  {
    found_topt =
            mucs_config_option_lookup_option_table( tablist->options, found_topt, variantid, arg, nextarg, eq, force_value, &has_value,
                                                    &string_value );
    tablist = tablist->next;
  }
  if ( phas_value )
    *phas_value = has_value;
  if ( pstring_value )
    *pstring_value = string_value;
  return found_topt;
}
