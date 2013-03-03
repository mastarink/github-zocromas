#ifndef MAS_LOG_H
#  define MAS_LOG_H

#  include <stdarg.h>
#  include <errno.h>

#  include "mas_log_types.h"

void mas_log_delete_loginfo( mas_loginfo_t * li );

/* int mas_vlog( const char *func, int line, int merrno, const char *fmt, va_list args );     */
/* int mas_vlog_lim( const char *func, int line, int merrno, const char *fmt, va_list args ); */
int mas_log( const char *func, int line, int merrno, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 4, 5 ) ) )
      __attribute__ ( ( weak ) );
int mas_log_unlim( const char *func, int line, int merrno, const char *fmt, ... );

void mas_log_clean_queue( void );

#  ifdef MAS_NO_LOG
#    define MAS_LOG(...)
#    define MAS_LOGERR(lerrno, ...)
#  else
#    define MAS_LOG(...) { if (&mas_log) mas_log( FL, errno, __VA_ARGS__ ); }
#    define MAS_LOGERR(lerrno, ...) { if (&mas_log) mas_log( FL, lerrno, __VA_ARGS__ ); }
#  endif



#endif
