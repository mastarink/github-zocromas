#ifndef MAS_DUF_CONFIG_DISABLE_TYPES_H
#define  MAS_DUF_CONFIG_DISABLE_TYPES_H


typedef enum
{
  DUF_FLAG_disable_calculate = 0,
  DUF_FLAG_disable_insert,
  DUF_FLAG_disable_update,
  DUF_FLAG_disable_fs,
  DUF_FLAG_disable_memusage,
  DUF_FLAG_disable_testflag,
  DUF_FLAG_disable_testiflag,
  DUF_FLAG_disable_testnoflag,
} duf_config_opt_disable_flags_enum_t;
typedef struct
{
  unsigned calculate:1;
  unsigned insert:1;
  unsigned update:1;
  unsigned fs:1;
  unsigned memusage:1;
  unsigned testflag:1;
  unsigned testiflag:1;
  unsigned testnoflag:1;
} duf_config_opt_disable_flags_t;

typedef union
{
  duf_config_opt_disable_flags_t flag;
  unsigned short sbit;
} duf_config_opt_disable_flags_combo_t;


#endif
