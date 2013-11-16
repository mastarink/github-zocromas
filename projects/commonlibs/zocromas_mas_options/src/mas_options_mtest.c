#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_opts_types.h>
#include <mastar/types/mas_control_types.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/control/mas_control.h>

#include <mastar/tools/mas_tools.h>


#include "mas_cli_opts.h"
#include "mas_cli_opts_init.h"
#include "mas_cli_opts_data.h"

#include "mas_opts.h"
int tested_total = 0;
int tested_good = 0;

/* run --test --info --quit --single --logger --msg=mem */
int
tested( mas_options_t * popts, int condition )
{
  tested_total++;
  if ( condition )
    tested_good++;
  if ( popts )
  {
    char fname[512];
    FILE *tsave = NULL;

    snprintf( fname, sizeof( fname ), "tmp/mas_options_mtest-%04u.tmp", tested_total );
    tsave = fopen( fname, "w" );
    if ( tsave )
    {
      int r;

      r = mas_cli_opts_save( popts, tsave );
      fclose( tsave );
      if ( r <= 0 )
        unlink( fname );
    }
  }
  return condition;
}

int
testing1( int id, int targc, char **targv, char **tenv, unsigned msgflags, unsigned port, unsigned exitsleep, unsigned womaster,
          unsigned wolisten, unsigned wolistener )
{
  int r = 0;
  mas_options_t opts;

  memset( &opts, 0, sizeof( opts ) );
  mas_cli_options_data_init( &opts, NULL, 1 );
  mas_cli_options_argv_init( &opts, targc, targv, tenv );
  /* for ( int i = 0; i < targc; i++ )             */
  /*   HMSG( "%d. %s", i, targv[i] ); */
  /* {                                                              */
  /*   struct option *topt = opts.cli_longopts;                     */
  /*                                                                */
  /*   while ( topt && topt->name )                                 */
  /*   {                                                            */
  /*     HMSG( "%s : %d", topt->name, topt->has_arg ); */
  /*     topt++;                                                    */
  /*   }                                                            */
  /* }                                                              */
  r = _mas_cli_options_init( &opts, NULL, 1 );
  mas_ctrl_init( &opts, NULL, 1 );
  HMSG( "_mas_cli_options_init:%d opts", r );

  if ( port )
  {
    HMSG( "1 testing1:%d %s (port %u ? %u)", r, tested( &opts, port == opts.default_port ) ? "OK" : "FAIL", port, opts.default_port );
  }
  if ( exitsleep )
  {
    HMSG( "2 testing1:%d %s (exitsleep %u ? %u)", r, tested( &opts, exitsleep == opts.exitsleep ) ? "OK" : "FAIL", exitsleep,
          opts.exitsleep );
  }
  if ( womaster )
  {
    HMSG( "3 testing1:%d %s (womaster %u ? %u)", r, tested( &opts, womaster == opts.womaster ) ? "OK" : "FAIL", womaster, opts.womaster );
  }
  if ( wolisten )
  {
    HMSG( "4 testing1:%d %s (wolisten %u ? %u)", r, tested( &opts, wolisten == opts.wolisten ) ? "OK" : "FAIL", wolisten, opts.wolisten );
  }
  if ( wolistener )
  {
    HMSG( "5 testing1:%d %s (wolistener %u ? %u)", r, tested( &opts, wolistener == opts.wolistener ) ? "OK" : "FAIL", wolistener,
          opts.wolistener );
  }
  if ( r >= 0 )
  {
    HMSG( "TEST %d.1.1a %s", id,
          tested( &opts, !opts.flag.name.msg.name.msg_notice && !opts.flag.name.msg.name.msg_watch
                  && !opts.flag.name.msg.name.msg_funline ) ? "OK" : "FAIL" );
    HMSG( "TEST %d.1.1b %s %X ? %X targc=%d", id, ( opts.flag.name.msg.bits == msgflags ) ? "OK" : "FAIL", opts.flag.name.msg.bits,
          msgflags, targc );
    opts.flag.name.msg.name.msg_notice = 1;
    HMSG( "TEST %d.1.2a %s", id,
          tested( &opts, opts.flag.name.msg.name.msg_notice && !opts.flag.name.msg.name.msg_watch
                  && !opts.flag.name.msg.name.msg_funline ) ? "OK" : "FAIL" );
    HMSG( "TEST %d.1.2b %s : %X ? %X", id,
          tested( &opts, opts.flag.name.msg.bits == ( 1U << ( MAS_OPT_BITNUM_MSG_NOTICE - 1 ) | msgflags ) ) ? "OK" : "FAIL",
          opts.flag.name.msg.bits, 1U << ( MAS_OPT_BITNUM_MSG_NOTICE - 1 ) | msgflags );
    opts.flag.name.msg.name.msg_watch = 1;
    HMSG( "TEST %d.1.3a %s", id,
          tested( &opts, opts.flag.name.msg.name.msg_notice && opts.flag.name.msg.name.msg_watch
                  && !opts.flag.name.msg.name.msg_funline ) ? "OK" : "FAIL" );
    HMSG( "TEST %d.1.3b %s : %X ? %X", id,
          tested( &opts, opts.flag.name.msg.bits ==
                  ( 1U << ( MAS_OPT_BITNUM_MSG_NOTICE - 1 ) | 1U << ( MAS_OPT_BITNUM_MSG_WATCH - 1 ) | msgflags ) ) ? "OK" : "FAIL",
          opts.flag.name.msg.bits, ( 1U << ( MAS_OPT_BITNUM_MSG_NOTICE - 1 ) | 1U << ( MAS_OPT_BITNUM_MSG_WATCH - 1 ) | msgflags ) );
    opts.flag.name.msg.name.msg_funline = 1;
  }
  mas_opts_destroy( &opts );
  return r;
}

