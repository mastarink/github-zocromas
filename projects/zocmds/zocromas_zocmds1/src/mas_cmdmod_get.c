#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


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


#include <mastar/fileinfo/mas_unidata.h>

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





static mas_evaluated_t *
opts_cmd( STD_CMD_ARGS )
{
  mas_options_t *result = NULL;

#ifdef cMSG
  cMSG( "get opts args:%s", args );
#endif
  if ( prcontrol )
    prcontrol->qbin = MSG_BIN_OPTS;
  result = mas_malloc( sizeof( *( popts ) ) );
  *result = *( popts );
#ifdef cMSG
  cMSG( "opts:%x", ( *( unsigned int * ) result ) );
#endif
  return mas_evaluated_wrap_pchar( ( char * ) result );
}

static mas_evaluated_t *
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
  return mas_evaluated_wrap_pchar( val );
}

static mas_evaluated_t *
date_cmd( STD_CMD_ARGS )
{
  char outstr[64];

  strftime( outstr, sizeof( outstr ), "%a, %d %b %Y %T %z", mas_xlocaltime(  ) );
  return mas_evaluated_wrap_pchar( mas_strdup( outstr ) );
}

static mas_evaluated_t *
args_cmd( STD_CMD_ARGS )
{
  return mas_evaluated_wrap_pchar( mas_argv_string( ctrl.launchervv.c, ctrl.launchervv.v, 1 ) );
}

#ifdef MAS_USE_VARIABLES
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
  return mas_evaluated_wrap_pchar( result );
}
#endif

static mas_evaluated_t *
uuid_cmd( STD_CMD_ARGS )
{
  char *uuid = NULL;

  if ( prcontrol && prcontrol->uuid )
    uuid = mas_strdup( prcontrol->uuid );
  return mas_evaluated_wrap_pchar( uuid );
}

static mas_evaluated_t *
version_cmd( STD_CMD_ARGS )
{
  char *s;

  s = mas_strdup( MAS_C_DATE );
  s = mas_strcat_x( s, " / " );
  s = mas_strcat_x( s, PACKAGE_VERSION );
  return mas_evaluated_wrap_pchar( s );
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
