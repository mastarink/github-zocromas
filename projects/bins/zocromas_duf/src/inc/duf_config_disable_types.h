#ifndef MAS_DUF_CONFIG_DISABLE_TYPES_H
# define  MAS_DUF_CONFIG_DISABLE_TYPES_H

typedef enum
{
  DUF_FLAG_disable_NONE,
# define ENUM_WRAP(_x, _xn) DUF_FLAG_disable_ ## _x,
# include  "duf_config_disable_enum.def"
# undef ENUM_WRAP
} duf_config_opt_disable_flags_enum_t;
typedef struct
{
# define ENUM_WRAP(_x, _xn) unsigned _x:1;
# include  "duf_config_disable_enum.def"
# undef ENUM_WRAP
} duf_config_opt_disable_flags_t;

typedef union
{
  duf_config_opt_disable_flags_t flag;
  unsigned short sbit;
} duf_config_opt_disable_flags_combo_t;

#endif
