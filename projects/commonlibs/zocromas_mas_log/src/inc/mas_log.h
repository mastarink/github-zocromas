#ifndef MAS_LOG_H
#  define MAS_LOG_H

#  include <stdarg.h>
#  include <errno.h>

#  include "mas_log_types.h"


/* int mas_vlog(MAS_PASS_OPTS_DECLARE const char *func, int line, int merrno, const char *fmt, va_list args );     */
/* int mas_vlog_lim(MAS_PASS_OPTS_DECLARE const char *func, int line, int merrno, const char *fmt, va_list args ); */
int mas_log( const char *func, int line, int merrno, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 4, 5 ) ) ) __attribute__ ( ( weak ) );


#  ifdef MAS_NO_LOG
#    define MAS_LOG(...)
#    define MAS_LOGERR(lerrno, ...)
#  else
#    define MAS_LOG(...) { if (&mas_log) mas_log( FL, errno, __VA_ARGS__ ); }
#    define MAS_LOGERR(lerrno, ...) { if (&mas_log) mas_log( FL, lerrno, __VA_ARGS__ ); }
#  endif



#endif
