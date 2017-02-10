#ifndef MASTEST_H
# define MASTEST_H

int mastest_vexam( int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args );
int mastest_exam( int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... );
void mastest_series( int nseries, const char *suff );
void mastest_next_group( void );
void mastest_next( void );

#endif
