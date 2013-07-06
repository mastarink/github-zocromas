#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_opts_types.h>


#include <mastar/fileinfo/mas_unidata.h>

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_varvec_types.h>
#include <mastar/types/mas_varset_types.h>
#include <mastar/varvec/mas_varvec_namevalue.h>



#include <mastar/thvariables/mas_thread_variables.h>



/*
this:
  mas_cmdmod_check.c
related:
  mas_cmdmod_top.c
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/




static mas_evaluated_t *
version_cmd( STD_CMD_ARGS )
{
  return NULL;
}

static mas_evaluated_t *
args_cmd( STD_CMD_ARGS )
{
  char *result = NULL;

  mas_varvec_element_t *var;

  var = mas_thread_variables_find( "client", "args" );
  if ( var )
  {
    /* cMSG( "CHECK ARGS: %s ::: %s", args, var->value ); */
    result = mas_varvec_variable_get_value( var );
  }
  else if ( args )
  {
    /* cMSG( "CHECK ARGS: %s", args ); */
    mas_thread_variable_set_text( MAS_THREAD_TRANSACTION, "client", "args", args );
    result = mas_strdup( args );
  }
  return mas_evaluated_wrap_pchar( result );
}

static mas_evaluated_t *
uuid_cmd( STD_CMD_ARGS )
{
  char *uuid = NULL;

  /* if ( prcontrol ) */
  {
    /* mas_lcontrol_t *plcontrol;       */
    /*                                   */
    /* plcontrol = prcontrol->plcontrol; */

    if ( prcontrol )
    {
      if ( !prcontrol->uuid && args )
      {
        prcontrol->uuid = mas_strdup( args );
      }
      if ( prcontrol->uuid )
        uuid = mas_strdup( prcontrol->uuid );
    }
    /* if ( plcontrol )                                                              */
    /* {                                                                             */
    /*   if ( args )                                                                 */
    /*   {                                                                           */
    /*     cMSG( "CHECK UUID: %s", args );                                           */
    /*     mas_lcontrol_variable_set_text( plcontrol, MAS_THREAD_LISTENER, uuid,  ); */
    /*   }                                                                           */
    /*   else                                                                        */
    /*   {                                                                           */
    /*     var = mas_lcontrol_variables_find( plcontrol, "client", uuid );           */
    /*     if ( var )                                                                */
    /*     {                                                                         */
    /*       cMSG( "CHECK UUID: %s ::: %s", args, var->value );                      */
    /*     }                                                                         */
    /*   }                                                                           */
    /* }                                                                             */
  }
  return mas_evaluated_wrap_pchar( uuid );
}


mas_cmd_t subcmdtable[] = {
  {0, NULL, mas_evaluate_list_cmd, NULL}
  ,
  {1, "version", version_cmd, NULL} /* check version */
  ,
  {2, "args", args_cmd, NULL}   /* check args */
  ,
  {3, "uuid", uuid_cmd, NULL}   /* check uuid */
  ,
  {999, NULL, NULL, NULL}
};
