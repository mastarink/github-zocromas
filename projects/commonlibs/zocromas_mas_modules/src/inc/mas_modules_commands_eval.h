#ifndef MAS_MODULES_COMMANDS_EVAL_H
#  define MAS_MODULES_COMMANDS_EVAL_H

#  include <mastar/types/mas_modules_types.h>

char *mas_evaluate_cmd( STD_CMD_ARGS );

char *mas_evaluate_command( MAS_PASS_OPTS_DECLARE const char *question );
char *mas_evaluate_transaction_command( MAS_PASS_OPTS_DECLARE mas_rcontrol_t * prcontrol, const char *question );
char *mas_evaluate_transaction_command_slash( MAS_PASS_OPTS_DECLARE mas_rcontrol_t * prcontrol, const char *uri );
char *mas_evaluate_command_slash_plus( MAS_PASS_OPTS_DECLARE const char *root, const char *uri, size_t size, size_t * ptruesize,
                                       ino_t * ptrueinode, time_t * ptruefiletime, const void *arg );

char *mas_evaluate_list_cmd( STD_CMD_ARGS );

#  undef MAS_WEAK
#endif
