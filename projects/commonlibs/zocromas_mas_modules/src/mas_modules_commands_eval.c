#include <mastar/wrap/mas_std_def.h>

#include <stdio.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>


#include "mas_modules_load_module.h"
#include "mas_modules_commands.h"
#include "mas_modules_commands_eval.h"

/*
this:
  mas_modules_commands_eval.c
related:
  mas_modules_commands.c
  mas_modules_commands_eval.h
  mas_modules_commands.h
  mas_modules_load_module.c
  mas_modules_load_module.h
  mas_modules_types.h
more:
  mas_cmdmod_client.c
  mas_cmdmod_get.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_cmdmod_top.c
  mas_control.c
  mas_opts.c

*/





char *
list_commands_cmd( STD_CMD_ARGS )
{
  char *ans = NULL;

  mas_cmd_t *cmd = this_table;

  /* cMSG( "(%s)HELP_CMD %p", cmd ? cmd->name : "?", ( void * ) cmd ); */
  while ( cmd && ( cmd->name || cmd->function || cmd->libname /* || cmd->subtable */  ) )
  {
    if ( cmd->name )
    {
      char str[1024];

      snprintf( str, sizeof( str ), "%d> %3d. %4s %s (%d)\n", level, ( int ) ( cmd - this_table ), cmd->subtable ? " @ " : "", cmd->name,
                cmd->only_level );
      ans = mas_strcat_x( ans, str );
    }
    cmd++;
  }
  return ans;
}

static char *
universal_complex_cmd( STD_CMD_ARGS )
{
  char *answer = NULL;

  if ( this_command )
  {
    if ( this_command->subtable )
    {
      /* MAS_LOG( "(%s) UNI command table %p", this_command->name, ( void * ) this_command->subtable ); */
      MAS_LOG( "UNI command %s", this_command->name );
      WMSG( "UNI command %s", this_command->name );
      question = args;
      args = NULL;
      answer = mas_modules_commands( STD_CMD_PASS );
    }
    else
    {
      EMSG( "(%s) no sub command table", this_command->name );
    }
  }
  return answer;
}

static int
mas_missing_funsetup( mas_cmd_t * pcommand, unsigned level )
{
  int r = -1;

  if ( pcommand )
  {
    const char *libname;
    const char *name;
    mas_cmd_fun_t cmd_fun = NULL;
    mas_cmd_t *loaded_subtable = NULL;

    name = pcommand->name;
    libname = pcommand->libname;
    HMSG( "MISSING FUNC for %s [sub:%d]", name, pcommand->subtable ? 1 : 0 );
    if ( !libname )
    {
      HMSG( "LIB by CMD %s", name );
      libname = name;
      name = "";
    }

    MAS_LOG( "to look for module %s.%s : %d", libname, name, pcommand->function ? 1 : 0 );
    tMSG( "to look for module %s : %s", libname, name );
    {

      HMSG( "CMD NAME %s.%s L%d", libname, name, level );
      if ( name && *name )
      {
      }
      {
        char *full_libname = NULL;

        /* full_libname = mas_strdup( "mas_cmdmod_" ); */
        tMSG( "lib %s + %s", full_libname, libname );
        MAS_LOG( "lib %s + %s", full_libname, libname );
        full_libname = mas_strcat_x( full_libname, libname );
        if ( 0 && name && *name )
        {
          char *full_fun_name = NULL;

          MAS_LOG( "loading  func. %s:%s", full_libname, full_fun_name );
          cmd_fun = mas_modules_load_cmd_func( full_libname, full_fun_name );
          mas_free( full_fun_name );
        }
        if ( !cmd_fun )
        {
          loaded_subtable = mas_modules_load_subtable( full_libname );
          if ( loaded_subtable )
          {
            cmd_fun = universal_complex_cmd;
            WMSG( "SET UNI for %s", name );
            MAS_LOG( "universal command for %s {%p}", libname, ( void * ) ( unsigned long ) cmd_fun );
            tMSG( "universal command for %s.%s", libname, name );
          }
          else
          {
            EMSG( "No subtable at %s.%s", libname, name );
          }
        }
        mas_free( full_libname );
      }
    }
    tMSG( "cmd_fun:%d", cmd_fun ? 1 : 0 );
    if ( cmd_fun )
    {
      if ( pcommand )
      {
        pcommand->function = cmd_fun;
        pcommand->subtable = loaded_subtable;
        /* MAS_LOG( "cmd_fun:%p ; subtable:%p", ( void * ) ( unsigned long ) cmd_fun, ( void * ) loaded_subtable ); */
        MAS_LOG( "{%p} set fun/sbt for module %s.%s : %d : %p", ( void * ) pcommand, libname, name,
                 pcommand->function ? 1 : 0, ( void * ) pcommand->subtable );
        r = 0;
      }
    }
    else if ( pcommand )
    {
      EMSG( "no function (cmd id=%d; cmdname='%s') libname:'%s'", pcommand->id, name, libname );
      MAS_LOG( "no function (cmd id=%d; cmdname='%s') libname:'%s'", pcommand->id, name, libname );
      r = -1;
    }
  }
  return r;
}

