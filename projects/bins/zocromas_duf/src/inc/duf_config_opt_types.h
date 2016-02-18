#ifndef MAS_DUF_CONFIG_OPT_TYPES_H
# define MAS_DUF_CONFIG_OPT_TYPES_H

# include "duf_config_opt_debug_types.h"
# include "duf_config_opt_trace_types.h"
/* # include "duf_flags_types.h" */
# include "duf_config_flow_types.h"
# include "duf_config_act_types.h"
# include "duf_config_output_types.h"




typedef struct
{
  duf_config_opt_disable_flags_combo_t disable;
  duf_config_flow_t flow;
  duf_config_opt_actions_t act;
  duf_config_opt_debug_t dbg;
  duf_config_opt_trace_t trace;
  duf_config_output_t output;
} duf_config_opt_t;

#endif
