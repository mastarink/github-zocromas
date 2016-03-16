#ifndef MAS_DUF_CONFIG_PUZ_TYPES_H
# define MAS_DUF_CONFIG_PUZ_TYPES_H

typedef enum
{
  DUF_FLAG_puz_NONE,
# if 0
  DUF_FLAG_puz_recursive = 1 /* !! */ ,
  DUF_FLAG_puz_linear,
  DUF_FLAG_puz_testflag,
  DUF_FLAG_puz_testiflag,
  DUF_FLAG_puz_testnoflag,
# else
#  define ENUM_WRAP(_x, _xn) DUF_FLAG_puz_ ## _x,
#  include  "duf_config_puz_enum.def"
#  undef ENUM_WRAP
# endif
} duf_config_puz_flags_enum_t;
typedef struct
{
# if 0
  unsigned recursive:1;
  unsigned linear:1;
  unsigned testflag:1;
  unsigned testiflag:1;
  unsigned testnoflag:1;
# else
#  define ENUM_WRAP(_x, _xn) unsigned _x:1;
#  include  "duf_config_puz_enum.def"
#  undef ENUM_WRAP
# endif
} duf_config_puz_flags_t;

typedef union
{
  duf_config_puz_flags_t flag;
  unsigned short sbit;
} duf_config_puz_flags_combo_t;

#endif