int
testing2( int id, int targc, char **targv, char **tenv, unsigned msgflags, unsigned long expected_flags, const char *expected_init_msg,
          const char *expected_jhosts, int expected_nhosts )
{
  int r = 0;
  mas_options_t opts;

  memset( &opts, 0, sizeof( opts ) );
  mas_cli_options_data_init( &opts, NULL, 1 );
  mas_cli_options_argv_init( &opts, targc, targv, tenv );
  /* for ( int i = 0; i < targc; i++ )             */
  /*   HMSG( "%d. %s", i, targv[i] ); */
  r = _mas_cli_options_init( &opts, NULL, 1 );
  mas_ctrl_init( &opts, NULL, 1 );

  /* OPT_SFLAG( &opts, test, 1 ); */

  HMSG( "TEST %d.2.1a %s : %lX ? %lX [%lX]", id,
        tested( &opts, opts.flag.bits == ( expected_flags | ( ( unsigned long ) msgflags ) << 32 ) ) ? "OK" : "FAIL", opts.flag.bits,
        ( expected_flags | ( ( unsigned long ) msgflags ) << 32 ), ( ( unsigned long ) msgflags ) << 32 );
  HMSG( "TEST %d.2.2a %s : %s ? %s", id,
        tested( &opts, ( !( ( opts.init_message ? 1 : 0 ) || ( expected_init_msg ? 1 : 0 ) ) )
                ||
                ( ( opts.init_message ) && ( expected_init_msg )
                  && 0 == strcmp( opts.init_message, expected_init_msg ) ) ) ? "OK" : "FAIL", opts.init_message, expected_init_msg );
  {
    char *hosts = NULL;

    if ( opts.hostsv.c > 0 && opts.hostsv.v )
      hosts = mas_argv_string( opts.hostsv.c, opts.hostsv.v, 0 );

    HMSG( "TEST %d.2.3a %s : '%s':%lu ? '%s':%lu N:(%u ? %u)** C:%d", id,
          tested( &opts, ( !( opts.hostsv.c || expected_jhosts ? 1 : 0 ) )
                  ||
                  ( opts.hostsv.c == expected_nhosts && expected_jhosts
                    && 0 == strcmp( hosts, expected_jhosts ) ) ) ? "OK" : "FAIL", hosts, hosts ? strlen( hosts ) : 0, expected_jhosts,
          expected_jhosts ? strlen( expected_jhosts ) : 0, opts.hostsv.c, expected_nhosts, ( hosts
                                                                                             && expected_jhosts ) ? strcmp( hosts,
                                                                                                                            expected_jhosts )
          : -999 );
    mas_free( hosts );
  }
  mas_cli_print_optx_table( &opts );
  mas_opts_destroy( &opts );
  return r;
}

int
testing_general( int id, const char *test_opts, int expected_result, unsigned long expected_flags, const char *expected_init_msg,
                 const char *expected_jhosts, int expected_nhosts, unsigned msgflags, unsigned port, unsigned exitsleep, unsigned womaster,
                 unsigned wolisten, unsigned wolistener )
{
  int r = 0;
  int targc = 0;
  char **targv = NULL;
  char **tenv = NULL;

  targc = mas_add_argv_args( targc, &targv, test_opts, 0 );
  HMSG( "1 $$$$$$$$$$$$$$$$$$$[%d] %s", targc, test_opts );
  {
    r = testing1( id, targc, targv, tenv, msgflags, port, exitsleep, womaster, wolisten, wolistener );
    HMSG( "1 testing1:%d %s %d ? %d", r, tested( NULL, r == expected_result ) ? "OK" : "FAIL", r, expected_result );
  }
  HMSG( "2 $$$$$$$$$$$$$$$$$$$[%d] %s", targc, test_opts );
  {
    r = testing1( id, targc, targv, tenv, msgflags, port, exitsleep, womaster, wolisten, wolistener );
    HMSG( "2 testing1:%d %s %d ? %d", r, tested( NULL, r == expected_result ) ? "OK" : "FAIL", r, expected_result );
  }
  HMSG( "3 $$$$$$$$$$$$$$$$$$$[%d] %s", targc, test_opts );
  {
    r = testing1( id, targc, targv, tenv, msgflags, port, exitsleep, womaster, wolisten, wolistener );
    HMSG( "3 testing1:%d %s %d ? %d", r, tested( NULL, r == expected_result ) ? "OK" : "FAIL", r, expected_result );
  }
  HMSG( "4 $$$$$$$$$$$$$$$$$$$[%d] %s", targc, test_opts );
  {
    r = testing2( id, targc, targv, tenv, msgflags, expected_flags, expected_init_msg, expected_jhosts, expected_nhosts );
    HMSG( "4 testing2:%d %s %d ? %d", r, tested( NULL, r == expected_result ) ? "OK" : "FAIL", r, expected_result );
  }
  HMSG( "5 $$$$$$$$$$$$$$$$$$$[%d] %s", targc, test_opts );
  mas_del_argv( targc, targv, 0 );
  return r;
}

