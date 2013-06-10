#include <mastar/wrap/mas_std_def.h>

#include <stdlib.h>
#include <time.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>


#include <mastar/thvariables/mas_thread_variables.h>
#ifdef MAS_USE_LISTENER
#  include "listener/inc/mas_listener_control.h"
#endif

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;


/*
this:
  mas_cmdmod_get.c
related:
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_cmdmod_root.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/





static char *
opts_cmd( STD_CMD_ARGS )
{
  MAS_PASS_OPTS_DECL_PREF;
  mas_options_t *result = NULL;

#ifdef cMSG
  cMSG( "get opts args:%s", args );
#endif
  if ( prcontrol )
    prcontrol->qbin = MSG_BIN_OPTS;
  result = mas_malloc( sizeof( *( MAS_PASS_OPTS_REF ) ) );
  *result = *( MAS_PASS_OPTS_REF );
#ifdef cMSG
  cMSG( "opts:%x", ( *( unsigned int * ) result ) );
#endif
  return ( char * ) result;
}

static char *
env_cmd( STD_CMD_ARGS )
{
  char *val = NULL;

  if ( args && *args )
  {
    val = getenv( args );
    if ( val )
      val = mas_strdup( val );
#ifdef cMSG
    cMSG( "env(%s)=%s", args, val );
#endif
  }
  else
  {
    if ( ctrl.launcherev.v )
    {
      char *const *pe;
      const char *notan;

      val = NULL;
      pe = ctrl.launcherev.v;
      while ( pe && *pe )
      {
        notan = mas_find_eq_value( *pe );
        if ( **pe < ' ' || **pe == '}' || ( notan && notan[0] == '(' && notan[1] == ')' ) )
        {
        }
        else
        {
          val = mas_strcat_x( val, *pe );
          val = mas_strcat_x( val, "\n" );
        }
        pe++;
      }
    }
  }
  return val;
}

static char *
date_cmd( STD_CMD_ARGS )
{
  char outstr[64];

  strftime( outstr, sizeof( outstr ), "%a, %d %b %Y %T %z", mas_xlocaltime(  ) );
  return mas_strdup( outstr );
}

static char *
args_cmd( STD_CMD_ARGS )
{
  return mas_argv_string( ctrl.launchervv.c, ctrl.launchervv.v, 1 );
}

#ifdef MAS_USE_VARIABLES
static char *
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

    /* uuid = var->value; */
    if ( prcontrol )
    {
      uuid = prcontrol->uuid;
      plcontrol = prcontrol->plcontrol;
    }
    if ( plcontrol )
    {
      var = mas_lcontrol_variables_find( plcontrol, uuid, args );
      if ( var )
        result = mas_strdup( var->value );
    }
  }
  return result;
}
#endif

static char *
uuid_cmd( STD_CMD_ARGS )
{
  char *uuid = NULL;

  if ( prcontrol && prcontrol->uuid )
    uuid = mas_strdup( prcontrol->uuid );
  return uuid;
}

static char *
version_cmd( STD_CMD_ARGS )
{
  char *s;

  s = mas_strdup( MAS_C_DATE );
  s = mas_strcat_x( s, " / " );
  s = mas_strcat_x( s, PACKAGE_VERSION );
  return s;
}

mas_cmd_t subcmdtable[] = {
  {0, NULL, mas_evaluate_list_cmd, NULL}
  ,
  {1, "opts", opts_cmd, NULL}   /* get opts */// in this module, name: opts_cmd
  ,
  {2, "env", env_cmd, NULL}     /* get env */// in this module, name: env_cmd
  ,
  {3, "mem", NULL, NULL}        /* get mem */// in module 'mem'
  ,
  {4, "client", NULL, NULL}     /* get client */
  ,
  {5, "server", NULL, NULL}     /* get server */
  ,
  {5, "server_old", NULL, NULL} /* get server_old */
  ,
  {6, "date", date_cmd, NULL}   /* get date */
  ,
  {7, "args", args_cmd, NULL}   /* get args */
  ,
#ifdef MAS_USE_VARIABLES
  {8, "var", var_cmd, NULL}     /* get var */
  ,
#endif
  {9, "uuid", uuid_cmd, NULL}   /* get uuid */
  ,
  {10,.name = "version",.function = version_cmd,.libname = NULL} /* get version */
  ,
  {11, "readline", NULL, NULL}  /* get readline */
  ,
  {999, NULL, NULL, NULL}
};
