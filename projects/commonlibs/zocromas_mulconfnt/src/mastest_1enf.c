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

#define UINT_MIN 0
#define ULONG_MIN 0
#define ULLONG_MIN 0

/* test for not found option */
int
test_1enf( int argc _uUu_, const char *argv[], int nseries, const char *series_suffix, int variant _uUu_ )
{
  const char *arg _uUu_;
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
    "--aliasnum0=5437",
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

  mucs_option_t options[] = {
    {"num0other", 0, MUCS_RTYP_UINT, &v_uint0}
    , {"aliasnum0", 0, MUCS_RTYP_ALIAS, "num0"}
    , {"num1", 0, MUCS_RTYP_UINT, &v_uint1}
    , {"num2", 0, MUCS_RTYP_UINT, &v_uint2}
    , {"num3", 0, MUCS_RTYP_UINT, &v_uint3}
    , {"num4", 0, MUCS_RTYP_UINT, &v_uint4}
    , {"lnum0", 0, MUCS_RTYP_ULONG, &v_ulong0}
    , {"lnum1", 0, MUCS_RTYP_UINT, &v_ulong1}
    , {"lnum2", 0, MUCS_RTYP_ULONG, &v_ulong2}
    , {"lnum3", 0, MUCS_RTYP_ULONG, &v_ulong3}
    , {"lnum4", 0, MUCS_RTYP_ULONG, &v_ulong4}
    , {"llnum0", 0, MUCS_RTYP_ULLONG, &v_ullong0}
    , {"llnum1", 0, MUCS_RTYP_ULLONG, &v_ullong1}
    , {"llnum2", 0, MUCS_RTYP_ULLONG, &v_ullong2}
    , {"llnum3", 0, MUCS_RTYP_ULLONG, &v_ullong3}
    , {"llnum4", 0, MUCS_RTYP_ULLONG, &v_ullong4}
    , {"bwi", 0, MUCS_RTYP_ULONG | MUCS_RTYP_BW_NOT, &bitwise1, 0, "bitwise", "value"}
    , {"bwi+", 0, MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR, &bitwise2, 0, "bitwise", "value"}
    , {"bwi-", 0, MUCS_RTYP_ULONG | MUCS_RTYP_BW_NOT | MUCS_RTYP_BW_AND, &bitwise3, 0, "bitwise", "value"}

    , {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  mucs_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.options = options, /* */
  };
  {
    FILE *f;
    char fname[128];

    snprintf( fname, sizeof( fname ), "mastest_%d%s.commands", nseries, series_suffix );
    f = fopen( fname, "w" );

    if ( f )
    {
      for ( int i = 1; i < xargc; i++ )
      {
        fprintf( f, "%s\n", xargv[i] );
      }
      fclose( f );
    }
  }

  {
    mucs_source_list_t *plist = mucs_source_list_create(  );
    mucs_source_t *osrc = mucs_source_list_add_source_x( plist, MUCS_SOURCE_ARGV, xargc, xargv, NULL, "=", NULL );

    if ( osrc )
      osrc->flags |= MUCS_FLAG_SILENT;

    masexam_next_group(  );
    EXAMX( plist ? 1 : 0, "plist: %p", plist );
    EXAMX( osrc ? 1 : 0, "osrc: %p", osrc );

    if ( osrc && osrc->oldtarg.argc )
    {
      FILE *f;
      char fname[128];

      snprintf( fname, sizeof( fname ), "mastest_%d%s.args", nseries, series_suffix );
      f = fopen( fname, "w" );
      if ( f )
      {
        for ( int i = 1; i < osrc->oldtarg.argc; i++ )
        {
          fprintf( f, "%s\n", osrc->oldtarg.argv[i] );
        }
        fclose( f );
      }
    }
#if 1
    mucs_source_list_lookup_all( plist, &test_tablist );
#else
    mucs_source_lookup_all( osrc, &test_tablist );
#endif

    masexam_next_group(  );
  /* EXAMX( mucs_error_source( osrc ),  "mulconfnt_error: %d", mucs_error_source( osrc ) ); */
    EXAMX( masregerrs_count_all_default( NULL, TRUE ), "mulconfnt_error: %d", masregerrs_count_all_default( NULL, TRUE ) );
    masexam_next_group(  );
  /* EXAMX( sizeof( v_uint0 ) == 4 && v_uint0 == 0,  "num0=%u ? %u [%d]", v_uint0, 0, mucs_error_source( osrc ) ); */
    EXAMX( sizeof( v_uint0 ) == 4 && v_uint0 == 0, "num0=%u ? %u [%d]", v_uint0, 0, masregerrs_count_all_default( NULL, TRUE ) );
    EXAMX( sizeof( v_uint1 ) == 4 && v_uint1 == 0, "num1=%u ? %u", v_uint1, 0 );
    EXAMX( sizeof( v_uint2 ) == 4 && v_uint2 == 0, "num2=%u ? %u", v_uint2, 0 );
    EXAMX( sizeof( v_uint3 ) == 4 && v_uint3 == 0, "num3=%u ? %u", v_uint3, 0 );
    EXAMX( sizeof( v_uint4 ) == 4 && v_uint4 == 0, "num4=%u ? %u", v_uint4, 0 );

    masexam_next_group(  );
    EXAMX( sizeof( v_ulong0 ) == 8 && v_ulong0 == 0, "lnum0=%lu ? %lu", v_ulong0, 0 );
    EXAMX( sizeof( v_ulong1 ) == 8 && v_ulong1 == 0, "lnum1=%lu ? %lu", v_ulong1, 0 );
    EXAMX( sizeof( v_ulong2 ) == 8 && v_ulong2 == 0, "lnum2=%lu ? %lu", v_ulong2, 0 );
    EXAMX( sizeof( v_ulong2 ) == 8 && v_ulong2 == 0, "lnum2=%lx ? %lx", v_ulong2, 0 );
    EXAMX( sizeof( v_ulong2 ) == 8 && v_ulong2 == 0, "lnum2=%lu ? %lu", v_ulong2, 0 );
    EXAMX( sizeof( v_ulong3 ) == 8 && v_ulong3 == 0, "lnum3=%lu ? %lu", v_ulong3, 0 );
    EXAMX( sizeof( v_ulong4 ) == 8 && v_ulong4 == 0, "lnum4=%lu ? %lu", v_ulong4, 0 );

    masexam_next_group(  );
    EXAMX( sizeof( v_ullong0 ) == 8 && v_ullong0 == 0, "%llu ? %llu (%d)", v_ullong0, 0, sizeof( v_ullong0 ) );
    EXAMX( sizeof( v_ullong1 ) == 8 && v_ullong1 == 0, "%llu ? %llu (%d)", v_ullong1, 0, sizeof( v_ullong1 ) );
    EXAMX( sizeof( v_ullong2 ) == 8 && v_ullong2 == 0, "%llu ? %llu (%d)", v_ullong2, 0, sizeof( v_ullong2 ) );
    EXAMX( sizeof( v_ullong3 ) == 8 && v_ullong3 == 0, "%llu ? %llu (%d)", v_ullong3, 0, sizeof( v_ullong2 ) );
    EXAMX( sizeof( v_ullong4 ) == 8 && v_ullong4 == 0, "%llu ? %llu (%d)", v_ullong4, 0, sizeof( v_ullong2 ) );

    char **argvno _uUu_ = mucs_source_argv_no( osrc );
    int argcno = mucs_source_argc_no( osrc );

    masexam_next_group(  );
    EXAMX( argcno == 2, "%d", argcno );

    arg = mucs_source_arg_no( osrc, 1 );
    EXAMX( argcno > 1 && arg && 0 == mas_strcmp( "something", arg ), "'%s' ? '%s'", "something", arg );
#if 0
    arg = mucs_source_arg_no( osrc, 2 );
    EXAMX( argcno > 2 && arg && 0 == mas_strcmp( "wow", arg ), "'%s' ? '%s'", "wow", arg );
    arg = mucs_source_arg_no( osrc, 3 );
    EXAMX( argcno > 3 && arg && 0 == mas_strcmp( "abrakadabra", arg ), "'%s' ? '%s'", "abrakadabra", arg );

    EXAMX( argcno > 1 && argvno && argvno[1]
           && 0 == mas_strcmp( "something", argvno[1] ), "Error", "'%s' ? '%s'", "something", argcno > 1 && argvno && argvno[1] ? argvno[1] : "?" );
    EXAMX( argcno > 2 && argvno && argvno[2] && 0 == mas_strcmp( "wow", argvno[2] ), "'%s' ? '%s'", "wow", argcno > 2
           && argvno && argvno[2] ? argvno[2] : "?" );
    EXAMX( argcno > 3 && argvno && argvno[3]
           && 0 == mas_strcmp( "abrakadabra", argvno[3] ), "Error", "'%s' ? '%s'", "abrakadabra", argcno > 3 && argvno
           && argvno[3] ? argvno[3] : "?" );
#endif
    masexam_next_group(  );
    EXAMX( bitwise1 == ( unsigned long ) 0x10204, "bitwise1=%lx ? %lx", 0x10204, bitwise1 );
    EXAMX( bitwise2 == ( unsigned long ) 0x10204, "bitwise2=%lx ? %lx", 0x10204, bitwise2 );
    EXAMX( bitwise3 == ( unsigned long ) 0x10204, "bitwise3=%lx ? %lx", 0x10204, bitwise3 );

#if 0
    fprintf( stderr, "\nINT_MIN:%x;UINT_MAX:%x\nLONG_MIN:%lx;ULONG_MAX:%lx\nLLONG_MIN:%llx;ULLONG_MAX:%llx\n", UINT_MIN, UINT_MAX, ULONG_MIN,
             ULONG_MAX, ULLONG_MIN, ULLONG_MAX );
    fprintf( stderr, "\nINT_MIN:%d;UINT_MAX:%d\nLONG_MIN:%ld;ULONG_MAX:%ld\nLLONG_MIN:%lld;ULLONG_MAX:%lld\n", UINT_MIN, UINT_MAX, ULONG_MIN,
             ULONG_MAX, ULLONG_MIN, ULLONG_MAX );
#endif
    mucs_source_list_delete( plist );
  }
  mucs_config_option_tablist_reset( &test_tablist );
  
  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  return 0;
}
