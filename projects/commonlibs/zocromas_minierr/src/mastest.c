#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include "minierr.h"

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
/* minierr(); */
  INFO( "This is warn %s, %d", "message", 1 );
  INFOB( 3, "This is warn %s, %d", "message", 1 );
  WARN( "This is warn %s, %d", "message", 1 );
  DIE( "This is die %s %d; must exit", "message", 1 );
  WARN( "This is warn %s %d; shold not appear", "message", 2 );
  return 0;
}