#define TEST_SEPARATE_E(sopt, field, value, toget) \
{ \
  mas_options_t *popts = testing_separate( sopt, toget ); \
 \
  HMSG( "testing separate popts [%s / " #field "]: %s", sopt, \
      		tested(popts, !popts ) ? "OK" : "FAIL" ); \
  mas_opts_destroy( popts ); mas_free(popts); \
}
#define TEST_SEPARATE_N(sopt, field, value, toget) \
{ \
  mas_options_t *popts = testing_separate( sopt, toget ); \
 \
  HMSG( "testing separate [%s / " #field "]: %s : %d ? %d", sopt, \
      		tested(popts, popts && ( popts->field == value )) ? "OK" : "FAIL", popts?popts->field:0, value ); \
  mas_opts_destroy( popts ); mas_free(popts); \
}
#define TEST_SEPARATE_F(sopt, field, value, toget) \
{ \
  mas_options_t *popts = testing_separate( sopt, toget ); \
 \
  HMSG( "testing separate [%s / " #field "]: %s : %d ? %d", sopt, \
      		tested(popts, popts && (( popts->flag.name.field?1:0 ) == value )) ? "OK" : "FAIL", popts?popts->flag.name.field:0, value ); \
  mas_opts_destroy( popts ); \
  mas_free(popts); \
}
#define TEST_SEPARATE_S(sopt, field, value, toget) \
{ \
  mas_options_t *popts = testing_separate( sopt, toget ); \
 \
  HMSG( "testing separate [%s / " #field "]: %s : %s ? %s", sopt, \
      		tested(popts, popts && ( !( popts->field || value ) || \
		    		    ( popts->field && value && 0==strcmp( popts->field, value )) \
		                 )) ? "OK" : "FAIL", popts?popts->field:NULL, value ); \
  mas_opts_destroy( popts ); \
  mas_free(popts); \
}
#define TEST_SEPARATE_I(sopt, field, value, toget) \
{ \
  mas_options_t *popts = testing_separate( sopt, toget ); \
 \
  HMSG( "testing separate [%s / " #field "]: %s : %s ? %s", sopt, \
      		tested(popts, popts && ( !( popts->field || value ) || \
		    		    ( popts->field && value && 0==strcmp( popts->field, value )) \
		                 )) ? "OK" : "FAIL", popts?popts->field:NULL, value ); \
  mas_opts_destroy( popts ); \
  mas_free(popts); \
}

#define TEST_SEPARATE_A(sopt, field, value, toget) \
{ \
  char *fldvalue=NULL; \
  mas_options_t *popts = testing_separate( sopt, toget ); \
 \
  if (popts) \
    fldvalue = mas_argv_string( popts->field.c, popts->field.v, 0 ); \
  HMSG( "testing separate [%s / " #field "]: %s : %s ? %s", sopt, \
      		tested(popts, popts && (!(fldvalue || value) || \
		  	( fldvalue && value && 0==strcmp( fldvalue, value )))) ? "OK" : "FAIL", fldvalue, value ); \
  mas_free(fldvalue); \
  mas_opts_destroy( popts ); \
  mas_free(popts); \
}
#define TEST_SEPARATE_AC(sopt, field, value, toget) \
{ \
  CTRL_PREPARE; \
  char *fldvalue=NULL; \
  mas_options_t *popts = testing_separate( sopt, toget ); \
 \
  fldvalue = mas_argv_string( ctrl.field.c, ctrl.field.v, 0 ); \
  HMSG( "testing separate [%s / " #field "]: %s : %s ? %s", sopt, \
      		tested(popts, popts && (!(fldvalue || value) || \
		  	( fldvalue && value && 0==strcmp( fldvalue, value )))) ? "OK" : "FAIL", fldvalue, value ); \
  mas_free(fldvalue); \
  mas_opts_destroy( popts ); \
  mas_free(popts); \
}

void
justdo( int targc, char *targv[], char *tenv[] )
{
  mas_options_t opts;

  memset( &opts, 0, sizeof( mas_options_t ) );
  mas_cli_options_data_init( &opts, NULL, 1 );
  mas_cli_options_argv_init( &opts, targc, targv, tenv );
  _mas_cli_options_init( &opts, NULL, 1 );
  mas_ctrl_init( &opts, NULL, 1 );
  mas_opts_destroy( &opts );
}

