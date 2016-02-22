#ifndef MAS_DUF_CONFIG_OPT_TYPES_H
# define MAS_DUF_CONFIG_OPT_TYPES_H

# include "duf_config_opt_debug_types.h"                             /* duf_config_opt_debug_t */
# include "duf_config_trace_types.h"                                 /* duf_config_trace_t */
# include "duf_config_flow_types.h"                                  /* duf_config_flow_t */
# include "duf_config_act_types.h"                                   /* duf_config_opt_actions_t */
# include "duf_config_output_types.h"                                /* duf_config_output_t */
# include "duf_config_disable_types.h"                               /* duf_config_opt_disable_flags_combo_t; */

typedef struct
{
  duf_config_opt_disable_flags_combo_t disable;
  duf_config_flow_t flow;
  duf_config_opt_actions_t act;
  duf_config_opt_debug_t dbg;
  duf_config_trace_t *ptracecfg;
  duf_config_output_t output;
} duf_config_opt_t;

#endif
