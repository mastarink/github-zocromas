#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"

#include "option.h"

#include "option_tablist.h"

/*
 *
 * mucs_config_option_tablist_...
 *
 * */

mucs_option_han_t *
mucs_config_option_tablist_lookup( const mucs_option_table_list_t * tablist, mucs_variant_t variantid,
                                   const char *arg, const char *nextarg, const char *eq, const char *force_value, unsigned long flags )
{
  mucs_option_han_t *opt = NULL;

  while ( !opt && tablist )
  {
    opt = mucs_config_option_lookup_option_table( tablist->options, variantid, arg, nextarg, eq, force_value, flags );
    tablist = tablist->next;
  }
  return opt;
}
