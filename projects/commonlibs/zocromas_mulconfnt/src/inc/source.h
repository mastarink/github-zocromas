#ifndef MUCS_SOURCE_H
# define MUCS_SOURCE_H

# include "mulconfnt_types.h"

char *mucs_source_load_string( mucs_source_han_t * osrc );
mas_argvc_t *mucs_source_load_targ( mucs_source_han_t * osrc );

int mucs_source_lookup_seq( mucs_source_han_t * osrc, const mucs_option_table_list_t * tablist );
void mucs_source_lookup_all( mucs_source_han_t * osrc, const mucs_option_table_list_t * tablist );
int mucs_source_lookup( mucs_source_han_t * osrc, const mucs_option_table_list_t * tablist );

int mucs_source_argsno_count( mucs_source_han_t * osrc );
char **mucs_source_argv_no( mucs_source_han_t * osrc );
int mucs_source_argc_no( mucs_source_han_t * osrc );
const char *mucs_source_arg_no( mucs_source_han_t * osrc, int i );

unsigned long mucs_source_flags( mucs_source_han_t * osrc );
int mucs_source_flag( mucs_source_han_t * osrc, unsigned long mask );

void mucs_source_set_common_callback( mucs_source_han_t * osrc, mucs_option_callback_t cb );
void mucs_source_set_type_callback( mucs_source_han_t * osrc, mucs_restype_t restype, mucs_option_callback_t cb );

#endif