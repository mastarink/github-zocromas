#define R_GOOD(_r) ((_r)>=0)
#include <malloc.h>
#include "dufnx.h"

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/regerr/masregerr_defs.h>
#include <mastar/exam/masexam.h>

#include <mastar/levinfo/masxfs_levinfo_types.h>

int testdropcreate( const char *path );
int testfill( const char *path, masxfs_depth_t maxdepth );
int testtreefromfs( const char *path, masxfs_depth_t maxdepth, FILE * f );
int testtreefromdb( const char *path, masxfs_depth_t maxdepth, FILE * f );

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  rDECLBAD;
/* dufnx( argc, argv ); */
  if ( 1 )
  {
    const char *path __attribute__ ( ( unused ) ) = "mastest";

    rC( testdropcreate( path ) );
    rC( testfill( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */  ) );
  }
  {
    {
      if ( 1 )
      {
/* const char *path __attribute__ ( ( unused ) ) = "/home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_dufnx/mastest"; */
/* const char *path __attribute__ ( ( unused ) ) = "mastest/tree"; */
        const char *path __attribute__ ( ( unused ) ) = "mastest";

        rC( testtreefromfs( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , stdout ) );
      }
      if ( 1 )
      {
        const char *path __attribute__ ( ( unused ) ) = "mastest/tree/config.h.in";

        rC( testtreefromfs( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , stdout ) );
      }
    }
    {
      if ( 0 )
      {
        const char *path __attribute__ ( ( unused ) ) = "mastest";

        rC( testtreefromdb( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , stdout ) );
      }
      if ( 0 )
      {
        const char *path __attribute__ ( ( unused ) ) = "mastest/tree/config.h.in";

        rC( testtreefromdb( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , stdout ) );
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
        rC( testtreefromfs( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , fil ) );
        fclose( fil );
      }
    }

    if ( 1 )
    {
      const char *path __attribute__ ( ( unused ) ) = "mastest";

      FILE *fil = fopen( "testtreefromdbd.tree", "w" );

      if ( fil )
      {
        rC( testtreefromdb( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , fil ) );
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
        rC( testtreefromfs( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , fil ) );
        fclose( fil );
      }
    }

    if ( 1 )
    {
      const char *path __attribute__ ( ( unused ) ) = "mastest/tree/config.h.in";

      FILE *fil = fopen( "testtreefromdbf.tree", "w" );

      if ( fil )
      {
        rC( testtreefromdb( path, ( masxfs_depth_t ) 0 /* maxdepth OR 0 for all */ , fil ) );
        fclose( fil );
      }
    }
  }
  rRET;
}

static void constructor_main(  ) __attribute__ ( ( constructor( 2011 ) ) );
static void
constructor_main( void )
{
  masregerrs_set_max_print( 15 );
  INFO( "START" );
}
