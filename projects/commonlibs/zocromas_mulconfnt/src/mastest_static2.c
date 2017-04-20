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

#include "option_interface_base.h"
#include "option_interface.h"

#include "mastest.h"

static int _variant = 0;
static int v_int5 = 0;

static int
s2callback( mucs_option_t * opt _uUu_, void *userdata )
{
  EXAMS( opt->d.string_value, "Phasellus congue bibendum magna", "string_value=%s ? %s" );
  EXAMS( ( const char * ) userdata, "something to pass: consectetuer adipiscing", "userdata=%s ? %s" );
/* WARN( "SMTHNG:%s", ( const char * ) userdata ); */
  return 0;
}

static int
num5callback( mucs_option_t * opt, void *userdata _uUu_ )
{
//fprintf( stderr, "NUM5: %d\n", opt->d.nvalue.v_int );
  v_int5 = opt->d.nvalue.v_int;
  return 0;
}

int
test_static2( int argc _uUu_, const char *argv[], int nseries, const char *series_suffix, int variant _uUu_ )
{
  int ifds _uUu_ = mastest_fds(  );
  char *v_string0 = NULL;
  char *v_string1 = NULL;
  char *v_string2 _uUu_ = NULL;
  char *v_string3 _uUu_ = NULL;

  mas_argvc_t v_targ0 = { 0 };
  mas_argvc_t v_targ_n = { 0 };

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
  long bitwise4 = 0x10204;
  long bitwise5 = 0x10204;

  double v_double0 = 0;
  long double v_ldouble0 = 0;

  _variant = variant;

  mucs_option_static_t soptions[] = {
    {.name = MUCS_NONOPT_NAME,.shortn = '\0',.restype = MUCS_RTYP_TARG,.cust_ptr = &v_targ_n,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "string0",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &v_string0,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "string1",.shortn = '\0',.restype = MUCS_RTYP_STRING,.cust_ptr = &v_string1},
    {.name = "string2",.shortn = '\0',.restype = MUCS_RTYP_STRING,.callback = s2callback},
    {.name = "string3",.shortn = 'p',.restype = MUCS_RTYP_STRING,.cust_ptr = &v_string3,.flags = 0 | MUCS_FLAG_NO_CB_IF_VALUE},
    {.name = "string4",.shortn = 'x',.restype = MUCS_RTYP_STRING,.flags = 0 | MUCS_FLAG_NO_COMMON_CB | MUCS_FLAG_NO_TYPE_CB},
    {.name = "targ0",.shortn = '\0',.restype = MUCS_RTYP_TARG,.cust_ptr = &v_targ0,.flags = MUCS_FLAG_AUTOFREE},
    {.name = "cnum0",.shortn = '\0',.restype = MUCS_RTYP_CHAR,.cust_ptr = &v_char0},
    {.name = "snum0",.shortn = '\0',.restype = MUCS_RTYP_SHORT,.cust_ptr = &v_short0},
    {.name = "num0",.shortn = '\0',.restype = MUCS_RTYP_INT,.cust_ptr = &v_int0},
    {.name = "num1",.shortn = '\0',.restype = MUCS_RTYP_INT,.cust_ptr = &v_int1},
    {.name = "num2",.shortn = '\0',.restype = MUCS_RTYP_INT,.cust_ptr = &v_int2},
    {.name = "num3",.shortn = '\0',.restype = MUCS_RTYP_INT,.cust_ptr = &v_int3},
    {.name = "num4",.shortn = '\0',.restype = MUCS_RTYP_INT,.cust_ptr = &v_int4},
    {.name = "num5",.shortn = '\0',.restype = MUCS_RTYP_INT,.callback = num5callback},
    {.name = "lnum0",.shortn = '\0',.restype = MUCS_RTYP_LONG,.cust_ptr = &v_long0},
    {.name = "lnum1",.shortn = '\0',.restype = MUCS_RTYP_INT,.cust_ptr = &v_long1},
    {.name = "lnum2",.shortn = '\0',.restype = MUCS_RTYP_LONG,.cust_ptr = &v_long2},
    {.name = "lnum3",.shortn = '\0',.restype = MUCS_RTYP_LONG,.cust_ptr = &v_long3},
    {.name = "lnum4",.shortn = '\0',.restype = MUCS_RTYP_LONG,.cust_ptr = &v_long4},
    {.name = "llnum0",.shortn = '\0',.restype = MUCS_RTYP_LLONG,.cust_ptr = &v_llong0},
    {.name = "llnum1",.shortn = '\0',.restype = MUCS_RTYP_LLONG,.cust_ptr = &v_llong1},
    {.name = "llnum2",.shortn = '\0',.restype = MUCS_RTYP_LLONG,.cust_ptr = &v_llong2},
    {.name = "llnum3",.shortn = '\0',.restype = MUCS_RTYP_LLONG,.cust_ptr = &v_llong3},
    {.name = "llnum4",.shortn = '\0',.restype = MUCS_RTYP_LLONG,.cust_ptr = &v_llong4},
    {.name = "pi",.shortn = '\0',.restype = MUCS_RTYP_DOUBLE,.cust_ptr = &v_double0},
    {.name = "longpi",.shortn = '\0',.restype = MUCS_RTYP_LDOUBLE,.cust_ptr = &v_ldouble0},
    {.name = "bwi",.shortn = '\0',.restype = MUCS_RTYP_LONG | MUCS_RTYP_BW_NOT,.cust_ptr = &bitwise1, 0, "bitwise", "value"},
    {.name = "bwi+",.shortn = '\0',.restype = MUCS_RTYP_LONG | MUCS_RTYP_BW_OR,.cust_ptr = &bitwise2, 0, "bitwise", "value"},
    {.name = "bwi-",.shortn = '\0',.restype = MUCS_RTYP_LONG | MUCS_RTYP_BW_NOT | MUCS_RTYP_BW_AND,.cust_ptr = &bitwise3, 0, "bitwise", "value"},
    {.name = "bwix",.shortn = '\0',.restype = MUCS_RTYP_LONG | MUCS_RTYP_BW_XOR,.cust_ptr = &bitwise4, 0, "bitwise", "value"},
    {.name = "bwix1",.shortn = '\0',.restype = MUCS_RTYP_LONG | MUCS_RTYP_BW_XOR,.cust_ptr = &bitwise5, 0, "bitwise", "value"},
    {.name = "",.shortn = '$',.flags = MUCS_FLAG_LASTOPT},

    {.name = NULL,.shortn = '\0',.restype = 0,.cust_ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };

  const char *xargv[] = {
    argv[0],
    "something",
    "--string0=lorem-ipsum",
    "--string1", "lorem ipsum",
    "--string2", "Phasellus congue bibendum magna",
  /* TODO  "--string3 Something right here" */
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
  /* "--num6=235234534253245345345324532453245324532453425324532453245235234534253245", */
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
    "--bwix=0x10f00",
    "--bwix1=0x10f00", "--bwix1=0x10f00",
    "--pi=3.141592653589793",
    "--longpi=3.1415926535897932384626433832795028841971693993751058209749445",
    "-p", "Congue bibendum magna phasellus",
    "-x", "Bibendum magna phasellus congue",
    "--",
    "--this-is-not-an-option1=345",
    "--this-is-not-an-option2",
    "--",
    "abrakadabra",
  };
  int xargc = sizeof( xargv ) / sizeof( xargv[0] );

#define NUM_NOPTS 4 + 2 + 1

  mucs_option_interface_t *interface = mucs_config_soption_interface_create_setup( "table-manual", soptions, TRUE );

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
    mucs_source_t *osrc = mucs_option_interface_add_source( interface, MUCS_SOURCE_ARGV, NULL /* name */ , xargc, xargv, 0 );

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
    mucs_option_interface_lookup_all( interface, ( void * ) "something to pass: consectetuer adipiscing" );
    if ( osrc && osrc->oldtarg.argc )

      masexam_next_group(  );
    EXAM( ( masregerrs_count_all_default( NULL, FALSE ) ), ( unsigned ) 0, "ERRORS: %d ? %d" );

    masexam_next_group(  );
    EXAMS( v_string0, "lorem-ipsum", "v_string0=%s ? %s" );
#if 0
  /* Don't: Auto-free */
    if ( v_string0 )
      mas_free( v_string0 );
    v_string0 = NULL;
#endif
    EXAMS( v_string1, "lorem ipsum", "v_string1=%s ? %s" );
/* No Don't: Auto-free */
    if ( v_string1 )                                                 // if !MUCS_RTYP_FLAG_AUTOFREE or !ptr
      mas_free( v_string1 );
    v_string1 = NULL;

    if ( v_string3 )                                                 // if !MUCS_RTYP_FLAG_AUTOFREE or !ptr
      mas_free( v_string3 );
    v_string3 = NULL;

    masexam_next_group(  );
    EXAM( v_targ0.argc, 3, "targ0.argc=%d ? %d" );
    EXAMSN( v_targ0.argc, v_targ0.argv, 0, "asta", "'%s' ? '%s'" );
    EXAMSN( v_targ0.argc, v_targ0.argv, 1, "manyana", "'%s' ? '%s'" );
    EXAMSN( v_targ0.argc, v_targ0.argv, 2, "venenatis", "'%s' ? '%s'" );

#if 0
  /* Don't: Auto-free */
    mas_argvc_delete( &v_targ0 );
#endif
    masexam_next_group(  );
    EXAM( sizeof( v_char0 ), 1U, "sizeof( v_char0 )=%d ? %d" );
    EXAMZ( 1, v_char0, 107, "char0=%d ? %d" );
    EXAM( sizeof( v_short0 ), 2U, "sizeof( v_short0 )=%d ? %d" );
    EXAMZ( 2, v_short0, 5437, "short0=%d ? %d" );

    masexam_next_group(  );
    EXAM( sizeof( v_int0 ), 4U, "sizeof( v_int0 )=%d ? %d" );
    EXAMZ( 4, v_int0, 5437, "num0=%d ? %d" );
    EXAMZ( 4, v_int1, 0x12, "num1=%d ? %d" );
    EXAMZ( 4, v_int2, 012, "num2=%d ? %d" );
    EXAMZ( 4, v_int3, INT_MAX, "num3=%d ? %d" );
    EXAMZ( 4, v_int4, INT_MIN, "num4=%d ? %d" );
    EXAMZ( 4, v_int5, 1111111111, "num5=%d ? %d" );

    masexam_next_group(  );
    EXAM( sizeof( v_long0 ), 8U, "sizeof( v_long0 )=%d ? %d" );
    EXAMZ( 8, v_long0, 1099511627775L, "long0=%ld ? %ld" );
    EXAMZ( 8, v_long1, 0xffL, "long0=%ld ? %ld" );
    EXAMZ( 8, v_long2, LONG_MAX, "long2=%ld ? %ld" );
    EXAMZ( 8, v_long2, 0x7fffffffffffffffL, "long2=%ld ? %ld" );
    EXAMZ( 8, v_long2, 9223372036854775807, "long2=%ld ? %ld" );
    EXAMZ( 8, v_long3, -12L, "long3=%ld ? %ld" );
    EXAMZ( 8, v_long4, LONG_MIN, "long4=%ld ? %ld" );

    masexam_next_group(  );
    EXAM( sizeof( v_llong0 ), 8U, "sizeof( v_llong0 )=%d ? %d" );
    EXAMZ( 8, v_llong0, 5437LL, "llong0=%lld ? %lld" );
    EXAMZ( 8, v_llong1, 0x12LL, "llong1=%lld ? %lld" );
    EXAMZ( 8, v_llong2, 012LL, "llong2=%lld ? %lld" );
    EXAMZ( 8, v_llong3, LLONG_MAX, "llong3=%lld ? %lld" );
    EXAMZ( 8, v_llong4, LLONG_MIN, "llong4=%lld ? %lld" );

    masexam_next_group(  );
    EXAMZ( 8, v_double0, 3.141592653589793, "double0=%3.15f ? %3.15f" );
    EXAMZ( 16, v_ldouble0, ( long double ) 3.14159265358979323846L, "ldouble0=%3.25Lf ? %3.25Lf" );

    char **argvno = mucs_source_argv_no( osrc );
    int argcno = mucs_source_argc_no( osrc );

    masexam_next_group(  );
    EXAMZ( 4, argcno, NUM_NOPTS, "NUM_NOPTS=%d ? %d" );
    if ( !( argcno == NUM_NOPTS ) )
    {
      for ( int i = 0; i < argcno; i++ )
      {
        fprintf( stderr, " @@ %d %s\n", i, mucs_source_arg_no( osrc, i ) );
      }
    }
    {
      int nn = 0;

      nn = 0;
      EXAMS( mucs_source_arg_no( osrc, ++nn ), "something", "'%s' ? '%s'" );
      EXAMS( mucs_source_arg_no( osrc, ++nn ), "wow", "'%s' ? '%s'" );
      EXAMS( mucs_source_arg_no( osrc, ++nn ), "--this-is-not-an-option1=345", "'%s' ? '%s'" );
      EXAMS( mucs_source_arg_no( osrc, ++nn ), "--this-is-not-an-option2", "'%s' ? '%s'" );
      EXAMS( mucs_source_arg_no( osrc, ++nn ), "--", "'%s' ? '%s'" );
      EXAMS( mucs_source_arg_no( osrc, ++nn ), "abrakadabra", "'%s' ? '%s'" );

      nn = 0;
      EXAMSN( argcno, argvno, ++nn, "something", "'%s' ? '%s'" );
      EXAMSN( argcno, argvno, ++nn, "wow", "'%s' ? '%s'" );
      EXAMSN( argcno, argvno, ++nn, "--this-is-not-an-option1=345", "'%s' ? '%s'" );
      EXAMSN( argcno, argvno, ++nn, "--this-is-not-an-option2", "'%s' ? '%s'" );
      EXAMSN( argcno, argvno, ++nn, "--", "'%s' ? '%s'" );
      EXAMSN( argcno, argvno, ++nn, "abrakadabra", "'%s' ? '%s'" );

      masexam_next_group(  );
      EXAM( bitwise1, ( long ) 0xfffffffffffff8ffL, "%lx ? %lx" );
      EXAM( bitwise2, ( long ) 0x10304L, "%lx ? %lx" );
      EXAM( bitwise3, ( long ) 0x10004L, "%lx ? %lx" );
      EXAM( bitwise4, ( long ) 0xd04L, "%lx ? %lx" );
      EXAM( bitwise5, ( long ) 0x10204L, "%lx ? %lx" );

      EXAM( v_targ_n.argc, NUM_NOPTS, "spec list size:%d ? %d" );
      nn = 0;
      EXAMSN( v_targ_n.argc, v_targ_n.argv, ++nn, "something", "'%s' ? '%s'" );
      EXAMSN( v_targ_n.argc, v_targ_n.argv, ++nn, "wow", "'%s' ? '%s'" );
      EXAMSN( v_targ_n.argc, v_targ_n.argv, ++nn, "--this-is-not-an-option1=345", "'%s' ? '%s'" );
      EXAMSN( v_targ_n.argc, v_targ_n.argv, ++nn, "--this-is-not-an-option2", "'%s' ? '%s'" );
      EXAMSN( v_targ_n.argc, v_targ_n.argv, ++nn, "--", "'%s' ? '%s'" );
      EXAMSN( v_targ_n.argc, v_targ_n.argv, ++nn, "abrakadabra", "'%s' ? '%s'" );
    }
  }
  EXAMX( v_string0 != NULL, "v_string0 %s", v_string0 );
/* WARN("v_string0:%p",v_string0); */

  WARN( "v_string0:%p", v_string0 );

  mucs_config_option_interface_delete( interface );
  interface = NULL;
  mucs_clear_global_flags(  );
  EXAMX( v_string0 == NULL, "v_string0 %s", v_string0 );

  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  WARN("### FDs:%d" ,mastest_fds(  ));
  EXAM( mastest_fds(  ), ifds, "ifds=%d ? %d" );
  return 0;
}
