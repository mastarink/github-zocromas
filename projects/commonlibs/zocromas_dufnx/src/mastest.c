#define R_GOOD(_r) ((_r)>=0)
#include <malloc.h>
#include "dufnx.h"

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/exam/masexam.h>

#include <mastar/levinfo/masxfs_levinfo_types.h>

void
mastest_print_allocated( const char *msg, int line, const char *func )
{
  struct mallinfo mi;

  mi = mallinfo(  );
  INFO( "\n\x1b[0;1;44;35m%s %d bytes at %d:%s\x1b[0m", msg, mi.uordblks, line, func );
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
  masregerrs_set_max_print( 15 );
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

int testdropcreate( const char *path );
int testfill( const char *path, masxfs_depth_t maxdepth );
int testtreefromfs( const char *path, masxfs_depth_t maxdepth, FILE * f );
int testtreefromdb( const char *path, masxfs_depth_t maxdepth, FILE * f );

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
/* dufnx( argc, argv ); */
  if ( 1 )
  {
    const char *path __attribute__ ( ( unused ) ) = "mastest";

    testdropcreate( path );
    testfill( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */  );
  }
  {
    {
      if ( 1 )
      {
/* const char *path __attribute__ ( ( unused ) ) = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_dufnx/mastest"; */
/* const char *path __attribute__ ( ( unused ) ) = "mastest/tree"; */
        const char *path __attribute__ ( ( unused ) ) = "mastest";

        testtreefromfs( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , stdout );
      }
      if ( 1 )
      {
        const char *path __attribute__ ( ( unused ) ) = "mastest/tree/config.h.in";

        testtreefromfs( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , stdout );
      }
    }
    {
      if ( 0 )
      {
        const char *path __attribute__ ( ( unused ) ) = "mastest";

        testtreefromdb( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , stdout );
      }
      if ( 0 )
      {
        const char *path __attribute__ ( ( unused ) ) = "mastest/tree/config.h.in";

        testtreefromdb( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , stdout );
      }
    }
  }
  {
    if ( 1 )
    {
      const char *path __attribute__ ( ( unused ) ) = "mastest";

      FILE *fil = fopen( "testtreefromfsd.tree", "w" );

      if ( fil )
      {
        testtreefromfs( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , fil );
        fclose( fil );
      }
    }

    if ( 1 )
    {
      const char *path __attribute__ ( ( unused ) ) = "mastest";

      FILE *fil = fopen( "testtreefromdbd.tree", "w" );

      if ( fil )
      {
        testtreefromdb( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , fil );
        fclose( fil );
      }
    }
  }
  {
    if ( 1 )
    {
      const char *path __attribute__ ( ( unused ) ) = "mastest/tree/config.h.in";

      FILE *fil = fopen( "testtreefromfsf.tree", "w" );

      if ( fil )
      {
        testtreefromfs( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , fil );
        fclose( fil );
      }
    }

    if ( 1 )
    {
      const char *path __attribute__ ( ( unused ) ) = "mastest/tree/config.h.in";

      FILE *fil = fopen( "testtreefromdbf.tree", "w" );

      if ( fil )
      {
        testtreefromdb( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , fil );
        fclose( fil );
      }
    }
  }
  return 0;
}
