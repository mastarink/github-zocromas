#ifndef MULCONF_OPTION_H
# define MULCONF_OPTION_H

# include "mulconfnt_types.h"

void mulconfnt_config_option_set_value( config_option_t * opt, const char *string_value );
config_option_t *mulconfnt_config_option_lookup_tablist( const config_option_table_list_t * tablist, config_variant_t variantid,
                                                         const char *arg, const char *nextarg, const char *eq, const char *force_value );

config_option_t *mulconfnt_config_option_lookup_option_table( const config_option_t * option_table, config_variant_t variantid, const char *arg,
                                                              const char *nextarg, const char *eq, const char *force_value );
#endif
