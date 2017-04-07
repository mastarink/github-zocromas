#define R_GOOD(_r) (!(_r))
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>
#include <libgen.h>
#include <pthread.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/regerr/masregerr.h>
#include <mastar/regerr/masregerr_defs.h>

/* #include "masxfs_defs.h" */
/* #include "masxfs_structs.h" */

#include <mastar/minierr/minierr.h>

#include "masexam.h"

int variant = 0;
int beep_on_error = 1;
int assert_on_error = 0;
int stop_on_error = 0;
int sleep_on_error = 0;
int f_print_ok = 0;
int f_print_error = 1;
static int series_seq = 0;
static long tests_count = 0, tests_count_good = 0, tests_count_bad = 0;
static int test_series = 0, test_group = 0, test_seq = 0;
static const char *test_series_suffix = NULL;

static int exam_silent = 0;

static void destructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( destructor( 2001 ) ) );
static void
destructor_main( int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ), char **envp __attribute__ ( ( unused ) ) )
{
  if ( !exam_silent && tests_count )
    fprintf( stderr, "**** \x1b[0;1;44;37m* * * * TESTS DONE: %ld (OK:%ld / Fail:%ld) * * * *\x1b[0m ****\n\n", tests_count, tests_count_good,
             tests_count_bad );
}

long
masexam_tests_count( void )
{
  return tests_count;
}

