#ifndef MASTEST_H
# define MASTEST_H

void mastest_print_allocated( const char *msg, int line, const char *func );
#if 0
int mastest_vexam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args );
int mastest_exam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... );
void mastest_next_group( void );
void mastest_next( void );

# define EXAM( var, val, fmt ) mastest_exam( __LINE__, val == var, "OK", "Error", fmt, val, var );
# define EXAMZ(sz, var, val, fmt ) mastest_exam( __LINE__, sizeof(var)==sz && val == var, "OK", "Error", fmt, val, var /* , val-var, sz */ );
# define EXAMS(vars, vals, fmt)   { const char *s=vars;mastest_exam( __LINE__, s && 0 == mas_strcmp( vals, s ), "OK", "Error", fmt, vals, s?s:"<NULL>" ); }
# define EXAMSN(varn, arrs, valn, vals, fmt) \
  	mastest_exam( __LINE__, \
	    varn > valn && arrs && arrs[valn] && 0 == mas_strcmp( vals, arrs[valn] ), \
	    "OK", "Error", fmt, vals, varn > valn ? (arrs[valn]?arrs[valn]:"<NULL>") : "?" );
#endif

#endif
