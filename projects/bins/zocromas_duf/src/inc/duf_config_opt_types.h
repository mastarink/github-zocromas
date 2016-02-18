#ifndef MAS_DUF_CONFIG_OPT_TYPES_H
# define MAS_DUF_CONFIG_OPT_TYPES_H

# include "duf_trace_types.h"
/* # include "duf_flags_types.h" */
# include "duf_config_act_types.h"
# include "duf_config_output_types.h"

typedef struct
{
  unsigned dry_run:1;
} duf_config_opt_flags_t;

typedef union
{
  duf_config_opt_flags_t flag;
  unsigned short sbit;
} duf_config_opt_flags_combo_t;


typedef struct
{
  int debug;
  unsigned nosqlite;
/* int verbose; */
  unsigned long min_line;
  unsigned long max_line;
  unsigned long lines;
} duf_config_opt_debug_t;

typedef struct
{
  duf_config_opt_flags_combo_t v;
  duf_config_opt_actions_t act;
  duf_config_opt_disable_flags_combo_t disable;
  duf_config_opt_debug_t dbg;
  duf_config_opt_trace_t trace;
  duf_config_output_t output;
} duf_config_opt_t;

#endif
