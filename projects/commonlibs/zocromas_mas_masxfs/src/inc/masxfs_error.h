#ifndef MASXFS_ERROR_H
# define MASXFS_ERROR_H

# include "masxfs_types.h"
# define DIE( ...) masxfs_error_die(__LINE__, __func__, __FILE__,  __VA_ARGS__)

void masxfs_error_die( int line, const char *func, const char *file, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 4, 5 ) ) );
void masxfs_error_vdie( int line, const char *func, const char *file, const char *fmt, va_list args );

#endif
