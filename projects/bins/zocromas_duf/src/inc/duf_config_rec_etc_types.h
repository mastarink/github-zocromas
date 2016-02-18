#ifndef MAS_DUF_CONFIG_REC_ETC_TYPES_H
#define MAS_DUF_CONFIG_REC_ETC_TYPES_H

typedef enum
{
  DUF_FLAG_rec_etc_recursive = 0,
  DUF_FLAG_rec_etc_linear,
  DUF_FLAG_rec_etc_testflag,
  DUF_FLAG_rec_etc_testiflag,
  DUF_FLAG_rec_etc_testnoflag,
} duf_rec_etc_flags_enum_t;
typedef struct
{
  unsigned recursive:1;
  unsigned linear:1;
  unsigned testflag:1;
  unsigned testiflag:1;
  unsigned testnoflag:1;
} duf_rec_etc_flags_t;

typedef union
{
  duf_rec_etc_flags_t flag;
  unsigned short sbit;
} duf_rec_etc_flags_combo_t;

#endif
