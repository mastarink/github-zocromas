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

# define DIE( ...)         minierr_die( __LINE__, __func__, __FILE__ , 34, MAS_MIER_X         ,  "DIE", __VA_ARGS__ )
# define SDIE( ...)        minierr_die( __LINE__, __func__, __FILE__ , 37, MAS_MIER_SX        ,  "DIE", __VA_ARGS__ )
# define ADIE( ...)        minierr_die( __LINE__, __func__, __FILE__ , 37, MAS_MIER_AX        ,  "DIE", __VA_ARGS__ )
# define NIMP( ...)        minierr_die( __LINE__, __func__, __FILE__ , 14, MAS_MIER_NI        ,  "NOT IMPLEMENTED", __VA_ARGS__ )
# define WARN( ...)        minierr_die( __LINE__, __func__, __FILE__,   0, MAS_MIER_W         , "WARN", __VA_ARGS__ )
# define SWARN( ...)       minierr_die( __LINE__, __func__, __FILE__,   0, MAS_MIER_SW        , "WARN", __VA_ARGS__ )
# define INFO( ...)        minierr_die( __LINE__, __func__, __FILE__,   0, MAS_MIER_I         , "INFO", __VA_ARGS__ )
# define MARK( _mark, ...) minierr_die( __LINE__, __func__, __FILE__,   0, MAS_MIER_ALL       ,  _mark, __VA_ARGS__ )
# define RDIE( ...)        minierr_die( __LINE__, __func__, __FILE__,  24, MAS_MIER_X         ,  "DIE", __VA_ARGS__ )
# define RWARN( ...)       minierr_die( __LINE__, __func__, __FILE__,   0, MAS_MIER_ALL       , "WARN", __VA_ARGS__ )

# define QR(r)		{ if (r<0) RWARN("R: %d; errno:%d:%s", r, errno, strerror(errno)); }

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

void minierr_die( int line, const char *func, const char *file, int fexit, unsigned flags, const char *sid, const char *fmt, ... )
        __attribute__ ( ( format( __printf__, 7, 8 ) ) );
void minierr_vdie( int line, const char *func, const char *file, int fexit, unsigned flags, const char *sid, const char *fmt, va_list args );

#endif
