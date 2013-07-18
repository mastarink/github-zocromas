#define MAS_USE_LISTENER

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifdef MAS_USE_LISTENER
#  include <mastar/listener/mas_listener_control_list.h>
#  include <mastar/listener/mas_listener.h>
#endif


#include <mastar/fileinfo/mas_unidata.h>

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

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

#ifdef MAS_USE_LISTENER
static mas_evaluated_t *
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

static mas_evaluated_t *
listener_add_cmd( STD_CMD_ARGS )
{
  mas_lcontrol_t *plcontrol = NULL;
  int targc = 0;
  unsigned port;
  char **targv = NULL;

  targc = mas_add_argv_args( targc, &targv, args, 0 );

  if ( targc > 0 && targv[0] )
  {
    if ( targc > 1 && targv[1] )
      sscanf( targv[1], "%u", &port );
    plcontrol = mas_lcontrol_find( targv[0], port );

    if ( !plcontrol )
    {
      mas_listener_start( popts, targv[0], port );
    }

    cMSG( "Listener add %s:%u", targv[0], port );
  }
  mas_del_argv( targc, targv, 0 );
  return NULL;
}


mas_cmd_t subcmdtable[] = {
  {0, NULL, mas_evaluate_list_cmd, NULL}
  ,
  {1, "add", listener_add_cmd, NULL,.min_args = 2,.max_args = 2} /* server listener add */
  ,
  {2, "remove", listener_remove_cmd, NULL,.min_args = 2,.max_args = 2} /* server listener remove */
  ,
  {999, NULL, NULL, NULL}
};
#endif
