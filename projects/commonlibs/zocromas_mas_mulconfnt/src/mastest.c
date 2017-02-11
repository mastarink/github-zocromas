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

int test_popt( int argc, const char *argv[] );
int test_popt1( int argc, const char *argv[] );
int test_0( int argc, const char *argv[] );
int test_1( int argc, const char *argv[] );
int test_1u( int argc, const char *argv[] );
int test_2( int argc, const char *argv[] );
int test_2a( int argc, const char *argv[] );

int do_fprintf = 0;
int f_print_ok = 0;
int f_print_error = 1;
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

void
mastest_series( int nseries, const char *suff )
{
  test_series = nseries;
  test_series_suffix = suff;
  test_group = 0;
  test_seq = 0;
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
mastest_vexam( int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args )
{
  mastest_next(  );
  if ( !tests_count )
    fprintf( stderr, "\n\n\x1b[0;1;44;37mTESTS\x1b[0m:\n" );
  if ( cond )
    tests_count_good++;
  else
    tests_count_bad++;
  tests_count++;
  if ( ( cond && f_print_ok ) || ( !cond && f_print_error ) )
  {
    fprintf( stderr, "%4d\t**** [%d%s.%d.%d] %-30s --\t", tests_count, test_series, test_series_suffix ? test_series_suffix : "", test_group,
             test_seq, cond ? goodmsg : badmsg );
    vfprintf( stderr, fmt, args );
    fprintf( stderr, "\n" );
  }
  return cond;
}

int
mastest_exam( int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  cond = mastest_vexam( cond, goodmsg, badmsg, fmt, args );
  va_end( args );

  return cond;
}

/* getopt_long */
int
main( int argc, const char *argv[] )
{
// mas_strdup( "abrakadabra" );

  mastest_series( 0, "popt" );
  if ( 0 )
    test_popt( argc, argv );
  mastest_series( 1, "popt" );
  if ( 0 )
    test_popt1( argc, argv );

  mastest_series( 0, "" );
  if ( 0 )
    test_0( argc, argv );

  mastest_series( 1, "" );
  f_print_ok++;
  if ( 1 )
    test_1( argc, argv );
  f_print_ok--;
  mastest_series( 1, "u" );
  if ( 1 )
    test_1u( argc, argv );

  mastest_series( 2, "" );
  if ( 1 )
    test_2( argc, argv );
  mastest_series( 2, "a" );
  if ( 1 )
    test_2a( argc, argv );

  return 0;
}
