#define R_GOOD(_r) ((_r)>=0)
#include "masxfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/exam/masexam.h>

#include "masxfs_structs.h"

#include "masxfs_scan.h"

#include "mastest.h"

char *gs0 __attribute__ ( ( section( "DABRA" ) ) ) = "abrakadabra_g0";
char *gs1 __attribute__ ( ( section( "DABRA" ) ) ) = "abrakadabra_g1";

void
mastest_print_allocated( const char *msg, int line, const char *func )
{
  struct mallinfo mi;

  mi = mallinfo(  );
  INFO( "\x1b[0;1;44;35m%s %d bytes at %d:%s\x1b[0m", msg, mi.uordblks, line, func );
}

static void constructor_main(  ) __attribute__ ( ( constructor( 2001 ) ) );
static void
constructor_main( void )
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

  INFO( "START MEM" );
#endif
  INFO( "START" );
}

static void destructor_main(  ) __attribute__ ( ( destructor( 2001 ) ) );
static void
destructor_main( void )
{
  mastest_print_allocated( "Still allocated", __LINE__, __func__ );
//malloc_info(0, stderr);
//malloc_stats(  );
}

int
main( int argc _uUu_, const char *argv[]_uUu_ )
{
  int r = 0;
  struct rlimit lim = { 0 };
  errno = 0;
  {
    int fd = open( "Makefile.in", O_RDONLY );

    INFO( "FD:%d\n", fd );
    close( fd );
  }
  r = getrlimit( RLIMIT_NOFILE, &lim );
  if ( r >= 0 )
  {
    lim.rlim_cur = lim.rlim_max;
    r = setrlimit( RLIMIT_NOFILE, &lim );
    if ( r >= 0 )
    {
      int masxfs_test_0( int argc, const char *argv[], int nseries, const char *series_suffix, int do_fprintf );

      masexam_do_t funlist[] _uUu_ = {
        {1, masxfs_test_0, 0, "",.f_print_ok = 0,.assert_on_error = 0,.sleep_on_error = 0},
        {0},
      };

      masexam_test( argc, argv, funlist );
      masexam_next_group(  );
# define TOTAL_TESTS TEST_0_EXAMS
      masexam_exam( 0, masexam_tests_count(  ) == TOTAL_TESTS, "OK", "Error", "tests_count=%d ? %d", masexam_tests_count(  ), TOTAL_TESTS );
    }
  }

  {
    char *s1 = "abrakadabra1";
    const char *s2 = "abrakadabra2";
    char *s3 = mas_strdup( s1 );
    char *s4 = strdup( s1 );

    INFO( "%p %p %p %p - %p %p", s1, s2, s3, s4, gs0, gs1 );
    mas_free( s3 );
    free( s4 );
  }

  INFO( "----------------forward:" );
  masregerr_print_simple_all( NULL, NULL, 0 /* max.print */ );

  return 0;
}
