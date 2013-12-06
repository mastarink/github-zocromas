#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>


/* mas_clients_count */
#ifdef MAS_USE_SERVER_TOOLS
#  include "server/inc/mas_server_tools.h"
#endif


#include <mastar/fileinfo/mas_unidata.h>

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

/*
this:
  mas_cmdmod_client.c
related:
  mas_cmdmod_server.c
  mas_cmdmod_get.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/


mas_cmd_t subcmdtable[] = {
  {0, NULL, mas_evaluate_list_cmd, NULL}
  ,
  {999, NULL, NULL, NULL}
};
