#include "mas_basic_def.h"

#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "mas_common.h"

#include "listener/inc/mas_listener_control_list.h"
#include "listener/inc/mas_listener.h"

#include "modules/inc/mas_modules_commands_eval.h"
#include "modules/inc/mas_modules_commands.h"

/*
this:
  mas_cmdmod_listener.c
related:
  mas_cmdmod_client.c
  mas_cmdmod_get.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/

static char *
listener_remove_cmd( STD_CMD_ARGS )
{
  mas_lcontrol_t *plcontrol = NULL;
  int targc = 0;
  unsigned port;
  char **targv = NULL;

  targc = mas_add_argv_args( targc, &targv, args, 0 );
  if ( targc % 2 == 0 )
  {
    for ( int ih = 0; ih < targc; ih += 2 )
    {
      cMSG( "Listener remove %u(%u): %s:%s", ih, targc, targv[ih], targv[ih + 1] );
      /* cMSG( "Listener stop 1:'%s'", targv[1] ); */
      /* cMSG( "Listener stop 2:'%s'", targv[2] ); */
      sscanf( targv[ih + 1], "%u", &port );
      plcontrol = mas_lcontrol_find( targv[ih], port );
      if ( plcontrol )
      {
      }
      else
      {
        EMSG( "Not found %s:%s", targv[ih], targv[ih + 1] );
      }
      cMSG( "Listener remove plcontrol: %lx (%s:%u)", ( unsigned long ) plcontrol, targv[ih], port );
      if ( plcontrol )
        mas_listener_cancel( plcontrol );
    }
  }
  else
  {
    EMSG( "Even number of args needed" );
  }
  /* cMSG( "[%d] Listener remove %s:%u", targc, targv[0], port ); */
  mas_del_argv( targc, targv, 0 );
  return NULL;
}

static char *
listener_add_cmd( STD_CMD_ARGS )
{
  mas_lcontrol_t *plcontrol = NULL;
  int targc = 0;
  unsigned port;
  char **targv = NULL;

  targc = mas_add_argv_args( targc, &targv, args, 0 );

  /* cMSG( "Listener add 0:'%s'", targv[0] ); */
  /* cMSG( "Listener add 1:'%s'", targv[1] ); */
  /* cMSG( "Listener add 2:'%s'", targv[2] ); */
  sscanf( targv[1], "%u", &port );
  plcontrol = mas_lcontrol_find( targv[0], port );

  {
    mas_listener_start( targv[0], port );
  }

  cMSG( "Listener add %s:%u", targv[0], port );
  mas_del_argv( targc, targv, 0 );
  return NULL;
}


mas_cmd_t subcmdtable[] = {
  {0, NULL, list_commands_cmd, NULL}
  ,
  {1, "add", listener_add_cmd, NULL,.min_args = 2,.max_args = 2} /* server listener add */
  ,
  {2, "remove", listener_remove_cmd, NULL,.min_args = 2,.max_args = 2} /* server listener remove */
  ,
  {999, NULL, NULL, NULL}
};
