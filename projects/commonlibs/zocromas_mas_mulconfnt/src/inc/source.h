#ifndef MULCONF_SOURCE_H
# define MULCONF_SOURCE_H

# include "mulconfnt_types.h"

char *mucs_source_load_string( config_source_desc_t * osrc );
mas_argvc_t *mucs_source_load_targ( config_source_desc_t * osrc );

int mucs_source_lookup_seq( config_source_desc_t * osrc, const config_option_table_list_t * tablist );
void mucs_source_lookup_all( config_source_desc_t * osrc, const config_option_table_list_t * tablist );
int mucs_source_lookup( config_source_desc_t * osrc, const config_option_table_list_t * tablist );

int mucs_source_argsno_count( config_source_desc_t * osrc );
char **mucs_source_argv_no( config_source_desc_t * osrc );
int mucs_source_argc_no( config_source_desc_t * osrc );
const char *mucs_source_arg_no( config_source_desc_t * osrc, int i );

unsigned long mucs_source_flags( config_source_desc_t * osrc );
int mucs_source_flag( config_source_desc_t * osrc, unsigned long mask );

void mucs_source_set_common_callback( config_source_desc_t * osrc, option_callback_t cb );
void mucs_source_set_type_callback( config_source_desc_t * osrc, config_restype_t restype, option_callback_t cb );

#endif
