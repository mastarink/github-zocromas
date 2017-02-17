#ifndef MASXFS_ERROR_H
# define MASXFS_ERROR_H

# include "masxfs_types.h"
# define DIE( ...) masxfs_error_die(__LINE__, __func__, __FILE__, 34, __VA_ARGS__)
# define WARN( ...) masxfs_error_die(__LINE__, __func__, __FILE__, 0, __VA_ARGS__)
# define RDIE( ...) masxfs_error_die(__LINE__, __func__, __FILE__, 24, __VA_ARGS__)
# define QRDIE(r) { if (r) RDIE("R: %d; errno:%d:%s", r, errno, strerror(errno)); }
void masxfs_error_die( int line, const char *func, const char *file, int fexit, const char *fmt, ... )
        __attribute__ ( ( format( __printf__, 5, 6 ) ) );
void masxfs_error_vdie( int line, const char *func, const char *file, int fexit, const char *fmt, va_list args );

#endif
