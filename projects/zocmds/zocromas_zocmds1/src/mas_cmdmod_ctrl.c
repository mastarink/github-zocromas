#  include <mastar/wrap/mas_std_def.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

/* #include "mas_common.h" */

/* #include "server/inc/mas_server_tools.h" */

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>


/*
this:
  mas_cmdmod_ctrl.c
related:
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/

/*
 * if there is f-n non-static <libname>_cmd than subtable ignored ! - is it FIXME ?
*/

char *
ctrl_cmd( STD_CMD_ARGS )
{
  char *r = NULL;

  return r;
}

static char *
sleep_cmd( STD_CMD_ARGS )
{
  char *r = NULL;
  int secs = 0;

  sscanf( args, "%d", &secs );
  sleep( secs );
  r = mas_strdup( "Ha-ha-ha" );
  return r;
}

mas_cmd_t subcmdtable[] = {
  {0, NULL, list_commands_cmd, NULL}
  ,
  {1, "ctrl", ctrl_cmd, NULL}   /* ctrl ctrl */
  ,
  {2, "sleep", sleep_cmd, NULL} /* ctrl sleep */
  ,
  {999, NULL, NULL, NULL}
};
