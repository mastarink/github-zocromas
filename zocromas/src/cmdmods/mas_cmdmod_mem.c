#include "mas_basic_def.h"

#include <string.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>

#include "mas_common.h"

#include "modules/inc/mas_modules_commands_eval.h"
#include "modules/inc/mas_modules_commands.h"

/*
this:
  mas_cmdmod_mem.c
related:
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_server.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/


static char *
info_cmd( STD_CMD_ARGS )
{
  char *rs;

  tMSG( "args:%s", args );
  char *buf;
  size_t bufsz;

  bufsz = 1024 * 10;
  buf = mas_malloc( bufsz );
  snprintf( buf, bufsz, "\n%20s%20s%20s\n%20lu%20lu%20lu\n%20lu%20lu%20lu\n>>>>>> %lu * %lu <<<<<<\n", "Alloc", "Free",
            "Delta", memory_allocated, memory_freed, ( memory_allocated - memory_freed ), memory_allocated_cnt, memory_freed_cnt,
            ( memory_allocated_cnt - memory_freed_cnt ), memory_balance, memory_balance_cnt );
  rs = mas_strdup( buf );
  mas_free( buf );
  return rs;
}

mas_cmd_t subcmdtable[] = {
  {0, NULL, list_commands_cmd, NULL}
  ,
  {1, "info", info_cmd, NULL}
  ,
  {999, NULL, NULL, NULL}
};
