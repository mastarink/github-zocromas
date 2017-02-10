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

#define UINT_MIN 0
#define ULONG_MIN 0
#define ULLONG_MIN 0
int
test_1u( int argc _uUu_, const char *argv[]_uUu_ )
{
  unsigned int v_uint0 = 0;
  unsigned int v_uint1 = 0;
  unsigned int v_uint2 = 0;
  unsigned int v_uint3 = 0;
  unsigned int v_uint4 = 0;

  unsigned long v_ulong0 = 0;
  unsigned long v_ulong1 = 0;
  unsigned long v_ulong2 = 0;
  unsigned long v_ulong3 = 0;
  unsigned long v_ulong4 = 0;

  unsigned long long v_ullong0 = 0;
  unsigned long long v_ullong1 = 0;
  unsigned long long v_ullong2 = 0;
  unsigned long long v_ullong3 = 0;
  unsigned long long v_ullong4 = 0;

  unsigned long bitwise1 = 0x10204;
  unsigned long bitwise2 = 0x10204;
  unsigned long bitwise3 = 0x10204;

  const char *xargv[] = {
    argv[0],
    "something",
    "--num0=5437",
    "--num1=0x12",
    "--num2=012",
    "--num3=2147483647",
    "--num4=4294967295",
    "--lnum0=0xffffffffff",
    "--lnum1=0xff",
    "--lnum2=0xffffffffffffffff",
    "wow",
    "--lnum3=-12",
    "--lnum4=0x8000000000000000",
    "--llnum0=5437",
    "--llnum1=0x12",
    "--llnum2=012",
    "--llnum3=18446744073709551615",
    "--llnum4=-9223372036854775808",
    "--bwi=0x700",
    "--bwi+=0x100",
    "--bwi-=0x200",
    "abrakadabra",
  };
  int xargc = sizeof( xargv ) / sizeof( xargv[0] );

  config_option_t options[] = {
    {"num0", 0, MULCONF_RESTYPE_UINT, &v_uint0}
    , {"num1", 0, MULCONF_RESTYPE_UINT, &v_uint1}
    , {"num2", 0, MULCONF_RESTYPE_UINT, &v_uint2}
    , {"num3", 0, MULCONF_RESTYPE_UINT, &v_uint3}
    , {"num4", 0, MULCONF_RESTYPE_UINT, &v_uint4}
    , {"lnum0", 0, MULCONF_RESTYPE_ULONG, &v_ulong0}
    , {"lnum1", 0, MULCONF_RESTYPE_UINT, &v_ulong1}
    , {"lnum2", 0, MULCONF_RESTYPE_ULONG, &v_ulong2}
    , {"lnum3", 0, MULCONF_RESTYPE_ULONG, &v_ulong3}
    , {"lnum4", 0, MULCONF_RESTYPE_ULONG, &v_ulong4}
    , {"llnum0", 0, MULCONF_RESTYPE_ULLONG, &v_ullong0}
    , {"llnum1", 0, MULCONF_RESTYPE_ULLONG, &v_ullong1}
    , {"llnum2", 0, MULCONF_RESTYPE_ULLONG, &v_ullong2}
    , {"llnum3", 0, MULCONF_RESTYPE_ULLONG, &v_ullong3}
    , {"llnum4", 0, MULCONF_RESTYPE_ULLONG, &v_ullong4}
    , {"bwi", 0, MULCONF_RESTYPE_ULONG | MULCONF_BITWISE_NOT, &bitwise1, 0, "bitwise", "value"}
    , {"bwi+", 0, MULCONF_RESTYPE_ULONG | MULCONF_BITWISE_OR, &bitwise2, 0, "bitwise", "value"}
    , {"bwi-", 0, MULCONF_RESTYPE_ULONG | MULCONF_BITWISE_NOT | MULCONF_BITWISE_AND, &bitwise3, 0, "bitwise", "value"}

    , {.name = NULL,.shortname = 0,.restype = 0,.ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  config_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.options = options, /* */
  };

  {
    config_source_list_t *plist = mulconfnt_source_list_create(  );
    config_source_desc_t *osrc = mulconfnt_source_list_add_source( plist, MULCONF_SOURCE_ARGV, xargc, xargv, NULL, "=", NULL );

    mulconfnt_source_lookup( osrc, &test_tablist );
    mastest_next_group(  );
    mastest_exam( sizeof( v_uint0 ) == 4 && !mulconfnt_error(  ) && v_uint0 == 5437, "OK", "Error", "%u ? %u", v_uint0, 5437 );
    mastest_exam( sizeof( v_uint1 ) == 4 && !mulconfnt_error(  ) && v_uint1 == 0x12, "OK", "Error", "%u ? %u", v_uint1, 0x12 );
    mastest_exam( sizeof( v_uint2 ) == 4 && !mulconfnt_error(  ) && v_uint2 == 012, "OK", "Error", "%u ? %u", v_uint2, 012 );
    mastest_exam( sizeof( v_uint3 ) == 4 && !mulconfnt_error(  ) && v_uint3 == 2147483647, "OK", "Error", "%u ? %u", v_uint3, 2147483647 );
    mastest_exam( sizeof( v_uint4 ) == 4 && !mulconfnt_error(  ) && v_uint4 == UINT_MAX, "OK", "Error", "%u ? %u", v_uint4, UINT_MAX );

    mastest_next_group(  );
    mastest_exam( sizeof( v_ulong0 ) == 8 && !mulconfnt_error(  )
                  && v_ulong0 == 1099511627775UL, "OK", "Error", "%lu ? %lu", v_ulong0, 0xffffffffffUL );
    mastest_exam( sizeof( v_ulong1 ) == 8 && !mulconfnt_error(  ) && v_ulong1 == 0xff, "OK", "Error", "%lu ? %lu", v_ulong1, 0xffUL );
    mastest_exam( sizeof( v_ulong2 ) == 8 && !mulconfnt_error(  ) && v_ulong2 == ULONG_MAX, "OK", "Error", "%lu ? %lu", v_ulong2, ULONG_MAX );
    mastest_exam( sizeof( v_ulong2 ) == 8 && !mulconfnt_error(  )
                  && v_ulong2 == 0xffffffffffffffffL, "OK", "Error", "%lx ? %lx", v_ulong2, 0xffffffffffffffffUL );
    mastest_exam( sizeof( v_ulong2 ) == 8 && !mulconfnt_error(  )
                  && v_ulong2 == 18446744073709551615UL, "OK", "Error", "%lu ? %lu", v_ulong2, 18446744073709551615UL );
    mastest_exam( sizeof( v_ulong3 ) == 8 && !mulconfnt_error(  ) && v_ulong3 == -12UL, "OK", "Error", "%lu ? %lu", v_ulong3, -12UL );
    mastest_exam( sizeof( v_ulong4 ) == 8 && !mulconfnt_error(  )
                  && v_ulong4 == ( ( unsigned long ) LONG_MAX ) + 1, "OK", "Error", "%lu ? %lu", v_ulong4, ( ( unsigned long ) LONG_MAX ) + 1 );

    mastest_next_group(  );
    mastest_exam( sizeof( v_ullong0 ) == 8 && !mulconfnt_error(  )
                  && v_ullong0 == 5437ULL, "OK", "Error", "%llu ? %llu (%d)", v_ullong0, 5437ULL, sizeof( v_ullong0 ) );
    mastest_exam( sizeof( v_ullong1 ) == 8 && !mulconfnt_error(  )
                  && v_ullong1 == 0x12ULL, "OK", "Error", "%llu ? %llu (%d)", v_ullong1, 0x12ULL, sizeof( v_ullong1 ) );
    mastest_exam( sizeof( v_ullong2 ) == 8 && !mulconfnt_error(  )
                  && v_ullong2 == 012ULL, "OK", "Error", "%llu ? %llu (%d)", v_ullong2, 012ULL, sizeof( v_ullong2 ) );
    mastest_exam( sizeof( v_ullong3 ) == 8 && !mulconfnt_error(  )
                  && v_ullong3 == ULLONG_MAX, "OK", "Error", "%llu ? %llu (%d)", v_ullong3, ULLONG_MAX, sizeof( v_ullong2 ) );
    mastest_exam( sizeof( v_ullong4 ) == 8 && !mulconfnt_error(  )
                  && v_ullong4 == ( ( unsigned long long ) LONG_MAX ) + 1, "OK", "Error", "%llu ? %llu (%d)", v_ullong4,
                  ( ( unsigned long long ) LONG_MAX ) + 1, sizeof( v_ullong2 ) );
    mastest_next_group(  );
    mastest_exam( mulconfnt_source_argno_count( osrc ) == 4, "OK", "Error", "%d", mulconfnt_source_argno_count( osrc ) );
    mastest_exam( 0 == strcmp( "something", mulconfnt_source_argno( osrc, 1 ) ), "OK", "Error", "'%s' ? '%s'", "something",
                  mulconfnt_source_argno( osrc, 1 ) );
    mastest_exam( 0 == strcmp( "wow", mulconfnt_source_argno( osrc, 2 ) ), "OK", "Error", "'%s' ? '%s'", "wow", mulconfnt_source_argno( osrc, 2 ) );
    mastest_exam( 0 == strcmp( "abrakadabra", mulconfnt_source_argno( osrc, 3 ) ), "OK", "Error", "'%s' ? '%s'", "abrakadabra",
                  mulconfnt_source_argno( osrc, 3 ) );

    char **argsno = mulconfnt_source_argsno( osrc );

    mastest_exam( 0 == strcmp( "something", argsno[1] ), "OK", "Error", "'%s' ? '%s'", "something", argsno[1] );
    mastest_exam( 0 == strcmp( "wow", argsno[2] ), "OK", "Error", "'%s' ? '%s'", "wow", argsno[2] );
    mastest_exam( 0 == strcmp( "abrakadabra", argsno[3] ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", argsno[3] );

    mastest_next_group(  );
    mastest_exam( bitwise1 == ( unsigned long ) 0xfffffffffffff8ffUL, "OK", "Error", "%lx ? %lx", 0xfffffffffffff8ffULL, bitwise1 );
    mastest_exam( bitwise2 == ( unsigned long ) 0x10304UL, "OK", "Error", "%lx ? %lx", 0x10304ULL, bitwise2 );
    mastest_exam( bitwise3 == ( unsigned long ) 0x10004UL, "OK", "Error", "%lx ? %lx", 0x10004ULL, bitwise3 );

#if 0
    fprintf( stderr, "\nINT_MIN:%x;UINT_MAX:%x\nLONG_MIN:%lx;ULONG_MAX:%lx\nLLONG_MIN:%llx;ULLONG_MAX:%llx\n", UINT_MIN, UINT_MAX, ULONG_MIN,
             ULONG_MAX, ULLONG_MIN, ULLONG_MAX );
    fprintf( stderr, "\nINT_MIN:%d;UINT_MAX:%d\nLONG_MIN:%ld;ULONG_MAX:%ld\nLLONG_MIN:%lld;ULLONG_MAX:%lld\n", UINT_MIN, UINT_MAX, ULONG_MIN,
             ULONG_MAX, ULLONG_MIN, ULLONG_MAX );
#endif
    mulconfnt_source_list_delete( plist );
  }
  return 0;
}