static mas_cmd_t root_command = {.name = "root",.libname = "root",.only_level = 0 };

char *
mas_evaluate_command_slash_plus( const char *root, const char *uri, size_t size, size_t * ptruesize, const void *arg )
{
  const char *p;
  char *answer;
  mas_rcontrol_t *prcontrol;

  prcontrol = ( mas_rcontrol_t * ) arg;
  p = uri;
  if ( p && p )
    do
      p++;
    while ( *p != '/' );
  if ( p && *p )
    p++;
  answer = mas_evaluate_transaction_command_slash( prcontrol, p );
  if ( ptruesize )
  {
    if ( answer )
      *ptruesize = strlen( answer );
    else
      *ptruesize = 0;
  }
  return answer;
}

char *
mas_evaluate_transaction_command_slash( mas_rcontrol_t * prcontrol, const char *uri )
{
  char *p;
  char *question = NULL;
  char *answer = NULL;

  /* bin_type_t bin = 0; */
  question = mas_strdup( uri );
  p = question;
  while ( ( p = strchr( p, '/' ) ) )
    *p = ' ';
  MAS_LOG( "to make out XCROMAS %s", question );
  answer = mas_evaluate_transaction_command( prcontrol, question );
  if ( answer == ( char * ) -1L )
    answer = NULL;
  else if ( prcontrol && prcontrol->qbin )
  {
    MAS_LOG( "to make out XCROMAS %s => %s", question, answer );
    MAS_LOG( "to make out XCROMAS %p => %p", ( void * ) question, ( void * ) answer );
    if ( answer )
      mas_free( answer );
    answer = NULL;
  }
  if ( question )
    mas_free( question );
  return answer;
}

char *
mas_evaluate_command( const char *question )
{
  WMSG( "EVAL CMD %s", question );
  return mas_evaluate_transaction_command( NULL, question );
}

char *
mas_evaluate_transaction_command( mas_rcontrol_t * prcontrol, const char *question )
{
  WMSG( "EVAL TR '%s'", question );
  return mas_evaluate_cmd( 0, NULL, NULL, prcontrol, question, question /* args */ , 1 /*level */  );
}

char *
mas_evaluate_cmd( STD_CMD_ARGS )
{
  int r = 0;
  char *answer = NULL;

  if ( prcontrol )
    prcontrol->qbin = MSG_BIN_NONE;
  MAS_LOG( "evaluate: cmd %p", ( void * ) this_command );
  if ( this_command )
  {
    WMSG( "EVAL %s", this_command->name );
  }
  else
  {
    this_command = &root_command;
    MAS_LOG( "evaluate: ROOT %s cmd %p subtab:%p", this_command ? this_command->name : NULL, ( void * ) this_command,
             ( void * ) this_command->subtable );
    WMSG( "EVAL ROOT %s", this_command->name );
  }
  if ( this_command )
  {
    MAS_LOG( "evaluate: '%s' args: '%s' cmd:%p", this_command->name, args, ( void * ) ( unsigned long ) this_command );
    /* missing function ptr (this_command->function): */
    if ( !this_command->function && this_command->name && *( this_command->name ) )
    {
      MAS_LOG( "{%p} must set fun/sbt for module %s.%s : %d : %p", ( void * ) this_command, this_command->libname,
               this_command->name, this_command->function ? 1 : 0, ( void * ) this_command->subtable );
      r = mas_missing_funsetup( this_command, level );
      MAS_LOG( "evaluate : missing function - '%s' args: '%s'", this_command->name, args );
    }
    tMSG( "(%d) function:%d", r, this_command->function ? 1 : 0 );
    if ( r >= 0 && this_command->function )
    {
      tMSG( "eval %d. %s : %d : %d", this_command->id, this_command->name,
            this_command->function ? 1 : 0, this_command->function == universal_complex_cmd );
      /* EVALUATING COMMAND */
      HMSG( "EVAL %s", this_command->name );
      answer = ( this_command->function ) ( STD_CMD_PASS );
      tMSG( "eval'd A(%s) B(%d) Q(%d)", answer ? ( answer == ( char * ) -1L ? "-" : answer ) : NULL, prcontrol ? prcontrol->qbin : 0,
            ctrl.do_exit );
      HMSG( "QUIT (%s) %u", this_command->name, ctrl.do_exit );
      if ( MAS_VALID_ANSWER( answer ) )
      {
        /* cMSG( "answer for %s : %s", this_command->name, level == 1 ? answer : "SKIPPED" ); */
        MAS_LOG( "(lev.%d) evaluated: %s(%s); answer: '%s'", level, this_command->name, args, level == 1 ? answer : "SKIPPED" );
      }
    }
    else
    {
      EMSG( "no function (cmd id=%d; cmdname='%s') libname:'%s'", this_command->id, this_command->name, this_command->libname );
      MAS_LOG( "no function (cmd id=%d; cmdname='%s') libname:'%s'", this_command->id, this_command->name, this_command->libname );
      answer = MAS_INVALID_ANSWER;
    }
  }
  return answer;
}
