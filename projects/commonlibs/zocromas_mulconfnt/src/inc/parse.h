#ifndef MUCS_H
# define MUCS_H

# include "mulconfnt_types.h"


int mucs_parse( int argc, const char *argv[], const mucs_option_table_list_t * tablist );
mas_argvc_t mucs_load_targ( mucs_source_t * optr );
char *mucs_load_string( mucs_source_t * optr );

#endif
