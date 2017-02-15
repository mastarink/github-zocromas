#ifndef MULCONF_H
# define MULCONF_H

# include "mulconfnt_types.h"


int mucs_parse( int argc, const char *argv[], const config_option_table_list_t * tablist );
mas_argvc_t mucs_load_targ( config_source_desc_t * optr );
char *mucs_load_string( config_source_desc_t * optr );

#endif
