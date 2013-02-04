#include "mas_basic_def.h"

#include "mas_common.h"


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
mas_options_t opts = {
  .msg_c = 1,
  .msg_s = 0,
  .msg_io = 0,

  .msg_tr = 1,
  .msg_trace_main = 1,
  .msg_trace_listener = 1,
  .msg_trace_transaction = 1,

  .msg_m = 0,
/* respoder */
  .msg_r = 1,
  .msg_l = 1,
  .msg_w = 1,
  .msg_th = 1,
  .msg_sg = 1,
  .msg_cmd = 1,
  .msg_mem = 0,
  .msg_funline = 0,
};
#else
mas_options_t opts = {
  .f = {
        .bit = {
                .msg_c = 1,
                .msg_s = 0,
                .msg_io = 0,

                .msg_tr = 0,    /* tMSG */
                .msg_trace_main = 1,
                .msg_trace_listener = 1,
                .msg_trace_transaction = 1,

                .msg_m = 1,     /* mMSG */
/* respoder */
                .msg_r = 1,
                .msg_l = 1,
                .msg_w = 1,
                .msg_th = 1,
                .msg_sg = 1,
                .msg_cmd = 1,
                .msg_mem = 0,
                .msg_funline = 1,
                }
        }
  ,
  .save_opts = 0,
  .save_opts_plus = 1,
  .disconnect_prompt = 1,
  .wait_server = 0,
  .listener_single = 0,
  .transaction_single = 0,
  .nologger = 0,
  .nolog = 0,
  .noticker = 0,
  .nowatcher = 0,
  .nomaster = 0,
  .nolistener = 0,
  .nolisten = 0,
  .nomessages = 0,
  .exitsleep = 0,
  .nodaemon = 0,
  .default_port = MAS_DEFAULT_PORT,
  .env_optsname = "MAS_ZOCROMAS_DEFOPTS",
  .env_hostname = "MAS_ZOCROMAS_DEFHOST",
  .argc = 0,
  .argv = NULL,
  .hosts_num = 0,
  .hosts = NULL,
  .commands_num = 0,
  .commands = NULL,
  .client_attempts = 1,
  .logdir = NULL,
  .configdir = NULL,
  .configfilename = NULL,
  .max_config_backup = 5,
  .restart_sleep = 0.25,
};
#endif
