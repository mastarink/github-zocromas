#ifndef MULCONF_SOURCE_H
# define MULCONF_SOURCE_H

# include "mulconfnt_types.h"

char *mulconfnt_source_load_string( config_source_desc_t * osrc );
mas_argvc_t *mulconfnt_source_load_targ( config_source_desc_t * osrc );

void mulconfnt_source_lookup( config_source_desc_t * osrc, const config_option_table_list_t * tablist );

char **mulconfnt_source_argsno( config_source_desc_t * osrc );
int mulconfnt_source_argno_count( config_source_desc_t * osrc );
const char *mulconfnt_source_argno( config_source_desc_t * osrc, int i );

#endif
