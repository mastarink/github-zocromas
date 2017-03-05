#ifndef MASXFS_ERROR_H
# define MASXFS_ERROR_H

# include <assert.h>
# include <errno.h>
# include <stdarg.h>

# define DIE( ...) minierr_die(__LINE__, __func__, __FILE__, 34, "DIE", __VA_ARGS__)
# define WARN( ...) minierr_die(__LINE__, __func__, __FILE__, 0, "WARN", __VA_ARGS__)
# define INFO( ...) minierr_die(__LINE__, __func__, __FILE__, 0, "INFO", __VA_ARGS__)
# define MARK( _mark, ...) minierr_die(__LINE__, __func__, __FILE__, 0, _mark, __VA_ARGS__)
# define RDIE( ...) minierr_die(__LINE__, __func__, __FILE__, 24, "DIE", __VA_ARGS__)
# define RWARN( ...) minierr_die(__LINE__, __func__, __FILE__, 0, "WARN", __VA_ARGS__)

# define QR(r)		{ if (r<0) RWARN("R: %d; errno:%d:%s", r, errno, strerror(errno)); }

void minierr_die( int line, const char *func, const char *file, int fexit, const char *sid, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 6, 7 ) ) );
void minierr_vdie( int line, const char *func, const char *file, int fexit, const char *sid, const char *fmt, va_list args );

#endif
