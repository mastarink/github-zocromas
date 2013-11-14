#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_opts_types.h>

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
tested( int condition )
{
  tested_total++;
  if ( condition )
    tested_good++;
  return condition;
}

int
testing1( int id, int targc, char **targv, char **tenv, unsigned msgflags, unsigned port, unsigned exitsleep, unsigned womaster,
          unsigned wolisten, unsigned wolistener )
{
  int r = 0;
  mas_options_t opts;

  memset( &opts, 0, sizeof( opts ) );
  mas_cli_options_data_init( &opts, NULL );
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
  r = _mas_cli_options_init( &opts, NULL );
  mas_ctrl_init( &opts, NULL );
  HMSG( "_mas_cli_options_init:%d opts", r );


  if ( port )
  {
    HMSG( "1 testing1:%d %s (port %u ? %u)", r, tested( port == opts.default_port ) ? "OK" : "FAIL", port, opts.default_port );
  }
  if ( exitsleep )
  {
    HMSG( "2 testing1:%d %s (exitsleep %u ? %u)", r, tested( exitsleep == opts.exitsleep ) ? "OK" : "FAIL", exitsleep, opts.exitsleep );
  }
  if ( womaster )
  {
    HMSG( "3 testing1:%d %s (womaster %u ? %u)", r, tested( womaster == opts.womaster ) ? "OK" : "FAIL", womaster, opts.womaster );
  }
  if ( wolisten )
  {
    HMSG( "4 testing1:%d %s (wolisten %u ? %u)", r, tested( wolisten == opts.wolisten ) ? "OK" : "FAIL", wolisten, opts.wolisten );
  }
  if ( wolistener )
  {
    HMSG( "5 testing1:%d %s (wolistener %u ? %u)", r, tested( wolistener == opts.wolistener ) ? "OK" : "FAIL", wolistener,
          opts.wolistener );
  }
  if ( r >= 0 )
  {
    HMSG( "TEST %d.1.1a %s", id,
          tested( !opts.flag.name.msg.name.msg_notice && !opts.flag.name.msg.name.msg_watch
                  && !opts.flag.name.msg.name.msg_funline ) ? "OK" : "FAIL" );
    HMSG( "TEST %d.1.1b %s %X ? %X targc=%d", id, ( opts.flag.name.msg.bits == msgflags ) ? "OK" : "FAIL", opts.flag.name.msg.bits,
          msgflags, targc );
    opts.flag.name.msg.name.msg_notice = 1;
    HMSG( "TEST %d.1.2a %s", id,
          tested( opts.flag.name.msg.name.msg_notice && !opts.flag.name.msg.name.msg_watch
                  && !opts.flag.name.msg.name.msg_funline ) ? "OK" : "FAIL" );
    HMSG( "TEST %d.1.2b %s : %X ? %X", id,
          tested( opts.flag.name.msg.bits == ( 1U << ( MAS_OPT_BITNUM_MSG_NOTICE - 1 ) | msgflags ) ) ? "OK" : "FAIL",
          opts.flag.name.msg.bits, 1U << ( MAS_OPT_BITNUM_MSG_NOTICE - 1 ) | msgflags );
    opts.flag.name.msg.name.msg_watch = 1;
    HMSG( "TEST %d.1.3a %s", id,
          tested( opts.flag.name.msg.name.msg_notice && opts.flag.name.msg.name.msg_watch
                  && !opts.flag.name.msg.name.msg_funline ) ? "OK" : "FAIL" );
    HMSG( "TEST %d.1.3b %s : %X ? %X", id,
          tested( opts.flag.name.msg.bits ==
                  ( 1U << ( MAS_OPT_BITNUM_MSG_NOTICE - 1 ) | 1U << ( MAS_OPT_BITNUM_MSG_WATCH - 1 ) | msgflags ) ) ? "OK" : "FAIL",
          opts.flag.name.msg.bits, ( 1U << ( MAS_OPT_BITNUM_MSG_NOTICE - 1 ) | 1U << ( MAS_OPT_BITNUM_MSG_WATCH - 1 ) | msgflags ) );
    opts.flag.name.msg.name.msg_funline = 1;
  }
  mas_opts_destroy( &opts );
  return r;
}

int
testing2( int id, int targc, char **targv, char **tenv, unsigned msgflags, unsigned long expected_flags, const char *expected_init_msg,
          const char *expected_host )
{
  int r = 0;
  mas_options_t opts;

  memset( &opts, 0, sizeof( opts ) );
  mas_cli_options_data_init( &opts, NULL );
  mas_cli_options_argv_init( &opts, targc, targv, tenv );
  /* for ( int i = 0; i < targc; i++ )             */
  /*   HMSG( "%d. %s", i, targv[i] ); */
  r = _mas_cli_options_init( &opts, NULL );
  mas_ctrl_init( &opts, NULL );


  /* OPT_SFLAG( &opts, test, 1 ); */

  HMSG( "TEST %d.2.1a %s : %lX ? %lX [%lX]", id,
        tested( opts.flag.bits == ( expected_flags | ( ( unsigned long ) msgflags ) << 32 ) ) ? "OK" : "FAIL", opts.flag.bits,
        ( expected_flags | ( ( unsigned long ) msgflags ) << 32 ), ( ( unsigned long ) msgflags ) << 32 );
  HMSG( "TEST %d.2.2a %s : %s ? %s", id,
        tested( ( !( ( opts.init_message ? 1 : 0 ) || ( expected_init_msg ? 1 : 0 ) ) )
                ||
                ( ( opts.init_message ) && ( expected_init_msg )
                  && 0 == strcmp( opts.init_message, expected_init_msg ) ) ) ? "OK" : "FAIL", opts.init_message, expected_init_msg );

  HMSG( "TEST %d.2.3a %s : %s ? %s", id,
        tested( ( !( opts.hostsv.c || expected_host ? 1 : 0 ) )
                ||
                ( opts.hostsv.c == 1 && opts.hostsv.v && opts.hostsv.v[0] && expected_host
                  && 0 == strcmp( opts.hostsv.v[0], expected_host ) ) ) ? "OK" : "FAIL", opts.hostsv.v ? opts.hostsv.v[0] : NULL,
        expected_host );

  mas_opts_destroy( &opts );
  return r;
}

