#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_structs.h"

#include "masxfs_error.h"

#include "masxfs_exam.h"

int do_fprintf = 0;
int beep_on_error = 1;
int assert_on_error = 1;
int stop_on_error = 0;
int sleep_on_error = 0;
int f_print_ok = 0;
int f_print_error = 1;
static int series_seq = 0;
static int tests_count = 0, tests_count_good = 0, tests_count_bad = 0;
static int test_series = 0, test_group = 0, test_seq = 0;
static const char *test_series_suffix = NULL;

static void destructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( destructor( 2001 ) ) );
static void
destructor_main( int argc _uUu_, char **argv _uUu_, char **envp _uUu_ )
{
  if ( tests_count )
    fprintf( stderr, "**** \x1b[0;1;44;37m* * * * TESTS DONE: %d (OK:%d / Fail:%d) * * * *\x1b[0m ****\n\n", tests_count, tests_count_good,
             tests_count_bad );
}

int
masexam_tests_count( void )
{
  return tests_count;
}

static void
masexam_series( int nseries, const char *suff, int do_fprintf )
{
  test_series = nseries;
  test_series_suffix = suff;
  test_group = 0;
  test_seq = 0;
  if ( do_fprintf >= 0 )
  {
    if ( !series_seq && !tests_count )
      fprintf( stderr, "\n\n\x1b[0;1;44;37mTESTS\x1b[0m:\n" );
    fprintf( stderr, "*** series %d%-20s\tBEFORE it: {total:%d;\ttotal good:%d;\ttotal bad: %d;}\n", test_series, test_series_suffix, tests_count,
             tests_count_good, tests_count_bad );
  }
  series_seq++;
}

void
masexam_next( void )
{
  test_seq++;
}

void
masexam_next_group( void )
{
  test_group++;
  test_seq = 0;
}

int
masexam_vexam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, va_list args )
{
  masexam_next(  );
  if ( cond )
    tests_count_good++;
  else
    tests_count_bad++;
  tests_count++;
  if ( ( cond && f_print_ok ) || ( !cond && f_print_error ) )
  {
    fprintf( stderr, "%d. %4d\t**** [%d%s.%d.%-2d] %-10s%s\t", line, tests_count, test_series, test_series_suffix ? test_series_suffix : "",
             test_group, test_seq, cond ? goodmsg : badmsg, !cond && beep_on_error ? "\x07" : "" );
    vfprintf( stderr, fmt, args );
    fprintf( stderr, "\n" );
  }
  if ( !cond && sleep_on_error )
    sleep( 2 );
  if ( !cond && stop_on_error )
    DIE( "Stop on error %d", cond );
  if ( !cond && assert_on_error )
    assert( 0 );
  return cond;
}

int
masexam_exam( int line, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  cond = masexam_vexam( line, cond, goodmsg, badmsg, fmt, args );
  va_end( args );

  return cond;
}

int
masexam_test( masexam_do_t * funlist )
{
  for ( unsigned ntest = 0; funlist[ntest].func; ntest++ )
  {
    if ( funlist[ntest].doit )
    {
      masexam_series( funlist[ntest].nseries, funlist[ntest].series_suffix, do_fprintf );
      do_fprintf += funlist[ntest].do_fprintf;
      f_print_ok += funlist[ntest].f_print_ok;
      f_print_ok -= funlist[ntest].f_noprint_error;
      assert_on_error += funlist[ntest].assert_on_error;
      beep_on_error += funlist[ntest].beep_on_error;
      sleep_on_error += funlist[ntest].sleep_on_error;
      stop_on_error += funlist[ntest].stop_on_error;

      funlist[ntest].func( funlist[ntest].nseries, funlist[ntest].series_suffix, do_fprintf );

      stop_on_error -= funlist[ntest].stop_on_error;
      sleep_on_error -= funlist[ntest].sleep_on_error;
      beep_on_error -= funlist[ntest].beep_on_error;
      assert_on_error -= funlist[ntest].assert_on_error;
      f_print_ok += funlist[ntest].f_noprint_error;
      f_print_ok -= funlist[ntest].f_print_ok;
      do_fprintf -= funlist[ntest].do_fprintf;
    }
  }
#if 0
  masexam_next_group(  );
# define TOTAL_TESTS 2 - 1
  masexam_exam( 0, tests_count == TOTAL_TESTS, "OK", "Error", "tests_count=%d ? %d", tests_count, TOTAL_TESTS );
#endif
  return tests_count;
}
