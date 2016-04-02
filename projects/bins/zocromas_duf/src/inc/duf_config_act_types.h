#ifndef MAS_DUF_CONFIG_ACT_TYPES_H
# define MAS_DUF_CONFIG_ACT_TYPES_H

typedef enum
{
  DUF_FLAG_act_NONE = 0,
# define ENUM_WRAP(_x, _xn) DUF_FLAG_act_ ## _x,
# include  "duf_config_act_enum.def"
# undef ENUM_WRAP

} duf_config_act_flags_enum_t;

typedef struct
{
# define ENUM_WRAP(_x, _xn) unsigned _x:1;
# include  "duf_config_act_enum.def"
# undef ENUM_WRAP

} duf_config_act_flags_t;

typedef union
{
  duf_config_act_flags_t flag;
  unsigned bit;
} duf_config_act_flags_combo_t;

typedef struct
{
  duf_config_act_flags_combo_t v;
  unsigned sample;
  unsigned sampupd;
} duf_config_opt_actions_t;

#endif
