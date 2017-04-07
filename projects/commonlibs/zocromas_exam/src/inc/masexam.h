#ifndef MAS_MASEXAM_H
# define MAS_MASEXAM_H

# define _uUu_ __attribute__(( unused ))

typedef int ( *masexam_fun_t ) ( int argc, const char *argv[], int nseries, const char *series_suffix, int variant );

typedef struct masexam_call_s masexam_call_t;
typedef struct masexam_do_s masexam_do_t;

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
  int variant;
  const char *stdin_text;
};
struct masexam_call_s
{
  int argc;
  const char **argv;
  masexam_do_t *fundata;
  int variant;
  int iopipe[2];
};

int masexam_test( int argc, const char *argv[], masexam_do_t * funlist );

int masexam_vexam( const char *func, int line, const char *file, int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args );
int masexam_exam( const char *func, int line, const char *file, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... );
void masexam_next_group( void );
void masexam_next( void );
long masexam_tests_count( void );

/* for special mas_strcmp -- includes NULL's  */
# include <mastar/tools/mas_arg_tools.h>

# define EXAMX( q, fmt, ... ) masexam_exam(__func__, __LINE__, __FILE__, \
    		(q), "OK", "Error", fmt, __VA_ARGS__ )

# define EXAM( var, val, fmt ) masexam_exam(__func__, __LINE__, __FILE__, \
    		(val) == (var), "OK", "Error", fmt, var, val )
# define EXAMT( cond, var, val, fmt ) masexam_exam( __func__, __LINE__, __FILE__, \
    		(cond) && (var) == (val), "OK", "Error", fmt, ((cond)?var:((typeof(var)) 0 )), val )
# define EXAMZ(sz, var, val, fmt ) masexam_exam( __func__, __LINE__, __FILE__, \
    		sizeof(var)==sz && var == val, "OK", "Error", fmt, var, val )
# define EXAMS(vars, vals, fmt) \
	{ const char *s=vars;masexam_exam( __func__, __LINE__, __FILE__, s && 0 == mas_strcmp( vals, s ), "OK", "Error", fmt, vals, s ); }
# define EXAMTS(cond, vars, vals, fmt) \
	{ const char *s=(cond)?vars:NULL;masexam_exam( __func__, __LINE__, __FILE__, ( s && 0 == mas_strcmp( vals, s )), "OK", "Error", fmt, vals, s ); }
# define EXAMSN(varn, arrs, valn, vals, fmt) \
  	masexam_exam( __func__, __LINE__, __FILE__, \
	    varn > valn && arrs && arrs[valn] && 0 == mas_strcmp( vals, arrs[valn] ), \
	    "OK", "Error", fmt, vals, varn > valn ? (arrs[valn]) : "?" )

#endif
