#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <limits.h>
#include <stdlib.h>

#include "masregerr.h"
int
function_one( void )
{
  RGE;
  return 0;
}

int
function_two( void )
{
  RGE;
  return 0;
}

int
function_three( void )
{
  RGE;
  return 0;
}

int
function_four( void )
{
  int r = 0;

  RGESR( r = open( "something_not_exist.txt", O_RDONLY ) );
  return r;
}

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  for ( int i = 0; i < 35; i++ )
  {
    function_one(  );
    function_two(  );
    function_three(  );
    function_four(  );
  }
  fprintf( stderr, "----------------last:\n" );
  masregerr_print_simple_last( NULL );
  fprintf( stderr, "----------------forward:\n" );
  masregerr_print_simple_all( NULL );
  fprintf( stderr, "----------------back:\n" );
  masregerr_print_simple_all_back( NULL );
  fprintf( stderr, "----------------\n" );
  return 0;
}