mas_options_t *
testing_separate( const char *sopt, const char *toget )
{
  int r1 = 0;
  int r2 = 0;
  int r3 = 0;
  int r4 = 0;
  int targc = 0;
  char **targv = NULL;
  char **tenv = NULL;
  mas_options_t *popts = NULL;


  popts = mas_malloc( sizeof( mas_options_t ) );
  memset( popts, 0, sizeof( mas_options_t ) );
  targc = mas_add_argv_args( targc, &targv, "cmd", 0 );
  targc = mas_add_argv_args( targc, &targv, sopt, 0 );
  {
    r1 = mas_cli_options_data_init( popts, NULL, 1 );
    r2 = mas_cli_options_argv_init( popts, targc, targv, tenv );
    r3 = _mas_cli_options_init( popts, NULL, 1 );
    r4 = mas_ctrl_init( popts, NULL, 1 );
  }
  mas_del_argv( targc, targv, 0 );

  {
    char *args;

    args = mas_cli_opts_to_str( popts );
    HMSG( "match rebuilt %s src:'%s' ? to get:'%s'",
          tested( popts, !( args || toget ) || ( args && toget && 0 == strcmp( args, toget ) ) ) ? "OK" : "FAIL", args, toget );

    mas_free( args );
  }


  if ( r1 < 0 || r2 < 0 || r3 < 0 || r4 < 0 )
  {
    mas_opts_destroy( popts );
    mas_free( popts );
    popts = NULL;
  }
  if ( popts )
  {
    char *enabled_opts = mas_cli_enabled_options( popts );
    const char *value = "thiqM:L:dH:P:";

    HMSG( "testing enabled_options: %s : '%s' ? '%s'", tested( popts, 0 == strcmp( value, enabled_opts ) ) ? "OK" : "FAIL", value,
          enabled_opts );
    mas_free( enabled_opts );
  }
  return popts;
}

