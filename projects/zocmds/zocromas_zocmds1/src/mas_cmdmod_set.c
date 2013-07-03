#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

/* #include "mas_common.h" */
/* #include "server/inc/mas_ticker.h" */

#include <mastar/log/mas_logger.h>
#include <mastar/log/mas_log.h>

#include <mastar/thvariables/mas_thread_variables.h>
#ifdef MAS_USE_LISTENER
#  include "listener/inc/mas_listener_control.h"
#endif



#include <mastar/fileinfo/mas_unidata.h>

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

#include <mastar/types/mas_control_types.h>
extern mas_control_t ctrl;


/*
this:
  mas_cmdmod_set.c
related:
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/


static mas_evaluated_t *
msg_cmd( STD_CMD_ARGS )
{
  cMSG( "set msg args:%s", args );
  if ( ( 0 == strcmp( args, "on" ) ) || ( 0 == strcmp( args, "yes" ) ) || ( 0 == strcmp( args, "1" ) ) )
  {
    ctrl.messages = 1;
    /* gopts.f.bit.msg_s = 1;  */
    /* gopts.f.bit.msg_c = 1;  */
    /* gopts.f.bit.msg_tr = 1; */
    /* gopts.f.bit.msg_m = 1;  */
  }
  else if ( ( 0 == strcmp( args, "off" ) ) || ( 0 == strcmp( args, "no" ) ) || ( 0 == strcmp( args, "0" ) ) )
  {
    ctrl.messages = 0;
    /* gopts.f.bit.msg_c = 0;                 */
    /* gopts.f.bit.msg_cmd = 0;               */
    /* gopts.f.bit.msg_funline = 1;           */
    /* gopts.f.bit.msg_io = 0;                */
    /* gopts.f.bit.msg_l = 0;                 */
    /* gopts.f.bit.msg_m = 0;                 */
    /* gopts.f.bit.msg_mem = 0;               */
    /* gopts.f.bit.msg_r = 0;                 */
    /* gopts.f.bit.msg_s = 0;                 */
    /* gopts.f.bit.msg_sg = 0;                */
    /* gopts.f.bit.msg_th = 0;                */
    /* gopts.f.bit.msg_tr = 0;                */
    /* gopts.f.bit.msg_trace_listener = 0;    */
    /* gopts.f.bit.msg_trace_main = 0;        */
    /* gopts.f.bit.msg_trace_transaction = 0; */
    /* gopts.f.bit.msg_w = 0;                 */
  }
  return NULL;
}

static mas_evaluated_t *
log_cmd( STD_CMD_ARGS )
{
  if ( ( 0 == strcmp( args, "on" ) ) || ( 0 == strcmp( args, "yes" ) ) || ( 0 == strcmp( args, "1" ) ) )
  {
    ctrl.log_disabled = 0;
  }
  else if ( ( 0 == strcmp( args, "off" ) ) || ( 0 == strcmp( args, "no" ) ) || ( 0 == strcmp( args, "0" ) ) )
  {
    ctrl.log_disabled = 1;
  }
  else if ( ( 0 == strcmp( args, "start" ) ) )
  {
    mas_logger_start(  );
  }
  else if ( ( 0 == strcmp( args, "stop" ) ) )
  {
    mas_logger_stop(  );
  }
  return NULL;
}

#ifdef MAS_USE_TICKER
static mas_evaluated_t *
ticker_cmd( STD_CMD_ARGS )
{
  if ( ( 0 == strcmp( args, "on" ) ) || ( 0 == strcmp( args, "yes" ) ) || ( 0 == strcmp( args, "1" ) ) )
  {
    mas_ticker_start(  );
  }
  else if ( ( 0 == strcmp( args, "off" ) ) || ( 0 == strcmp( args, "no" ) ) || ( 0 == strcmp( args, "0" ) ) )
  {
    mas_ticker_stop(  );
  }
  else if ( ( 0 == strcmp( args, "hide" ) ) )
  {
    ctrl.ticker_hide = 1;
  }
  else if ( ( 0 == strcmp( args, "show" ) ) )
  {
    ctrl.ticker_hide = 0;
  }
  return NULL;
}
#endif

#ifdef MAS_USE_LISTENER
static mas_evaluated_t *
var_cmd( STD_CMD_ARGS )
{
  char *result = NULL;

  mas_variable_t *var;

  /*                                                      */
  /* var = mas_thread_variables_find( "client", "uuid" ); */
  /* if ( var )                                           */
  {
    mas_lcontrol_t *plcontrol = NULL;
    const char *uuid = NULL;

    if ( prcontrol )
    {
      uuid = prcontrol->uuid;
      plcontrol = prcontrol->plcontrol;
    }
    if ( plcontrol )
    {
      const char *cname;
      const char *cvalue;
      char *name = NULL;
      char *value = NULL;
      size_t len = 0;
      int quot;

      cvalue = mas_find_next_arg( args, &cname, &len, &quot );
      name = mas_strndup( cname, len );
      value = mas_strdup( cvalue );
      cMSG( "CHECK V: args=%s ::: name=%s :: cname=%s :: cvalue=%s :: value=%s :: len=%lu", args, name, cname, cvalue, value, len );

      if ( args )
      {
        mas_lcontrol_variable_set_text( plcontrol, MAS_THREAD_LISTENER, uuid, name, value );
        result = mas_strdup( value );
      }
      else
      {
        var = mas_lcontrol_variables_find( plcontrol, uuid, name );
        if ( var )
          result = mas_strdup( var->value );
      }
      mas_free( value );
      mas_free( name );
    }
  }
  return mas_evaluated_wrap_pchar( result );
}
#endif

mas_cmd_t subcmdtable[] = {
  {0, NULL, mas_evaluate_list_cmd, NULL}
  ,
  {1, "msg", msg_cmd, NULL}
  ,
  {2, "log", log_cmd, NULL}     /* set log ... */
  ,
#ifdef MAS_USE_TICKER
  {3, "ticker", ticker_cmd, NULL} /* set ticker ... */
  ,
#endif
#ifdef MAS_USE_LISTENER
  {4, "var", var_cmd, NULL}     /* set var ... */
  ,
#endif
  {999, NULL, NULL, NULL}
};
