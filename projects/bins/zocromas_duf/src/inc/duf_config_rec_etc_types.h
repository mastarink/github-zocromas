#ifndef MAS_DUF_CONFIG_REC_ETC_TYPES_H
#define MAS_DUF_CONFIG_REC_ETC_TYPES_H

typedef enum
{
  DUF_FLAG_puz_NONE,
  DUF_FLAG_puz_recursive = 1 /* !! */,
  DUF_FLAG_puz_linear,
  DUF_FLAG_puz_testflag,
  DUF_FLAG_puz_testiflag,
  DUF_FLAG_puz_testnoflag,
} duf_config_puz_flags_enum_t;
typedef struct
{
  unsigned recursive:1;
  unsigned linear:1;
  unsigned testflag:1;
  unsigned testiflag:1;
  unsigned testnoflag:1;
} duf_config_puz_flags_t;

typedef union
{
  duf_config_puz_flags_t flag;
  unsigned short sbit;
} duf_config_puz_flags_combo_t;

#endif
