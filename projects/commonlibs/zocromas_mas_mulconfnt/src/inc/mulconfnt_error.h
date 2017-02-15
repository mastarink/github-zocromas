#ifndef MULCONF_ERROR_H
# define MULCONF_ERROR_H

# include "mulconfnt_types.h"
# define DIE( ...) mucs_error_die(__LINE__, __func__, __FILE__,  __VA_ARGS__)
//# define VDIE( ...) mucs_error_vdie(__LINE__, __func__, __FILE__,  __VA_ARGS__)
void mucs_error_die( int line, const char *func, const char *file, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 4, 5 ) ) );
void mucs_error_vdie( int line, const char *func, const char *file, const char *fmt, va_list args );

int mucs_error_vset_at_source( mucs_source_han_t * osrc, int line, const char *func, const char *file, const char *fmt, va_list args );
int mucs_error_set_at_source( mucs_source_han_t * osrc, int line, const char *func, const char *file, const char *fmt, ... )
        __attribute__ ( ( format( __printf__, 5, 6 ) ) );

int
mucs_error_vset_at_option( mucs_option_han_t * opt, int line, const char *func, const char *file, unsigned long flags, const char *fmt,
                                va_list args );
int mucs_error_set_at_option( mucs_option_han_t * opt, int line, const char *func, const char *file, unsigned long flags, const char *fmt, ... )
        __attribute__ ( ( format( __printf__, 6, 7 ) ) );

int mucs_error_set_at_source_from_option( mucs_source_han_t * osrc, mucs_option_han_t * opt );

int mucs_error_source( const mucs_source_han_t * osrc );
const char *mucs_error_source_msg( const mucs_source_han_t * osrc );
int mucs_error_option( const mucs_option_han_t * opt );
const char *mucs_error_option_msg( const mucs_option_han_t * opt );

#endif
