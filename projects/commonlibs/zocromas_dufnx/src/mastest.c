#define R_GOOD(_r) (_r>=0)
#include "dufnx.h"

int test0( void );
int test1( void );
int test2( void );
int test3( void );

int
main( int argc, char *argv[] )
{
  dufnx( argc, argv );
 
  test0(  );
  test1(  );
  test2(  );
  test3(  );

  return 0;
}
