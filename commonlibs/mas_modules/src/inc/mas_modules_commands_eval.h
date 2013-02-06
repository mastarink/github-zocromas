#ifndef MAS_MODULES_COMMANDS_EVAL_H
#  define MAS_MODULES_COMMANDS_EVAL_H

#  include <mastar/types/mas_modules_types.h>

char *mas_evaluate_command( STD_CMD_ARGS );
char *mas_evaluate_command_slash( mas_rcontrol_t * prcontrol, const char *uri );
char *mas_evaluate_command_slash_plus( const char *root, const char *uri, size_t size, size_t * ptruesize, const void *arg );

char *list_commands_cmd( STD_CMD_ARGS );

/* char *mas_evaluate_cmd( STD_CMD_ARGS_BASE, mas_rcontrol_t * prcontrol, mas_cmd_t ** pcommand ); */

#  undef MAS_WEAK
#endif
