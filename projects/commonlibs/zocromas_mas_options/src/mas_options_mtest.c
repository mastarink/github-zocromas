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

/* run --test --info --quit --single --logger --msg=mem */

int
testing1( int targc, char **targv, char **tenv, unsigned msgflags )
{
  mas_options_t opts;

  memset( &opts, 0, sizeof( opts ) );
  mas_cli_options_data_init( &opts, NULL );
  mas_cli_options_argv_init( &opts, targc, targv, tenv );
  /* for ( int i = 0; i < targc; i++ )             */
  /*   fprintf( stderr, "%d. %s\n", i, targv[i] ); */
  /* {                                                              */
  /*   struct option *topt = opts.cli_longopts;                     */
  /*                                                                */
  /*   while ( topt && topt->name )                                 */
  /*   {                                                            */
  /*     fprintf( stderr, "%s : %d\n", topt->name, topt->has_arg ); */
  /*     topt++;                                                    */
  /*   }                                                            */
  /* }                                                              */
  mas_cli_options_init( &opts, NULL );
  if ( 0 )
  {
    fprintf( stderr, "\n" );
    fprintf( stderr, "TEST 1.1a %s\n",
             ( !opts.flag.name.msg.name.msg_notice && !opts.flag.name.msg.name.msg_watch
               && !opts.flag.name.msg.name.msg_funline ) ? "OK" : "FAIL" );
    fprintf( stderr, "TEST 1.1b %s\n", ( opts.flag.name.msg.bits == msgflags ) ? "OK" : "FAIL" );
    opts.flag.name.msg.name.msg_notice = 1;
    fprintf( stderr, "TEST 1.2a %s\n",
             ( opts.flag.name.msg.name.msg_notice && !opts.flag.name.msg.name.msg_watch
               && !opts.flag.name.msg.name.msg_funline ) ? "OK" : "FAIL" );
    fprintf( stderr, "TEST 1.2b %s : %X ? %X\n",
             ( opts.flag.name.msg.bits == ( 1U << MAS_OPT_SHIFT_MSG_NOTICE | msgflags ) ) ? "OK" : "FAIL",
             opts.flag.name.msg.bits, 1U << MAS_OPT_SHIFT_MSG_NOTICE | msgflags );
    opts.flag.name.msg.name.msg_watch = 1;
    fprintf( stderr, "TEST 1.3a %s\n",
             ( opts.flag.name.msg.name.msg_notice && opts.flag.name.msg.name.msg_watch
               && !opts.flag.name.msg.name.msg_funline ) ? "OK" : "FAIL" );
    fprintf( stderr, "TEST 1.3b %s : %X ? %X\n",
             ( opts.flag.name.msg.bits ==
               ( 1U << MAS_OPT_SHIFT_MSG_NOTICE | 1U << MAS_OPT_SHIFT_MSG_WATCH | msgflags ) ) ? "OK" : "FAIL",
             opts.flag.name.msg.bits, ( 1U << MAS_OPT_SHIFT_MSG_NOTICE | 1U << MAS_OPT_SHIFT_MSG_WATCH | msgflags ) );
    opts.flag.name.msg.name.msg_funline = 1;
  }
  /* mas_del_argv( targc, targv, 0 ); */
  mas_opts_destroy( &opts );
  return 0;
}

int
testing2( int targc, char **targv, char **tenv, unsigned long xflags )
{
  mas_options_t opts;

  memset( &opts, 0, sizeof( opts ) );
  mas_cli_options_data_init( &opts, NULL );
  mas_cli_options_argv_init( &opts, targc, targv, tenv );
  /* for ( int i = 0; i < targc; i++ )             */
  /*   fprintf( stderr, "%d. %s\n", i, targv[i] ); */
  mas_cli_options_init( &opts, NULL );

  fprintf( stderr, "\n" );
  /* OPT_SFLAG( &opts, test, 1 ); */

  fprintf( stderr, "TEST 2.1a %s : %lX ? %lX\n", opts.flag.bits == xflags ? "OK" : "FAIL", opts.flag.bits, xflags );
  mas_opts_destroy( &opts );
  return 0;
}

int
main( int argc, char *argv[], char *env[] )
{
  int targc = 0;
  char **targv = NULL;
  char **tenv = NULL;

  /* const char *test_opts = "cmd --test --info --quit --single --logger --noticker --nowatcher --log  --read-user-config --msg=mem"; */
  /* const char *test_opts1 = "cmd --test --info --quit --single --noticker --nowatcher --msg=mem --read-user-config --read-user-config-plus"; */
  const char *test_opts1 =
        "cmd --info --quit --test --single --noticker --nowatcher " " --msg=mem " " --read-user-config --read-user-config-plus "
        " --overwrite-user-config " " --overwrite-user-config-plus --log --logger " " --msg=mem " " --daemon";
  /* test_opts1 = "cmd --logger        "; */

  targc = mas_add_argv_args( targc, &targv, test_opts1, 0 );

  testing1( targc, targv, tenv, ( 1U << MAS_OPT_SHIFT_MSG_MEM ) );
  testing1( targc, targv, tenv, ( 1U << MAS_OPT_SHIFT_MSG_MEM ) );
  testing1( targc, targv, tenv, ( 1U << MAS_OPT_SHIFT_MSG_MEM ) );
  testing1( targc, targv, tenv, ( 1U << MAS_OPT_SHIFT_MSG_MEM ) );
  testing2( targc, targv, tenv,
            ( 1LU << MAS_OPT_SHIFT_TEST | 1LU << MAS_OPT_SHIFT_QUIT | 1LU << MAS_OPT_SHIFT_INFO | 1LU << MAS_OPT_SHIFT_SINGLE_INSTANCE |
              1LU << MAS_OPT_SHIFT_NOTICKER | 1LU << MAS_OPT_SHIFT_NOWATCHER | 1LU << MAS_OPT_SHIFT_READ_USER_OPTS | 1LU <<
              MAS_OPT_SHIFT_READ_USER_OPTS_PLUS | 1LU << MAS_OPT_SHIFT_OVERWRITE_USER_OPTS | 1LU <<
              MAS_OPT_SHIFT_OVERWRITE_USER_OPTS_PLUS ) );
  mas_del_argv( targc, targv, 0 );
  mas_ctrl_destroy(  );
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif

  return 0;
}
