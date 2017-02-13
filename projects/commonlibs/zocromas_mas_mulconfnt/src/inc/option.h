#ifndef MULCONF_OPTION_H
# define MULCONF_OPTION_H

# include "mulconfnt_types.h"

void mulconfnt_config_option_set_value( config_option_t * opt, const char *string_value, unsigned long flags );

unsigned long mulconfnt_config_option_flags( const config_option_t * opt );
int mulconfnt_config_option_flag( const config_option_t * opt, unsigned long mask );

config_option_t *mulconfnt_config_option_lookup_option_table( const config_option_t * option_table, config_variant_t variantid, const char *arg,
                                                              const char *nextarg, const char *eq, const char *force_value,
                                                              unsigned long flags _uUu_ );

void mulconfnt_option_set_source( config_option_t * opt, config_source_desc_t * osrc );

#endif
