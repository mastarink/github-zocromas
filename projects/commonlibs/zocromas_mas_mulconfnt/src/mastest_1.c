#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "option_tablist_base.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"
#include "mulconfnt_error.h"

#include "mastest.h"

static int _do_fprintf = 0;
static int v_int5 = 0;
static char *sc_string = NULL;
static char *cc_string = NULL;

int
ccallback_string( mucs_option_han_t * opt )
{
  if ( opt )
  {
    if ( _do_fprintf )
    {
      if ( opt->string_value )
        fprintf( stderr, "CB STRING: %s='%s'\n", opt->name ? ( *opt->name ? opt->name : "<EMPTY NAME>" ) : "<NONAME>", opt->string_value );
      else
        fprintf( stderr, "CB STRING: %s\n", opt->name ? ( *opt->name ? opt->name : "<EMPTY NAME>" ) : "<NONAME>" );
    }

    if ( opt->restype == MULCONF_RTYP_STRING )
    {
      if ( cc_string )
        mas_free( cc_string );
      cc_string = mas_strdup( opt->string_value );
    }
  }
  return 0;
}

int
scallback_string( mucs_option_han_t * opt )
{
  if ( sc_string )
    mas_free( sc_string );
//fprintf( stderr, "CB STRING: %s\n", opt->string_value );
  sc_string = mas_strdup( opt->string_value );
  return 0;
}

int
num5callback( mucs_option_han_t * opt )
{
//fprintf( stderr, "NUM5: %d\n", opt->nvalue.v_int );
  v_int5 = opt->nvalue.v_int;
  return 0;
}

int
test_1( int argc _uUu_, const char *argv[], int nseries, const char *series_suffix, int do_fprintf _uUu_ )
{
  const char *arg;
  char *v_string0 = NULL;
  char *v_string1 = NULL;

  mas_argvc_t v_targ0;

  char v_char0 = 0;
  short v_short0 = 0;

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
    "--string1", "lorem ipsum",
    "--string2", "Phasellus congue bibendum magna",
    "--targ0=asta",
    "--cnum0=107",
    "--snum0=5437",
    "--targ0=manyana",
    "--num0=5437",
    "--num1=0x12",
    "--num2=012",
    "--num3=2147483647",
    "--num4=-2147483648",
    "--num5=1111111111",
    "--targ0=venenatis",
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
    "--",
    "--this-is-not-an-option1=345",
    "--this-is-not-an-option2",
    "--",
    "abrakadabra",
  };
  int xargc = sizeof( xargv ) / sizeof( xargv[0] );

  _do_fprintf = do_fprintf;
