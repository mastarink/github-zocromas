#include <mastar/wrap/mas_std_def.h>

#include <stdio.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/thvariables/mas_thread_variables.h>

/*
this:
  mas_cmdmod_test.c
related:
  mas_cmdmod_check.c
  mas_cmdmod_top.c
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_set.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
*/


static char *
cmd_1( STD_CMD_ARGS )
{
  return mas_strdup( "T1" );
}

static char *
cmd_2( STD_CMD_ARGS )
{
  return mas_strdup( "T2" );
}

static char *
cmd_3( STD_CMD_ARGS )
{
  return "T3";
}

static char *
cmd_4( STD_CMD_ARGS )
{
  return NULL;
}

/* static char *         */
/* cmd_5( STD_CMD_ARGS ) */
/* {                     */
/*   return NULL;        */
/* }                     */

static char *
cmd_6( STD_CMD_ARGS )
{
  return NULL;
}

static char *
cmd_7( STD_CMD_ARGS )
{
  return NULL;
}

static char *
cmd_8( STD_CMD_ARGS )
{
  return NULL;
}

static char *
cmd_9( STD_CMD_ARGS )
{
  return NULL;
}

mas_cmd_t subcmdtable[] = {
  {0, NULL, mas_evaluate_list_cmd, NULL}
  ,
  {1, "1", cmd_1, NULL}
  ,
  {2, "2", cmd_2, NULL}
  ,
  {3, "3", cmd_3, NULL,.result_type = MAS_CMD_CONST_PCHAR}
  ,
  {4, "4", cmd_4, NULL}
  ,
  {5, "5", NULL, NULL,.result_type = MAS_CMD_CONST_FNAME_LOAD,.vid = ( void * ) "/mnt/new_misc/develop/autotools/zoc/projects/projects.list"}
  ,
  {6, "6", cmd_6, NULL}
  ,
  {7, "7", cmd_7, NULL}
  ,
  {8, "8", cmd_8, NULL}
  ,
  {9, "9", cmd_9, NULL}
  ,
  {999, NULL, NULL, NULL}
};
