#include "mas_basic_def.h"

#include <stdlib.h>
#include <time.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mas_common.h"

#include "variables/inc/mas_thread_variables.h"
#include "listener/inc/mas_listener_control.h"

#include "modules/inc/mas_modules_commands_eval.h"
#include "modules/inc/mas_modules_commands.h"

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
  mas_options_t *result = NULL;

  cMSG( "get opts args:%s", args );
  if ( pbinary )
  {
    *pbinary = MSG_BIN_OPTS;
    result = mas_malloc( sizeof( opts ) );
    *result = opts;
  }
  cMSG( "msg:%d / %d", result->f.bit.msg_c, result->f.bit.msg_s );
  cMSG( "msg:%d / %d", opts.f.bit.msg_c, opts.f.bit.msg_s );
  cMSG( "opts:%x", ( *( unsigned int * ) result ) );
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
    cMSG( "env(%s)=%s", args, val );
  }
  else
  {
    if ( ctrl.launchere )
    {
      char *const *pe;
      const char *notan;

      val = NULL;
      pe = ctrl.launchere;
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
  return mas_argv_string( ctrl.launcherc, ctrl.launcherv, 1 );
}

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
    const char *uuid;

    /* uuid = var->value; */
    uuid = prcontrol->uuid;
    if ( prcontrol )
    {
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
  {0, NULL, list_commands_cmd, NULL}
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
  {6, "date", date_cmd, NULL}   /* get date */
  ,
  {7, "args", args_cmd, NULL}   /* get args */
  ,
  {8, "var", var_cmd, NULL}     /* get var */
  ,
  {9, "uuid", uuid_cmd, NULL}   /* get uuid */
  ,
  {10,.name = "version",.function = version_cmd,.libname = NULL} /* get version */
  ,
  {11, "readline", NULL, NULL}  /* get readline */
  ,
  {999, NULL, NULL, NULL}
};
