#ifndef MAS_MODULES_TYPES_H
#  define MAS_MODULES_TYPES_H

#  define MAS_INVALID_ANSWER ((void *)-1L)
#  define MAS_VALID_ANSWER(a) (a && a->data!=MAS_INVALID_ANSWER)

#  define STD_CMD_ARGS_BASE const char *question, const char *args, unsigned level
/* #  define STD_CMD_ARGS mas_channel_t * pchannel, STD_CMD_ARGS_BASE */
#  define STD_CMD_ARGS const mas_options_t * pqopts, unsigned nesting, mas_cmd_t * this_table, mas_cmd_t * this_command, mas_rcontrol_t * prcontrol, STD_CMD_ARGS_BASE

#  define STD_CMD_PASS_BASE question, args, level
#  define STD_CMD_PASS pqopts, nesting, this_table, this_command, prcontrol, STD_CMD_PASS_BASE

#  include <mastar/types/mas_transaction_control_types.h>

#  include <mastar/types/mas_unidata_types.h>
#  include <mastar/types/mas_evaluate_types.h>

typedef struct mas_cmd_s mas_cmd_t;
typedef mas_evaluated_t *( *mas_cmd_fun_t ) ( STD_CMD_ARGS );

struct mas_cmd_s
{
  unsigned id;
  const char *name;
  /* char *( *function ) (  mas_channel_t * chn, char *args, mas_header_t * header ); */
  mas_cmd_fun_t function;
  const char *libname;
  unsigned unknown:1;
  unsigned only_level;
  unsigned min_args;
  unsigned max_args;
  mas_cmd_t *subtable;
  mas_cmdtype_t evaluate_type;
  void *vid;
};

#  undef MAS_WEAK
#endif
