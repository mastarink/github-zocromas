#include "mas_basic_def.h"

#include <stdio.h>

int
main( int argc, char *argv[1] )
{
  for ( int ia = 0; ia < argc; ia++ )
  {
    printf( "%d. %s\n", ia, argv[ia] );
  }
  return 0;
}
