#define MAS_MAX_INT_2 10
#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/prctl.h>

#include <pthread.h>
#include <signal.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/thtools/mas_thread_tools.h>

/* mas_evaluated_delete */
#include <mastar/fileinfo/mas_unidata.h>
#include <mastar/modules/mas_modules_commands_eval.h>

/* mas_destroy_server */
#include "mas_init_server.h"

#include <mastar/listener/mas_listeners.h>

#include "mas_server_sig.h"


void
mas_atexit( void )
{
  CTRL_PREPARE;
  extern mas_options_t gopts;

  {
    int rn = 0;
    char name_buffer[32] = "?";

    IEVAL( rn, prctl( PR_GET_NAME, ( unsigned long ) name_buffer ) );
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocMainAtexit" ) );
    HMSG( "AT EXIT %s: logQ: %lu - %lu = %lu", name_buffer, ctrl.log_q_came, ctrl.log_q_gone, ctrl.log_q_came - ctrl.log_q_gone );
  }

  mas_destroy_server( &gopts );
  /* mas_destroy_server( MAS_PASS_OPTS_REF ); */
#ifdef MAS_TRACEMEM
  {
    extern unsigned long memory_balance;

    /* IMSG( "AT EXIT, memory_balance:%ld", memory_balance );                                                         */
    /* IMSG( "AT EXIT, memory_balance:%ld : logQ: %lu - %lu = %lu", memory_balance, ctrl.log_q_came, ctrl.log_q_gone, */
    /*       ctrl.log_q_came - ctrl.log_q_gone );                                                                     */
    /* print_memlist_any( FL, 4, ctrl.stderrfile, ctrl.old_stderrfile, ctrl.msgfile, stderr ); */
    print_memlist_any( FL, 4, ctrl.msgfile, ctrl.stderrfile, ctrl.old_stderrfile, stderr );
    IMSG( "(1)AT EXIT, memory_balance:%ld : logQ: %lu - %lu = %lu", memory_balance, ctrl.log_q_came, ctrl.log_q_gone,
          ctrl.log_q_came - ctrl.log_q_gone );
  }
#else
  IMSG( "AT EXIT" );
#endif
  IMSG( "=====[%u @ %u]=================================================================", getpid(  ), getppid(  ) );
  if ( ctrl.msgfile )
  {
    IMSG( "CLOSE MSG" );
    fclose( ctrl.msgfile );
    ctrl.msgfile = NULL;
  }
  {
    extern unsigned long memory_balance;
    int rn = 0;
    char name_buffer[32] = "?";

    IEVAL( rn, prctl( PR_GET_NAME, ( unsigned long ) name_buffer ) );
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocMainAtexit" ) );
    /* EMSG( "AT EXIT %s: logQ: %lu - %lu = %lu", name_buffer, ctrl.log_q_came, ctrl.log_q_gone, ctrl.log_q_came - ctrl.log_q_gone ); */
    IMSG( "(2)AT EXIT, memory_balance:%ld : logQ: %lu - %lu = %lu", memory_balance, ctrl.log_q_came, ctrl.log_q_gone,
          ctrl.log_q_came - ctrl.log_q_gone );
  }
  sync(  );
  /* sleep( 3 ); */
  _exit( 0 );
  /* exit_group( 0 ); */
}

__attribute__ ( ( constructor ) )
     static void f_constructor( void )
{
  CTRL_PREPARE;
  if ( stderr )
    fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno );
  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;
  ctrl.is_client = 0;
  ctrl.is_server = 1;
  atexit( mas_atexit );
}

__attribute__ ( ( destructor ) )
     static void f_destructor( void )
{
  if ( stderr )
    fprintf( stderr, "******************** DESTRUCTOR %s e%d\n", __FILE__, errno );
}