int
main( int argc, char *argv[], char *env[] )
{
  int r = 0;

  /* const char *test_opts = "cmd --test --info --quit --single --logger --noticker --nowatcher --log  --read-user-config --msg=mem"; */
  /* const char *test_opts1 = "cmd --test --info --quit --single --noticker --nowatcher --msg=mem --read-user-config --read-user-config-plus"; */
  const char *test_opts[] = {
    "cmd --info --quit --test --single --noticker --nowatcher --msg=mem --read-user-config --read-user-config-plus "
          " --overwrite-user-config --overwrite-user-config-plus --log --logger --msg=mem --nodaemon "
          " --port=5035 --exitsleep=27 --nomaster=42 --nolisten=27 --nolistener=52",
    "cmd --info --quit --test --single --noticker --nowatcher --msg=mem --read-user-config --read-user-config-plus --nomaster=72 --master"
          " --overwrite-user-config --overwrite-user-config-plus --log --logger --msg=mem --damon",
    "cmd --info --quit --test --single --noticker --nowatcher --msg=mem --read-user-config --read-user-config-plus "
          " --overwrite-user-config --overwrite-user-config-plus --log --logger --msg=mem --nodaemon "
          " --init-msg=abrakadabra  --host=abra.mastar.lan",
    "cmd --single --noticker --nowatcher --msg=mem --read-user-config --read-user-config-plus --init-msg=abrakadabra "
          " --overwrite-user-config --overwrite-user-config-plus --log --logger --msg=mem --nodaemon "
          " --port=5115 --exitsleep=33 --host=abra.mastar.lan --host=kobra.mastar.lan ",
    "cmd --msg=mem --port=6420 --exitsleep=92 --host=arba.mastar.lan --host=cobra.mastar.lan --nohosts ",
    "cmd --port=6420 --exitsleep=92 --host=arba.mastar.lan --host=cobra.mastar.lan ",
  };
  int expected_result[] = { 21, -87, 18, 18, 7, 5 };
  int expected_port[] = { 5035, 0, 0, 5115, 6420, 6420 };
  int expected_extsleep[] = { 27, 0, 0, 33, 92, 92 };
  const char *expected_init_msg[] = { NULL, NULL, "abrakadabra", "abrakadabra", NULL, NULL };
  const char *expected_jhosts[] =
        { NULL, NULL, "abra.mastar.lan", "abra.mastar.lan kobra.mastar.lan", NULL, "arba.mastar.lan cobra.mastar.lan" };
  int expected_nhosts[] = { 0, 0, 1, 2, 0, 2 };
  unsigned long expected_flags[] = {
    ( 1LU << ( MAS_OPT_BITNUM_TEST - 1 ) |
      1LU << ( MAS_OPT_BITNUM_QUIT - 1 ) |
      1LU << ( MAS_OPT_BITNUM_INFO - 1 ) |
      1LU << ( MAS_OPT_BITNUM_SINGLE_INSTANCE - 1 ) |
      1LU << ( MAS_OPT_BITNUM_WOTICKER - 1 ) |
      1LU << ( MAS_OPT_BITNUM_WATCHER - 1 ) |
      1LU << ( MAS_OPT_BITNUM_DAEMON_DISABLE - 1 ) |
      1LU << ( MAS_OPT_BITNUM_READ_USER_OPTS - 1 ) |
      1LU << ( MAS_OPT_BITNUM_READ_USER_OPTS_PLUS - 1 ) |
      1LU << ( MAS_OPT_BITNUM_LOG_ENABLE - 1 ) |
      1LU << ( MAS_OPT_BITNUM_LOG_RUN - 1 ) |
      1LU << ( MAS_OPT_BITNUM_OVERWRITE_USER_OPTS - 1 ) | 1LU << ( MAS_OPT_BITNUM_OVERWRITE_USER_OPTS_PLUS - 1 ) ),
    ( 1LU << ( MAS_OPT_BITNUM_TEST - 1 ) |
      1LU << ( MAS_OPT_BITNUM_QUIT - 1 ) |
      1LU << ( MAS_OPT_BITNUM_INFO - 1 ) |
      1LU << ( MAS_OPT_BITNUM_SINGLE_INSTANCE - 1 ) | 1LU << ( MAS_OPT_BITNUM_WOTICKER - 1 ) | 1LU << ( MAS_OPT_BITNUM_WATCHER - 1 ) |
      /* 1LU << ( MAS_OPT_BITNUM_DAEMON_DISABLE - 1 ) | */
      1LU << ( MAS_OPT_BITNUM_READ_USER_OPTS - 1 ) |
      1LU << ( MAS_OPT_BITNUM_READ_USER_OPTS_PLUS - 1 ) |
      1LU << ( MAS_OPT_BITNUM_LOG_ENABLE - 1 ) |
      1LU << ( MAS_OPT_BITNUM_LOG_RUN - 1 ) |
      1LU << ( MAS_OPT_BITNUM_OVERWRITE_USER_OPTS - 1 ) | 1LU << ( MAS_OPT_BITNUM_OVERWRITE_USER_OPTS_PLUS - 1 ) ),
    ( 1LU << ( MAS_OPT_BITNUM_TEST - 1 ) |
      1LU << ( MAS_OPT_BITNUM_QUIT - 1 ) |
      1LU << ( MAS_OPT_BITNUM_INFO - 1 ) |
      1LU << ( MAS_OPT_BITNUM_SINGLE_INSTANCE - 1 ) |
      1LU << ( MAS_OPT_BITNUM_WOTICKER - 1 ) |
      1LU << ( MAS_OPT_BITNUM_WATCHER - 1 ) |
      1LU << ( MAS_OPT_BITNUM_DAEMON_DISABLE - 1 ) |
      1LU << ( MAS_OPT_BITNUM_READ_USER_OPTS - 1 ) |
      1LU << ( MAS_OPT_BITNUM_READ_USER_OPTS_PLUS - 1 ) |
      1LU << ( MAS_OPT_BITNUM_LOG_ENABLE - 1 ) |
      1LU << ( MAS_OPT_BITNUM_LOG_RUN - 1 ) |
      1LU << ( MAS_OPT_BITNUM_OVERWRITE_USER_OPTS - 1 ) | 1LU << ( MAS_OPT_BITNUM_OVERWRITE_USER_OPTS_PLUS - 1 ) ),
    ( 1LU << ( MAS_OPT_BITNUM_SINGLE_INSTANCE - 1 ) |
      1LU << ( MAS_OPT_BITNUM_WOTICKER - 1 ) |
      1LU << ( MAS_OPT_BITNUM_WATCHER - 1 ) |
      1LU << ( MAS_OPT_BITNUM_DAEMON_DISABLE - 1 ) |
      1LU << ( MAS_OPT_BITNUM_READ_USER_OPTS - 1 ) |
      1LU << ( MAS_OPT_BITNUM_READ_USER_OPTS_PLUS - 1 ) |
      1LU << ( MAS_OPT_BITNUM_LOG_ENABLE - 1 ) |
      1LU << ( MAS_OPT_BITNUM_LOG_RUN - 1 ) |
      1LU << ( MAS_OPT_BITNUM_OVERWRITE_USER_OPTS - 1 ) | 1LU << ( MAS_OPT_BITNUM_OVERWRITE_USER_OPTS_PLUS - 1 ) ),
    ( 0 ),
    ( 0 )
  };
  unsigned expected_msg_flags[] = {
    ( 1U << ( MAS_OPT_BITNUM_MSG_MEM - 1 ) ),
    ( 1U << ( MAS_OPT_BITNUM_MSG_MEM - 1 ) ),
    ( 1U << ( MAS_OPT_BITNUM_MSG_MEM - 1 ) ),
    ( 1U << ( MAS_OPT_BITNUM_MSG_MEM - 1 ) ),
    ( 1U << ( MAS_OPT_BITNUM_MSG_MEM - 1 ) ),
    0
  };

  HMSG( "testing common: %s",
        tested( NULL, sizeof( expected_result ) / sizeof( expected_result[0] ) ==
                sizeof( test_opts ) / sizeof( test_opts[0] ) ) ? "OK" : "FAIL" );
  HMSG( "testing common: %s",
        tested( NULL,
                sizeof( expected_port ) / sizeof( expected_port[0] ) == sizeof( test_opts ) / sizeof( test_opts[0] ) ) ? "OK" : "FAIL" );
  HMSG( "testing common: %s",
        tested( NULL,
                sizeof( expected_extsleep ) / sizeof( expected_extsleep[0] ) ==
                sizeof( test_opts ) / sizeof( test_opts[0] ) ) ? "OK" : "FAIL" );
  HMSG( "testing common: %s",
        tested( NULL,
                sizeof( expected_init_msg ) / sizeof( expected_init_msg[0] ) ==
                sizeof( test_opts ) / sizeof( test_opts[0] ) ) ? "OK" : "FAIL" );
  HMSG( "testing common: %s",
        tested( NULL,
                sizeof( expected_jhosts ) / sizeof( expected_jhosts[0] ) ==
                sizeof( test_opts ) / sizeof( test_opts[0] ) ) ? "OK" : "FAIL" );
  HMSG( "testing common: %s",
        tested( NULL,
                sizeof( expected_nhosts ) / sizeof( expected_nhosts[0] ) ==
                sizeof( test_opts ) / sizeof( test_opts[0] ) ) ? "OK" : "FAIL" );
  HMSG( "testing common: %s",
        tested( NULL,
                sizeof( expected_flags ) / sizeof( expected_flags[0] ) == sizeof( test_opts ) / sizeof( test_opts[0] ) ) ? "OK" : "FAIL" );
  HMSG( "testing common: %s",
        tested( NULL,
                sizeof( expected_msg_flags ) / sizeof( expected_msg_flags[0] ) ==
                sizeof( test_opts ) / sizeof( test_opts[0] ) ) ? "OK" : "FAIL" );

/* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> */
/* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> */
  /* test_opts1 = "cmd --logger        "; */
  r = testing_general( 1, test_opts[0], expected_result[0], expected_flags[0], expected_init_msg[0], expected_jhosts[0], expected_nhosts[0],
                       expected_msg_flags[0], expected_port[0], expected_extsleep[0], 42, 27, 52 );
  HMSG( "testing_general:%d %s", r, tested( NULL, r == expected_result[0] ) ? "OK" : "FAIL" );
  r = testing_general( 2, test_opts[0], expected_result[0], expected_flags[0], expected_init_msg[0], expected_jhosts[0], expected_nhosts[0],
                       expected_msg_flags[0], expected_port[0], expected_extsleep[0], 42, 27, 52 );
  HMSG( "testing_general:%d %s", r, tested( NULL, r == expected_result[0] ) ? "OK" : "FAIL" );
/* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> */
/* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> */
  for ( int it = 0; it < sizeof( test_opts ) / sizeof( test_opts[0] ); it++ )
  {
    r = testing_general( 3 + it, test_opts[it], expected_result[it], expected_flags[it], expected_init_msg[it], expected_jhosts[it],
                         expected_nhosts[it], expected_msg_flags[it], expected_port[it], expected_extsleep[it], 0, 0, 0 );
    HMSG( "testing_general:%d %s", r, tested( NULL, r == expected_result[it] ) ? "OK" : "FAIL" );
  }

  TEST_SEPARATE_F( "--test", test, 1, "--test" );
  TEST_SEPARATE_F( "-t", test, 1, "--test" );
  TEST_SEPARATE_F( "", test, 0, NULL );

  TEST_SEPARATE_F( "--info", info, 1, "--info" );
  TEST_SEPARATE_F( "-i", info, 1, "--info" );
  TEST_SEPARATE_F( "", info, 0, NULL );

  TEST_SEPARATE_F( "--quit", quit, 1, "--quit" );
  TEST_SEPARATE_F( "-q", quit, 1, "--quit" );
  TEST_SEPARATE_F( "", quit, 0, NULL );

  TEST_SEPARATE_N( "--exitsleep", exitsleep, 29, "--exitsleep=29" );
  TEST_SEPARATE_N( "--exitsleep=177", exitsleep, 177, "--exitsleep=177" );
  TEST_SEPARATE_N( "", exitsleep, 0, NULL );

  TEST_SEPARATE_F( "--single", single_instance, 1, "--single-instance" );
  TEST_SEPARATE_F( "", single_instance, 0, NULL );
  TEST_SEPARATE_F( "--single-instance", single_instance, 1, "--single-instance" );
  TEST_SEPARATE_F( "--multi-instance", single_instance, 0, NULL );
  TEST_SEPARATE_F( "--single-instance --multi-instance", single_instance, 0, NULL );
  TEST_SEPARATE_F( "--multi-instance --single", single_instance, 1, "--single-instance" );
  TEST_SEPARATE_F( "--multi-instance --single-instance", single_instance, 1, "--single-instance" );

  TEST_SEPARATE_F( "--single-child", single_child, 1, "--single-child" );
  TEST_SEPARATE_F( "", single_child, 0, NULL );
  TEST_SEPARATE_F( "--multi-child", single_child, 0, NULL );
  TEST_SEPARATE_F( "--single --single-child --multi-child", single_child, 0, "--single-instance" );
  TEST_SEPARATE_F( "--multi-child --single-child", single_child, 1, "--single-child" );
  TEST_SEPARATE_F( "--multi-child --multi-instance --single-child", single_child, 1, "--single-child" );

  TEST_SEPARATE_S( "--save-user-opts-file=some-file-to-save.tmp", save_user_opts_filename, "some-file-to-save.tmp",
                   "--save-user-opts-file=some-file-to-save.tmp" );
  TEST_SEPARATE_F( "--save-user-opts", save_user_opts, 1, "--save-user-opts" );
  TEST_SEPARATE_F( "--nosave-user-opts", save_user_opts, 0, NULL );
  /* ctrl!!! TEST_SEPARATE_AC( "--command=something", commandsv, "something", "--command=something" ); */
  TEST_SEPARATE_S( "--redirect-messages=/dev/tty11", msgfilename, "/dev/tty11", "--redirect-messages=/dev/tty11" );
  TEST_SEPARATE_S( "--redirect-stderr=/dev/tty21", stderr_filename, "/dev/tty21", "--redirect-stderr=/dev/tty21" );
  TEST_SEPARATE_S( "--redirect-stdout=/dev/tty31", stdout_filename, "/dev/tty31", "--redirect-stdout=/dev/tty31" );

  TEST_SEPARATE_F( "--listener-single", listener_single, 1, "--listener-single" );
  TEST_SEPARATE_F( "", listener_single, 0, NULL );
  TEST_SEPARATE_F( "--transaction-single", transaction_single, 1, "--transaction-single" );
  TEST_SEPARATE_F( "", transaction_single, 0, NULL );

  TEST_SEPARATE_F( "--parent-messages", messages_parent, 1, "--parent-messages" );
  TEST_SEPARATE_F( "--noparent-messages", messages_parent, 0, NULL );
  TEST_SEPARATE_F( "", messages_parent, 0, NULL );

  TEST_SEPARATE_F( "--child-messages", messages_child, 1, "--child-messages" );
  TEST_SEPARATE_F( "--nochild-messages", messages_child, 0, NULL );
  TEST_SEPARATE_F( "", messages_child, 0, NULL );

  TEST_SEPARATE_F( "--messages", messages, 1, "--messages" );
  TEST_SEPARATE_F( "--nomessages", messages, 0, NULL );
  TEST_SEPARATE_F( "", messages, 0, NULL );

  TEST_SEPARATE_F( "--redirect-std", daemon_disable_redirect_std, 0, NULL );
  TEST_SEPARATE_F( "--noredirect-std", daemon_disable_redirect_std, 1, "--noredirect-std" );
  TEST_SEPARATE_F( "", daemon_disable_redirect_std, 0, NULL );

  TEST_SEPARATE_F( "--close-std", daemon_disable_close_std, 0, NULL );
  TEST_SEPARATE_F( "--noclose-std", daemon_disable_close_std, 1, "--noclose-std" );
  TEST_SEPARATE_F( "", daemon_disable_close_std, 0, NULL );

  TEST_SEPARATE_F( "--logger", log_run, 1, "--logger" );
  TEST_SEPARATE_F( "--nologger", log_run, 0, NULL );
  TEST_SEPARATE_F( "", log_run, 0, NULL );

  TEST_SEPARATE_F( "--log", log_enable, 1, "--log" );
  TEST_SEPARATE_F( "--nolog", log_enable, 0, NULL );
  TEST_SEPARATE_F( "", log_enable, 0, NULL );

  TEST_SEPARATE_S( "--modsdir=/tmpx1", dir.mods, "/tmpx1", "--modsdir=/tmpx1" );
  TEST_SEPARATE_S( "-M /tmpx1short", dir.mods, "/tmpx1short", "--modsdir=/tmpx1short" );
  TEST_SEPARATE_S( "--protodir=/tmpx2", dir.proto, "/tmpx2", "--protodir=/tmpx2" );
  TEST_SEPARATE_S( "--logdir=/tmpx3", dir.log, "/tmpx3", "--logdir=/tmpx3" );
  TEST_SEPARATE_S( "-L /tmpx3short", dir.log, "/tmpx3short", "--logdir=/tmpx3short" );
  TEST_SEPARATE_S( "--historydir=/tmpx4", dir.history, "/tmpx4", "--historydir=/tmpx4" );

  TEST_SEPARATE_F( "--read-user-config", read_user_opts, 1, "--read-user-config" );
  TEST_SEPARATE_F( "--noread-user-config", read_user_opts, 0, NULL );
  TEST_SEPARATE_F( "", read_user_opts, 0, NULL );

  TEST_SEPARATE_F( "--read-user-config-plus", read_user_opts_plus, 1, "--read-user-config-plus" );
  TEST_SEPARATE_F( "--noread-user-config-plus", read_user_opts_plus, 0, NULL );
  TEST_SEPARATE_F( "", read_user_opts_plus, 0, NULL );

  TEST_SEPARATE_F( "--overwrite-user-config", overwrite_user_opts, 1, "--overwrite-user-config" );
  TEST_SEPARATE_F( "--nooverwrite-user-config", overwrite_user_opts, 0, NULL );
  TEST_SEPARATE_F( "", overwrite_user_opts, 0, NULL );

  TEST_SEPARATE_F( "--overwrite-user-config-plus", overwrite_user_opts_plus, 1, "--overwrite-user-config-plus" );
  TEST_SEPARATE_F( "--nooverwrite-user-config-plus", overwrite_user_opts_plus, 0, NULL );
  TEST_SEPARATE_F( "", overwrite_user_opts_plus, 0, NULL );

  TEST_SEPARATE_F( "--watcher", wowatcher, 0, NULL );
  TEST_SEPARATE_F( "--nowatcher", wowatcher, 1, "--nowatcher" );
  TEST_SEPARATE_F( "", wowatcher, 0, NULL );

  TEST_SEPARATE_F( "--ticker", woticker, 0, NULL );
  TEST_SEPARATE_F( "--noticker", woticker, 1, "--noticker" );
  TEST_SEPARATE_F( "", woticker, 0, NULL );

  TEST_SEPARATE_N( "--ticker-mode=", ticker_mode, 0, NULL );
  TEST_SEPARATE_N( "--ticker-mode=7", ticker_mode, 7, "--ticker-mode=7" );
  TEST_SEPARATE_N( "", ticker_mode, 0, NULL );

  TEST_SEPARATE_F( "--pidfile", pidfile, 1, "--pidfile" );
  TEST_SEPARATE_F( "--nopidfile", pidfile, 0, NULL );
  TEST_SEPARATE_F( "", pidfile, 0, NULL );

  TEST_SEPARATE_N( "--master", womaster, 0, NULL );
  TEST_SEPARATE_E( "--nomaster", womaster, 0, NULL );
  TEST_SEPARATE_N( "--nomaster=", womaster, 28, "--nomaster=28" );
  TEST_SEPARATE_N( "--nomaster=90", womaster, 90, "--nomaster=90" );
  TEST_SEPARATE_N( "", womaster, 0, NULL );

  TEST_SEPARATE_F( "--mthread", make_master_thread, 1, "--mthread" );
  TEST_SEPARATE_F( "--nomthread", make_master_thread, 0, NULL );
  TEST_SEPARATE_F( "", make_master_thread, 0, NULL );

  TEST_SEPARATE_N( "--listener", wolistener, 0, NULL );
  TEST_SEPARATE_E( "--nolistener", wolistener, 0, NULL );
  TEST_SEPARATE_N( "--nolistener=", wolistener, 27, "--nolistener=27" );
  TEST_SEPARATE_N( "--nolistener=90", wolistener, 90, "--nolistener=90" );
  TEST_SEPARATE_N( "", wolistener, 0, NULL );

  TEST_SEPARATE_F( "--daemon", daemon_disable, 0, NULL );
  TEST_SEPARATE_F( "--nodaemon --daemon", daemon_disable, 0, NULL );
  TEST_SEPARATE_F( "--nodaemon", daemon_disable, 1, "--nodaemon" );
  TEST_SEPARATE_F( "--nodaemon -d", daemon_disable, 0, NULL );
  TEST_SEPARATE_F( "", daemon_disable, 0, NULL );

  TEST_SEPARATE_F( "--sysdaemon", daemon_sys, 1, "--sysdaemon" );
  TEST_SEPARATE_F( "--nosysdaemon", daemon_sys, 0, NULL );
  TEST_SEPARATE_F( "", daemon_sys, 0, NULL );

  TEST_SEPARATE_A( "--proto=http", protosv, "http", "--proto=http" );
  TEST_SEPARATE_A( "--proto=http --proto=xcromas", protosv, "http xcromas", "--proto=http --proto=xcromas" );
  TEST_SEPARATE_A( "--proto=xcromas --noprotos --proto=http", protosv, "http", "--proto=http" );

  TEST_SEPARATE_A( "--host=a.mastar.lan", hostsv, "a.mastar.lan", "--host=a.mastar.lan" );
  TEST_SEPARATE_A( "-H H.mastar.lan", hostsv, "H.mastar.lan", "--host=H.mastar.lan" );
  TEST_SEPARATE_A( "--host=a.mastar.lan --host=b.mastar.lan", hostsv, "a.mastar.lan b.mastar.lan",
                   "--host=a.mastar.lan --host=b.mastar.lan" );
  TEST_SEPARATE_A( "--host=a.mastar.lan --nohosts --host=b.mastar.lan", hostsv, "b.mastar.lan", "--host=b.mastar.lan" );

  TEST_SEPARATE_N( "--port=44 --port=", default_port, 0, NULL );
  TEST_SEPARATE_N( "--port=79", default_port, 79, "--port=79" );
  TEST_SEPARATE_N( "--port=79 -P 97", default_port, 97, "--port=97" );
  TEST_SEPARATE_N( "--port=33 --port=65", default_port, 65, "--port=65" );

  TEST_SEPARATE_S( "--init-msg=abrakadabra", init_message, "abrakadabra", "--init-message=abrakadabra" );
  TEST_SEPARATE_S( "--init-msg=abrakadabra", init_message, "abrakadabra", "--init-message=abrakadabra" );

  TEST_SEPARATE_S( "--init-msg=wowowow", init_message, "wowowow", "--init-message=wowowow" );
  TEST_SEPARATE_S( "--init-message=wawawaw", init_message, "wawawaw", "--init-message=wawawaw" );

  TEST_SEPARATE_F( "--help", help, 1, "--help" );
  TEST_SEPARATE_F( "-h", help, 1, "--help" );
  TEST_SEPARATE_F( "", help, 0, NULL );

  TEST_SEPARATE_E( "-R", 0, 0, NULL );
  TEST_SEPARATE_E( "-z", 0, 0, NULL );
  TEST_SEPARATE_E( "-7", 0, 0, NULL );
  TEST_SEPARATE_I( "--env-hostname=zoro --env-optsname=coro", env_optsname, "coro", "--env-optsname=coro --env-hostname=zoro" );
  TEST_SEPARATE_I( "--env-optsname=loro --env-hostname=goro", env_hostname, "goro", "--env-optsname=loro --env-hostname=goro" );
  TEST_SEPARATE_I( "--uuid=zgzggrtrtrtrtrt", uuid, "zgzggrtrtrtrtrt", "--uuid=zgzggrtrtrtrtrt" );
  TEST_SEPARATE_I( "--help --test --user=mastar", user, "mastar", "--test --help --user=mastar" );
  justdo( argc, argv, env );
  HMSG( "total passed %d of %d (%d failures)", tested_good, tested_total, tested_total - tested_good );
  mas_ctrl_destroy(  );
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif
  return tested_total - tested_good ? 1 : 0;
}
