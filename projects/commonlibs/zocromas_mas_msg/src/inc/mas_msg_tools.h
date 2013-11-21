#ifndef MAS_MSG_TOOLS_H
#  define MAS_MSG_TOOLS_H

#  include <errno.h>

int mas_msg_set_file( const char *path, int force );

/* int mas_msg( const char *func, int line, int allow, int is_trace, int details, int msgcolor, const char *prefix_fmt,           */
/*              const char *prefix, const char *suffix, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 10, 11 ) ) ) */
/*       __attribute__ ( ( weak ) );                                                                                              */

int mas_msg( const char *func, int line, mas_msg_type_t msgt, int allow, int details, const char *prefix_fmt, const char *prefix,
             const char *suffix, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 9, 10 ) ) ) __attribute__ ( ( weak ) );
int
mas_msgv( const char *func, int line, mas_msg_type_t msgt, int allow, int details,
          const char *prefix_fmt, const char *prefix, const char *suffix, const char *fmt, va_list args );


void *mas_fatal( void );
int mas_error( const char *func, int line, int merrno, int *perrno, int *psaveerrno, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 6, 7 ) ) ) __attribute__ ( ( weak ) );
void _mas_perr( const char *func, int line, int merrno, int *perrno ) __attribute__ ( ( weak ) );


void mas_clear_last_error( void );
const mas_std_error_t *mas_get_last_error( void );


/* #include <mastar/types/mas_common_cdefs.h> */
#endif
