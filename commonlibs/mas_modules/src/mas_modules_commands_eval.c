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

      snprintf( str, sizeof( str ), "%d> %3ld. %4s %s (%d)\n", level, cmd - this_table, cmd->subtable ? " @ " : "", cmd->name,
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

#ifdef MAS_LOG
  MAS_LOG( "(%s) UNI command table %p", this_command ? this_command->name : "?", ( void * ) this_command->subtable );
#endif
  if ( this_command->subtable )
  {
    question = args;
    args = NULL;
    answer = mas_modules_commands( STD_CMD_PASS );
  }
  else
  {
    EMSG( "(%s) no sub command table", this_command ? this_command->name : NULL );
  }
  return answer;
}

static int
mas_missing_funsetup( mas_cmd_t * pcommand )
{
  int r = -1;
  char *full_libname = NULL;
  mas_cmd_t *loaded_subtable = NULL;
  mas_cmd_fun_t cmd_fun = NULL;
  const char *libname;

  if ( pcommand )
  {
    char *full_fun_name = NULL;

    libname = pcommand->libname;
    if ( !libname )
      libname = pcommand->name;

#ifdef MAS_LOG
    MAS_LOG( "to look for module %s.%s : %p", libname, pcommand->name, ( void * ) ( unsigned long long ) pcommand->function );
#endif
    cMSG( "to look for module %s : %s", libname, pcommand->name );
    {
      full_libname = mas_strdup( "mas_cmdmod_" );
      cMSG( "lib %s + %s", full_libname, libname );
#ifdef MAS_LOG
      MAS_LOG( "lib %s + %s", full_libname, libname );
#endif
      full_libname = mas_strcat_x( full_libname, libname );

      full_fun_name = mas_strdup( pcommand->name );
      full_fun_name = mas_strcat_x( full_fun_name, "_cmd" );
      if ( full_fun_name )
      {
        cmd_fun = mas_modules_load_func( full_libname, full_fun_name );
        loaded_subtable = mas_modules_load_subtable( full_libname );
        cMSG( "cmd_fun %s.%s: %p", full_libname, full_fun_name, ( void * ) ( unsigned long long ) cmd_fun );
#ifdef MAS_LOG
        MAS_LOG( "loading  func. %s:%s => func:%p subt:%p", full_libname, full_fun_name, ( void * ) ( unsigned long ) cmd_fun,
                 ( void * ) loaded_subtable );
#endif
        if ( !cmd_fun )
        {
          if ( loaded_subtable )
          {
            cmd_fun = universal_complex_cmd;
#ifdef MAS_LOG
            MAS_LOG( "universal command for %s {%p}", libname, ( void * ) ( unsigned long ) cmd_fun );
#endif
            cMSG( "universal command for %s.%s", full_libname, full_fun_name );
          }
          else
          {
            EMSG( "No subtable at %s.%s", full_libname, full_fun_name );
          }
        }
        mas_free( full_fun_name );
      }
    }
    cMSG( "cmd_fun:%p", ( void * ) ( unsigned long ) cmd_fun );
    if ( cmd_fun )
    {
      if ( pcommand )
      {
        pcommand->function = cmd_fun;
        pcommand->subtable = loaded_subtable;
#ifdef MAS_LOG
        /* MAS_LOG( "cmd_fun:%p ; subtable:%p", ( void * ) ( unsigned long ) cmd_fun, ( void * ) loaded_subtable ); */
        MAS_LOG( "{%p} set fun/sbt for module %s.%s : %p : %p", ( void * ) pcommand, libname, pcommand->name,
                 ( void * ) ( unsigned long ) pcommand->function, ( void * ) pcommand->subtable );
#endif
        r = 0;
      }
    }
    else if ( pcommand )
    {
#ifdef EMSG
      EMSG( "no function (cmd id=%d; cmdname='%s') libname:'%s'", pcommand->id, pcommand->name, libname );
#endif
#ifdef MAS_LOG
      MAS_LOG( "no function (cmd id=%d; cmdname='%s') libname:'%s'", pcommand->id, pcommand->name, libname );
#endif
      r = -1;
    }
    mas_free( full_libname );
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
  answer = mas_evaluate_command_slash( prcontrol, p );
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
mas_evaluate_command_slash( mas_rcontrol_t * prcontrol, const char *uri )
{
  char *p;
  char *question = NULL;
  char *answer = NULL;

  /* bin_type_t bin = 0; */
  question = mas_strdup( uri );
  p = question;
  while ( ( p = strchr( p, '/' ) ) )
    *p = ' ';
#ifdef MAS_LOG
  MAS_LOG( "to make out XCROMAS %s", question );
#endif
  answer = mas_evaluate_command( 0, NULL, NULL, prcontrol, question, question /* args */ , 1 /*level */  );
  if ( answer == ( char * ) -1L )
    answer = NULL;
  else if ( prcontrol->qbin )
  {
#ifdef MAS_LOG
    MAS_LOG( "to make out XCROMAS %s => %s", question, answer );
    MAS_LOG( "to make out XCROMAS %p => %p", ( void * ) question, ( void * ) answer );
#endif
    if ( answer )
      mas_free( answer );
    answer = NULL;
  }
  if ( question )
    mas_free( question );
  return answer;
}

/* char *                                                                                                                             */
/* mas_evaluate_command( mas_cmd_t * ptable, mas_cmd_t * this_command,                                                                */
/*                       mas_rcontrol_t * prcontrol, const char *question, const char *args, unsigned level, mas_header_t * pheader ) */
char *
mas_evaluate_command( STD_CMD_ARGS )
{
  int r = 0;
  char *answer = NULL;

  prcontrol->qbin = MSG_BIN_NONE;
#ifdef MAS_LOG
  MAS_LOG( "evaluate: cmd %p", ( void * ) this_command );
#endif
  if ( !this_command )
  {
    this_command = &root_command;
#ifdef MAS_LOG
    MAS_LOG( "evaluate: ROOT %s cmd %p subtab:%p", this_command ? this_command->name : NULL, ( void * ) this_command,
             ( void * ) this_command->subtable );
#endif
  }
  if ( this_command )
  {
#ifdef MAS_LOG
    MAS_LOG( "evaluate: '%s' args: '%s' cmd:%p", this_command->name, args, ( void * ) ( unsigned long ) this_command );
#endif
    /* missing function ptr (this_command->function): */
    if ( !this_command->function && this_command->name && *( this_command->name ) )
    {
#ifdef MAS_LOG
      MAS_LOG( "{%p} must set fun/sbt for module %s.%s : %p : %p", ( void * ) this_command, this_command->libname,
               this_command->name, ( void * ) ( unsigned long ) this_command->function, ( void * ) this_command->subtable );
#endif
      r = mas_missing_funsetup( this_command );
#ifdef MAS_LOG
      MAS_LOG( "evaluate : missing function - '%s' args: '%s'", this_command->name, args );
#endif
    }
    cMSG( "(%d) function:%p", r, ( void * ) ( unsigned long long ) this_command->function );
    if ( r >= 0 && this_command->function )
    {
      cMSG( "eval %d. %s : %p : %d", this_command->id, this_command->name,
            ( void * ) ( unsigned long long ) ( this_command->function ), this_command->function == universal_complex_cmd );
      /* EVALUATING COMMAND */
      answer = ( this_command->function ) ( STD_CMD_PASS );
      cMSG( "eval'd ans(%s) bin(%d) q(%d)", answer ? ( answer == ( char * ) -1L ? "-" : answer ) : NULL, prcontrol->qbin, ctrl.do_quit );
      if ( MAS_VALID_ANSWER( answer ) )
      {
        cMSG( "answer for %s : %s", this_command->name, level == 1 ? answer : "SKIPPED" );
#ifdef MAS_LOG
        MAS_LOG( "(lev.%d) evaluated: %s(%s); answer: '%s'", level, this_command->name, args, level == 1 ? answer : "SKIPPED" );
#endif
      }
    }
    else
    {
#ifdef EMSG
      EMSG( "no function (cmd id=%d; cmdname='%s') libname:'%s'", this_command->id, this_command->name, this_command->libname );
#endif
#ifdef MAS_LOG
      MAS_LOG( "no function (cmd id=%d; cmdname='%s') libname:'%s'", this_command->id, this_command->name,
               this_command->libname );
#endif
      answer = MAS_INVALID_ANSWER;
    }
  }
  return answer;
}
