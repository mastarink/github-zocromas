#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include <mastar/types/mas_control_types.h>

extern unsigned long __MAS_LINK_DATE__;
extern unsigned long __MAS_LINK_TIME__;
extern unsigned long __MAS_LINK_TIMESTAMP__;

/*
this:
  mas_control_data.c
related:
  mas_control.c
  mas_common.h
  mas_tools.c
  mas_opts.c
  mas_cs.c
  mas_cs.h

*/

static int mas_error_handler( const char *func, int line, int issys, int rcode, int ierrno, int *perrno, int *pserrno, const char *fmt,
                              const char *msg );


mas_control_t ctrl = {
  .main_exit = 0,
  .threads = {.n = {
                    .main = {.pid = 0,.tid = 0,.thread = ( pthread_t ) 0},
                    .master = {.pid = 0,.tid = 0,.thread = ( pthread_t ) 0},
                    .daemon = {.pid = 0,.tid = 0,.thread = ( pthread_t ) 0},
                    .ticker = {.pid = 0,.tid = 0,.thread = ( pthread_t ) 0},
                    .logger = {.pid = 0,.tid = 0,.thread = ( pthread_t ) 0},
                    .watcher = {.pid = 0,.tid = 0,.thread = ( pthread_t ) 0}}
              },
  .sigint_time = 0,
  .start_time = 0.0,
  .old_stderr = -1,
  .old_stderrfile = NULL,
  .old_stdout = -1,
  .transactions_time = 0.0,
  .fatal = 0,

  .int_cnt = 0,
  .term_cnt = 0,
  .try_cnt = 0,
  .restart_cnt = 0,

  .in_client = 0,
  .in_pipe = 0,
  /* .is_client = 0, */
  /* .is_server = 0, */
  .messages = 1,

  .daemon = 0,
  .close_std = 0,
  .redirect_std = 0,

  .keep_listening = 0,
  .keep_logging = 0,

  .disconnect = 0,
  .restart = 0,
  .exit = 0,

  .forget_transactions = 1,


  .listener_serial = 0,
  /* .npthclients = 100, */
  /* .listening_max = 0, */
  .lcontrols_list = NULL,
  /* .listening_controls_list = NULL, */

  .launchervv = {0, NULL},
  .launcherev = {0, NULL},
  /* .launcherv = NULL,   */
  /* .launcherc = 0,      */
  /* .launchere = NULL,   */
  .argv_nonoptind = 0,

  .stamp.vtsc = MAS_S_DATE,


  /* .stamp.vdate = ( uint64_t ) ( unsigned long long ) ( &__MAS_LINK_DATE__ ),    */
  /* .stamp.vtime = ( uint64_t ) ( unsigned long long ) ( &__MAS_LINK_TIME__ ),    */
  /* .stamp.vts = ( uint64_t ) ( unsigned long long ) ( &__MAS_LINK_TIMESTAMP__ ), */


  .stamp.first_lts = 0,
  .stamp.prev_lts = 0,
  .stamp.lts = 0,
  .stamp.start_time = 0,

  .status = MAS_STATUS_NONE,
  .clients_created = 0,
  .clients_removed = 0,
  .clients_came0 = 0,
  .clients_came = 0,
  .clients_gone = 0,
  .clients_gone2 = 0,
  .log_q_came = 0,
  .log_q_gone = 0,
  .maxclients = 0,
  .maxtrtime = 0.0,

  .proto_descs = NULL,

  .pidfile = NULL,

  .logpath = NULL,
  .logfile = NULL,
  .log_disabled = 0,
  .opts_saved = 0,
  .opts_saved_plus = 0,
  .mas_thread_key_once = PTHREAD_ONCE_INIT,
  .ticker_hide = 0,
  .ticker_mode = 0,
  .tick_cnt = 0,
  .watch_cnt = 0,
  .watcher_hide = 0,
  .watcher_stop = 0,
  /* .pthread_custom_attr = ..., */
  .thglob = {
             .malloc_mutex = PTHREAD_MUTEX_INITIALIZER,
             .cleanup_transactions_mutex = PTHREAD_MUTEX_INITIALIZER,
             .msg_mutex = PTHREAD_MUTEX_INITIALIZER,
             .emsg_mutex = PTHREAD_MUTEX_INITIALIZER,
             .mfp_mutex = PTHREAD_MUTEX_INITIALIZER,
             .cnttr1_mutex = PTHREAD_MUTEX_INITIALIZER,
             .cnttr2_mutex = PTHREAD_MUTEX_INITIALIZER,
             .cnttr3_mutex = PTHREAD_MUTEX_INITIALIZER,
             .logger_mutex = PTHREAD_MUTEX_INITIALIZER,
             },
  /* .commands_num = 0, */
  /* .commands = NULL,  */
  .commandsv = {0, NULL},
  .error_handler = mas_error_handler,
};

static int
mas_error_handler( const char *func, int line, int issys, int rcode, int ierrno, int *perrno, int *pserrno, const char *fmt, const char *msg )
{
  if ( !ctrl.stderrfile
       || fprintf( ctrl.stderrfile, "[%d]************ (%d) ERROR %s:%d [%s]\n", __LINE__, rcode, func, line, msg ? msg : "-" ) < 0 )
    if ( !ctrl.old_stderrfile
         || fprintf( ctrl.old_stderrfile, "[%d]************ (%d) ERROR %s:%d [%s]\n", __LINE__, rcode, func, line, msg ? msg : "-" ) < 0 )
      if ( ctrl.msgfile )
        fprintf( ctrl.msgfile, "[%d]************ (%d) ERROR %s:%d [%s]\n", __LINE__, rcode, func, line, msg ? msg : "-" );

  if ( 0 )
  {
    if ( ctrl.stderrfile )
      fprintf( ctrl.stderrfile, "[%d]************ (%d) ERROR %s:%d [%s]\n", __LINE__, rcode, func, line, msg ? msg : "-" );
    if ( ctrl.old_stderrfile )
      fprintf( ctrl.old_stderrfile, "[%d]************ (%d) ERROR %s:%d [%s]\n", __LINE__, rcode, func, line, msg ? msg : "-" );
    if ( ctrl.msgfile )
      fprintf( ctrl.msgfile, "[%d]************ (%d) ERROR %s:%d [%s]\n", __LINE__, rcode, func, line, msg ? msg : "-" );
  }
  if ( pserrno )
    *pserrno = ierrno;
  if ( perrno )
    *perrno = 0;
  return rcode;
}

__attribute__ ( ( constructor( 10000 ) ) )
     static void f_constructor( void )
{
  /* if ( stderr )                                                           */
  /*   fprintf( stderr, "******************** CONSTRUCTOR %s\n", __FILE__ ); */
}
