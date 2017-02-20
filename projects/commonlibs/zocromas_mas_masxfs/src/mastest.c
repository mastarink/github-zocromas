#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "masxfs_structs.h"
#include "masxfs_exam.h"

#include "masxfs_defs.h"
#include "masxfs_scan.h"

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
  mastest_print_allocated( "Still allocated", __LINE__, __func__ );
//malloc_info(0, stderr);
//malloc_stats(  );
}

int
main( int argc _uUu_, char *argv[]_uUu_ )
{
  int masxfs_test_0( int nseries, const char *series_suffix, int do_fprintf );

  mastest_do_t funlist[] _uUu_ = {
    {1, masxfs_test_0, 0, "",.f_print_ok = 0,.stop_on_error = 1},
    {0},
  };

  mastest_test( funlist );
  return 0;
}
