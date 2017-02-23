#ifndef MAS_MASEXAM_H
# define MAS_MASEXAM_H

typedef int ( *masexam_fun_t ) ( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );

struct masexam_do_s
{
  int doit;
  masexam_fun_t func;
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
typedef struct masexam_do_s masexam_do_t;

int masexam_test( int argc, const char *argv[], masexam_do_t * funlist );

int masexam_vexam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args );
int masexam_exam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... );
void masexam_next_group( void );
void masexam_next( void );
int masexam_tests_count( void );

# define EXAM( var, val, fmt ) masexam_exam( __LINE__, (val) == (var), "OK", "Error", fmt, val, var );
# define EXAMT( cond, var, val, fmt ) masexam_exam( __LINE__, (cond) && (val) == (var), "OK", "Error", fmt, val, (cond)?var:((typeof(var)) 0 ) );
# define EXAMZ(sz, var, val, fmt ) masexam_exam( __LINE__, sizeof(var)==sz && val == var, "OK", "Error", fmt, val, var /* , val-var, sz */ );
# define EXAMS(vars, vals, fmt) \
	{ const char *s=vars;masexam_exam( __LINE__, s && 0 == mas_strcmp( vals, s ), "OK", "Error", fmt, vals, s ); }
# define EXAMTS(cond, vars, vals, fmt) \
	{ const char *s=(cond)?vars:NULL;masexam_exam( __LINE__, ( s && 0 == mas_strcmp( vals, s )), "OK", "Error", fmt, vals, s ); }
# define EXAMSN(varn, arrs, valn, vals, fmt) \
  	masexam_exam( __LINE__, \
	    varn > valn && arrs && arrs[valn] && 0 == mas_strcmp( vals, arrs[valn] ), \
	    "OK", "Error", fmt, vals, varn > valn ? (arrs[valn]) : "?" );

#endif
