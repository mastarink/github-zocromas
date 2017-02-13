#ifndef MULCONF_ERROR_H
# define MULCONF_ERROR_H

# include "mulconfnt_types.h"
# define DIE( ...) mulconfnt_error_die(__LINE__, __func__, __FILE__,  __VA_ARGS__)
//# define VDIE( ...) mulconfnt_error_vdie(__LINE__, __func__, __FILE__,  __VA_ARGS__)
void mulconfnt_error_die( int line, const char *func, const char *file, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 4, 5 ) ) );
void mulconfnt_error_vdie( int line, const char *func, const char *file, const char *fmt, va_list args );

int mulconfnt_error_vset_at_source( config_source_desc_t * osrc, int line, const char *func, const char *file, const char *fmt, va_list args );
int mulconfnt_error_set_at_source( config_source_desc_t * osrc, int line, const char *func, const char *file, const char *fmt, ... )
        __attribute__ ( ( format( __printf__, 5, 6 ) ) );

int
mulconfnt_error_vset_at_option( config_option_t * opt, int line, const char *func, const char *file, unsigned long flags, const char *fmt,
                                va_list args );
int mulconfnt_error_set_at_option( config_option_t * opt, int line, const char *func, const char *file, unsigned long flags, const char *fmt, ... )
        __attribute__ ( ( format( __printf__, 6, 7 ) ) );

int mulconfnt_error_set_at_source_from_option( config_source_desc_t * osrc, config_option_t * opt );

int mulconfnt_error_source( const config_source_desc_t * osrc );
const char *mulconfnt_error_source_msg( const config_source_desc_t * osrc );
int mulconfnt_error_option( const config_option_t * opt );
const char *mulconfnt_error_option_msg( const config_option_t * opt );

#endif
