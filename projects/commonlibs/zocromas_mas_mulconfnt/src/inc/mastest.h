#ifndef MASTEST_H
# define MASTEST_H

int mastest_vexam( const char *name, int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args );
int mastest_exam( const char *name, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... );

#endif
