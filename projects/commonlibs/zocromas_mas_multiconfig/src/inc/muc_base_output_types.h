#ifndef MAS_MUC_BASE_OUTPUT_TYPES_H
# define MAS_MUC_BASE_OUTPUT_TYPES_H

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
} muc_base_output_flags_t;

typedef union
{
  muc_base_output_flags_t flag;
  unsigned bit;
} muc_base_output_flags_combo_t;

typedef struct
{
  muc_base_output_flags_combo_t v;
  unsigned level;
  char *file;
  FILE *out;
  char *header_tty;
} mas_base_output_t;
# else
#  include <mastar/types/mas_base_output_types.h>                    /* mas_base_output_flags_t */
typedef enum
{
  MUC_FLAG_output_NONE = MAS_FLAG_output_NONE,
  MUC_FLAG_output_overwrite = MAS_FLAG_output_overwrite,
  MUC_FLAG_output_append = MAS_FLAG_output_append,
  MUC_FLAG_output_nocolor = MAS_FLAG_output_nocolor,
  MUC_FLAG_output_force_color = MAS_FLAG_output_force_color,
  MUC_FLAG_output_progress = MAS_FLAG_output_progress,
} muc_base_output_flags_enum_t;
# endif

#endif
