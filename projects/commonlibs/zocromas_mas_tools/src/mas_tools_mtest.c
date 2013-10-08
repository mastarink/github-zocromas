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
  {
    char cmdline[] = "abc\0def\0ghi";
    char **targv = NULL;
    int targc = 0;

    targc = mas_add_argv_cmdline( targc, &targv, cmdline, sizeof( cmdline ) );
    if ( targc )
      for ( int i = 0; i < targc; i++ )
      {
        fprintf( stderr, "targv[%d]='%s'\n", i, targv[i] );
      }
    mas_del_argv( targc, targv, 0 );
  }
  {
    char cmdline[] = "abc\0def\0ghi\0";
    char **targv = NULL;
    int targc = 0;

    targc = mas_add_argv_cmdline( targc, &targv, cmdline, sizeof( cmdline ) );
    if ( targc )
      for ( int i = 0; i < targc; i++ )
      {
        fprintf( stderr, "targv[%d]='%s'\n", i, targv[i] );
      }
    mas_del_argv( targc, targv, 0 );
  }
  {
    char cmdline[] = "abc\0def\0ghi\0\0";
    char **targv = NULL;
    int targc = 0;

    targc = mas_add_argv_cmdline( targc, &targv, cmdline, sizeof( cmdline ) );
    if ( targc )
      for ( int i = 0; i < targc; i++ )
      {
        fprintf( stderr, "targv[%d]='%s'\n", i, targv[i] );
      }
    mas_del_argv( targc, targv, 0 );
  }
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif


  return 0;
}
