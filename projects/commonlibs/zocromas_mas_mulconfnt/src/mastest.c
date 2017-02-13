#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"

int do_fprintf = 0;
int f_print_ok = 0;
int f_print_error = 1;
static int series_seq = 0;
static int tests_count = 0, tests_count_good = 0, tests_count_bad = 0;
static int test_series = 0, test_group = 0, test_seq = 0;
static const char *test_series_suffix = NULL;

extern void *__libc_malloc( size_t size );

int malloc_hook_active = 0;
void *
my_malloc_hook( size_t size, void *caller _uUu_ )
{
  void *result;

// deactivate hooks for logging
  malloc_hook_active = 0;

  result = malloc( size );

// do logging
//  [ ...]
  fprintf( stderr, "Wow\n" );
// reactivate hooks
  malloc_hook_active = 1;

  return result;
}

void *
malloc( size_t size )
{
  void *caller _uUu_ = __builtin_return_address( 0 );

  if ( malloc_hook_active )
    return my_malloc_hook( size, caller );
  return __libc_malloc( size );
}

void
mastest_print_allocated( const char *msg, int line, const char *func )
{
  struct mallinfo mi;

  mi = mallinfo(  );
  fprintf( stderr, "\n\x1b[0;1;44;35m%s %d bytes at %d:%s\x1b[0m\n", msg, mi.uordblks, line, func );
}

static void constructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( constructor( 2001 ) ) );
static void
constructor_main( int argc _uUu_, char **argv _uUu_, char **envp _uUu_ )
{
  mastest_print_allocated( "Allocated", __LINE__, __func__ );
  mallopt( M_CHECK_ACTION, 1 );
/* configure my zocromas_mas_wrap library (malloc/free wrapper) not to print memory usage map; may be enabled later */
#ifdef MAS_TRACEMEM
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 0;
    }
  }

#endif
  fprintf( stderr, "START\n" );
}

static void destructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( destructor( 2001 ) ) );
static void
destructor_main( int argc _uUu_, char **argv _uUu_, char **envp _uUu_ )
{
  if ( tests_count )
    fprintf( stderr, "**** \x1b[0;1;44;37m* * * * TESTS DONE: %d (OK:%d / Fail:%d) * * * *\x1b[0m ****\n\n", tests_count, tests_count_good,
             tests_count_bad );

  mastest_print_allocated( "Still allocated", __LINE__, __func__ );
//malloc_info(0, stderr);
//malloc_stats(  );
}

static void
mastest_series( int nseries, const char *suff )
{
  test_series = nseries;
  test_series_suffix = suff;
  test_group = 0;
  test_seq = 0;
  if ( !series_seq && !tests_count )
    fprintf( stderr, "\n\n\x1b[0;1;44;37mTESTS\x1b[0m:\n" );
  fprintf( stderr, "*** series %d%-20s\tBEFORE it: {total:%d;\ttotal good:%d;\ttotal bad: %d;}\n", test_series, test_series_suffix, tests_count, tests_count_good,
           tests_count_bad );
  series_seq++;
}

void
mastest_next( void )
{
  test_seq++;
}

void
mastest_next_group( void )
{
  test_group++;
  test_seq = 0;
}

int
mastest_vexam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args )
{
  mastest_next(  );
  if ( cond )
    tests_count_good++;
  else
    tests_count_bad++;
  tests_count++;
  if ( ( cond && f_print_ok ) || ( !cond && f_print_error ) )
  {
    fprintf( stderr, "%d. %4d\t**** [%d%s.%d.%-2d] %-10s\t", line, tests_count, test_series, test_series_suffix ? test_series_suffix : "",
             test_group, test_seq, cond ? goodmsg : badmsg );
    vfprintf( stderr, fmt, args );
    fprintf( stderr, "\n" );
  }
  return cond;
}

int
mastest_exam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  cond = mastest_vexam( line, cond, goodmsg, badmsg, fmt, args );
  va_end( args );

  return cond;
}

/* getopt_long */
int
main( int argc, const char *argv[] )
{
// mas_strdup( "abrakadabra" );
  typedef int ( *test_fun_t ) ( int argc, const char *argv[], int nseries, const char *series_suffix );
  struct dotest_s
  {
    int doit;
    test_fun_t func;
    int nseries;
    char *series_suffix;
    int f_print_ok;
    int f_noprint_error;
  };
  typedef struct dotest_s dotest_t;

  int test_popt( int argc, const char *argv[], int nseries, const char *series_suffix );
  int test_popt1( int argc, const char *argv[], int nseries, const char *series_suffix );
  int test_0( int argc, const char *argv[], int nseries, const char *series_suffix );
  int test_1( int argc, const char *argv[], int nseries, const char *series_suffix );
  int test_1enf( int argc, const char *argv[], int nseries, const char *series_suffix );
  int test_1u( int argc, const char *argv[], int nseries, const char *series_suffix );
  int test_2( int argc, const char *argv[], int nseries, const char *series_suffix );
  int test_2a( int argc, const char *argv[], int nseries, const char *series_suffix );
  int test_3( int argc, const char *argv[], int nseries, const char *series_suffix );
  int test_3a( int argc, const char *argv[], int nseries, const char *series_suffix );
  int test_3q( int argc, const char *argv[], int nseries, const char *series_suffix );

  dotest_t funlist[] _uUu_ = {
    {0, test_popt, 0, "popt"},
    {0, test_popt1, 1, "popt"},
    {0, test_0, 0, ""},
    {1, test_1, 1, ""},
    {1, test_1u, 1, "u"},
    {1, test_1enf, 1, "enf"},
    {1, test_2, 2, ""},
    {1, test_2a, 2, "a"},
    {1, test_3, 3, ""},
    {1, test_3a, 3, "a"},
    {1, test_3q, 3, "q"},
  };
  for ( int u = 0; u < argc; u++ )
  {
    fprintf( stderr, "argv[%d]=%s\n", u, argv[u] );
  }
  for ( unsigned ntest = 0; ntest < sizeof( funlist ) / sizeof( funlist[0] ); ntest++ )
  {
    if ( funlist[ntest].doit )
    {
      mastest_series( funlist[ntest].nseries, funlist[ntest].series_suffix );
      f_print_ok += funlist[ntest].f_print_ok;
      f_print_ok -= funlist[ntest].f_noprint_error;
      funlist[ntest].func( argc, argv, funlist[ntest].nseries, funlist[ntest].series_suffix );
      f_print_ok += funlist[ntest].f_noprint_error;
      f_print_ok -= funlist[ntest].f_print_ok;
    }
  }
  return 0;
}
