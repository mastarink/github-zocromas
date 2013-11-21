#include <unistd.h>
#include <fcntl.h>

#include <pthread.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/types/mas_control_types.h>

#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>


#include <mastar/modules/mas_modules_ctrl_module.h>
/* #include <mastar/modules/mas_modules_load_module.h> */

#include <mastar/log/mas_log.h>
#include <mastar/log/mas_logger.h>

#include <mastar/thtools/mas_thread_tools.h>



#include <mastar/init/mas_init.h>
#include <mastar/init/mas_init_modules.h>

#include <mastar/options/mas_cli_opts_init.h>
#include <mastar/options/mas_cli_opts_data.h>

#include <mastar/init/mas_sig.h>

#include <mastar/control/mas_control.h>

#include <mastar/listener/mas_listener_control_list.h>

#include "mas_init_threads.h"
#include "mas_init_protos.h"
#include "mas_init_pid.h"

#include "mas_init_daemon.h"
#include "mas_init_server.h"



#include "mas_master.h"
#include "mas_bunch.h"

/*
this:
  mas_master.c
related:
  mas_server_main.c
  mas_listener.c
  mas_listener_wait_client.c
more:
  mas_ticker.c
  mas_watcher.c
  mas_cmdmod_listener.c
  mas_listener_control.c
  mas_listener_control.h
  mas_listener_control_list.c
  mas_listener_control_list.h
  mas_listener_control_types.h
  mas_listener.h
  mas_listeners.c
  mas_listeners.h
  mas_listener_wait_client.h

  mas_client.c
*/

/*
threads created:
  mas_server_main.c ("main")
  mas_listener_wait_client.c (mas_transaction_th)
  mas_master.c (mas_listener_th)
*/



/* static void                                   */
/* mas_server_cleanup( void *arg )               */
/* {                                             */
/*   tMSG( "%s in", __func__ );                  */
/*   thMSG( "%s MAIN TH. CLEANUP", __func__ );   */
/*   tMSG( "%s out", __func__ );                 */
/* }                                             */



static int
mas_master_bunch_init( mas_options_t * popts, int argc, char *argv[], char *env[] )
{
  EVAL_PREPARE;
  int r = 0, rn = 0;

  HMSG( "BUNCH START e:%d", errno );
  MAS_LOG( "bunch start e:%d", errno );

  IEVAL( rn,
         mas_thself_set_name( ctrl.is_parent ? popts->thname.parent_bunchi : popts->thname.daemon_bunchi,
                              ctrl.is_parent ? "zocParBunchI" : "zocDaeBunchI" ) );

  MAS_LOG( "(%d) bunch: to init +", r );
#if 0
  /* r = mas_init_plus( argc, argv, env, mas_pids_init, mas_daemon_init, mas_threads_init, mas_init_load_protos, mas_lcontrols_list_create, */
  /*                    NULL );                                                                                                             */
  /* uuid BEFORE opt_files !! */

  IEVAL( r,
         mas_init_plus( popts, argc, argv, env, mas_proc_init, mas_uuid_init, mas_init_opt_files, mas_sig_init, mas_cli_options_init,
                        mas_ctrl_init, mas_init_set_msg_file, mas_message_init, mas_daemon_init, mas_pids_init,
                        mas_threads_init, mas_init_load_protos, mas_lcontrols_init, mas_post_init, NULL ) );
#else
  /* INIT_HANDLER's */
  static mas_init_fun_t init_funcs[] = {
    /* Moved to mas_control.c constructor */
    /* mas_proc_init, */
    mas_opt_files_init,
    mas_cli_options_data_init,
    mas_cli_options_init,
    mas_ctrl_init,
    mas_msg_file_init,
    mas_message_init,
    mas_sig_init,
    mas_daemon_init,
    mas_pids_init,
    mas_threads_init,
    mas_protos_init,
    mas_lcontrols_init,
    mas_uuid_init,
    mas_post_init,
    NULL
  };
#  if 0
  {
    for ( int i = 0; i < argc; i++ )
    {
      HMSG( "argv[%d]='%s'", i, argv[i] );
    }
  }
#  endif
  HMSG( "INIT+ %s : %s", ctrl.is_server ? "SERVER" : "CLIENT", !ctrl.is_client ? "SERVER" : "CLIENT" );
  IEVAL( r, mas_init( popts, argc, argv, env ) );
//  IEVAL( r, mas_init_set_n( popts, /* argc, argv, env, */ sizeof( init_funcs ) / sizeof( init_funcs[0] ), init_funcs ) );
  HMSG( "INIT SET Z" );
  IEVAL( r, mas_init_set_z( popts, /* argc, argv, env, */ init_funcs ) );
#endif
  if ( r >= 0 )
  {
    IEVAL( rn,
           mas_thself_set_name( ctrl.is_parent ? popts->thname.parent_bunchm : popts->thname.daemon_bunchm,
                                ctrl.is_parent ? "zocParBunch" : "zocDaeBunch" ) );
  }
  MAS_LOG( "(%d) bunch: init + done", r );
  return r;
}

