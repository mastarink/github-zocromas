#ifndef MUCS_OPTION_TABLIST_H
# define MUCS_OPTION_TABLIST_H

# include "mulconfnt_types.h"

mucs_option_han_t *mucs_config_option_tablist_lookup( const mucs_option_table_list_t * tablist, mucs_variant_t variantid,
                                                         const char *arg, const char *nextarg, const char *eq, const char *force_value,
                                                         unsigned long flags );

#endif