#define NUM_NOPTS 4 + 2 + 1

  mucs_option_han_t options[] = {
    {"string0", 0, MULCONF_RTYP_STRING | MULCONF_RTYP_FLAG_AUTOFREE, &v_string0}
    , {"string1", 0, MULCONF_RTYP_STRING, &v_string1}
    , {"string2", 0, MULCONF_RTYP_STRING}
    , {"targ0", 0, MULCONF_RTYP_TARG | MULCONF_RTYP_FLAG_AUTOFREE, &v_targ0}
    , {"cnum0", 0, MULCONF_RTYP_CHAR, &v_char0}
    , {"snum0", 0, MULCONF_RTYP_SHORT, &v_short0}
    , {"num0", 0, MULCONF_RTYP_INT, &v_int0}
    , {"num1", 0, MULCONF_RTYP_INT, &v_int1}
    , {"num2", 0, MULCONF_RTYP_INT, &v_int2}
    , {"num3", 0, MULCONF_RTYP_INT, &v_int3}
    , {"num4", 0, MULCONF_RTYP_INT, &v_int4}
    , {"num5", 0, MULCONF_RTYP_INT,.callback = num5callback}
    , {"lnum0", 0, MULCONF_RTYP_LONG, &v_long0}
    , {"lnum1", 0, MULCONF_RTYP_INT, &v_long1}
    , {"lnum2", 0, MULCONF_RTYP_LONG, &v_long2}
    , {"lnum3", 0, MULCONF_RTYP_LONG, &v_long3}
    , {"lnum4", 0, MULCONF_RTYP_LONG, &v_long4}
    , {"llnum0", 0, MULCONF_RTYP_LLONG, &v_llong0}
    , {"llnum1", 0, MULCONF_RTYP_LLONG, &v_llong1}
    , {"llnum2", 0, MULCONF_RTYP_LLONG, &v_llong2}
    , {"llnum3", 0, MULCONF_RTYP_LLONG, &v_llong3}
    , {"llnum4", 0, MULCONF_RTYP_LLONG, &v_llong4}
    , {"pi", 0, MULCONF_RTYP_DOUBLE, &v_double0}
    , {"longpi", 0, MULCONF_RTYP_LDOUBLE, &v_ldouble0}
    , {"bwi", 0, MULCONF_RTYP_LONG | MULCONF_RTYP_FLAG_BITWISE_NOT, &bitwise1, 0, "bitwise", "value"}
    , {"bwi+", 0, MULCONF_RTYP_LONG | MULCONF_RTYP_FLAG_BITWISE_OR, &bitwise2, 0, "bitwise", "value"}
    , {"bwi-", 0, MULCONF_RTYP_LONG | MULCONF_RTYP_FLAG_BITWISE_NOT | MULCONF_RTYP_FLAG_BITWISE_AND, &bitwise3, 0, "bitwise", "value"}
    , {"", 0, MULCONF_RTYP_FLAG_LASTOPT}

    , {.name = NULL,.shortname = 0,.restype = 0,.ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
#if 0
  mucs_option_table_list_t *test_tablist =
          mucs_config_option_tablist_create_setup( "test-table", options, sizeof( options ) / sizeof( options[0] ) );
#elif 0
  mucs_option_table_list_t *test_tablist =
          mucs_config_option_tablist_add( NULL, test_tablist, "test-table", options, sizeof( options ) / sizeof( options[0] ) );
#else
  mucs_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.options = options,
  };
#endif
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

    mastest_next_group(  );
    mastest_exam( __LINE__, plist ? 1 : 0, "OK", "Error", "plist: %p", plist );
    mucs_source_han_t *osrc = mucs_source_list_add_source( plist, MULCONF_SOURCE_ARGV, xargc, xargv, NULL, "=", NULL );

    mucs_source_set_common_callback( osrc, ccallback_string );
    mucs_source_set_type_callback( osrc, MULCONF_RTYP_STRING, scallback_string );
    mastest_exam( __LINE__, osrc ? 1 : 0, "OK", "Error", "osrc: %p", osrc );

    mucs_source_lookup_all( osrc, &test_tablist );
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

    mastest_next_group(  );
    mastest_exam( __LINE__, !mucs_error_source( osrc ), "OK", "Error", "mulconfnt_error: %d", mucs_error_source( osrc ) );
    mastest_next_group(  );
    mastest_exam( __LINE__, v_string0
                  && 0 == mas_strcmp( v_string0, "lorem-ipsum" ), "OK", "Error", "string0=%s ? %s", v_string0 ? v_string0 : "<NULL>", "lorem-ipsum" );
  /* if ( v_string0 )         */
  /*   mas_free( v_string0 ); */
  /* v_string0 = NULL; */
    mastest_exam( __LINE__, v_string1
                  && 0 == mas_strcmp( v_string1, "lorem ipsum" ), "OK", "Error", "string1=%s ? %s", v_string1 ? v_string1 : "<NULL>", "lorem ipsum" );
    if ( v_string1 )                                                 // if !MULCONF_RTYP_FLAG_AUTOFREE or !ptr
      mas_free( v_string1 );
    v_string1 = NULL;

    mastest_exam( __LINE__, sc_string
                  && 0 == mas_strcmp( sc_string, "Phasellus congue bibendum magna" ), "OK", "Error", "string1=%s ? %s",
                  sc_string ? sc_string : "<NULL>", "Phasellus congue bibendum magna" );
    if ( sc_string )
      mas_free( sc_string );
    sc_string = NULL;

    mastest_exam( __LINE__, cc_string
                  && 0 == mas_strcmp( cc_string, "Phasellus congue bibendum magna" ), "OK", "Error", "string1=%s ? %s",
                  cc_string ? cc_string : "<NULL>", "Phasellus congue bibendum magna" );
    if ( cc_string )
      mas_free( cc_string );
    cc_string = NULL;

    mastest_next_group(  );
    mastest_exam( __LINE__, v_targ0.argc == 3, "OK", "Error", "targ0.argc=%d ? %d", v_targ0.argc, 3 );
    mastest_exam( __LINE__, v_targ0.argc == 3
                  && 0 == mas_strcmp( v_targ0.argv[0], "asta" ), "OK", "Error", "targ0.argv[0]=%s ? %s", v_targ0.argv[0], "asta" );
    mastest_exam( __LINE__, v_targ0.argc == 3
                  && 0 == mas_strcmp( v_targ0.argv[1], "manyana" ), "OK", "Error", "targ0.argv[1]=%s ? %s", v_targ0.argv[1], "manyana" );
    mastest_exam( __LINE__, v_targ0.argc == 3
                  && 0 == mas_strcmp( v_targ0.argv[2], "venenatis" ), "OK", "Error", "targ0.argv[2]=%s ? %s", v_targ0.argv[2], "venenatis" );
  /* mas_argvc_delete( &v_targ0 ); */

    mastest_next_group(  );
    mastest_exam( __LINE__, sizeof( v_char0 ) == 1, "OK", "Error", "num0=%d ? %d", sizeof( v_char0 ), 1 );
    mastest_exam( __LINE__, sizeof( v_char0 ) == 1 && v_char0 == 107, "OK", "Error", "num0=%d ? %d", v_char0, 107 );
    mastest_exam( __LINE__, sizeof( v_short0 ) == 2, "OK", "Error", "num0=%d ? %d", sizeof( v_short0 ), 2 );
    mastest_exam( __LINE__, sizeof( v_short0 ) == 2 && v_short0 == 5437, "OK", "Error", "num0=%d ? %d", v_short0, 5437 );

    mastest_next_group(  );
    mastest_exam( __LINE__, sizeof( v_int0 ) == 4, "OK", "Error", "num0=%d ? %d", sizeof( v_int0 ), 4 );
    mastest_exam( __LINE__, sizeof( v_int0 ) == 4 && v_int0 == 5437, "OK", "Error", "num0=%d ? %d", v_int0, 5437 );
    mastest_exam( __LINE__, sizeof( v_int1 ) == 4 && v_int1 == 0x12, "OK", "Error", "num1=%d ? %d", v_int1, 0x12 );
    mastest_exam( __LINE__, sizeof( v_int2 ) == 4 && v_int2 == 012, "OK", "Error", "num2=%d ? %d", v_int2, 012 );
    mastest_exam( __LINE__, sizeof( v_int3 ) == 4 && v_int3 == INT_MAX, "OK", "Error", "num3=%d ? %d", v_int3, INT_MAX );
    mastest_exam( __LINE__, sizeof( v_int4 ) == 4 && v_int4 == INT_MIN, "OK", "Error", "num4=%d ? %d", v_int4, INT_MIN );
    mastest_exam( __LINE__, sizeof( v_int5 ) == 4 && v_int5 == 1111111111, "OK", "Error", "num4=%d ? %d", v_int5, 1111111111 );

    mastest_next_group(  );
    mastest_exam( __LINE__, sizeof( v_long0 ) == 8, "OK", "Error", "%ld ? %ld", sizeof( v_long0 ), 8 );
    mastest_exam( __LINE__, sizeof( v_long0 ) == 8 && v_long0 == 1099511627775L, "OK", "Error", "%ld ? %ld", v_long0, 0xffffffffffL );
    mastest_exam( __LINE__, sizeof( v_long1 ) == 8 && v_long1 == 0xff, "OK", "Error", "%ld ? %ld", v_long1, 0xffL );
    mastest_exam( __LINE__, sizeof( v_long2 ) == 8 && v_long2 == LONG_MAX, "OK", "Error", "%ld ? %ld", v_long2, LONG_MAX );
    mastest_exam( __LINE__, sizeof( v_long2 ) == 8 && v_long2 == 0x7fffffffffffffffL, "OK", "Error", "%lx ? %lx", v_long2, 0x7fffffffffffffffL );
    mastest_exam( __LINE__, sizeof( v_long2 ) == 8 && v_long2 == 9223372036854775807, "OK", "Error", "%ld ? %ld", v_long2, 9223372036854775807 );
    mastest_exam( __LINE__, sizeof( v_long3 ) == 8 && v_long3 == -12L, "OK", "Error", "%ld ? %ld", v_long3, -12L );
    mastest_exam( __LINE__, sizeof( v_long4 ) == 8 && v_long4 == LONG_MIN, "OK", "Error", "%ld ? %ld", v_long4, LONG_MIN );

    mastest_next_group(  );
    mastest_exam( __LINE__, sizeof( v_llong0 ) == 8, "OK", "Error", "%lld ? %lld ", sizeof( v_llong0 ), 8 );
    mastest_exam( __LINE__, sizeof( v_llong0 ) == 8 && v_llong0 == 5437LL, "OK", "Error", "%lld ? %lld (%d)", v_llong0, 5437LL, sizeof( v_llong0 ) );
    mastest_exam( __LINE__, sizeof( v_llong1 ) == 8 && v_llong1 == 0x12LL, "OK", "Error", "%lld ? %lld (%d)", v_llong1, 0x12LL, sizeof( v_llong1 ) );
    mastest_exam( __LINE__, sizeof( v_llong2 ) == 8 && v_llong2 == 012LL, "OK", "Error", "%lld ? %lld (%d)", v_llong2, 012LL, sizeof( v_llong2 ) );
    mastest_exam( __LINE__, sizeof( v_llong3 ) == 8
                  && v_llong3 == LLONG_MAX, "OK", "Error", "%lld ? %lld (%d)", v_llong3, LLONG_MAX, sizeof( v_llong2 ) );
    mastest_exam( __LINE__, sizeof( v_llong4 ) == 8
                  && v_llong4 == LLONG_MIN, "OK", "Error", "%lld ? %lld (%d)", v_llong4, LLONG_MIN, sizeof( v_llong2 ) );

    mastest_next_group(  );
    mastest_exam( __LINE__, sizeof( v_double0 ) == 8
                  && v_double0 == 3.141592653589793, "OK", "Error", "%3.15f ? %3.15f (%d)", v_double0, 3.141592653589793, sizeof( v_double0 ) );
    mastest_exam( __LINE__, sizeof( v_ldouble0 ) == 16
                  && v_ldouble0 == ( long double ) 3.14159265358979323846L, "OK", "Error", "%3.20Lf ? %3.20Lf (%d)", v_ldouble0,
                  ( long double ) 3.14159265358979323846L, sizeof( v_ldouble0 ) );

    char **argvno = mucs_source_argv_no( osrc );
    int argcno = mucs_source_argc_no( osrc );

    mastest_next_group(  );
    mastest_exam( __LINE__, argcno == NUM_NOPTS, "OK", "Error", "%d", argcno, NUM_NOPTS );
    if ( !( argcno == NUM_NOPTS ) )
    {
      for ( int i = 0; i < argcno; i++ )
      {
        fprintf( stderr, " @@ %d %s\n", i, mucs_source_arg_no( osrc, i ) );
      }
    }
    arg = mucs_source_arg_no( osrc, 1 );
    mastest_exam( __LINE__, arg && 0 == mas_strcmp( "something", arg ), "OK", "Error", "'%s' ? '%s'", "something", arg );
    arg = mucs_source_arg_no( osrc, 2 );
    mastest_exam( __LINE__, arg && 0 == mas_strcmp( "wow", arg ), "OK", "Error", "'%s' ? '%s'", "wow", arg );
    arg = mucs_source_arg_no( osrc, 3 );
    mastest_exam( __LINE__, arg
                  && 0 == mas_strcmp( "--this-is-not-an-option1=345", arg ), "OK", "Error", "'%s' ? '%s'", "--this-is-not-an-option1=345", arg );
    arg = mucs_source_arg_no( osrc, 4 );
    mastest_exam( __LINE__, arg
                  && 0 == mas_strcmp( "--this-is-not-an-option2", arg ), "OK", "Error", "'%s' ? '%s'", "--this-is-not-an-option2", arg );
    arg = mucs_source_arg_no( osrc, 5 );
    mastest_exam( __LINE__, arg && 0 == mas_strcmp( "--", arg ), "OK", "Error", "'%s' ? '%s'", "--", arg );
    arg = mucs_source_arg_no( osrc, 6 );
    mastest_exam( __LINE__, arg && 0 == mas_strcmp( "abrakadabra", arg ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", arg );

    mastest_exam( __LINE__, argcno > 1 && argvno && argvno[1]
                  && 0 == mas_strcmp( "something", argvno[1] ), "OK", "Error", "'%s' ? '%s'", "something", argcno > 1 ? argvno[1] : "?" );
    mastest_exam( __LINE__, argcno > 2 && argvno && argvno[2]
                  && 0 == mas_strcmp( "wow", argvno[2] ), "OK", "Error", "'%s' ? '%s'", "wow", argcno > 2 ? argvno[2] : "?" );
    mastest_exam( __LINE__, argcno > 3 && argvno && argvno[3]
                  && 0 == mas_strcmp( "--this-is-not-an-option1=345", argvno[3] ), "OK", "Error", "'%s' ? '%s'", "--this-is-not-an-option1=345",
                  argcno > 3 ? argvno[3] : "?" );
    mastest_exam( __LINE__, argcno > 4 && argvno && argvno[4]
                  && 0 == mas_strcmp( "--this-is-not-an-option2", argvno[4] ), "OK", "Error", "'%s' ? '%s'", "--this-is-not-an-option2",
                  argcno > 4 ? argvno[4] : "?" );
    mastest_exam( __LINE__, argcno > 5 && argvno && argvno[5]
                  && 0 == mas_strcmp( "--", argvno[5] ), "OK", "Error", "'%s' ? '%s'", "--", argcno > 5 ? argvno[5] : "?" );
    mastest_exam( __LINE__, argcno > 6 && argvno && argvno[6]
                  && 0 == mas_strcmp( "abrakadabra", argvno[6] ), "OK", "Error", "'%s' ? '%s'", "abrakadabra", argcno > 6 ? argvno[6] : "?" );

    mastest_next_group(  );
    mastest_exam( __LINE__, bitwise1 == ( long ) 0xfffffffffffff8ffL, "OK", "Error", "%lx ? %lx", ( long ) 0xfffffffffffff8ffL, bitwise1 );
    mastest_exam( __LINE__, bitwise2 == ( long ) 0x10304L, "OK", "Error", "%lx ? %lx", ( long ) 0x10304L, bitwise2 );
    mastest_exam( __LINE__, bitwise3 == ( long ) 0x10004L, "OK", "Error", "%lx ? %lx", ( long ) 0x10004L, bitwise3 );

#if 0
    fprintf( stderr, "\nINT_MIN:%x;INT_MAX:%x\nLONG_MIN:%lx;LONG_MAX:%lx\nLLONG_MIN:%llx;LLONG_MAX:%llx\n", INT_MIN, INT_MAX, LONG_MIN, LONG_MAX,
             LLONG_MIN, LLONG_MAX );
    fprintf( stderr, "\nINT_MIN:%d;INT_MAX:%d\nLONG_MIN:%ld;LONG_MAX:%ld\nLLONG_MIN:%lld;LLONG_MAX:%lld\n", INT_MIN, INT_MAX, LONG_MIN, LONG_MAX,
             LLONG_MIN, LLONG_MAX );
#endif
    mucs_source_list_delete( plist );
  }
  mucs_config_option_tablist_close( &test_tablist );
  return 0;
}
