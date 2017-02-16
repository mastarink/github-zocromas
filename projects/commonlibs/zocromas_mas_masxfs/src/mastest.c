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

#include "masxfs_defs.h"
#include "masxfs_scan.h"
static int num = 0;

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

static int _uUu_
fscallback_dir( const char *path, const char *name, char **pnpath )
{
  char *npath = masxfs_get_normalized_path( path, name, pnpath );

  fprintf( stderr, "entry directory: '%s'\t '%s'\n   -- %s\n", path, name, npath );
  return 0;
}

static int
fscallback_regular( const char *path _uUu_, const char *name _uUu_, char **pnpath _uUu_ )
{
  char *npath _uUu_ = masxfs_get_normalized_path( path, name, pnpath );

  num++;
  fprintf( stderr, "b. %-2d. -- '%s'\n", num, npath );
  return 0;
}

static int _uUu_
fscallback( const char *path _uUu_, const char *name _uUu_, char **pnpath _uUu_ )
{
  char *npath _uUu_ = masxfs_get_normalized_path( path, name, pnpath );

  num++;
  fprintf( stderr, "a. %-2d. -- '%s'\n", num, npath );
  return 0;
}

int
main( int argc _uUu_, char *argv[]_uUu_ )
{
  masxfs_entry_callback_t callbacks[] = {
  /* {MASXFS_ENTRY_BIT_LINK | MASXFS_ENTRY_BIT_REG, fscallback}, */
    {MASXFS_ENTRY_BIT_REG, fscallback},
    {MASXFS_ENTRY_BIT_REG, fscallback_regular},
    {0, NULL}
  };
  masxfs_scanpath_real( NULL, callbacks, TRUE );

  return 0;
}
