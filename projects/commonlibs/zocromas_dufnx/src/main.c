#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/regerr/masregerr_defs.h>

#include "dufnx.h"

int
main( int argc, char *argv[] )
{
  int r = 0;

  r = dufnx( argc, argv );
  return r;
}

static void constructor_main(  ) __attribute__ ( ( constructor( 2011 ) ) );
static void
constructor_main( void )
{
  masregerrs_set_max_print( 15 );
  INFO( "START" );
}
