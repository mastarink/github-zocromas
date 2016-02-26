#ifndef MAS_DUF_BASE_OUTPUT_TYPES_H
# define MAS_DUF_BASE_OUTPUT_TYPES_H

# include <stdio.h>                                                  /* FILE */

# if 0
typedef struct
{
  unsigned overwrite:1;                                              /* => output options */
  unsigned append:1;                                                 /* => output options */
  unsigned nocolor:1;                                                /* => output options */
  unsigned force_color:1;                                            /* => output options */
  unsigned progress:1;                                               /* => output options */
/* unsigned use_binformat:1;                                          (* => output options *) */
} duf_base_output_flags_t;

typedef union
{
  duf_base_output_flags_t flag;
  unsigned bit;
} duf_base_output_flags_combo_t;

typedef struct
{
  duf_base_output_flags_combo_t v;
  unsigned level;
  char *file;
  FILE *out;
  char *header_tty;
} mas_base_output_t;
# else
#  include <mastar/types/mas_base_output_types.h>                    /* mas_base_output_flags_t */
typedef enum
{
  DUF_FLAG_output_NONE = MAS_FLAG_output_NONE,
  DUF_FLAG_output_overwrite = MAS_FLAG_output_overwrite,
  DUF_FLAG_output_append = MAS_FLAG_output_append,
  DUF_FLAG_output_nocolor = MAS_FLAG_output_nocolor,
  DUF_FLAG_output_force_color = MAS_FLAG_output_force_color,
  DUF_FLAG_output_progress = MAS_FLAG_output_progress,
} duf_base_output_flags_enum_t;
# endif

#endif