static int
mas_master_bunch_do_parent( const mas_options_t * popts, int argc, char *argv[], char *env[] )
{
  HMSG( "PARENT to exit" );
  return 0;
}

static int
mas_master_bunch_do_daemon( const mas_options_t * popts, int argc, char *argv[], char *env[] )
{
  EVAL_PREPARE;
  int r = 0;

  HMSG( "BUNCH DO DAEMON" );
  IEVAL( r, mas_master_do( popts ) );
#ifdef MAS_TRACEMEM
  {
    extern unsigned long memory_balance;

    mMSG( "bunch end, memory_balance:%lu - Ticker:%lx;Logger:%lx;", memory_balance, ctrl.threads.n.ticker.thread,
          ctrl.threads.n.logger.thread );
    MAS_LOG( "bunch end, m/b:%lu", memory_balance );
#  if 0
    if ( print_memlist( FL, ctrl.stderrfile ) < 0 )
      if ( print_memlist( FL, ctrl.old_stderrfile ) < 0 )
        if ( print_memlist( FL, ctrl.msgfile ) < 0 )
          print_memlist( FL, stderr );
#  endif
  }
#endif
  return r;
}

static int
mas_master_bunch_do( const mas_options_t * popts, int argc, char *argv[], char *env[] )
{
  EVAL_PREPARE;
  int r = 0, rn = 0;

  if ( ctrl.is_parent )
  {
    r = mas_master_bunch_do_parent( popts, argc, argv, env );
  }
  else
  {
    r = mas_master_bunch_do_daemon( popts, argc, argv, env );
    HMSG( "BUNCH TO END" );
  }
  IEVAL( rn,
         mas_thself_set_name( ctrl.is_parent ? popts->thname.parent_bunchx : popts->thname.daemon_bunchx,
                              ctrl.is_parent ? "zocParBunchX" : "zocDaeBunchX" ) );
  WMSG( "TO DESTROY MODULES" );
  /* mas_modules_unregister(  ); */
  for ( int ith = 0; ith < sizeof( ctrl.threads.a ) / sizeof( ctrl.threads.a[0] ); ith++ )
  {
    WMSG( "TO JOIN #%d of %u", ith, ( unsigned ) ( sizeof( ctrl.threads.a ) / sizeof( ctrl.threads.a[0] ) ) );
    if ( ctrl.threads.a[ith].thread )
      mas_xpthread_join( ctrl.threads.a[ith].thread );
    ctrl.threads.a[ith].thread = 0;
  }
  WMSG( "/ JOIN" );
  WMSG( "BUNCH END DATA master:[%lx] log:[%lx] t[%lx] w[%lx] %d", ctrl.threads.n.master.thread,
        ctrl.threads.n.logger.thread, ctrl.threads.n.ticker.thread, ctrl.threads.n.watcher.thread, ctrl.lcontrols_list ? 1 : 0 );
  HMSG( "BUNCH %s END", ctrl.is_parent ? "(parent)" : "" );
  return r;
}

int
mas_master_bunch( mas_options_t * popts, int argc, char *argv[], char *env[] )
{
  EVAL_PREPARE;
  int r = 0;

  IEVAL( r, mas_master_bunch_init( popts, argc, argv, env ) );
  IEVAL( r, mas_master_bunch_do( popts, argc, argv, env ) );
  return r;
}

__attribute__ ( ( constructor( 1100 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_constructor( IL, 0 );
}
