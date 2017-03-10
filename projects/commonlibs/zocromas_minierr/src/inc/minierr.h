#ifndef MASXFS_ERROR_H
# define MASXFS_ERROR_H

# include <assert.h>
# include <errno.h>
# include <stdarg.h>
# define MAS_MIER_SHOW_ALL  MAS_MIER_FLAG_LINE | MAS_MIER_FLAG_FUNC | MAS_MIER_FLAG_FILE
# define DIE( ...) minierr_die(__LINE__, __func__, __FILE__, MAS_MIER_FLAG_SLEEP | MAS_MIER_FLAG_EXIT , 34, "DIE", __VA_ARGS__)
# define WARN( ...) minierr_die(__LINE__, __func__, __FILE__, MAS_MIER_SHOW_ALL, 0, "WARN", __VA_ARGS__)
# define INFO( ...) minierr_die(__LINE__, __func__, __FILE__, 0, 0, "INFO", __VA_ARGS__)
# define MARK( _mark, ...) minierr_die(__LINE__, __func__, __FILE__, MAS_MIER_SHOW_ALL, 0, _mark, __VA_ARGS__)
# define RDIE( ...) minierr_die(__LINE__, __func__, __FILE__, MAS_MIER_SHOW_ALL | MAS_MIER_FLAG_EXIT, 24, "DIE", __VA_ARGS__)
# define RWARN( ...) minierr_die(__LINE__, __func__, __FILE__, MAS_MIER_SHOW_ALL, 0, "WARN", __VA_ARGS__)

# define QR(r)		{ if (r<0) RWARN("R: %d; errno:%d:%s", r, errno, strerror(errno)); }

typedef enum mas_minierr_flag_num_e
{
  MAS_MIER_FLAG_LINE_NUM,
  MAS_MIER_FLAG_FUNC_NUM,
  MAS_MIER_FLAG_FILE_NUM,
  MAS_MIER_FLAG_EXIT_NUM,
  MAS_MIER_FLAG_SLEEP_NUM,
} mas_minierr_flag_num_t;
typedef enum mas_minierr_flag_e
{
  MAS_MIER_FLAG_LINE = 1 << MAS_MIER_FLAG_LINE_NUM,
  MAS_MIER_FLAG_FUNC = 1 << MAS_MIER_FLAG_FUNC_NUM,
  MAS_MIER_FLAG_FILE = 1 << MAS_MIER_FLAG_FILE_NUM,
  MAS_MIER_FLAG_EXIT = 1 << MAS_MIER_FLAG_EXIT_NUM,
  MAS_MIER_FLAG_SLEEP = 1 << MAS_MIER_FLAG_SLEEP_NUM,
} mas_minierr_flag_t;

void minierr_die( int line, const char *func, const char *file, int fexit, unsigned flags, const char *sid, const char *fmt, ... )
        __attribute__ ( ( format( __printf__, 7, 8 ) ) );
void minierr_vdie( int line, const char *func, const char *file, int fexit, unsigned flags, const char *sid, const char *fmt, va_list args );

#endif
