#ifndef MAS_MSG_TOOLS_H
#  define MAS_MSG_TOOLS_H

int mas_msg_set_file( const char *path );

int mas_msg( const char *func, int line, int allow, int is_trace, int details, int msgcolor, const char *prefix_fmt,
             const char *prefix, const char *suffix, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 10, 11 ) ) )
      __attribute__ ( ( weak ) );
void *mas_fatal( void );
int mas_error( const char *func, int line, int merrno, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 4, 5 ) ) )
      __attribute__ ( ( weak ) );
void mas_perr( const char *func, int line );

#endif
