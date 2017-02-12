#ifndef MULCONF_ERROR_H
# define MULCONF_ERROR_H

# include "mulconfnt_types.h"


int mulconfnt_error_set_at_source( config_source_desc_t * osrc, int line, const char *func, const char *file, const char *fmt, ... );
int mulconfnt_error_set_at_option( config_option_t * opt, int line, const char *func, const char *file, const char *fmt, ... );

int mulconfnt_error_set_at_source_from_option( config_source_desc_t * osrc, config_option_t * opt );

int mulconfnt_error_source( const config_source_desc_t * osrc );
const char *mulconfnt_error_source_msg( const config_source_desc_t * osrc );
int mulconfnt_error_option( const config_option_t * opt );
const char *mulconfnt_error_option_msg( const config_option_t * opt );

#endif
