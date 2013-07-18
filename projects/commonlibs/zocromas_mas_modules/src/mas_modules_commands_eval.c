#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_common_defs.h>
#include <mastar/types/mas_opts_types.h>

#include <mastar/types/mas_modules_types.h>


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>


#include <mastar/fileinfo/mas_unidata.h>


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


__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno ); */
}


static mas_evaluated_t *
_universal_complex_cmd( STD_CMD_ARGS )
{
  mas_evaluated_t *answer = NULL;

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

static mas_cmd_fun_t
_load_cmd_func( const mas_options_t * popts, const char *libname, const char *funname )
{
  mas_cmd_fun_t cmd_fun = NULL;

  cmd_fun = ( mas_cmd_fun_t ) mas_modules_load_func_from( libname, funname, popts->dir.mods );
  return cmd_fun;
}

static mas_cmd_t *
_load_subtable_from( const char *libname, const char *path )
{
  mas_cmd_t *cmd_tab = NULL;

  cmd_tab = ( mas_cmd_t * ) mas_modules_load_symbol_from( libname, "subcmdtable", path );
  MAS_LOG( "load subtable from %s => %p", libname, ( void * ) cmd_tab );
  return cmd_tab;
}

static int
_missing_funsetup( const mas_options_t * popts, mas_cmd_t * pcommand, unsigned level )
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
          cmd_fun = _load_cmd_func( popts, full_libname, full_fun_name );
          mas_free( full_fun_name );
        }
        if ( !cmd_fun )
        {
          loaded_subtable = _load_subtable_from( full_libname, popts->dir.mods );
          if ( loaded_subtable )
          {
            cmd_fun = _universal_complex_cmd;
            WMSG( "SET UNI for %s", name );
            MAS_LOG( "universal command for %s {%p}", libname, ( void * ) ( unsigned long ) cmd_fun );
            tMSG( "universal command for %s.%s", libname, name );
          }
          else
          {
            EMSG( "No subtable at %s.%s ( opts.dir.mods: '%s' )", libname, name, popts->dir.mods );
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

mas_cmd_t root_cmdtable[] = {
  {.name = "root",.libname = "root",.only_level = 0}
};
static mas_cmd_t root_command = {.name = "root",.libname = "root",.only_level = 0 };

mas_evaluated_t *
mas_evaluate_command_slash_plus( const mas_options_t * _popts, const char *root, const char *uri, size_t size, size_t * ptruesize,
                                 ino_t * ptrueinode, time_t * ptruefiletime, const void *arg )
{
  const char *p;
  mas_evaluated_t *answer;
  mas_rcontrol_t *prcontrol;

  prcontrol = ( mas_rcontrol_t * ) arg;
  p = uri;
  /* find first non-separator */
  if ( p && p )
    do
      p++;
    while ( *p != '/' );
  if ( p && *p )
    p++;
  answer = mas_evaluate_transaction_command_slash( prcontrol, p );
  if ( ptruesize )
  {
    if ( answer && answer->data )
      *ptruesize = strlen( ( char * ) answer->data );
    else
      *ptruesize = 0;
  }
  EMSG( "ANSWER: %p : %p", ( void * ) answer, ( void * ) answer ? answer->data : NULL );
  return answer;
}

mas_evaluated_t *
mas_evaluate_transaction_command_slash( mas_rcontrol_t * prcontrol, const char *uri )
{
  char *p;
  char *question = NULL;
  mas_evaluated_t *answer = NULL;

  /* bin_type_t bin = 0; */
  question = mas_strdup( uri );
  p = question;

  /* replace all separators with std. separators */
  while ( ( p = strchr( p, '/' ) ) )
    *p = ' ';
  MAS_LOG( "to make out XCROMAS %s", question );
  answer = mas_evaluate_transaction_command( prcontrol, question );

  if ( answer == ( mas_evaluated_t * ) - 1L ) /* ??? */
    answer = NULL;
  else if ( prcontrol && prcontrol->qbin )
  {
    MAS_LOG( "to make out XCROMAS %s => %s", question, answer ? ( const char * ) answer->data : NULL );
    MAS_LOG( "to make out XCROMAS %p => %p", ( void * ) question, ( void * ) answer ? answer->data : NULL );
    if ( answer )
    {
      if ( answer->data )
        mas_free( answer->data );
      mas_free( answer );
    }
    answer = NULL;
  }
  if ( question )
    mas_free( question );
  return answer;
}

mas_evaluated_t *
mas_evaluate_command( const mas_options_t * popts, const char *question )
{
  WMSG( "EVAL CMD %s", question );
  return mas_evaluate_transaction_command( NULL, question );
}

mas_evaluated_t *
mas_evaluate_transaction_command( mas_rcontrol_t * prcontrol, const char *question )
{
  WMSG( "EVAL TR '%s'", question );
  return mas_evaluate_cmd( prcontrol
                           && prcontrol->plcontrol ? prcontrol->plcontrol->popts : NULL, 0, NULL, NULL, prcontrol, question,
                           question /* args */ , 1 /*level */  );
}

mas_evaluated_t *
mas_evaluate_cmd( STD_CMD_ARGS )
{
  int r = 0;
  mas_evaluated_t *answer = NULL;

  /* char *sanswer = NULL; */

  if ( prcontrol )
  {
    prcontrol->qbin = MSG_BIN_NONE;
    if ( !popts )
      popts = prcontrol->plcontrol->popts;
  }
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
      r = _missing_funsetup( popts, this_command, level );
      MAS_LOG( "evaluate : missing function - '%s' args: '%s'", this_command->name, args );
    }
    tMSG( "(%d) function:%d", r, this_command->function ? 1 : 0 );
    if ( r >= 0 && this_command->function )
    {
      tMSG( "eval %d. %s : %d : %d", this_command->id, this_command->name,
            this_command->function ? 1 : 0, this_command->function == _universal_complex_cmd );
      /* EVALUATING COMMAND */
      HMSG( "EVAL %s", this_command->name );
      answer = ( this_command->function ) ( STD_CMD_PASS );

      /* tMSG( "eval'd A(%s) B(%d)", answer ? ( answer == ( char * ) -1L ? "-" : answer ) : NULL, prcontrol ? prcontrol->qbin : 0 ); */
      HMSG( "QUIT (%s)", this_command->name );
    }
    {
      FILE *file = NULL;
      size_t sz = 0;

      /* switch ( this_command->evaluate_type ) */
      switch ( answer ? answer->type : this_command->evaluate_type )
      {
      case MAS_CMD_PCHAR:
        break;
      case MAS_CMD_CONST_PCHAR:
        /* ??????? */
        if ( !( MAS_VALID_ANSWER( answer ) ) )
          answer->data = mas_strdup( ( char * ) answer->data );
        break;
      case MAS_CMD_CONST_FNAME_LOAD:
        {
          char *fname;

          fname = answer ? answer->data : mas_strdup( ( char * ) this_command->vid );
          answer->data = mas_load_filename_fd( fname, 0, &sz, NULL, NULL, NULL );
          mas_free( fname );
        }
        break;
      case MAS_CMD_FILE_LOAD:
        file = ( FILE * ) args;
        if ( file )
          answer->data = mas_load_file( file, 0, &sz, NULL, NULL, NULL );
        break;
      }
      if ( file )
        fclose( file );

      if ( MAS_VALID_ANSWER( answer ) )
      {
        /* cMSG( "answer for %s : %s", this_command->name, level == 1 ? answer : "SKIPPED" ); */
        MAS_LOG( "(lev.%d) evaluated: %s(%s); answer: '%s'", level, this_command->name, args,
                 level == 1 ? ( const char * ) answer->data : "SKIPPED" );
      }
      else if ( !this_command->function )
      {
        EMSG( "no function (cmd id=%d; cmdname='%s') libname:'%s'", this_command->id, this_command->name, this_command->libname );
        MAS_LOG( "no function (cmd id=%d; cmdname='%s') libname:'%s'", this_command->id, this_command->name, this_command->libname );
        if ( answer )
          answer->data = MAS_INVALID_ANSWER;
      }
    }
  }
  return answer;
}

mas_evaluated_t *
mas_evaluate_list_cmd( STD_CMD_ARGS )
{
  mas_evaluated_t *answer = NULL;
  char *sanswer = NULL;

  mas_cmd_t *cmd = this_table;

  /* cMSG( "(%s)HELP_CMD %p", cmd ? cmd->name : "?", ( void * ) cmd ); */
  while ( cmd && ( cmd->name || cmd->function || cmd->libname /* || cmd->subtable */  ) )
  {
    if ( cmd->name )
    {
      char str[1024];

      snprintf( str, sizeof( str ), "%d> %3d. %4s %s (%d)\n", level, ( int ) ( cmd - this_table ), cmd->subtable ? " @ " : "", cmd->name,
                cmd->only_level );
      sanswer = mas_strcat_x( sanswer, str );
    }
    cmd++;
  }
  if ( sanswer )
  {
    answer = mas_evaluated_create(  );
    if ( answer )
      answer->data = ( void * ) sanswer;
  }
  return answer;
}
