#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"
#include "mulconfnt_error.h"

#include "mastest.h"

int
test_1( int argc _uUu_, const char *argv[]_uUu_ )
{
  const char *arg;
  char *v_string0 = NULL;
  char *v_string1 = NULL;
  int v_int0 = 0;
  int v_int1 = 0;
  int v_int2 = 0;
  int v_int3 = 0;
  int v_int4 = 0;

  long v_long0 = 0;
  long v_long1 = 0;
  long v_long2 = 0;
  long v_long3 = 0;
  long v_long4 = 0;

  long long v_llong0 = 0;
  long long v_llong1 = 0;
  long long v_llong2 = 0;
  long long v_llong3 = 0;
  long long v_llong4 = 0;

  long bitwise1 = 0x10204;
  long bitwise2 = 0x10204;
  long bitwise3 = 0x10204;

  double v_double0 = 0;
  long double v_ldouble0 = 0;

  const char *xargv[] = {
    argv[0],
    "something",
    "--string0=lorem-ipsum",
    "--string1=lorem ipsum",
    "--num0=5437",
    "--num1=0x12",
    "--num2=012",
    "--num3=2147483647",
    "--num4=-2147483648",
    "--lnum0=0xffffffffff",
    "--lnum1=0xff",
    "--lnum2=0x7fffffffffffffff",
    "wow",
    "--lnum3=-12",
    "--lnum4=-0x8000000000000000",
    "--llnum0=5437",
    "--llnum1=0x12",
    "--llnum2=012",
    "--llnum3=9223372036854775807",
    "--llnum4=-9223372036854775808",
    "--bwi=0x700",
    "--bwi+=0x100",
    "--bwi-=0x200",
    "--pi=3.141592653589793",
    "--longpi=3.1415926535897932384626433832795028841971693993751058209749445",
    "abrakadabra",
  };
  int xargc = sizeof( xargv ) / sizeof( xargv[0] );

  config_option_t options[] = {
    {"string0", 0, MULCONF_RESTYPE_STRING, &v_string0}
    , {"string1", 0, MULCONF_RESTYPE_STRING, &v_string1}
    , {"num0", 0, MULCONF_RESTYPE_INT, &v_int0}
    , {"num1", 0, MULCONF_RESTYPE_INT, &v_int1}
    , {"num2", 0, MULCONF_RESTYPE_INT, &v_int2}
    , {"num3", 0, MULCONF_RESTYPE_INT, &v_int3}
    , {"num4", 0, MULCONF_RESTYPE_INT, &v_int4}
    , {"lnum0", 0, MULCONF_RESTYPE_LONG, &v_long0}
    , {"lnum1", 0, MULCONF_RESTYPE_INT, &v_long1}
    , {"lnum2", 0, MULCONF_RESTYPE_LONG, &v_long2}
    , {"lnum3", 0, MULCONF_RESTYPE_LONG, &v_long3}
    , {"lnum4", 0, MULCONF_RESTYPE_LONG, &v_long4}
    , {"llnum0", 0, MULCONF_RESTYPE_LLONG, &v_llong0}
    , {"llnum1", 0, MULCONF_RESTYPE_LLONG, &v_llong1}
    , {"llnum2", 0, MULCONF_RESTYPE_LLONG, &v_llong2}
    , {"llnum3", 0, MULCONF_RESTYPE_LLONG, &v_llong3}
    , {"llnum4", 0, MULCONF_RESTYPE_LLONG, &v_llong4}
    , {"pi", 0, MULCONF_RESTYPE_DOUBLE, &v_double0}
    , {"longpi", 0, MULCONF_RESTYPE_LDOUBLE, &v_ldouble0}
    , {"bwi", 0, MULCONF_RESTYPE_LONG | MULCONF_BITWISE_NOT, &bitwise1, 0, "bitwise", "value"}
    , {"bwi+", 0, MULCONF_RESTYPE_LONG | MULCONF_BITWISE_OR, &bitwise2, 0, "bitwise", "value"}
    , {"bwi-", 0, MULCONF_RESTYPE_LONG | MULCONF_BITWISE_NOT | MULCONF_BITWISE_AND, &bitwise3, 0, "bitwise", "value"}

    , {.name = NULL,.shortname = 0,.restype = 0,.ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  config_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.options = options, /* */
  };

  {
    FILE *f;

    f = fopen( "mastest_1.commands", "w" );
    if ( f )
    {
      for ( int i = 0; i < xargc; i++ )
      {
        fprintf( f, "%s\n", xargv[i] );
      }
      fclose( f );
    }
  }
  {
    config_source_list_t *plist = mulconfnt_source_list_create(  );
    config_source_desc_t *osrc = mulconfnt_source_list_add_source( plist, MULCONF_SOURCE_ARGV, xargc, xargv, NULL, "=", NULL );

    mulconfnt_source_lookup( osrc, &test_tablist );

    mastest_next_group(  );
    mastest_exam( !mulconfnt_error_list( plist ), "OK", "Error", "mulconfnt_error: %d", mulconfnt_error_list( plist ) );
    mastest_next_group(  );
    mastest_exam( v_string0
                  && 0 == strcmp( v_string0, "lorem-ipsum" ), "OK", "Error", "string0=%s ? %s", v_string0 ? v_string0 : "<NULL>", "lorem-ipsum" );
    if ( v_string0 )
      mas_free( v_string0 );
    v_string0 = NULL;
    mastest_exam( v_string1
                  && 0 == strcmp( v_string1, "lorem ipsum" ), "OK", "Error", "string0=%s ? %s", v_string1 ? v_string1 : "<NULL>", "lorem ipsum" );
    if ( v_string1 )
      mas_free( v_string1 );
    v_string1 = NULL;
    mastest_next_group(  );
    mastest_exam( sizeof( v_int0 ) == 4 && v_int0 == 5437, "OK", "Error", "num0=%d ? %d", v_int0, 5437 );
    mastest_exam( sizeof( v_int1 ) == 4 && v_int1 == 0x12, "OK", "Error", "num1=%d ? %d", v_int1, 0x12 );
    mastest_exam( sizeof( v_int2 ) == 4 && v_int2 == 012, "OK", "Error", "num2=%d ? %d", v_int2, 012 );
    mastest_exam( sizeof( v_int3 ) == 4 && v_int3 == INT_MAX, "OK", "Error", "num3=%d ? %d", v_int3, INT_MAX );
    mastest_exam( sizeof( v_int4 ) == 4 && v_int4 == INT_MIN, "OK", "Error", "num4=%d ? %d", v_int4, INT_MIN );

    mastest_next_group(  );
    mastest_exam( sizeof( v_long0 ) == 8 && v_long0 == 1099511627775L, "OK", "Error", "%ld ? %ld", v_long0, 0xffffffffffL );
    mastest_exam( sizeof( v_long1 ) == 8 && v_long1 == 0xff, "OK", "Error", "%ld ? %ld", v_long1, 0xffL );
    mastest_exam( sizeof( v_long2 ) == 8 && v_long2 == LONG_MAX, "OK", "Error", "%ld ? %ld", v_long2, LONG_MAX );
    mastest_exam( sizeof( v_long2 ) == 8 && v_long2 == 0x7fffffffffffffffL, "OK", "Error", "%lx ? %lx", v_long2, 0x7fffffffffffffffL );
    mastest_exam( sizeof( v_long2 ) == 8 && v_long2 == 9223372036854775807, "OK", "Error", "%ld ? %ld", v_long2, 9223372036854775807 );
    mastest_exam( sizeof( v_long3 ) == 8 && v_long3 == -12L, "OK", "Error", "%ld ? %ld", v_long3, -12L );
    mastest_exam( sizeof( v_long4 ) == 8 && v_long4 == LONG_MIN, "OK", "Error", "%ld ? %ld", v_long4, LONG_MIN );

    mastest_next_group(  );
    mastest_exam( sizeof( v_llong0 ) == 8 && v_llong0 == 5437LL, "OK", "Error", "%lld ? %lld (%d)", v_llong0, 5437LL, sizeof( v_llong0 ) );
    mastest_exam( sizeof( v_llong1 ) == 8 && v_llong1 == 0x12LL, "OK", "Error", "%lld ? %lld (%d)", v_llong1, 0x12LL, sizeof( v_llong1 ) );
    mastest_exam( sizeof( v_llong2 ) == 8 && v_llong2 == 012LL, "OK", "Error", "%lld ? %lld (%d)", v_llong2, 012LL, sizeof( v_llong2 ) );
    mastest_exam( sizeof( v_llong3 ) == 8 && v_llong3 == LLONG_MAX, "OK", "Error", "%lld ? %lld (%d)", v_llong3, LLONG_MAX, sizeof( v_llong2 ) );
    mastest_exam( sizeof( v_llong4 ) == 8 && v_llong4 == LLONG_MIN, "OK", "Error", "%lld ? %lld (%d)", v_llong4, LLONG_MIN, sizeof( v_llong2 ) );

    mastest_next_group(  );
    mastest_exam( sizeof( v_double0 ) == 8
                  && v_double0 == 3.141592653589793, "OK", "Error", "%3.15f ? %3.15f (%d)", v_double0, 3.141592653589793, sizeof( v_double0 ) );
    mastest_exam( sizeof( v_ldouble0 ) == 16
                  && v_ldouble0 == ( long double ) 3.14159265358979323846L, "OK", "Error", "%3.20Lf ? %3.20Lf (%d)", v_ldouble0,
                  ( long double ) 3.14159265358979323846L, sizeof( v_ldouble0 ) );
    mastest_next_group(  );
    mastest_exam( mulconfnt_source_argno_count( osrc ) == 4, "OK", "Error", "%d", mulconfnt_source_argno_count( osrc ) );
    arg = mulconfnt_source_argno( osrc, 1 );
    mastest_exam( arg && 0 == strcmp( "something", arg ), "OK", "Error", "'%s' ? '%s'", "something", arg );
    arg = mulconfnt_source_argno( osrc, 2 );
    mastest_exam( arg && 0 == strcmp( "wow", arg ), "OK", "Error", "'%s' ? '%s'", "wow", arg );
    arg = mulconfnt_source_argno( osrc, 3 );
    mastest_exam( arg && 0 == strcmp( "abrakadabra", arg ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", arg );

    char **argsno = mulconfnt_source_argsno( osrc );

    mastest_exam( argsno && argsno[1] && 0 == strcmp( "something", argsno[1] ), "OK", "Error", "'%s' ? '%s'", "something", argsno[1] );
    mastest_exam( argsno && argsno[2] && 0 == strcmp( "wow", argsno[2] ), "OK", "Error", "'%s' ? '%s'", "wow", argsno[2] );
    mastest_exam( argsno && argsno[3] && 0 == strcmp( "abrakadabra", argsno[3] ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", argsno[3] );

    mastest_next_group(  );
    mastest_exam( bitwise1 == ( long ) 0xfffffffffffff8ffL, "OK", "Error", "%lx ? %lx", ( long ) 0xfffffffffffff8ffL, bitwise1 );
    mastest_exam( bitwise2 == ( long ) 0x10304L, "OK", "Error", "%lx ? %lx", ( long ) 0x10304L, bitwise2 );
    mastest_exam( bitwise3 == ( long ) 0x10004L, "OK", "Error", "%lx ? %lx", ( long ) 0x10004L, bitwise3 );

#if 0
    fprintf( stderr, "\nINT_MIN:%x;INT_MAX:%x\nLONG_MIN:%lx;LONG_MAX:%lx\nLLONG_MIN:%llx;LLONG_MAX:%llx\n", INT_MIN, INT_MAX, LONG_MIN, LONG_MAX,
             LLONG_MIN, LLONG_MAX );
    fprintf( stderr, "\nINT_MIN:%d;INT_MAX:%d\nLONG_MIN:%ld;LONG_MAX:%ld\nLLONG_MIN:%lld;LLONG_MAX:%lld\n", INT_MIN, INT_MAX, LONG_MIN, LONG_MAX,
             LLONG_MIN, LLONG_MAX );
#endif
    mulconfnt_source_list_delete( plist );
  }
  return 0;
}
