#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"

#include "option.h"

#include "option_tablist.h"

config_option_t *
mulconfnt_config_option_lookup_tablist( const config_option_table_list_t * tablist, config_variant_t variantid,
                                        const char *arg, const char *nextarg, const char *eq, const char *force_value, unsigned long flags )
{
  config_option_t *opt = NULL;

  while ( !opt && tablist )
  {
    if ( do_fprintf )
      fprintf( stderr, "LOOKUP TABLIST %s\n", tablist->name );
    opt = mulconfnt_config_option_lookup_option_table( tablist->options, variantid, arg, nextarg, eq, force_value, flags  );
    tablist = tablist->next;
  }
  return opt;
}
