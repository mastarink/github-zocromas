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
test_4( int _uUu_ argc, const char _uUu_ * argv[], int nseries, const char *series_suffix )
{
  const char *arg _uUu_;
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

  const char *string_args = {
    "num0=5437:num1=0x12:num2=012:num3=2147483647:num4=-2147483648:" /* */
            "lnum0=0xffffffffff:lnum1=0xff:lnum2=0x7fffffffffffffff:lnum3=-12:lnum4=-0x8000000000000000:" /* */
            "llnum0=5437:llnum1=0x12:llnum2=012:llnum3=9223372036854775807:llnum4=-9223372036854775808:" /* */
            "bwi=0x700:bwi+=0x100:bwi-=0x200",
  };

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
    char fname[128];

    snprintf( fname, sizeof( fname ), "mastest_%d%s.commands", nseries, series_suffix );
    f = fopen( fname, "w" );

    if ( f )
    {
      fprintf( f, "%s\n", string_args );
      fclose( f );
    }
  }

  {
    setenv( "MASTEST_4", string_args, 0 );
    config_source_list_t *plist = mulconfnt_source_list_create(  );
    config_source_desc_t *osrc = mulconfnt_source_list_add_source( plist, MULCONF_SOURCE_ENV, 0, "MASTEST_4", ":", "=", NULL );

    mastest_next_group(  );
    mastest_exam( __LINE__, plist ? 1 : 0, "OK", "Error", "plist: %p", plist );

    mastest_exam( __LINE__, osrc ? 1 : 0, "OK", "Error", "osrc: %p", osrc );

    mulconfnt_source_lookup_all( osrc, &test_tablist );

    mastest_next_group(  );
    mastest_exam( __LINE__, 0 == strcmp( getenv( "MASTEST_4" ), string_args ), "OK", "Error", "MASTEST_4: %s ? %s", getenv( "MASTEST_4" ),
                  string_args );

    mastest_next_group(  );
    mastest_exam( __LINE__, !mulconfnt_error_source( osrc ), "OK", "Error", "mulconfnt_error: %d", mulconfnt_error_source( osrc ) );
    mastest_next_group(  );
    mastest_exam( __LINE__, sizeof( v_int0 ) == 4 && v_int0 == 5437, "OK", "Error", "num0=%d ? %d", v_int0, 5437 );
    mastest_exam( __LINE__, sizeof( v_int1 ) == 4 && v_int1 == 0x12, "OK", "Error", "num1=%d ? %d", v_int1, 0x12 );
    mastest_exam( __LINE__, sizeof( v_int2 ) == 4 && v_int2 == 012, "OK", "Error", "num2=%d ? %d", v_int2, 012 );
    mastest_exam( __LINE__, sizeof( v_int3 ) == 4 && v_int3 == INT_MAX, "OK", "Error", "num3=%d ? %d", v_int3, INT_MAX );
    mastest_exam( __LINE__, sizeof( v_int4 ) == 4 && v_int4 == INT_MIN, "OK", "Error", "num4=%d ? %d", v_int4, INT_MIN );

    mastest_next_group(  );
    mastest_exam( __LINE__, sizeof( v_long0 ) == 8 && v_long0 == 1099511627775L, "OK", "Error", "lnum0=%ld ? %ld", v_long0, 0xffffffffffL );
    mastest_exam( __LINE__, sizeof( v_long1 ) == 8 && v_long1 == 0xff, "OK", "Error", "lnum1=%ld ? %ld", v_long1, 0xffL );
    mastest_exam( __LINE__, sizeof( v_long2 ) == 8 && v_long2 == LONG_MAX, "OK", "Error", "lnum2=%ld ? %ld", v_long2, LONG_MAX );
    mastest_exam( __LINE__, sizeof( v_long2 ) == 8
                  && v_long2 == 0x7fffffffffffffffL, "OK", "Error", "lnum2=%lx ? %lx", v_long2, 0x7fffffffffffffffL );
    mastest_exam( __LINE__, sizeof( v_long2 ) == 8
                  && v_long2 == 9223372036854775807, "OK", "Error", "lnum2=%ld ? %ld", v_long2, 9223372036854775807 );
    mastest_exam( __LINE__, sizeof( v_long3 ) == 8 && v_long3 == -12L, "OK", "Error", "lnum3=%ld ? %ld", v_long3, -12L );
    mastest_exam( __LINE__, sizeof( v_long4 ) == 8 && v_long4 == LONG_MIN, "OK", "Error", "lnum4=%ld ? %ld", v_long4, LONG_MIN );

    mastest_next_group(  );
    mastest_exam( __LINE__, sizeof( v_llong0 ) == 8 && v_llong0 == 5437LL, "OK", "Error", "%lld ? %lld (%d)", v_llong0, 5437LL, sizeof( v_llong0 ) );
    mastest_exam( __LINE__, sizeof( v_llong1 ) == 8 && v_llong1 == 0x12LL, "OK", "Error", "%lld ? %lld (%d)", v_llong1, 0x12LL, sizeof( v_llong1 ) );
    mastest_exam( __LINE__, sizeof( v_llong2 ) == 8 && v_llong2 == 012LL, "OK", "Error", "%lld ? %lld (%d)", v_llong2, 012LL, sizeof( v_llong2 ) );
    mastest_exam( __LINE__, sizeof( v_llong3 ) == 8
                  && v_llong3 == LLONG_MAX, "OK", "Error", "%lld ? %lld (%d)", v_llong3, LLONG_MAX, sizeof( v_llong2 ) );
    mastest_exam( __LINE__, sizeof( v_llong4 ) == 8
                  && v_llong4 == LLONG_MIN, "OK", "Error", "%lld ? %lld (%d)", v_llong4, LLONG_MIN, sizeof( v_llong2 ) );

#if 0
    mastest_next_group(  );
    mastest_exam( __LINE__, mulconfnt_source_argc_no( osrc ) == 4, "OK", "Error", "%d", mulconfnt_source_argc_no( osrc ) );

    arg = mulconfnt_source_arg_no( osrc, 1 );
    mastest_exam( __LINE__, arg && 0 == strcmp( "something", arg ), "OK", "Error", "'%s' ? '%s'", "something", arg );
    arg = mulconfnt_source_arg_no( osrc, 2 );
    mastest_exam( __LINE__, arg && 0 == strcmp( "wow", arg ), "OK", "Error", "'%s' ? '%s'", "wow", arg );
    arg = mulconfnt_source_arg_no( osrc, 3 );
    mastest_exam( __LINE__, arg && 0 == strcmp( "abrakadabra", arg ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", arg );

    char **argsno = mulconfnt_source_argv_no( osrc );

    mastest_exam( __LINE__, argsno && argsno[1] && 0 == strcmp( "something", argsno[1] ), "OK", "Error", "'%s' ? '%s'", "something", argsno[1] );
    mastest_exam( __LINE__, argsno && argsno[2] && 0 == strcmp( "wow", argsno[2] ), "OK", "Error", "'%s' ? '%s'", "wow", argsno[2] );
    mastest_exam( __LINE__, argsno && argsno[3] && 0 == strcmp( "abrakadabra", argsno[3] ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", argsno[3] );

    mastest_next_group(  );
    mastest_exam( __LINE__, bitwise1 == ( long ) 0xfffffffffffff8ffL, "OK", "Error", "%lx ? %lx", ( long ) 0xfffffffffffff8ffL, bitwise1 );
    mastest_exam( __LINE__, bitwise2 == ( long ) 0x10304L, "OK", "Error", "%lx ? %lx", ( long ) 0x10304L, bitwise2 );
    mastest_exam( __LINE__, bitwise3 == ( long ) 0x10004L, "OK", "Error", "%lx ? %lx", ( long ) 0x10004L, bitwise3 );
#endif
    mulconfnt_source_list_delete( plist );
  }
  return 0;
}