int
testing_general( int id, const char *test_opts, int expected_result, unsigned long expected_flags, const char *expected_init_msg,
                 const char *expected_host, unsigned msgflags, unsigned port, unsigned exitsleep, unsigned womaster, unsigned wolisten,
                 unsigned wolistener )
{
  int r = 0;
  int targc = 0;
  char **targv = NULL;
  char **tenv = NULL;

  targc = mas_add_argv_args( targc, &targv, test_opts, 0 );
  HMSG( "1 $$$$$$$$$$$$$$$$$$$[%d] %s", targc, test_opts );
  {
    r = testing1( id, targc, targv, tenv, msgflags, port, exitsleep, womaster, wolisten, wolistener );
    HMSG( "1 testing1:%d %s %d ? %d", r, tested( r == expected_result ) ? "OK" : "FAIL", r, expected_result );
  }
  HMSG( "2 $$$$$$$$$$$$$$$$$$$[%d] %s", targc, test_opts );
  {
    r = testing1( id, targc, targv, tenv, msgflags, port, exitsleep, womaster, wolisten, wolistener );
    HMSG( "2 testing1:%d %s %d ? %d", r, tested( r == expected_result ) ? "OK" : "FAIL", r, expected_result );
  }
  HMSG( "3 $$$$$$$$$$$$$$$$$$$[%d] %s", targc, test_opts );
  {
    r = testing1( id, targc, targv, tenv, msgflags, port, exitsleep, womaster, wolisten, wolistener );
    HMSG( "3 testing1:%d %s %d ? %d", r, tested( r == expected_result ) ? "OK" : "FAIL", r, expected_result );
  }
  HMSG( "4 $$$$$$$$$$$$$$$$$$$[%d] %s", targc, test_opts );
  {
    r = testing2( id, targc, targv, tenv, msgflags, expected_flags, expected_init_msg, expected_host );
    HMSG( "4 testing2:%d %s %d ? %d", r, tested( r == expected_result ) ? "OK" : "FAIL", r, expected_result );
  }
  HMSG( "5 $$$$$$$$$$$$$$$$$$$[%d] %s", targc, test_opts );
  mas_del_argv( targc, targv, 0 );
  return r;
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
          " --port=5035 --exitsleep=21 --nomaster=42 --nolisten=27 --nolistener=52",
    "cmd --info --quit --test --single --noticker --nowatcher --msg=mem --read-user-config --read-user-config-plus --nomaster=72 --master"
          " --overwrite-user-config --overwrite-user-config-plus --log --logger --msg=mem --damon",
    "cmd --info --quit --test --single --noticker --nowatcher --msg=mem --read-user-config --read-user-config-plus "
          " --overwrite-user-config --overwrite-user-config-plus --log --logger --msg=mem --nodaemon "
          " --init-msg=abrakadabra  --host=abra.mastar.lan",
  };
  int expected_result[] = {
    21, -88, 18
  };
  const char *expected_init_msg[] = {
    NULL, NULL, "abrakadabra"
  };
  const char *expected_host[] = {
    NULL, NULL, "abra.mastar.lan"
  };
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
      1LU << ( MAS_OPT_BITNUM_OVERWRITE_USER_OPTS - 1 ) | 1LU << ( MAS_OPT_BITNUM_OVERWRITE_USER_OPTS_PLUS - 1 ) )
  };
  /* test_opts1 = "cmd --logger        "; */
  r = testing_general( 1, test_opts[0], expected_result[0], expected_flags[0], expected_init_msg[0], expected_host[0],
                       ( 1U << ( MAS_OPT_BITNUM_MSG_MEM - 1 ) ), 5035, 21, 42, 27, 52 );
  HMSG( "testing_general:%d %s", r, tested( r == expected_result[0] ) ? "OK" : "FAIL" );
  r = testing_general( 2, test_opts[0], expected_result[0], expected_flags[0], expected_init_msg[0], expected_host[0],
                       ( 1U << ( MAS_OPT_BITNUM_MSG_MEM - 1 ) ), 5035, 21, 42, 27, 52 );
  HMSG( "testing_general:%d %s", r, tested( r == expected_result[0] ) ? "OK" : "FAIL" );
  for ( int it = 0; it < sizeof( test_opts ) / sizeof( test_opts[0] ); it++ )
  {
    r = testing_general( 3 + it, test_opts[it], expected_result[it], expected_flags[it], expected_init_msg[it], expected_host[it],
                         ( 1U << ( MAS_OPT_BITNUM_MSG_MEM - 1 ) ), 0, 0, 0, 0, 0 );
    HMSG( "testing_general:%d %s", r, tested( r == expected_result[it] ) ? "OK" : "FAIL" );
  }

  HMSG( "total passed %d of %d", tested_good, tested_total );
  mas_ctrl_destroy(  );
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif
  return 0;
}
