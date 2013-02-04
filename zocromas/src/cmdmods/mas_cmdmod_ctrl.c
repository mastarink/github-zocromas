#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#include "mas_common.h"

#include "server/inc/mas_server_tools.h"

#include "modules/inc/mas_modules_commands_eval.h"
#include "modules/inc/mas_modules_commands.h"

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

char *
ctrl_cmd( STD_CMD_ARGS )
{
  char *r = NULL;
  return r;
}

mas_cmd_t subcmdtable[] = {
  {0, NULL, ctrl_cmd, NULL}
  ,
  {999, NULL, NULL, NULL}
};
