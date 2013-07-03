#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

/* #include "mas_common.h" */

/* #include "server/inc/mas_server_tools.h" */


#include <mastar/fileinfo/mas_unidata.h>

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

/*
this:
  mas_cmdmod_do.c
related:
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
  
  mas_client.c
*/





/* TODO : 
 FILE *popen(const char *command, const char *mode);
*/
static mas_evaluated_t *
do_cmd( STD_CMD_ARGS )
{
  int rtot = -1;
  char *s = NULL;
  char *buf;
  FILE *f = NULL;
  size_t bufsz = 1024 * 8;

  buf = mas_malloc( bufsz );
  f = popen( args, "r" );
  while ( f && !feof( f ) )
  {
    int r = 0;

    r = fread( buf, 1, bufsz, f );
    if ( r > 0 )
    {
      s = mas_strncat_x( s, buf, r );
      rtot += r;
    }
  }
  mas_free( buf );
  return mas_evaluated_wrap_pchar( s );
}


/*==========================================================================================*/

mas_cmd_t subcmdtable[] = {
  {0, NULL, do_cmd, NULL}       /* do ... */
  ,
  {999, NULL, NULL, NULL}
};
