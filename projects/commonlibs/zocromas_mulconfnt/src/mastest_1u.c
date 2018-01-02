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
#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "global.h"
#include "option_tablist_base.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"
/* #include "mulconfnt_error.h" */

#include "mastest.h"

int
test_1u( int argc _uUu_, const char *argv[], int nseries, const char *series_suffix, int variant _uUu_ )
{
  int ifds _uUu_ = mastest_fds(  );
  const char *arg;

  unsigned char v_uchar0 = 0;
  unsigned short v_ushort0 = 0;

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

  mucs_option_t options[] = {
    {.s = {"num0", 0,.restype= MUCS_RTYP_UINT, .p=&v_uint0}}
    , {.s = {"aliasnum0", 0,.restype= MUCS_RTYP_ALIAS,.p= "num0"}}
    , {.s = {"cnum0", 0,.restype= MUCS_RTYP_UCHAR, .p=&v_uchar0}}
    , {.s = {"snum0", 0,.restype= MUCS_RTYP_USHORT, .p=&v_ushort0}}
    , {.s = {"num1", 0,.restype= MUCS_RTYP_UINT, .p=&v_uint1}}
    , {.s = {"num2", 0,.restype= MUCS_RTYP_UINT, .p=&v_uint2}}
    , {.s = {"num3", 0,.restype= MUCS_RTYP_UINT, .p=&v_uint3}}
    , {.s = {"num4", 0,.restype= MUCS_RTYP_UINT, .p=&v_uint4}}
    , {.s = {"lnum0", 0,.restype= MUCS_RTYP_ULONG, .p=&v_ulong0}}
    , {.s = {"lnum1", 0,.restype= MUCS_RTYP_UINT, .p=&v_ulong1}}
    , {.s = {"lnum2", 0,.restype= MUCS_RTYP_ULONG, .p=&v_ulong2}}
    , {.s = {"lnum3", 0,.restype= MUCS_RTYP_ULONG, .p=&v_ulong3}}
    , {.s = {"lnum4", 0,.restype= MUCS_RTYP_ULONG, .p=&v_ulong4}}
    , {.s = {"llnum0", 0,.restype= MUCS_RTYP_ULLONG, .p=&v_ullong0}}
    , {.s = {"llnum1", 0,.restype= MUCS_RTYP_ULLONG, .p=&v_ullong1}}
    , {.s = {"llnum2", 0,.restype= MUCS_RTYP_ULLONG, .p=&v_ullong2}}
    , {.s = {"llnum3", 0,.restype= MUCS_RTYP_ULLONG, .p=&v_ullong3}}
    , {.s = {"llnum4", 0,.restype= MUCS_RTYP_ULLONG, .p=&v_ullong4}}
    , {.s = {"bwi", 0,.restype= MUCS_RTYP_ULONG | MUCS_RTYP_BW_NOT, .p=&bitwise1, 0, "bitwise", "value"}}
    , {.s = {"bwi+", 0,.restype= MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR, .p=&bitwise2, 0, "bitwise", "value"}}
    , {.s = {"bwi-", 0,.restype= MUCS_RTYP_ULONG | MUCS_RTYP_BW_NOT | MUCS_RTYP_BW_AND, .p=&bitwise3, 0, "bitwise", "value"}}

    , {.s = {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL}} /* */
  };
  mucs_option_table_list_t test_tablist = {
    .next = NULL,.optcount = ( sizeof( options ) / sizeof( options[0] ) ) - 1,.name = "table-1u",.coptions = options, /* */
  };

  const char *xargv[] = {
    argv[0],
    "something",
    "--aliasnum0=5437",
    "--cnum0=207",
    "--snum0=5437",
    "--num1=0x12",
    "--num2=012",
    "--num3=2147483647",
    "--num4=4294967295",
  /* "--num4=43465367546365636353463294967295", */
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
    "--bwi+=0x100",
    "--bwi=0x700",
    "--bwi-=0x200",
    "abrakadabra",
  };
  int xargc = sizeof( xargv ) / sizeof( xargv[0] );

  {
    FILE *f;
    char fname[128];

    snprintf( fname, sizeof( fname ), "test/mastest_%d%s.commands", nseries, series_suffix );
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
    mucs_source_t *osrc = mucs_source_list_add_source_x( plist, MUCS_SOURCE_ARGV, NULL /*name */ , xargc, xargv, 0 /* min_pass */ , 0 /* npasses */, NULL, "=", NULL );

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
        for ( int i = 1; i < osrc->oldtarg.argc; i++ )
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
    EXAMX( !masregerrs_count_all_default( NULL, TRUE ), "mulconfnt_error: %d", masregerrs_count_all_default( NULL, TRUE ) );

    masexam_next_group(  );
    EXAMX( sizeof( v_uchar0 ) == 1, "num0=%d ? %d", sizeof( v_uchar0 ), 1 );
    EXAMX( sizeof( v_uchar0 ) == 1 && v_uchar0 == 207, "num0=%d ? %d", v_uchar0, 207 );
    EXAMX( sizeof( v_ushort0 ) == 2, "num0=%d ? %d", sizeof( v_ushort0 ), 2 );
    EXAMX( sizeof( v_ushort0 ) == 2 && v_ushort0 == 5437, "num0=%d ? %d", v_ushort0, 5437 );

    masexam_next_group(  );
  /* EXAMX( sizeof( v_uint0 ) == 4                                                                    */
  /*               && v_uint0 == 5437,  "num0=%u ? %u [%d]", v_uint0, 5437, mucs_error_source( osrc ) ); */
    EXAMX( sizeof( v_uint0 ) == 4 && v_uint0 == 5437, "num0=%u ? %u [%d]", v_uint0, 5437, masregerrs_count_all_default( NULL, TRUE ) );
    EXAMX( sizeof( v_uint1 ) == 4 && v_uint1 == 0x12, "num1=%u ? %u", v_uint1, 0x12 );
    EXAMX( sizeof( v_uint2 ) == 4 && v_uint2 == 012, "num2=%u ? %u", v_uint2, 012 );
    EXAMX( sizeof( v_uint3 ) == 4 && v_uint3 == 2147483647, "num3=%u ? %u", v_uint3, 2147483647 );
    EXAMX( sizeof( v_uint4 ) == 4 && v_uint4 == UINT_MAX, "num4=%u ? %u", v_uint4, UINT_MAX );

    masexam_next_group(  );
    EXAMX( sizeof( v_ulong0 ) == 8 && v_ulong0 == 1099511627775UL, "lnum0=%lu ? %lu", v_ulong0, 0xffffffffffUL );
    EXAMX( sizeof( v_ulong1 ) == 8 && v_ulong1 == 0xff, "lnum1=%lu ? %lu", v_ulong1, 0xffUL );
    EXAMX( sizeof( v_ulong2 ) == 8 && v_ulong2 == ULONG_MAX, "lnum2=%lu ? %lu", v_ulong2, ULONG_MAX );
    EXAMX( sizeof( v_ulong2 ) == 8 && v_ulong2 == 0xffffffffffffffffL, "lnum2=%lx ? %lx", v_ulong2, 0xffffffffffffffffUL );
    EXAMX( sizeof( v_ulong2 ) == 8 && v_ulong2 == 18446744073709551615UL, "lnum2=%lu ? %lu", v_ulong2, 18446744073709551615UL );
    EXAMX( sizeof( v_ulong3 ) == 8 && v_ulong3 == -12UL, "lnum3=%lu ? %lu", v_ulong3, -12UL );
    EXAMX( sizeof( v_ulong4 ) == 8
           && v_ulong4 == ( ( unsigned long ) LONG_MAX ) + 1, "lnum4=%lu ? %lu", v_ulong4, ( ( unsigned long ) LONG_MAX ) + 1 );

    masexam_next_group(  );
    EXAMX( sizeof( v_ullong0 ) == 8 && v_ullong0 == 5437ULL, "%llu ? %llu (%d)", v_ullong0, 5437ULL, sizeof( v_ullong0 ) );
    EXAMX( sizeof( v_ullong1 ) == 8 && v_ullong1 == 0x12ULL, "%llu ? %llu (%d)", v_ullong1, 0x12ULL, sizeof( v_ullong1 ) );
    EXAMX( sizeof( v_ullong2 ) == 8 && v_ullong2 == 012ULL, "%llu ? %llu (%d)", v_ullong2, 012ULL, sizeof( v_ullong2 ) );
    EXAMX( sizeof( v_ullong3 ) == 8 && v_ullong3 == ULLONG_MAX, "%llu ? %llu (%d)", v_ullong3, ULLONG_MAX, sizeof( v_ullong2 ) );
    EXAMX( sizeof( v_ullong4 ) == 8
           && v_ullong4 == ( ( unsigned long long ) LONG_MAX ) + 1, "%llu ? %llu (%d)", v_ullong4,
           ( ( unsigned long long ) LONG_MAX ) + 1, sizeof( v_ullong2 ) );
    masexam_next_group(  );
    EXAMX( mucs_source_argc_no( osrc ) == 4, "%d", mucs_source_argc_no( osrc ) );

    arg = mucs_source_arg_no( osrc, 1 );
    EXAMX( arg && 0 == mas_strcmp( "something", arg ), "'%s' ? '%s'", "something", arg );
    arg = mucs_source_arg_no( osrc, 2 );
    EXAMX( arg && 0 == mas_strcmp( "wow", arg ), "'%s' ? '%s'", "wow", arg );
    arg = mucs_source_arg_no( osrc, 3 );
    EXAMX( arg && 0 == mas_strcmp( "abrakadabra", arg ), "'%s' ? '%s'", "abrakadabra", arg );

    char **argvno = mucs_source_argv_no( osrc );
    int argcno = mucs_source_argc_no( osrc );

    EXAMX( argcno > 1 && argvno && argvno[1] && 0 == mas_strcmp( "something", argvno[1] ), "'%s' ? '%s'", "something", argcno > 1 ? argvno[1] : "?" );
    EXAMX( argcno > 2 && argvno && argvno[2] && 0 == mas_strcmp( "wow", argvno[2] ), "'%s' ? '%s'", "wow", argcno > 2 ? argvno[2] : "?" );
    EXAMX( argcno > 3 && argvno && argvno[3]
           && 0 == mas_strcmp( "abrakadabra", argvno[3] ), "'%s' ? '%s'", "abrakadabra", argcno > 3 ? argvno[3] : "?" );

    masexam_next_group(  );
    EXAMX( bitwise1 == ( unsigned long ) 0xfffffffffffff8ffUL, "%lx ? %lx", 0xfffffffffffff8ffULL, bitwise1 );
    EXAMX( bitwise2 == ( unsigned long ) 0x10304UL, "%lx ? %lx", 0x10304ULL, bitwise2 );
    EXAMX( bitwise3 == ( unsigned long ) 0x10004UL, "%lx ? %lx", 0x10004ULL, bitwise3 );

    mucs_source_list_delete( plist );
  }
  mucs_config_option_tablist_reset( &test_tablist, 1 );
  mucs_clear_global_flags(  );

  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  EXAM( mastest_fds(  ), ifds, "ifds=%d ? %d" );
  return 0;
}
