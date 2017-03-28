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
test_1mul( int argc _uUu_, const char _uUu_ * argv[], int nseries, const char *series_suffix, int do_fprintf _uUu_ )
{
  masregerrs_delete_default( NULL );
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

  const char *xargv1[] = {
    "something",
    "--string0=lorem-ipsum",
    "--string1", "lorem ipsum",
    "--num0=5437",
    "--num1=0x12",
    "--num2=012",
    "--num3=2147483647",
    "--num4=-2147483648",
    "--lnum0=0xffffffffff",
    "--lnum1=0xff",
    "--lnum2=0x7fffffffffffffff",
    "wow",
    NULL
  };
  const char *xargv2[] = {
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
    NULL
  };
#define NUM_NOPTS 3

  const char **xxargv[2] = { &xargv1[0], &xargv2[0] };
  int xxargc = sizeof( xxargv ) / sizeof( xxargv[0] );
  int argc1 = sizeof( xxargv[0] ) / sizeof( xxargv[0][0] );
  int argc2 = sizeof( xxargv[1] ) / sizeof( xxargv[1][0] );
  int totargc _uUu_ = argc1 + argc2;

  mucs_option_han_t options[] = {
    {"string0", 0, MUCS_RTYP_STRING, &v_string0,.flags = 0 | MUCS_FLAG_AUTOFREE}
    , {"string1", 0, MUCS_RTYP_STRING, &v_string1,.flags = 0 | MUCS_FLAG_AUTOFREE}
    , {"num0", 0, MUCS_RTYP_INT, &v_int0}
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
    , {"pi", 0, MUCS_RTYP_DOUBLE, &v_double0}
    , {"longpi", 0, MUCS_RTYP_LDOUBLE, &v_ldouble0}
    , {"bwi", 0, MUCS_RTYP_LONG | MUCS_RTYP_BW_NOT, &bitwise1, 0, "bitwise", "value"}
    , {"bwi+", 0, MUCS_RTYP_LONG | MUCS_RTYP_BW_OR, &bitwise2, 0, "bitwise", "value"}
    , {"bwi-", 0, MUCS_RTYP_LONG | MUCS_RTYP_BW_NOT | MUCS_RTYP_BW_AND, &bitwise3, 0, "bitwise", "value"}

    , {.name = NULL,.shortname = 0,.restype = 0,.ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
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
      for ( int i = 0; i < xxargc; i++ )
      {
        for ( int j = 0; xxargv[i][j]; j++ )
        {
          fprintf( f, "%s\n", xxargv[i][j] );
        }
      }
      fclose( f );
    }
  }
  {
    mucs_source_list_t *plist = mucs_source_list_create(  );
    mucs_source_han_t *osrc = mucs_source_list_add_source( plist, MUCS_SOURCE_MARGV, xxargc, xxargv, NULL, "=", NULL );

    masexam_next_group(  );
    EXAMX( plist ? 1 : 0, "OK", "Error", "plist: %p", plist );
    EXAMX( osrc ? 1 : 0, "OK", "Error", "osrc: %p", osrc );
    mucs_source_lookup_all( osrc, &test_tablist );
    if ( osrc && osrc->oldtarg.argc )
    {
      FILE *f;
      char fname[128];

      snprintf( fname, sizeof( fname ), "mastest_%d%s.args", nseries, series_suffix );
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

    masexam_next_group(  );
    /* EXAMX( !mucs_error_source( osrc ), "OK", "Error", "mulconfnt_error: %d", mucs_error_source( osrc ) ); */
    EXAMX( !masregerrs_count_all_default( NULL, TRUE ), "OK", "Error", "mulconfnt_error: %d", masregerrs_count_all_default( NULL, TRUE ) );
    masexam_next_group(  );
    EXAMX( v_string0
                  && 0 == mas_strcmp( v_string0, "lorem-ipsum" ), "OK", "Error", "string0=%s ? %s", v_string0 ? v_string0 : "<NULL>", "lorem-ipsum" );
#if 0
  /* Don't: Auto-free */
    if ( v_string0 )
      mas_free( v_string0 );
    v_string0 = NULL;
#endif
    EXAMX( v_string1
                  && 0 == mas_strcmp( v_string1, "lorem ipsum" ), "OK", "Error", "string1=%s ? %s", v_string1 ? v_string1 : "<NULL>", "lorem ipsum" );
#if 0
  /* Don't: Don't: Auto-free */
    if ( v_string1 )
      mas_free( v_string1 );
    v_string1 = NULL;
#endif
    masexam_next_group(  );
    EXAMX( sizeof( v_int0 ) == 4 && v_int0 == 5437, "OK", "Error", "num0=%d ? %d", v_int0, 5437 );
    EXAMX( sizeof( v_int1 ) == 4 && v_int1 == 0x12, "OK", "Error", "num1=%d ? %d", v_int1, 0x12 );
    EXAMX( sizeof( v_int2 ) == 4 && v_int2 == 012, "OK", "Error", "num2=%d ? %d", v_int2, 012 );
    EXAMX( sizeof( v_int3 ) == 4 && v_int3 == INT_MAX, "OK", "Error", "num3=%d ? %d", v_int3, INT_MAX );
    EXAMX( sizeof( v_int4 ) == 4 && v_int4 == INT_MIN, "OK", "Error", "num4=%d ? %d", v_int4, INT_MIN );

    masexam_next_group(  );
    EXAMX( sizeof( v_long0 ) == 8 && v_long0 == 1099511627775L, "OK", "Error", "%ld ? %ld", v_long0, 0xffffffffffL );
    EXAMX( sizeof( v_long1 ) == 8 && v_long1 == 0xff, "OK", "Error", "%ld ? %ld", v_long1, 0xffL );
    EXAMX( sizeof( v_long2 ) == 8 && v_long2 == LONG_MAX, "OK", "Error", "%ld ? %ld", v_long2, LONG_MAX );
    EXAMX( sizeof( v_long2 ) == 8 && v_long2 == 0x7fffffffffffffffL, "OK", "Error", "%lx ? %lx", v_long2, 0x7fffffffffffffffL );
    EXAMX( sizeof( v_long2 ) == 8 && v_long2 == 9223372036854775807, "OK", "Error", "%ld ? %ld", v_long2, 9223372036854775807 );
    EXAMX( sizeof( v_long3 ) == 8 && v_long3 == -12L, "OK", "Error", "%ld ? %ld", v_long3, -12L );
    EXAMX( sizeof( v_long4 ) == 8 && v_long4 == LONG_MIN, "OK", "Error", "%ld ? %ld", v_long4, LONG_MIN );

    masexam_next_group(  );
    EXAMX( sizeof( v_llong0 ) == 8 && v_llong0 == 5437LL, "OK", "Error", "%lld ? %lld (%d)", v_llong0, 5437LL, sizeof( v_llong0 ) );
    EXAMX( sizeof( v_llong1 ) == 8 && v_llong1 == 0x12LL, "OK", "Error", "%lld ? %lld (%d)", v_llong1, 0x12LL, sizeof( v_llong1 ) );
    EXAMX( sizeof( v_llong2 ) == 8 && v_llong2 == 012LL, "OK", "Error", "%lld ? %lld (%d)", v_llong2, 012LL, sizeof( v_llong2 ) );
    EXAMX( sizeof( v_llong3 ) == 8
                  && v_llong3 == LLONG_MAX, "OK", "Error", "%lld ? %lld (%d)", v_llong3, LLONG_MAX, sizeof( v_llong2 ) );
    EXAMX( sizeof( v_llong4 ) == 8
                  && v_llong4 == LLONG_MIN, "OK", "Error", "%lld ? %lld (%d)", v_llong4, LLONG_MIN, sizeof( v_llong2 ) );

    masexam_next_group(  );
    EXAMX( sizeof( v_double0 ) == 8
                  && v_double0 == 3.141592653589793, "OK", "Error", "%3.15f ? %3.15f (%d)", v_double0, 3.141592653589793, sizeof( v_double0 ) );
    EXAMX( sizeof( v_ldouble0 ) == 16
                  && v_ldouble0 == ( long double ) 3.14159265358979323846L, "OK", "Error", "%3.20Lf ? %3.20Lf (%d)", v_ldouble0,
                  ( long double ) 3.14159265358979323846L, sizeof( v_ldouble0 ) );

    char **argvno = mucs_source_argv_no( osrc );
    int argcno = mucs_source_argc_no( osrc );

    masexam_next_group(  );
    EXAMX( argcno == NUM_NOPTS, "OK", "Error", "%d ? %d", argcno, NUM_NOPTS );

    arg = mucs_source_arg_no( osrc, 0 );
    EXAMX( arg && 0 == mas_strcmp( "something", arg ), "OK", "Error", "'%s' ? '%s'", "something", arg );
    arg = mucs_source_arg_no( osrc, 1 );
    EXAMX( arg && 0 == mas_strcmp( "wow", arg ), "OK", "Error", "'%s' ? '%s'", "wow", arg );
    arg = mucs_source_arg_no( osrc, 2 );
    EXAMX( arg && 0 == mas_strcmp( "abrakadabra", arg ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", arg );

    EXAMX( argcno > 0 && argvno && argvno[0]
                  && 0 == mas_strcmp( "something", argvno[0] ), "OK", "Error", "'%s' ? '%s'", "something", argcno > 0 ? argvno[0] : "?" );
    EXAMX( argcno > 1 && argvno && argvno[1]
                  && 0 == mas_strcmp( "wow", argvno[1] ), "OK", "Error", "'%s' ? '%s'", "wow", argcno > 1 ? argvno[1] : "?" );
    EXAMX( argcno > 2 && argvno && argvno[2]
                  && 0 == mas_strcmp( "abrakadabra", argvno[2] ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", argcno > 2 ? argvno[2] : "?" );

    masexam_next_group(  );
    EXAMX( bitwise1 == ( long ) 0xfffffffffffff8ffL, "OK", "Error", "%lx ? %lx", ( long ) 0xfffffffffffff8ffL, bitwise1 );
    EXAMX( bitwise2 == ( long ) 0x10304L, "OK", "Error", "%lx ? %lx", ( long ) 0x10304L, bitwise2 );
    EXAMX( bitwise3 == ( long ) 0x10004L, "OK", "Error", "%lx ? %lx", ( long ) 0x10004L, bitwise3 );

#if 0
    fprintf( stderr, "\nINT_MIN:%x;INT_MAX:%x\nLONG_MIN:%lx;LONG_MAX:%lx\nLLONG_MIN:%llx;LLONG_MAX:%llx\n", INT_MIN, INT_MAX, LONG_MIN, LONG_MAX,
             LLONG_MIN, LLONG_MAX );
    fprintf( stderr, "\nINT_MIN:%d;INT_MAX:%d\nLONG_MIN:%ld;LONG_MAX:%ld\nLLONG_MIN:%lld;LLONG_MAX:%lld\n", INT_MIN, INT_MAX, LONG_MIN, LONG_MAX,
             LLONG_MIN, LLONG_MAX );
#endif
    mucs_source_list_delete( plist );
  }
  mucs_config_option_tablist_reset( &test_tablist );
  return 0;
}
