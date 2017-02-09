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
test_1( int argc __attribute__ ( ( unused ) ), const char *argv[] __attribute__ ( ( unused ) ) )
{
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

  const char *xargv[] = {
    argv[0],
    "something",
    "--num0=5437",
    "--num1=0x12",
    "--num2=012",
    "--num3=2147483647",
    "--num4=-2147483648",
    "--lnum0=0xffffffffff",
    "--lnum1=0xff",
    "--lnum2=0x7fffffffffffffff",
    "--lnum3=-12",
    "--lnum4=-0x8000000000000000",
    "--llnum0=5437",
    "--llnum1=0x12",
    "--llnum2=012",
    "--llnum3=9223372036854775807",
    "--llnum4=-9223372036854775808",
    "abrakadabra",
  };
  int xargc = sizeof( xargv ) / sizeof( xargv[0] );

  config_option_t options[] = {
    {"num0", 0, MULCONF_RESTYPE_INT, &v_int0}
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

    , {.name = NULL,.shortname = 0,.restype = 0,.ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  config_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.options = options, /* */
  };

  {
    config_source_list_t *plist = mulconfnt_source_list_create(  );
    config_source_desc_t *osrc = mulconfnt_source_list_add_source( plist, MULCONF_SOURCE_ARGV, xargc, xargv, NULL, "=", NULL );

    mulconfnt_source_lookup( osrc, &test_tablist );

    mastest_exam( "test 1.1.1 ", sizeof( v_int0 ) == 4 && !mulconfnt_error(  ) && v_int0 == 5437, "OK", "Error", "%d ? %d", v_int0, 5437 );
    mastest_exam( "test 1.1.2 ", sizeof( v_int1 ) == 4 && !mulconfnt_error(  ) && v_int1 == 0x12, "OK", "Error", "%d ? %d", v_int1, 0x12 );
    mastest_exam( "test 1.1.3 ", sizeof( v_int2 ) == 4 && !mulconfnt_error(  ) && v_int2 == 012, "OK", "Error", "%d ? %d", v_int2, 012 );
    mastest_exam( "test 1.1.4 ", sizeof( v_int3 ) == 4 && !mulconfnt_error(  ) && v_int3 == INT_MAX, "OK", "Error", "%d ? %d", v_int3, INT_MAX );
    mastest_exam( "test 1.1.5 ", sizeof( v_int4 ) == 4 && !mulconfnt_error(  ) && v_int4 == INT_MIN, "OK", "Error", "%d ? %d", v_int4, INT_MIN );

    mastest_exam( "test 1.2.1 ", sizeof( v_long0 ) == 8 && !mulconfnt_error(  )
                  && v_long0 == 1099511627775L, "OK", "Error", "%ld ? %ld", v_long0, 0xffffffffffL );
    mastest_exam( "test 1.2.2 ", sizeof( v_long1 ) == 8 && !mulconfnt_error(  ) && v_long1 == 0xff, "OK", "Error", "%ld ? %ld", v_long1, 0xffL );
    mastest_exam( "test 1.2.3a", sizeof( v_long2 ) == 8 && !mulconfnt_error(  )
                  && v_long2 == LONG_MAX, "OK", "Error", "%ld ? %ld", v_long2, LONG_MAX );
    mastest_exam( "test 1.2.3b", sizeof( v_long2 ) == 8 && !mulconfnt_error(  )
                  && v_long2 == 0x7fffffffffffffffL, "OK", "Error", "%lx ? %lx", v_long2, 0x7fffffffffffffffL );
    mastest_exam( "test 1.2.3c", sizeof( v_long2 ) == 8 && !mulconfnt_error(  )
                  && v_long2 == 9223372036854775807, "OK", "Error", "%ld ? %ld", v_long2, 9223372036854775807 );
    mastest_exam( "test 1.2.4 ", sizeof( v_long3 ) == 8 && !mulconfnt_error(  ) && v_long3 == -12, "OK", "Error", "%ld ? %ld", v_long3, -12 );
    mastest_exam( "test 1.2.5 ", sizeof( v_long4 ) == 8 && !mulconfnt_error(  )
                  && v_long4 == LONG_MIN, "OK", "Error", "%ld ? %ld", v_long4, LONG_MIN );

    mastest_exam( "test 1.3.1 ", sizeof( v_llong0 ) == 8 && !mulconfnt_error(  )
                  && v_llong0 == 5437LL, "OK", "Error", "%lld ? %lld (%d)", v_llong0, 5437LL, sizeof( v_llong0 ) );
    mastest_exam( "test 1.3.2 ", sizeof( v_llong1 ) == 8 && !mulconfnt_error(  )
                  && v_llong1 == 0x12LL, "OK", "Error", "%lld ? %lld (%d)", v_llong1, 0x12LL, sizeof( v_llong1 ) );
    mastest_exam( "test 1.3.3 ", sizeof( v_llong2 ) == 8 && !mulconfnt_error(  )
                  && v_llong2 == 012LL, "OK", "Error", "%lld ? %lld (%d)", v_llong2, 012LL, sizeof( v_llong2 ) );
    mastest_exam( "test 1.3.4 ", sizeof( v_llong3 ) == 8 && !mulconfnt_error(  )
                  && v_llong3 == LLONG_MAX, "OK", "Error", "%lld ? %lld (%d)", v_llong3, LLONG_MAX, sizeof( v_llong2 ) );
    mastest_exam( "test 1.3.5 ", sizeof( v_llong4 ) == 8 && !mulconfnt_error(  )
                  && v_llong4 == LLONG_MIN, "OK", "Error", "%lld ? %lld (%d)", v_llong4, LLONG_MIN, sizeof( v_llong2 ) );
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
