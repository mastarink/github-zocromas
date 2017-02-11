#ifndef MULCONF_ERROR_H
# define MULCONF_ERROR_H

# include "mulconfnt_types.h"

int mulconfnt_set_error_list( config_source_list_t * list, int line, const char *func );
int mulconfnt_set_error_source( config_source_desc_t * osrc, int line, const char *func );
int mulconfnt_set_error_option( config_option_t * opt, int line, const char *func );

int mulconfnt_error_list( config_source_list_t * list );
int mulconfnt_error_source( config_source_desc_t * osrc );

#endif
