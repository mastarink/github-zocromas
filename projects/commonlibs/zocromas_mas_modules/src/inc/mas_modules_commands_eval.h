#ifndef MAS_MODULES_COMMANDS_EVAL_H
#  define MAS_MODULES_COMMANDS_EVAL_H

#  include <mastar/types/mas_modules_types.h>

mas_evaluated_t *mas_evaluate_cmd( STD_CMD_ARGS );

mas_evaluated_t *mas_evaluate_command( const mas_options_t * popts, const char *question );
mas_evaluated_t *mas_evaluate_transaction_command( mas_rcontrol_t * prcontrol, const char *question );
mas_evaluated_t *mas_evaluate_transaction_command_slash( mas_rcontrol_t * prcontrol, const char *uri );
mas_evaluated_t *mas_evaluate_command_slash_plus( const mas_options_t * popts, const char *root, const char *uri, size_t size,
                                                  size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg );

mas_evaluated_t *mas_evaluate_list_cmd( STD_CMD_ARGS );

#  undef MAS_WEAK
#endif
