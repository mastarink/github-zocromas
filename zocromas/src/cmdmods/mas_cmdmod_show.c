#include "mas_basic_def.h"

#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mas_common.h"

#include "modules/inc/mas_modules_commands_eval.h"
#include "modules/inc/mas_modules_commands.h"

/*
this:
  mas_cmdmod_show.c
related:
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/



static char *
msg_cmd( STD_CMD_ARGS )
{
  char *result = NULL;

  result = mas_strdup( "smth" );
  MFP( "msg: c%d\n", opts.f.bit.msg_c );
  MFP( "msg: s%d\n", opts.f.bit.msg_s );
  return ( char * ) result;
}

mas_cmd_t subcmdtable[] = {
  {0, NULL, list_commands_cmd, NULL}
  ,
  {1, "msg", msg_cmd, "show"} /* show msg ... */
  ,
  {999, NULL, NULL, NULL}
};
