#ifndef MULCONF_OPTION_TABLIST_H
# define MULCONF_OPTION_TABLIST_H

# include "mulconfnt_types.h"

config_option_t *mucs_config_option_tablist_lookup( const config_option_table_list_t * tablist, config_variant_t variantid,
                                                         const char *arg, const char *nextarg, const char *eq, const char *force_value,
                                                         unsigned long flags );

#endif