static void
masexam_series( int nseries, const char *suff, int variant )
{
  test_series = nseries;
  test_series_suffix = suff;
  test_group = 0;
  test_seq = 0;
  if ( variant >= 0 )
  {
    if ( !series_seq && !tests_count )
      fprintf( stderr, "\n\n\x1b[0;1;44;37mTESTS\x1b[0m:\n" );
  /* fprintf( stderr, "\x1b[0;1;46;37m<*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*>\x1b[0m:\n" ); */
    fprintf( stderr, "*** \x1b[0;1;46;37mseries %d%-20s\x1b[0;1;104;33m * {total:%ld; total good:%ld; total bad: %ld;} (BEFORE it) \x1b[0m\n",
             test_series, test_series_suffix, tests_count, tests_count_good, tests_count_bad );
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
masexam_vexam( const char *func
               __attribute__ ( ( unused ) ), int line, const char *file, int cond, const char *goodmsg, const char *badmsg, const char *fmt,
               va_list args )
{
  masexam_next(  );
  if ( cond )
    tests_count_good++;
  else
    tests_count_bad++;
  tests_count++;
  if ( ( cond && f_print_ok ) || ( !cond && f_print_error ) )
  {
    char *filepath = mas_strdup( file );
    char *fn = basename( filepath );

    fprintf( stderr, "\x1b[0;1;44;33m%s:%d. %4ld  *** [%d%s.%d.%-2d] %-10s%s : ", fn, line, tests_count, test_series,
             test_series_suffix ? test_series_suffix : "", test_group, test_seq, cond ? goodmsg : badmsg, !cond && beep_on_error ? "\x07" : "" );
    mas_free( filepath );
    vfprintf( stderr, fmt, args );
    fprintf( stderr, "\x1b[0m\n" );
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
masexam_exam( const char *func, int line, const char *file, int cond, const char *goodmsg, const char *badmsg, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  cond = masexam_vexam( func, line, file, cond, goodmsg, badmsg, fmt, args );
  va_end( args );

  return cond;
}

int
masexam_test_fork( int argc, const char *argv[], masexam_fun_t func, int nseries, const char *series_suffix, int variant, const char *stdin_text )
{
  int r = 0;
  int filedes[2];                                                    /* pipe */
  int file_rd = fileno( stdin );                                     /* filedescriptor = 0 */
  int file_wr = fileno( stdout );                                    /* filedescriptor = 1 */
  int pipe_rd;
  int pipe_wr;

  pipe( filedes );

  pipe_wr = filedes[1];
  pipe_rd = filedes[0];

/* */

  if ( fork(  ) )
  {
  /* child */
  /* stdin replaced by pipe output */
    dup2( pipe_rd, file_rd );

    close( pipe_wr );                                                /* close part of pipe unneeded */

    r = func( argc, argv, nseries, series_suffix, variant );

    close( pipe_rd );
  /* close(file_rd); */
  /* close(file_wr); */
  }
  else
  {
    exam_silent = 1;
  /* parent */
  /* stdout replaced by pipe input */
    dup2( pipe_wr, file_wr );

    close( pipe_rd );                                                /* close part of pipe unneeded */

    if ( stdin_text )
      fputs( stdin_text, stdout );

    close( pipe_wr );
  /* close(file_rd); */
  /* close(file_wr); */
    masregerrs_delete_default( NULL );
  }
  return r;
}

int
masexam_test_call( int argc, const char *argv[], masexam_fun_t func, int nseries, const char *series_suffix, int variant )
{
  return func( argc, argv, nseries, series_suffix, variant );
}

void *
masexam_test_call_th( void *ptr )
{
  int saved = 0;

  masexam_call_t *call = ( masexam_call_t * ) ptr;

  if ( call->fundata->stdin_text )
  {
    saved = dup( STDIN_FILENO );
    dup2( call->iopipe[0], STDIN_FILENO );
  }
  masexam_test_call( call->argc, call->argv, call->fundata->func, call->fundata->nseries, call->fundata->series_suffix, call->variant );
  if ( call->fundata->stdin_text )
  {
    close( call->iopipe[0] );
    dup2( saved, STDIN_FILENO );
  }
  return ptr;
}

void *
masexam_test_wtext_th( void *ptr )
{
  int saved = 0;

  masexam_call_t *call = ( masexam_call_t * ) ptr;

  if ( call->fundata->stdin_text )
  {
    saved = dup( STDOUT_FILENO );
    dup2( call->iopipe[1], STDOUT_FILENO );

    fputs( call->fundata->stdin_text, stdout );
    fflush( stdout );
    close( call->iopipe[1] );
    dup2( saved, STDOUT_FILENO );
  }
  return ptr;
}

int
masexam_test( int argc, const char *argv[], masexam_do_t * funlist )
{
  for ( unsigned ntest = 0; funlist[ntest].func; ntest++ )
  {
    if ( funlist[ntest].doit )
    {
      masexam_series( funlist[ntest].nseries, funlist[ntest].series_suffix, variant );
      variant += funlist[ntest].variant;
      f_print_ok += funlist[ntest].f_print_ok;
      f_print_ok -= funlist[ntest].f_noprint_error;
      assert_on_error += funlist[ntest].assert_on_error;
      beep_on_error += funlist[ntest].beep_on_error;
      sleep_on_error += funlist[ntest].sleep_on_error;
      stop_on_error += funlist[ntest].stop_on_error;
      {
        masexam_call_t joint_data _uUu_ = {
          .argc = argc,
          .argv = argv,
          .fundata = &funlist[ntest],
          .variant = variant,
        };
#if 0
        if ( funlist[ntest].stdin_text )
          masexam_test_fork( argc, argv, funlist[ntest].func, funlist[ntest].nseries, funlist[ntest].series_suffix, variant,
                             funlist[ntest].stdin_text );
        else
          masexam_test_call( argc, argv, funlist[ntest].func, funlist[ntest].nseries, funlist[ntest].series_suffix, variant );
#else
        {
          int r1 = 0, r2 = 0;
          pthread_t thread1 = 0;
          pthread_t thread2 = 0;

          pipe( joint_data.iopipe );
          r1 = pthread_create( &thread1, NULL, masexam_test_call_th, ( void * ) &joint_data );
          RGESRM( r1, "pthread_join %s", strerror( r1 ) );

          r2 = pthread_create( &thread2, NULL, masexam_test_wtext_th, ( void * ) &joint_data );
          RGESRM( r1, "pthread_join %s", strerror( r2 ) );

          r1 = pthread_join( thread1, NULL );
          RGESRM( r1, "pthread_join %s", strerror( r1 ) );

          r2 = pthread_join( thread2, NULL );
          RGESRM( r1, "pthread_join %s", strerror( r2 ) );
        }
      }
#endif
      stop_on_error -= funlist[ntest].stop_on_error;
      sleep_on_error -= funlist[ntest].sleep_on_error;
      beep_on_error -= funlist[ntest].beep_on_error;
      assert_on_error -= funlist[ntest].assert_on_error;
      f_print_ok += funlist[ntest].f_noprint_error;
      f_print_ok -= funlist[ntest].f_print_ok;
      variant -= funlist[ntest].variant;
    }
    masregerrs_delete_default( NULL );
  }
#if 0
  masexam_next_group(  );
# define TOTAL_TESTS 2 - 1
  masexam_exam( 0, tests_count == TOTAL_TESTS, "OK", "Error", "tests_count=%d ? %d", tests_count, TOTAL_TESTS );
#endif
  return tests_count;
}
