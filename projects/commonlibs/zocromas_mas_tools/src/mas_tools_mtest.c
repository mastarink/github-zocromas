#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_memory.h>
#include "mas_arg_tools.h"
#include "mas_tools.h"

int
main( int argc, char *argv[] )
{
  char *d;
  const char *s = "0123456789abcdefabcdefghijkl\r\nmnopqzABCDEF\tX";

  {
    struct tm *t;
    char buf[512];

    t = mas_xgmtime(  );
    strftime( buf, sizeof( buf ), "%a, %d %b %Y %T GMT", t );
    fprintf( stderr, "GMTime: %s\n", buf );
  }
  d = mas_dump1( s, strlen( s ) - 1, 0 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }
  d = mas_dump1( s, strlen( s ) - 1, 16 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }
  d = mas_dump1( s, strlen( s ) - 1, 32 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }

  d = mas_dump2( s, strlen( s ) - 1, 0 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }
  d = mas_dump2( s, strlen( s ) - 1, 16 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }
  d = mas_dump2( s, strlen( s ) - 1, 32 );
  if ( d )
  {
    fprintf( stderr, ">>>>>[\n%s\n]<<<<<<<\n", d );
    mas_free( d );
  }
  
#ifdef MAS_TRACEMEM
  print_memlist( stderr, FL );
#endif


  return 0;
}
