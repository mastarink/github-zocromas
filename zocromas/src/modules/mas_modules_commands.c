#include "mas_basic_def.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include <signal.h>
#include <dlfcn.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mas_common.h"
#include "log/inc/mas_log.h"

#include "mas_modules_commands_eval.h"
#include "mas_modules_commands.h"

/*
this:
  mas_modules_commands.c
related:
  mas_modules_commands.h
  mas_modules_commands_eval.c
  mas_modules_load_module.c
*/




static mas_cmd_t *
mas_server_lookup_command( mas_cmd_t * cmdtable, const char *question, const char **pargs )
{
  const char *args = NULL;
  const char *q = NULL;
  int quo = 0;
  size_t len;
  mas_cmd_t *found = NULL;
  mas_cmd_t *cmd = NULL;

  args = mas_find_next_arg( question, &q, &len, &quo );
  MAS_LOG( "looking for q:'%s' args:'%s'", q, args );
  cmd = cmdtable;
  while ( question && cmd && !found && ( cmd->name || cmd->function || cmd->libname /* || cp->subtable */  ) )
  {
    size_t l = 0;

    if ( ( !cmd->name && !*question ) || (  cmd->name && 0 == strncmp( cmd->name, question, ( l = strlen( cmd->name )) ) && l == len ) )
    {
      MAS_LOG( "located #%d: cmd '%s'", cmd->id, question );
      found = cmd;
      if ( pargs )
        ( *pargs ) = args;
    }
    cmd++;
  }
  MAS_LOG( "cmd %s : %s (%s)", question, found ? "FOUND" : "NOT found", found ? found->name : "" );
  cMSG( "look for '%s' : %s", question, found ? "FOUND" : "NOT found" );
  return found;
}


char *
mas_modules_commands( STD_CMD_ARGS )
{
  char *answer = NULL;
  static mas_cmd_t def_cmd = { 1, "unknown", NULL, NULL, 1 };
  mas_cmd_t *found = NULL;

  /* cMSG( "@ level:%d %s", level, question ); */

  if ( question )
  {
    if ( this_command )
    {
      found = mas_server_lookup_command( this_command->subtable, question, &args );
      MAS_LOG( "found command %p : %s", ( void * ) found, found ? found->name : NULL );
      if ( !found )
      {
        found = &def_cmd;
        MAS_LOG( "setting def_cmd %p : %s", ( void * ) found, found ? found->name : NULL );
      }
    }
  }
  else if ( !level && pbinary )
  {
    MAS_LOG( "empty command; level %d", level );
    *pbinary = MSG_BIN_EMPTY_COMMAND;
  }

  if ( found )
  {
    if ( found->unknown )
    {
      if ( pbinary )
        *pbinary = MSG_BIN_UNKNOWN_COMMAND;
      EMSG( "NOT found cmd '%s'", question );
      MAS_LOG( "NOT found cmd '%s'", question );
      answer = mas_strdup( question );
    }
    else if ( found->only_level && found->only_level != level )
    {
      if ( pbinary )
        *pbinary = MSG_BIN_UNKNOWN_COMMAND;
      EMSG( "NOT found @ level %u / %u cmd '%s'", level, found->only_level, question );
      MAS_LOG( "NOT found @ level %u / %u cmd '%s'", level, found->only_level, question );
      answer = mas_strdup( question );
    }
    else
    {
      tMSG( "evaluating %s ( %s )", question, args );
      MAS_LOG( "evaluating %s ( %s )", question, args );
      answer = mas_evaluate_command( 0, this_command->subtable, found, prcontrol, question, args, level + 1, pbinary );
    }
  }
  else
  {
    if ( pbinary )
      *pbinary = MSG_BIN_UNKNOWN_COMMAND;
    MAS_LOG( "NO question, no answer" );
    EMSG( "NOT FOUND; question:'%s'", question );
  }
  return answer;
}
