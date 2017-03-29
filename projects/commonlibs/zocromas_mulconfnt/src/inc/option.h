#ifndef MUCS_OPTION_H
# define MUCS_OPTION_H

# include "mulconfnt_types.h"

void mucs_config_option_set_value( mucs_option_han_t * opt, const char *string_value, unsigned long flags );

unsigned long mucs_config_option_flags( const mucs_option_han_t * opt );
int mucs_config_option_flag( const mucs_option_han_t * opt, unsigned long mask );

mucs_option_han_t *mucs_config_option_lookup_option_table( const mucs_option_han_t * option_table, mucs_variant_t variantid, const char *arg,
                                                              const char *nextarg, const char *eq, const char *force_value,
                                                              unsigned long flags  );

int mucs_option_set_source( mucs_option_han_t * opt, mucs_source_han_t * osrc );

#endif
