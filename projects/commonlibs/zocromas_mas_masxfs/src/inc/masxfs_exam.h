#ifndef MASXFS_EXAM_H
# define MASXFS_EXAM_H

typedef int ( *mastest_fun_t ) ( int nseries, const char *series_suffix, int do_fprintf );

struct mastest_do_s
{
  int doit;
  mastest_fun_t func;
  int nseries;
  char *series_suffix;
  int f_print_ok;
  int f_noprint_error;
  int stop_on_error;
  int sleep_on_error;
  int beep_on_error;
  int assert_on_error;
  int do_fprintf;
};
typedef struct mastest_do_s mastest_do_t;

int mastest_test( mastest_do_t * funlist );

int mastest_vexam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args );
int mastest_exam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... );
void mastest_next_group( void );
void mastest_next( void );
int mastest_tests_count( void );

# define EXAM( var, val, fmt ) mastest_exam( __LINE__, (val) == (var), "OK", "Error", fmt, val, var );
# define EXAMT( cond, var, val, fmt ) mastest_exam( __LINE__, (cond) && (val) == (var), "OK", "Error", fmt, val, (cond)?var:((typeof(var)) 0 ) );
# define EXAMZ(sz, var, val, fmt ) mastest_exam( __LINE__, sizeof(var)==sz && val == var, "OK", "Error", fmt, val, var /* , val-var, sz */ );
# define EXAMS(vars, vals, fmt) \
	{ const char *s=vars;mastest_exam( __LINE__, s && 0 == mas_strcmp( vals, s ), "OK", "Error", fmt, vals, s ); }
# define EXAMTS(cond, vars, vals, fmt) \
	{ const char *s=(cond)?vars:NULL;mastest_exam( __LINE__, ( s && 0 == mas_strcmp( vals, s )), "OK", "Error", fmt, vals, s ); }
# define EXAMSN(varn, arrs, valn, vals, fmt) \
  	mastest_exam( __LINE__, \
	    varn > valn && arrs && arrs[valn] && 0 == mas_strcmp( vals, arrs[valn] ), \
	    "OK", "Error", fmt, vals, varn > valn ? (arrs[valn]) : "?" );

#endif
