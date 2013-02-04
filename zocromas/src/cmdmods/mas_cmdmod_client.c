#include "mas_basic_def.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <mastar/wrap/mas_memory.h>

#include "mas_common.h"


/* mas_clients_count */
#include "server/inc/mas_server_tools.h"

#include "modules/inc/mas_modules_commands_eval.h"
#include "modules/inc/mas_modules_commands.h"

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

char *
get_client_info_cmd( STD_CMD_ARGS )
{
  char *sr;
  char *buf;
  size_t bufsz;

  bufsz = 1024 * 10;
  buf = mas_malloc( bufsz );

  snprintf( buf, bufsz, "Clients:%u", mas_clients_count( prcontrol ) );
  sr = mas_strdup( buf );
  mas_free( buf );
  return sr;
}

mas_cmd_t subcmdtable[] = {
  {0, NULL, list_commands_cmd, NULL}
  ,
  {1, "info", get_client_info_cmd, NULL}
  ,
  {999, NULL, NULL, NULL}
};
