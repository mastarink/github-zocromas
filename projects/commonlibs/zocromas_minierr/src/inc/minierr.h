#ifndef MASXFS_MINIERROR_H
# define MASXFS_MINIERROR_H

# include <assert.h>
# include <errno.h>
# include <stdarg.h>
# define MAS_MIER_ALL  MAS_MIER_FLAG_LINE | MAS_MIER_FLAG_FUNC | MAS_MIER_FLAG_FILE
# define MAS_MIER_X MAS_MIER_ALL  | MAS_MIER_FLAG_EXIT
# define MAS_MIER_W MAS_MIER_ALL
# define MAS_MIER_SW MAS_MIER_W   | MAS_MIER_FLAG_SLEEP

# define MAS_MIER_NI MAS_MIER_ALL | MAS_MIER_FLAG_ASSERT
# define MAS_MIER_SX MAS_MIER_X   | MAS_MIER_FLAG_SLEEP
# define MAS_MIER_AX MAS_MIER_X   | MAS_MIER_FLAG_ASSERT
# define MAS_MIER_I MAS_MIER_FLAG_COLPREF

# define DIE( ...)         minierr_die( __LINE__, __func__, __FILE__, 0, MAS_MIER_TYPE_DIE,   34, MAS_MIER_X,           "DIE", __VA_ARGS__ )
# define SDIE( ...)        minierr_die( __LINE__, __func__, __FILE__, 0, MAS_MIER_TYPE_SDIE,  37, MAS_MIER_SX,          "DIE", __VA_ARGS__ )
# define ADIE( ...)        minierr_die( __LINE__, __func__, __FILE__, 0, MAS_MIER_TYPE_ADIE,  37, MAS_MIER_AX,          "DIE", __VA_ARGS__ )
# define NIMP( ...)        minierr_die( __LINE__, __func__, __FILE__, 0, MAS_MIER_TYPE_NIMP,  14, MAS_MIER_NI,          "NOT IMPLEMENTED", __VA_ARGS__ )
# define WARN( ...)        minierr_die( __LINE__, __func__, __FILE__, 0, MAS_MIER_TYPE_WARN,   0, MAS_MIER_W,          "WARN", __VA_ARGS__ )
# define WARNB( _wraplines, ...)        minierr_die( __LINE__, __func__, __FILE__, _wraplines, MAS_MIER_TYPE_WARN,   0, MAS_MIER_W,          "WARN", __VA_ARGS__ )
# define SWARN( ...)       minierr_die( __LINE__, __func__, __FILE__, 0, MAS_MIER_TYPE_SWARN,  0, MAS_MIER_SW,         "WARN", __VA_ARGS__ )
# define INFO( ...)        minierr_die( __LINE__, __func__, __FILE__, 0, MAS_MIER_TYPE_INFO,   0, MAS_MIER_I,          "INFO", __VA_ARGS__ )
# define INFOB( _wraplines, ...)        minierr_die( __LINE__, __func__, __FILE__, _wraplines, MAS_MIER_TYPE_INFO,   0, MAS_MIER_I,          "INFO", __VA_ARGS__ )
# define MARK( _mark, ...) minierr_die( __LINE__, __func__, __FILE__, 0, MAS_MIER_TYPE_MARK,   0, MAS_MIER_ALL,         _mark, __VA_ARGS__ )
# define RDIE( ...)        minierr_die( __LINE__, __func__, __FILE__, 0, MAS_MIER_TYPE_RDIE,  24, MAS_MIER_X,           "DIE", __VA_ARGS__ )
# define RWARN( ...)       minierr_die( __LINE__, __func__, __FILE__, 0, MAS_MIER_TYPE_RWARN,  0, MAS_MIER_ALL,        "WARN", __VA_ARGS__ )

# define QR(r)		{ if (r<0) RWARN("R: %d; errno:%d:%s", r, errno, strerror(errno)); }

typedef enum mas_minierr_type_e
{
  MAS_MIER_TYPE_DIE,
  MAS_MIER_TYPE_SDIE,
  MAS_MIER_TYPE_ADIE,
  MAS_MIER_TYPE_NIMP,
  MAS_MIER_TYPE_WARN,
  MAS_MIER_TYPE_SWARN,
  MAS_MIER_TYPE_INFO,
  MAS_MIER_TYPE_MARK,
  MAS_MIER_TYPE_RDIE,
  MAS_MIER_TYPE_RWARN,
} mas_minierr_type_t;

typedef enum mas_minierr_flag_num_e
{
  MAS_MIER_FLAG_LINE_NUM,
  MAS_MIER_FLAG_FUNC_NUM,
  MAS_MIER_FLAG_FILE_NUM,
  MAS_MIER_FLAG_EXIT_NUM,
  MAS_MIER_FLAG_ASSERT_NUM,
  MAS_MIER_FLAG_SLEEP_NUM,
  MAS_MIER_FLAG_COLPREF_NUM,
} mas_minierr_flag_num_t;
typedef enum mas_minierr_flag_e
{
  MAS_MIER_FLAG_LINE = 1L << MAS_MIER_FLAG_LINE_NUM,
  MAS_MIER_FLAG_FUNC = 1L << MAS_MIER_FLAG_FUNC_NUM,
  MAS_MIER_FLAG_FILE = 1L << MAS_MIER_FLAG_FILE_NUM,
  MAS_MIER_FLAG_EXIT = 1L << MAS_MIER_FLAG_EXIT_NUM,
  MAS_MIER_FLAG_ASSERT = 1L << MAS_MIER_FLAG_ASSERT_NUM,
  MAS_MIER_FLAG_SLEEP = 1L << MAS_MIER_FLAG_SLEEP_NUM,
  MAS_MIER_FLAG_COLPREF = 1L << MAS_MIER_FLAG_COLPREF_NUM,
} mas_minierr_flag_t;

void minierr_die( int line, const char *func, const char *file, int wraplines, mas_minierr_type_t et, int fexit, unsigned flags, const char *sid,
                  const char *fmt, ... ) __attribute__ ( ( format( __printf__, 9, 10 ) ) );
void minierr_vdie( int line, const char *func, const char *file, int wraplines, mas_minierr_type_t et, int fexit, unsigned flags, const char *sid,
                   const char *fmt, va_list args );
void minierr_disable( mas_minierr_type_t typ );
void minierr_enable( mas_minierr_type_t typ );

#endif
