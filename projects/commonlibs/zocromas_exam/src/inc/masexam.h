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

int masexam_count( void );
int masexam_test( int argc, const char *argv[], masexam_do_t * funlist );

int masexam_vexam( const char *func, int line, const char *file, int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args );
int masexam_exam( const char *func, int line, const char *file, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... );
void masexam_next_group( void );
void masexam_next( void );
long masexam_tests_count( void );

int masexam_fds( void );

/* for special mas_strcmp -- includes NULL's  */
# include <mastar/tools/mas_arg_tools.h>

# define EXAMX( _q, _fmt, ... ) masexam_exam(__func__, __LINE__, __FILE__, \
    		(_q), "Test OK", "Test Error", _fmt, __VA_ARGS__ )

# define EXAMT( _cond, _var, _val, _fmt ) \
	{ \
	    typeof( _val ) val=(_val); \
	    typeof( _var ) var=(_var); \
	    typeof( _cond ) cond=(_cond); \
	    EXAMX(cond && var == val, _fmt, (cond?var:((typeof(var)) 0 )), val); \
	    /* masexam_exam( __func__, __LINE__, __FILE__, (cond) && (var) == (val), "OK", "Error", _fmt, ((cond)?var:((typeof(var)) 0 )), val ); */ \
	}
# define EXAM( _var, _val, _fmt )  EXAMT( 1, _var, _val, _fmt )

# define EXAMZ(sz, var, val, _fmt ) EXAMX( sizeof(var)==sz && var == val, _fmt, var, val )
   /* masexam_exam( __func__, __LINE__, __FILE__, sizeof(var)==sz && var == val, "OK", "Error", _fmt, var, val ) */
# define EXAMTS(_cond, _vars, _vals, _fmt) \
	{ \
	  typeof( _vars ) vars=(_cond)?(_vars):((typeof(_vars)) 0 ); \
	  typeof( _vals ) vals=(_vals); \
	  EXAMX( vars && 0 == mas_strcmp( vals, vars ), _fmt, vals, vars ); \
	  /* masexam_exam( __func__, __LINE__, __FILE__, ( vars && 0 == mas_strcmp( vals, vars )), "OK", "Error", _fmt, vals, vars ); */ \
	}
# define EXAMS(_vars, _vals, _fmt)  EXAMTS(1, _vars, _vals, _fmt)

# define EXAMSN(_varn, _arrs, _valn, _vals, _fmt) \
	{ \
	  typeof( _valn ) valn=(_valn); \
	  typeof( _varn ) varn=(_varn); \
	  typeof( _vals ) vals=(_vals); \
	  typeof( _arrs ) arrs=(_arrs); \
	  EXAMX( varn > valn && arrs && arrs[valn] && 0 == mas_strcmp( vals, arrs[valn] ), _fmt, vals, varn > valn ? (arrs[valn]) : "?" ); \
    /* masexam_exam( __func__, __LINE__, __FILE__, varn > valn && arrs && arrs[valn] && 0 == mas_strcmp( vals, arrs[valn] ), "OK", "Error", _fmt, vals, varn > valn ? (arrs[valn]) : "?" ); */ \
	}

#endif
