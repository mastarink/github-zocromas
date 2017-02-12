#ifndef MULCONF_OPTION_TABLE_H
# define MULCONF_OPTION_TABLE_H

# include "mulconfnt_types.h"

config_option_t *mulconfnt_config_option_lookup_tablist( const config_option_table_list_t * tablist, config_variant_t variantid,
                                                         const char *arg, const char *nextarg, const char *eq, const char *force_value );

#endif
