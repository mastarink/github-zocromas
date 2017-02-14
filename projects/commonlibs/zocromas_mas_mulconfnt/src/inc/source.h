#ifndef MULCONF_SOURCE_H
# define MULCONF_SOURCE_H

# include "mulconfnt_types.h"

char *mulconfnt_source_load_string( config_source_desc_t * osrc );
mas_argvc_t *mulconfnt_source_load_targ( config_source_desc_t * osrc );

int mulconfnt_source_lookup_seq( config_source_desc_t * osrc, const config_option_table_list_t * tablist );
void mulconfnt_source_lookup_all( config_source_desc_t * osrc, const config_option_table_list_t * tablist );
int mulconfnt_source_lookup( config_source_desc_t * osrc, const config_option_table_list_t * tablist );

int mulconfnt_source_argsno_count( config_source_desc_t * osrc );
char **mulconfnt_source_argv_no( config_source_desc_t * osrc );
int mulconfnt_source_argc_no( config_source_desc_t * osrc );
const char *mulconfnt_source_arg_no( config_source_desc_t * osrc, int i );

unsigned long mulconfnt_source_flags( config_source_desc_t * osrc );
int mulconfnt_source_flag( config_source_desc_t * osrc, unsigned long mask );

#endif
