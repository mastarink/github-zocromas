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

#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/regerr/masregerr.h>
#include <mastar/regerr/masregerr_defs.h>

/* #include "masxfs_defs.h" */
/* #include "masxfs_structs.h" */

#include <mastar/minierr/minierr.h>

#include "masexam.h"

#define MX_CLOSE(_fd) { int r=0;if ( _fd ) r = close( _fd ); _fd=0; if ( r < 0 ) WARN( "close error %s", strerror( errno ) ); }
#define MX_FPUTS_O(_text) { int r=0; if (_text) r=fputs(_text, stdout); if ( r == EOF ) WARN( "fputs error %s", strerror( errno ) ); }

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
    int r = 0;

  /* child */
  /* stdin replaced by pipe output */
    r = dup2( pipe_rd, file_rd );
    if ( r < 0 )
      WARN( "dup2 error %s", strerror( errno ) );

    MX_CLOSE( pipe_wr );                                             /* close part of pipe unneeded */

    r = func( argc, argv, nseries, series_suffix, variant );

    MX_CLOSE( pipe_rd );                                             /* close part of pipe unneeded */
  /* close(file_rd); */
  /* close(file_wr); */
  }
  else
  {
    int r = 0;

    exam_silent = 1;
  /* parent */
  /* stdout replaced by pipe input */
    r = dup2( pipe_wr, file_wr );
    if ( r < 0 )
      WARN( "dup2 error %s", strerror( errno ) );

    MX_CLOSE( pipe_rd );                                             /* close part of pipe unneeded */

    MX_FPUTS_O( stdin_text );

    MX_CLOSE( pipe_wr );                                             /* close part of pipe unneeded */
  /* close(file_rd); */
  /* close(file_wr); */
    masregerrs_delete_default( NULL );
  }
  return r;
}

int
masexam_test_call( int argc, const char *argv[], masexam_fun_t func, int nseries, const char *series_suffix, int variant )
{
  int r = 0;
  int ifds = masexam_fds(  );

  if ( ifds != 2 )
    WARN( "--- FDs:%d", ifds );
  r = func( argc, argv, nseries, series_suffix, variant );
  int ifds2 = masexam_fds(  );

  if ( ifds2 != 2 )
    WARN( "--- FDs:%d", ifds2 );
  if ( ifds2 != ifds )
  {
    WARN( "Unbalanced open/close" );
  }
  return r;
}

void *
masexam_test_call_th( void *ptr )
{
  int saved = 0;
  int ifds = masexam_fds(  );

  if ( 1 )
  {
    /* FIXME This is WRONG; needs something like mutex */
/* struct timespec ts={0,999999999}; */
    struct timespec ts = { 0, 100000 };
    struct timespec tsr = { 0 };
    nanosleep( &ts, &tsr );
  }
  {
    masexam_call_t *call = ( masexam_call_t * ) ptr;

    if ( ifds != 2 )
      WARN( "--- FDs:%d", ifds );

    if ( call->fundata->stdin_text )
    {
      int r = 0;

      if ( ifds != 2 )
        WARN( "--- FDs:%d", ifds );

      saved = dup( STDIN_FILENO );
      if ( saved < 0 )
        WARN( "dup error %s", strerror( errno ) );
      r = dup2( call->iopipe[0], STDIN_FILENO );
      if ( r < 0 )
        WARN( "dup2 error %s", strerror( errno ) );
    }
    ifds = masexam_fds(  );
    if ( ifds != 2 )
      WARN( "--- FDs:%d", ifds );
    masexam_test_call( call->argc, call->argv, call->fundata->func, call->fundata->nseries, call->fundata->series_suffix, call->variant );
    ifds = masexam_fds(  );
    if ( ifds != 2 )
      WARN( "--- FDs:%d", ifds );
    if ( call->fundata->stdin_text )
    {
      int r = 0;

      MX_CLOSE( call->iopipe[0] );                                   /* close part of pipe unneeded */
      r = dup2( saved, STDIN_FILENO );
      if ( r < 0 )
        WARN( "dup2 error %s", strerror( errno ) );
      MX_CLOSE( saved );
    }
  }
  return ptr;
}

void *
masexam_test_wtext_th( void *ptr )
{
  int saved = 0;

#if 0
  if ( 0 )
  {
/* struct timespec ts={0,999999999}; */
    struct timespec ts = { 0, 20000000 };
    struct timespec tsr = { 0 };
    nanosleep( &ts, &tsr );
  }
#endif
  {
    masexam_call_t *call = ( masexam_call_t * ) ptr;

    if ( call->fundata->stdin_text )
    {
      int r = 0;

      saved = dup( STDOUT_FILENO );
      if ( saved < 0 )
        WARN( "dup error %s", strerror( errno ) );
      r = dup2( call->iopipe[1], STDOUT_FILENO );
      if ( r < 0 )
        WARN( "dup2 error %s", strerror( errno ) );

      MX_FPUTS_O( call->fundata->stdin_text );
      r = fflush( stdout );
      if ( r == EOF )
        WARN( "fflush error %s", strerror( errno ) );
      MX_CLOSE( call->iopipe[1] );
      r = dup2( saved, STDOUT_FILENO );
      if ( r < 0 )
        WARN( "dup2 error %s", strerror( errno ) );
      r = 0;
      MX_CLOSE( saved );
    }
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
          MX_CLOSE( joint_data.iopipe[0] );
          MX_CLOSE( joint_data.iopipe[1] );

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

int
masexam_fds( void )
{
  int fdcnt = 0;
  int max_fd_number = 0;

/* struct rlimit rlimits; */

  max_fd_number = getdtablesize(  );
/* getrlimit( RLIMIT_NOFILE, &rlimits ); */
  for ( int i = 3; i <= max_fd_number; i++ )
  {
    int r = 0;
    struct stat st = { 0 };
    errno = 0;
    r = fstat( i, &st );
    if ( !r && errno != EBADF )
      fdcnt++;
  }
  return fdcnt;
}
