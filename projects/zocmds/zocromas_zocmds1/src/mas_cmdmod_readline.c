#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

/* #include "mas_common.h" */
#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>


#include <mastar/fileinfo/mas_unidata.h>

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>


/*
this:
  mas_cmdmod_readline.c
related:
  mas_cmdmod_get.c
  mas_cmdmod_mem.c
  mas_cmdmod_client.c
  mas_cmdmod_server.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/

static mas_evaluated_t *
readline_cmd( STD_CMD_ARGS )
{
  CTRL_PREPARE;
  char *buf;
  char *mas_readline_buffer;

  ctrl.ticker_hide = 1;
  mas_readline_buffer = readline( "\nserver> " );
  ctrl.ticker_hide = 0;
  buf = mas_strdup( mas_readline_buffer );
  mas_other_free( mas_readline_buffer );
  mas_readline_buffer = NULL;

  return mas_evaluated_wrap_pchar(buf);
}

mas_cmd_t subcmdtable[] = {
  {1, NULL, readline_cmd, NULL} /* get readline */
  ,
  {999, NULL, NULL, NULL}
};
