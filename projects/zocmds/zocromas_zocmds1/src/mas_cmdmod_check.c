#include <mastar/wrap/mas_std_def.h>

#include <stdio.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_opts_types.h>

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifdef MAS_OLD_VARIABLES_HTTP
#  include <mastar/variables/mas_variables.h>
#else
#  include <mastar/types/mas_varset_types.h>
/* #  include <mastar/varset/mas_varset_vclass.h> */
#  include <mastar/varset/mas_varset_vclass_namevalue.h>
/* #  include <mastar/varset/mas_varset.h> */
#endif

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




static char *
version_cmd( STD_CMD_ARGS )
{
  return NULL;
}

static char *
args_cmd( STD_CMD_ARGS )
{
  char *result = NULL;

#ifdef MAS_OLD_VARIABLES_HTTP
  mas_variable_t *var;
#else
  mas_vclass_element_t *var;
#endif

  var = mas_thread_variables_find( "client", "args" );
  if ( var )
  {
    /* cMSG( "CHECK ARGS: %s ::: %s", args, var->value ); */
#ifdef MAS_OLD_VARIABLES_HTTP
    result = mas_strdup( var->value );
#else
    result = mas_varset_vclass_variable_get_value( var );
#endif
  }
  else if ( args )
  {
    /* cMSG( "CHECK ARGS: %s", args ); */
    mas_thread_variable_set_text( MAS_THREAD_TRANSACTION, "client", "args", args );
    result = mas_strdup( args );
  }
  return result;
}

static char *
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
  return uuid;
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
