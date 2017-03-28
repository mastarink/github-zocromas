#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <popt.h>

#include <mastar/exam/masexam.h>

#include "mastest.h"

int
test_popt1( int argc _uUu_, const char _uUu_ * argv[], int nseries _uUu_, const char *series_suffix _uUu_, int do_fprintf _uUu_ )
{
  masregerrs_delete_default( NULL );
  int c = 0xffff;

  int v_int0 _uUu_ = 0;
  int v_int1 _uUu_ = 0;
  int v_int2 _uUu_ = 0;
  int v_int3 _uUu_ = 0;
  int v_int4 _uUu_ = 0;

  long v_long0 _uUu_ = 0;
  long v_long1 _uUu_ = 0;
  long v_long2 _uUu_ = 0;
  long v_long3 _uUu_ = 0;
  long v_long4 _uUu_ = 0;
  long v_long5 _uUu_ = 0;

  long long v_llong0 _uUu_ = 0;
  long long v_llong1 _uUu_ = 0;
  long long v_llong2 _uUu_ = 0;
  long long v_llong3 _uUu_ = 0;
  long long v_llong4 _uUu_ = 0;
  long long v_llong5 _uUu_ = 0;
  long long v_llong6 _uUu_ = 0;

  long bitwise1 _uUu_ = 0x10204;
  long bitwise2 _uUu_ = 0x10204;
  long bitwise3 _uUu_ = 0x10204;

  char *thestring _uUu_ = NULL;

  const char *argument;
  poptContext optCon;                                                /* context for parsing command-line options */

  const char *xargv[] = {
    argv[0],
//  "something",
//  "somethingmore",
    "--num0=5437",
    "--num1=0x12",
    "--num2=012",
    "--num3=2147483647",
    "--num4=-2147483648",
    "--lnum0=0xffffffffff",
    "--lnum1=0xff",
    "--lnum2=0x7fffffffffffffff",
    "--lnum3=9223372036854775807",
//  "wow",
    "--lnum4=-12",
    "--lnum5=-0x8000000000000000",
    "--llnum0=5437",
    "--llnum1=0x12",
    "--llnum2=012",
    "--bwi=0x700",
    "--bwiset=0x100",
//  "--bwis=0x100",
    "--bwiunset=0x200",
    "--llnum3=9223372036854775806",
    "--llnum4=-9223372036854775807",
//  "--llnum5=9223372036854775807",                                  /* for some reason popt makes LLONG_MAX and LLONG_MIN an error */
//  "--llnum6=-9223372036854775808",                                 /* for some reason popt makes LLONG_MAX and LLONG_MIN an error */
    "--thestring=asta manyana...",
    "abrakadabra",
  };
  int xargc = sizeof( xargv ) / sizeof( xargv[0] );

  struct poptOption optionsTable[] = {
    {"num0", 'n', POPT_ARG_INT, &v_int0}
    , {"num1", 0, POPT_ARG_INT, &v_int1}
    , {"num2", 0, POPT_ARG_INT, &v_int2}
    , {"num3", 0, POPT_ARG_INT, &v_int3}
    , {"num4", 0, POPT_ARG_INT, &v_int4}
    , {"lnum0", 0, POPT_ARG_LONG, &v_long0}
    , {"lnum1", 0, POPT_ARG_INT, &v_long1}
    , {"lnum2", 0, POPT_ARG_LONG, &v_long2}
    , {"lnum3", 0, POPT_ARG_LONG, &v_long3}
    , {"lnum4", 0, POPT_ARG_LONG, &v_long4}
    , {"lnum5", 0, POPT_ARG_LONG, &v_long5}
    , {"llnum0", 0, POPT_ARG_LONGLONG, &v_llong0}
    , {"llnum1", 0, POPT_ARG_LONGLONG, &v_llong1}
    , {"llnum2", 0, POPT_ARG_LONGLONG, &v_llong2}
    , {"llnum3", 0, POPT_ARG_LONGLONG, &v_llong3}
    , {"llnum4", 0, POPT_ARG_LONGLONG, &v_llong4}
    , {"llnum5", 0, POPT_ARG_LONGLONG, &v_llong5}                    /* for some reason popt doesn't allow LLONG_MAX and LLONG_MIN and marks it as overflow */
    , {"llnum6", 0, POPT_ARG_LONGLONG, &v_llong6}                    /* for some reason popt doesn't allow LLONG_MAX and LLONG_MIN and marks it as overflow */
    , {"bwi", 0, POPT_ARG_LONG | POPT_ARGFLAG_NOT, &bitwise1, 0, "bitwise", "value"}
    , {"bwiset", 0, POPT_ARG_LONG | POPT_ARGFLAG_OR, &bitwise2, 0, "bitwise", "value"}
    , {"bwiunset", 0, POPT_ARG_LONG | POPT_ARGFLAG_NOT | POPT_ARGFLAG_AND, &bitwise3, 0, "bitwise", "value"}
    , {"thestring", 0, POPT_ARG_STRING, &thestring, 0, "bitwise", "value"}

    , POPT_AUTOHELP {NULL, 0, 0, NULL, 0}
  };
  mastest_print_allocated( "Allocated", __LINE__, __func__ );

  optCon = poptGetContext( NULL, xargc, xargv, optionsTable, 0 );
  mastest_print_allocated( "Allocated after poptGetContext", __LINE__, __func__ );
  poptSetOtherOptionHelp( optCon, "[OPTIONS]* <port>" );
  mastest_print_allocated( "Allocated after poptSetOtherOptionHelp", __LINE__, __func__ );

  if ( xargc < 2 )
  {
    poptPrintUsage( optCon, stderr, 0 );
    exit( 1 );
  }

/* Now do options processing, get argument */
  while ( ( c = poptGetNextOpt( optCon ) ) >= 0 )
  {
    mastest_print_allocated( "Allocated after poptGetNextOpt", __LINE__, __func__ );
    fprintf( stderr, "~~c: '%c' %d 0x%x\n", c, c, c );
  }
  fprintf( stderr, "c: '%c' %d 0x%x\n", c, c, c );
  mastest_print_allocated( "Allocated after all poptGetNextOpt", __LINE__, __func__ );

  int err = 0;

  if ( c < 0 )
    switch ( c )
    {
    case POPT_ERROR_NOARG:
      err = c;
      break;
    case POPT_ERROR_BADOPT:
      err = c;
      break;
    case POPT_ERROR_OPTSTOODEEP:
      err = c;
      break;
    case POPT_ERROR_BADQUOTE:
      err = c;
      break;
    case POPT_ERROR_BADNUMBER:
      err = c;
      break;
    case POPT_ERROR_OVERFLOW:
      err = c;
      break;
    }

  mastest_print_allocated( "Allocated", __LINE__, __func__ );
  if ( err )
  {
    const char *badoption = poptBadOption( optCon, POPT_BADOPTION_NOALIAS );

    if ( badoption )
      fprintf( stderr, "%s: %s\n", badoption, poptStrerror( err ) );
  }
  fprintf( stderr, "--- c: 0x%x (%c) %d -- %d,%d,%d,%d,%d\n", c, c >= 0 ? '+' : '-', c, v_int0, v_int1, v_int2, v_int3, v_int4 );
  argument = poptGetArg( optCon );
  if ( ( argument == NULL ) || !( poptPeekArg( optCon ) == NULL ) )
  {
  }

  masexam_next_group(  );
  EXAMX( sizeof( v_int0 ) == 4 && v_int0 == 5437, "%d ? %d", v_int0, 5437 );
  EXAMX( sizeof( v_int1 ) == 4 && v_int1 == 0x12, "%d ? %d", v_int1, 0x12 );
  EXAMX( sizeof( v_int2 ) == 4 && v_int2 == 012, "%d ? %d", v_int2, 012 );
  EXAMX( sizeof( v_int3 ) == 4 && v_int3 == INT_MAX, "%d ? %d", v_int3, INT_MAX );
  EXAMX( sizeof( v_int4 ) == 4 && v_int4 == INT_MIN, "%d ? %d", v_int4, INT_MIN );

  masexam_next_group(  );
  EXAMX( sizeof( v_long0 ) == 8 && v_long0 == 1099511627775L, "%ld ? %ld", v_long0, 0xffffffffffL );
  EXAMX( sizeof( v_long1 ) == 8 && v_long1 == 0xff, "%ld ? %ld", v_long1, 0xffL );
  EXAMX( sizeof( v_long2 ) == 8 && v_long2 == LONG_MAX, "%ld ? %ld", v_long2, LONG_MAX );
  EXAMX( sizeof( v_long2 ) == 8 && v_long2 == 0x7fffffffffffffffL, "%lx ? %lx", v_long2, 0x7fffffffffffffffL );
  EXAMX( sizeof( v_long2 ) == 8 && v_long2 == 9223372036854775807L, "%ld ? %ld", v_long2, 9223372036854775807L );
  EXAMX( sizeof( v_long3 ) == 8 && v_long3 == 9223372036854775807L, "%ld ? %ld", v_long3, 9223372036854775807L );
  EXAMX( sizeof( v_long4 ) == 8 && v_long4 == -12L, "%ld ? %ld", v_long4, -12L );
  EXAMX( sizeof( v_long5 ) == 8 && v_long5 == LONG_MIN, "%ld ? %ld", v_long5, LONG_MIN );

  masexam_next_group(  );
  EXAMX( sizeof( v_llong0 ) == 8 && v_llong0 == 5437LL, "%lld ? %lld (%d)", v_llong0, 5437LL, sizeof( v_llong0 ) );
  EXAMX( sizeof( v_llong1 ) == 8 && v_llong1 == 0x12LL, "%lld ? %lld (%d)", v_llong1, 0x12LL, sizeof( v_llong1 ) );
  EXAMX( sizeof( v_llong2 ) == 8 && v_llong2 == 012LL, "%lld ? %lld (%d)", v_llong2, 012LL, sizeof( v_llong2 ) );
  EXAMX( sizeof( v_llong3 ) == 8 && v_llong3 == LLONG_MAX - 1, "%llx/%lld ? %lld (%d)", v_llong3, v_llong3, LLONG_MAX - 1, sizeof( v_llong3 ) );
  EXAMX( sizeof( v_llong4 ) == 8 && v_llong4 == LLONG_MIN + 1, "%llx/%lld ? %lld (%d)", v_llong4, v_llong4, LLONG_MIN + 1, sizeof( v_llong4 ) );
#if 0
/* for some reason popt doesn't allow LLONG_MAX and LLONG_MIN and marks it as overflow */
  EXAMX( sizeof( v_llong5 ) == 8 && v_llong5 == LLONG_MIN, "%llx/%lld ? %lld (%d)", v_llong5, v_llong5, LLONG_MIN, sizeof( v_llong5 ) );
  EXAMX( sizeof( v_llong6 ) == 8 && v_llong6 == LLONG_MIN, "%llx/%lld ? %lld (%d)", v_llong6, v_llong6, LLONG_MIN, sizeof( v_llong6 ) );
#endif
/*
   masexam_next_group(  );

   char **argsno = xargv;

   EXAMX( 0 == strcmp( "something", argsno[1] ),  "'%s' ? '%s'", "something", argsno[1] );
   EXAMX( 0 == strcmp( "wow", argsno[2] ),  "'%s' ? '%s'", "wow", argsno[2] );
   EXAMX( 0 == strcmp( "abrakadabra", argsno[3] ),  "'%s' ? '%s'", "abrakadabra", argsno[3] );
   * */
  masexam_next_group(  );
  EXAMX( thestring && 0 == strcmp( thestring, "asta manyana..." ), "%s ? %s", thestring, "asta manyana..." );
  masexam_next_group(  );
  EXAMX( bitwise1 == ( long ) 0xfffffffffffff8ffL, "%lx ? bitwise1:%lx", ( long ) 0xfffffffffffff8ffL, bitwise1 );
  EXAMX( bitwise2 == ( long ) 0x10304L, "%lx ? bitwise2:%lx", ( long ) 0x10304L, bitwise2 );
  EXAMX( bitwise3 == ( long ) 0x10004L, "%lx ? bitwise3:%lx", ( long ) 0x10004L, bitwise3 );
  masexam_next_group(  );
  EXAMX( c == -1, "err (c):%d", c );
  EXAMX( err == 0, "err:%d", err );

  {
    const char *arg;

    while ( ( arg = poptPeekArg( optCon ) ) )
    {
      arg = poptGetArg( optCon );
      fprintf( stderr, "@@@@@ %s\n", arg );
    }
  }
  mastest_print_allocated( "Allocated", __LINE__, __func__ );
  poptFreeContext( optCon );
  mastest_print_allocated( "Allocated", __LINE__, __func__ );
  if ( thestring )
    free( thestring );
  mastest_print_allocated( "Allocated", __LINE__, __func__ );
  return 0;
}
