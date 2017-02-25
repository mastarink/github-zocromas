#ifndef MASXFS_ERROR_H
# define MASXFS_ERROR_H

# include <assert.h>
# include <errno.h>

# define DIE( ...) minierr_die(__LINE__, __func__, __FILE__, 34, __VA_ARGS__)
# define WARN( ...) minierr_die(__LINE__, __func__, __FILE__, 0, __VA_ARGS__)
# define RDIE( ...) minierr_die(__LINE__, __func__, __FILE__, 24, __VA_ARGS__)
# define RWARN( ...) minierr_die(__LINE__, __func__, __FILE__, 0, __VA_ARGS__)

# define QR(r)		{ if (r<0) RWARN("R: %d; errno:%d:%s", r, errno, strerror(errno)); }

void minierr_die( int line, const char *func, const char *file, int fexit, const char *fmt, ... )
        __attribute__ ( ( format( __printf__, 5, 6 ) ) );
void minierr_vdie( int line, const char *func, const char *file, int fexit, const char *fmt, va_list args );

#endif
