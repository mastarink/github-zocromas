#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
/* #include <mastar/msg/mas_msg_def.h>   */
/* #include <mastar/msg/mas_msg_tools.h> */

void
testprint( int targc, char *targv[] )
{
  fprintf( stderr, ">>>>>>>>>> for (ia=0;ia < %d; ia++)\n", targc );
  for ( int ia = 0; ia < targc; ia++ )
  {
    fprintf( stderr, "%2d. '%s'\n", ia, targv[ia] );
  }
  fprintf( stderr, ">>>>>>>>>> while ( p )\n" );
  {
    char **p;
    int ia = 0;

    p = targv;
    while ( p && *p )
    {
      fprintf( stderr, "%2d. '%s'\n", ia, *p );
      ia++;
      p++;
    }
  }
}

void
test20130100( int argc, char *argv[], char *env[] )
{
  fprintf( stderr, "\n== A =====================\n" );
  {
    char **targv = NULL;
    int targc = 0;

    /* testprint( argc, argv ); */


    targc = mas_add_argv_arg( targc, &targv, "test 1" );
    targc = mas_add_argv_arg( targc, &targv, "test 2" );
    targc = mas_add_argv_argv( targc, &targv, argc, argv, 1 );
    testprint( targc, targv );
    targc = mas_add_argv_args( targc, &targv, "asta maniana x y	-r \"  \" \"u v w\"", 0 );
#ifdef MAS_TRACEMEM
    /* print_memlist( stderr, FL ); */
#endif
    testprint( targc, targv );

    fprintf( stderr, "\ntargc:%u; targv:%p\n", targc, ( void * ) targv );
    mas_del_argv( targc, targv, 0 );
#ifdef MAS_TRACEMEM
    print_memlist( stderr, __func__, __LINE__ );
#endif
  }
  fprintf( stderr, "\n== B =====================\n" );
  {
    int targc = 0;
    char **targv = NULL;

    targc = mas_add_argv_args( targc, &targv, "zocromas.mastar.lan 5003", 0 );
    fprintf( stderr, "\ntargc:%u;\n", targc );

    mas_del_argv( targc, targv, 0 );

#ifdef MAS_TRACEMEM
    print_memlist( stderr, __func__, __LINE__ );
#endif
  }
}

void
test20130120( void )
{
  struct timeval tv1;
  struct timeval tv2;

  /* fprintf( stderr, "\n-------------------------------------\n%lu.%06lu\n", tv1.tv_sec, tv1.tv_usec ); */
  for ( int ima2 = 0; ima2 < 10; ima2++ )
  {
    gettimeofday( &tv1, 0 );
    for ( int ima = 0; ima < 100000; ima++ )
    {
      char *p[100];

      for ( int j = 0; j < sizeof( p ) / sizeof( p[0] ); j++ )
        p[j] = malloc( 10000 );
      for ( int j = 0; j < sizeof( p ) / sizeof( p[0] ); j++ )
        free( p[j] );
    }
    gettimeofday( &tv2, 0 );
    /* fprintf( stderr, "%lu.%06lu\n", tv2.tv_sec, tv2.tv_usec ); */
    {
      tv2.tv_sec -= tv1.tv_sec;
      tv2.tv_usec -= tv1.tv_usec;
      if ( tv2.tv_usec < 0 )
      {
        tv2.tv_sec -= 1;
        tv2.tv_usec += 1000000;
      }
    }
    fprintf( stderr, "%lu.%06lu\n", tv2.tv_sec, tv2.tv_usec );
  }
}



int
main( int argc, char *argv[], char *env[] )
{
  /* test20130100( argc, argv, env ); */
  test20130120(  );


  return 0;
}
