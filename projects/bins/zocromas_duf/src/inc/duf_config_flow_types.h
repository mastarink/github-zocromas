#ifndef MAS_DUF_CONFIG_FLOW_TYPES_H
# define MAS_DUF_CONFIG_FLOW_TYPES_H

typedef struct
{
  unsigned dry_run:1;
} duf_config_flow_flags_t;

typedef union
{
  duf_config_flow_flags_t flag;
  unsigned short sbit;
} duf_config_flow_flags_combo_t;

typedef struct
{
  duf_config_flow_flags_combo_t v;
  int verbose;
} duf_config_flow_t;

#endif
