#include <mastar/wrap/mas_std_def.h>

#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <stdarg.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/thtools/mas_ocontrol_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
#include <mastar/types/mas_listener_control_types.h>

#include <mastar/channel/mas_channel.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

/* #include "mas_common.h" */

/* #ifdef MAS_USE_SERVER_TOOLS                */
/* #  include "server/inc/mas_server_tools.h" */
/* #endif                                     */

/* #include "transaction/inc/mas_transaction_control.h" */

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

/*
this:
  mas_cmdmod_server.c
related:
  mas_cmdmod_client.c
  mas_cmdmod_get.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
more:
  mas_watcher.c
  mas_ticker.c
  mas_server_sig.c

  mas_list_def.h
*/

static char *
stop_cmd( STD_CMD_ARGS )
{
  /* do_quit_server( prcontrol, pbinary ); */
  ctrl.do_quit = 1;
  return NULL;
}

static char *
cls_cmd( STD_CMD_ARGS )
{
/* echo -en "\e[H\e[2J" >&2 */
  fprintf( stderr, "\x1b[H\x1b[2JCLEARED\n" );
  return NULL;
}

static char *
exit_cmd( STD_CMD_ARGS )
{
  ctrl.quit = 1;
  if ( prcontrol )
    prcontrol->qbin = MSG_BIN_QUIT;
  return NULL;
}

static char *
restart_cmd( STD_CMD_ARGS )
{
  ctrl.restart = 1;
  /* do_quit_server( prcontrol, pbinary ); */
  ctrl.do_quit = 1;
  return NULL;
}

char *
quit_cmd( STD_CMD_ARGS )
{
  ctrl.quit = 1;
  /* do_quit_server( prcontrol, pbinary ); */
  ctrl.do_quit = 1;
  return NULL;
}




mas_cmd_t subcmdtable[] = {
  {0, NULL, list_commands_cmd, NULL}
  ,
  {2, "listener", NULL, "listener"} /* server listener ... */
  ,
  {3, "stop", stop_cmd, NULL}   /* server stop */
  ,
  {4, "cls", cls_cmd, NULL}     /* server cls */
  ,
  {5,.name = "restart",.function = restart_cmd,.libname = NULL} /* server restart */
  ,
  {6,.name = "exit",.function = exit_cmd,.libname = NULL} /* server exit */
  ,
  {7,.name = "quit",.function = quit_cmd,.libname = NULL} /* server quit */
  ,
  {999, NULL, NULL, NULL}
};
