#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdlib.h>

/* #include <mastar/types/mas_control_types.h> */
#include <mastar/types/mas_opts_types.h>

#include <mastar/tools/mas_tools.h>





#include "mas_opts.h"

/*
this:
  mas_opts_data.c
related:
  mas_opts.c
  mas_tools.c
  mas_msg_tools.c
  mas_control.c
  mas_cs.c
  mas_cs.h

*/



#if 0
static mas_options_t g_opts = {
  .msg_notice = 1,
  .msg_io = 0,

  .msg_trace = 1,
  .msg_trace_main = 1,
  .msg_trace_listener = 1,
  .msg_trace_transaction = 1,

  .msg_main = 0,
/* respoder */
  .msg_transaction = 1,
  .msg_listen = 1,
  .msg_wait = 1,
  .msg_thread = 1,
  .msg_signal = 1,
  .msg_cmd = 1,
  .msg_mem = 0,
  .msg_funline = 0,
};
#else
static mas_options_t g_opts = {
  .f = {
        .bit = {
                .msg_notice = 1,
                .msg_watch = 0,
                .msg_io = 0,

                .msg_trace = 0, /* tMSG */
                .msg_trace_main = 1,
                .msg_trace_listener = 1,
                .msg_trace_transaction = 1,

                .msg_main = 0,  /* mMSG */
/* respoder */
                .msg_transaction = 1,
                .msg_listen = 0,
                .msg_wait = 1,
                .msg_thread = 0,
                .msg_signal = 1,
                .msg_cmd = 1,
                .msg_mem = 0,
                .msg_funline = 1,
                }
        }
  ,
  .read_user_opts = 0,
  .read_user_opts_plus = 0,
  .save_user_opts = 0,
  .save_user_opts_plus = 0,
  .disconnect_prompt = 1,
  .wait_server = 0,
  .listener_single = 0,
  .transaction_single = 0,
  .log = {.enable = 0,.run = 0,}
  ,
  /* .nologger = 0, */
  /* .nolog = 0,    */
  .noticker = 0,
  .nowatcher = 0,
  .nomaster = 0,
  .nolistener = 0,
  .nolisten = 0,
  .nomessages = 0,
  .exitsleep = 0,

  .daemon = {
             .disable = 0,
             .disable_redirect_std = 0,
             .disable_close_std = 0,
             .disable_setsid = 0,
             .disable_chdir = 0,
             .sys = 0,
             }
  ,
  /* .nodaemon = 0,       */
  /* .noredirect_std = 0, */
  /* .noclose_std = 0,    */

#  ifdef MAS_DEFAULT_PORT
  .default_port = MAS_DEFAULT_PORT,
#  endif
  .env_optsname = "MAS_ZOCROMAS_DEFOPTS",
  .env_hostname = "MAS_ZOCROMAS_DEFHOST",
  .argvv = {0, NULL}
  ,
  /* .argc = 0,    */
  /* .argv = NULL, */
  .hostsv = {0, NULL}
  ,
  /* .hosts_num = 0, */
  /* .hosts = NULL,  */
  .protosv = {0, NULL}
  ,
  /* .protos_num = 0, */
  /* .protos = NULL,  */
  .commandsv = {0, NULL}
  ,
  /* .commands_num = 0, */
  /* .commands = NULL,  */
  .client_attempts = 1,
  .dir = {
          .mods = NULL,
          .proto = NULL,
          .log = NULL,
          .config = NULL,
          .pids = NULL,
          .history = NULL,
          .post = NULL,
          }
  ,
  .configfilename = NULL,
  .max_config_backup = 5,
  .restart_sleep = 0.25,
/* #  ifdef  MAS_PASS_OPTS                              */
/*   .new_section_func = mas_gopts_restore_new_section, */
/*   .at_section_func = mas_gopts_restore_at_section,   */
/*   .unknown_opt_func = mas_gopts_restore_flags,       */
/* #  else                                              */
/*   .new_section_func = mas_opts_restore_new_section,  */
/*   .at_section_func = mas_opts_restore_at_section,    */
/*   .unknown_opt_func = mas_opts_restore_flags,        */
/* #  endif                                             */
};

mas_options_t *gpopts = &g_opts;

__attribute__ ( ( constructor( 3000 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s\n", __FILE__ ); */
  mas_common_constructor( IL, 1 );
}
#endif
