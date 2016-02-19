#ifndef MAS_DUF_OPTION_STAGE_TYPES_H
# define MAS_DUF_OPTION_STAGE_TYPES_H

typedef enum
{
  DUF_OPTION_STAGE_NONE = 0,
  DUF_OPTION_STAGE_MIN = 1,
  DUF_OPTION_STAGE_DEBUG = DUF_OPTION_STAGE_MIN,
  DUF_OPTION_STAGE_BOOT,
  DUF_OPTION_STAGE_PRESETUP,
  DUF_OPTION_STAGE_PRESET = DUF_OPTION_STAGE_PRESETUP,
  DUF_OPTION_STAGE_SETUP,
  DUF_OPTION_STAGE_INIT,
  DUF_OPTION_STAGE_FIRST,
  DUF_OPTION_STAGE_LOOP,
  DUF_OPTION_STAGE_LOOPE = DUF_OPTION_STAGE_LOOP + 1000,
  DUF_OPTION_STAGE_INTERACTIVE,
  DUF_OPTION_STAGE_INTERAC = DUF_OPTION_STAGE_INTERACTIVE,
  DUF_OPTION_STAGE_IA = DUF_OPTION_STAGE_INTERACTIVE,
  DUF_OPTION_STAGE_MAX = DUF_OPTION_STAGE_INTERACTIVE,
  DUF_OPTION_STAGE_ANY = 8888888,
  DUF_OPTION_STAGE_ALL = 9999999,
} duf_option_stage_t;

typedef struct
{
/* unsigned flag:1; ??? */
  duf_option_stage_t min;
  duf_option_stage_t max;
} duf_limits_stage_t;

typedef struct duf_extended_stageopts_s
{
  unsigned use_stage:1;
  unsigned use_stage_mask:1;
  duf_limits_stage_t stage;
  unsigned long stage_mask;
} duf_extended_stageopts_t;



#endif
