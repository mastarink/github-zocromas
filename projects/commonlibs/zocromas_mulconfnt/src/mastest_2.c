#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/exam/masexam.h>

#include "mulconfnt_structs.h"

#include "option_tablist_base.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"
/* #include "mulconfnt_error.h" */

#include "mastest.h"

int
test_2( int _uUu_ argc, const char _uUu_ * argv[], int nseries, const char *series_suffix, int variant _uUu_ )
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

  mucs_option_t options[] = {
    {"num0", 0, MUCS_RTYP_INT, &v_int0}
    , {"num1", 0, MUCS_RTYP_INT, &v_int1}
    , {"num2", 0, MUCS_RTYP_INT, &v_int2}
    , {"num3", 0, MUCS_RTYP_INT, &v_int3}
    , {"num4", 0, MUCS_RTYP_INT, &v_int4}
    , {"lnum0", 0, MUCS_RTYP_LONG, &v_long0}
    , {"lnum1", 0, MUCS_RTYP_INT, &v_long1}
    , {"lnum2", 0, MUCS_RTYP_LONG, &v_long2}
    , {"lnum3", 0, MUCS_RTYP_LONG, &v_long3}
    , {"lnum4", 0, MUCS_RTYP_LONG, &v_long4}
    , {"llnum0", 0, MUCS_RTYP_LLONG, &v_llong0}
    , {"llnum1", 0, MUCS_RTYP_LLONG, &v_llong1}
    , {"llnum2", 0, MUCS_RTYP_LLONG, &v_llong2}
    , {"llnum3", 0, MUCS_RTYP_LLONG, &v_llong3}
    , {"llnum4", 0, MUCS_RTYP_LLONG, &v_llong4}
    , {"bwi", 0, MUCS_RTYP_LONG | MUCS_RTYP_BW_NOT, &bitwise1, 0, "bitwise", "value"}
    , {"bwi+", 0, MUCS_RTYP_LONG | MUCS_RTYP_BW_OR, &bitwise2, 0, "bitwise", "value"}
    , {"bwi-", 0, MUCS_RTYP_LONG | MUCS_RTYP_BW_NOT | MUCS_RTYP_BW_AND, &bitwise3, 0, "bitwise", "value"}

    , {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  mucs_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.coptions = options, /* */
  };
  {
    FILE *f;
    char fname[128];

    snprintf( fname, sizeof( fname ), "test/mastest_%d%s.commands", nseries, series_suffix );
    f = fopen( fname, "w" );

    if ( f )
    {
      fprintf( f, "%s\n", string_args );
      fclose( f );
    }
  }

  {
    mucs_source_list_t *plist = mucs_source_list_create(  );
    mucs_source_t *osrc = mucs_source_list_add_source_x( plist, MUCS_SOURCE_STRING, 0, string_args, ":", "=", NULL );

    masexam_next_group(  );
    EXAMX( plist ? 1 : 0, "plist: %p", plist );
    EXAMX( osrc ? 1 : 0, "osrc: %p", osrc );

    if ( osrc && osrc->oldtarg.argc )
    {
      FILE *f;
      char fname[128];

      snprintf( fname, sizeof( fname ), "test/mastest_%d%s.args", nseries, series_suffix );
      f = fopen( fname, "w" );
      if ( f )
      {
        for ( int i = 0; i < osrc->oldtarg.argc; i++ )
        {
          fprintf( f, "%s\n", osrc->oldtarg.argv[i] );
        }
        fclose( f );
      }
    }
#if 1
    mucs_source_list_lookup_all( plist, &test_tablist, NULL );
#else
    mucs_source_lookup_all( osrc, &test_tablist, NULL );
#endif

    masexam_next_group(  );
  /* EXAMX( !mucs_error_source( osrc ),  "mulconfnt_error: %d", mucs_error_source( osrc ) ); */
    EXAMX( !masregerrs_count_all_default( NULL, FALSE ), "mulconfnt_error: %d", masregerrs_count_all_default( NULL, TRUE ) );
    masexam_next_group(  );
    EXAMX( sizeof( v_int0 ) == 4 && v_int0 == 5437, "num0=%d ? %d", v_int0, 5437 );
    EXAMX( sizeof( v_int1 ) == 4 && v_int1 == 0x12, "num1=%d ? %d", v_int1, 0x12 );
    EXAMX( sizeof( v_int2 ) == 4 && v_int2 == 012, "num2=%d ? %d", v_int2, 012 );
    EXAMX( sizeof( v_int3 ) == 4 && v_int3 == INT_MAX, "num3=%d ? %d", v_int3, INT_MAX );
    EXAMX( sizeof( v_int4 ) == 4 && v_int4 == INT_MIN, "num4=%d ? %d", v_int4, INT_MIN );

    masexam_next_group(  );
    EXAMX( sizeof( v_long0 ) == 8 && v_long0 == 1099511627775L, "lnum0=%ld ? %ld", v_long0, 0xffffffffffL );
    EXAMX( sizeof( v_long1 ) == 8 && v_long1 == 0xff, "lnum1=%ld ? %ld", v_long1, 0xffL );
    EXAMX( sizeof( v_long2 ) == 8 && v_long2 == LONG_MAX, "lnum2=%ld ? %ld", v_long2, LONG_MAX );
    EXAMX( sizeof( v_long2 ) == 8 && v_long2 == 0x7fffffffffffffffL, "lnum2=%lx ? %lx", v_long2, 0x7fffffffffffffffL );
    EXAMX( sizeof( v_long2 ) == 8 && v_long2 == 9223372036854775807, "lnum2=%ld ? %ld", v_long2, 9223372036854775807 );
    EXAMX( sizeof( v_long3 ) == 8 && v_long3 == -12L, "lnum3=%ld ? %ld", v_long3, -12L );
    EXAMX( sizeof( v_long4 ) == 8 && v_long4 == LONG_MIN, "lnum4=%ld ? %ld", v_long4, LONG_MIN );

    masexam_next_group(  );
    EXAMX( sizeof( v_llong0 ) == 8 && v_llong0 == 5437LL, "%lld ? %lld (%d)", v_llong0, 5437LL, sizeof( v_llong0 ) );
    EXAMX( sizeof( v_llong1 ) == 8 && v_llong1 == 0x12LL, "%lld ? %lld (%d)", v_llong1, 0x12LL, sizeof( v_llong1 ) );
    EXAMX( sizeof( v_llong2 ) == 8 && v_llong2 == 012LL, "%lld ? %lld (%d)", v_llong2, 012LL, sizeof( v_llong2 ) );
    EXAMX( sizeof( v_llong3 ) == 8 && v_llong3 == LLONG_MAX, "%lld ? %lld (%d)", v_llong3, LLONG_MAX, sizeof( v_llong2 ) );
    EXAMX( sizeof( v_llong4 ) == 8 && v_llong4 == LLONG_MIN, "%lld ? %lld (%d)", v_llong4, LLONG_MIN, sizeof( v_llong2 ) );

#if 0
    masexam_next_group(  );
    EXAMX( mucs_source_argc_no( osrc ) == 4, "%d", mucs_source_argc_no( osrc ) );

    arg = mucs_source_arg_no( osrc, 1 );
    EXAMX( arg && 0 == mas_strcmp( "something", arg ), "'%s' ? '%s'", "something", arg );
    arg = mucs_source_arg_no( osrc, 2 );
    EXAMX( arg && 0 == mas_strcmp( "wow", arg ), "'%s' ? '%s'", "wow", arg );
    arg = mucs_source_arg_no( osrc, 3 );
    EXAMX( arg && 0 == mas_strcmp( "abrakadabra", arg ), "'%s' ? '%s'", "abrakadabra", arg );

    char **argvno = mucs_source_argv_no( osrc );

    EXAMX( argvno && argvno[1] && 0 == mas_strcmp( "something", argvno[1] ), "'%s' ? '%s'", "something", argvno[1] );
    EXAMX( argvno && argvno[2] && 0 == mas_strcmp( "wow", argvno[2] ), "'%s' ? '%s'", "wow", argvno[2] );
    EXAMX( argvno && argvno[3] && 0 == mas_strcmp( "abrakadabra", argvno[3] ), "'%s' ? '%s'", "abrakadabra", argvno[3] );

    masexam_next_group(  );
    EXAMX( bitwise1 == ( long ) 0xfffffffffffff8ffL, "%lx ? %lx", ( long ) 0xfffffffffffff8ffL, bitwise1 );
    EXAMX( bitwise2 == ( long ) 0x10304L, "%lx ? %lx", ( long ) 0x10304L, bitwise2 );
    EXAMX( bitwise3 == ( long ) 0x10004L, "%lx ? %lx", ( long ) 0x10004L, bitwise3 );
#endif
    mucs_source_list_delete( plist );
  }
  mucs_config_option_tablist_reset( &test_tablist );

  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  return 0;
}
