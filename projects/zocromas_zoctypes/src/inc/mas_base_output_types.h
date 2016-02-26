#ifndef MAS_BASE_OUTPUT_TYPES_H
# define MAS_BASE_OUTPUT_TYPES_H

# include <stdio.h>                                                  /* FILE */

typedef enum
{
  MAS_FLAG_output_NONE,
  MAS_FLAG_output_overwrite = 1 /* !! */ ,
  MAS_FLAG_output_append,
  MAS_FLAG_output_nocolor,
  MAS_FLAG_output_force_color,
  MAS_FLAG_output_progress,
} mas_base_output_flags_enum_t;

typedef struct
{
  unsigned overwrite:1;                                              /* => output options */
  unsigned append:1;                                                 /* => output options */
  unsigned nocolor:1;                                                /* => output options */
  unsigned force_color:1;                                            /* => output options */
  unsigned progress:1;                                               /* => output options */
} mas_base_output_flags_t;

typedef union
{
  mas_base_output_flags_t flag;
  unsigned bit;
} mas_base_output_flags_combo_t;

typedef struct
{
  mas_base_output_flags_combo_t v;
  unsigned level;
  char *file;
  FILE *out;
  char *header_tty;
} mas_base_output_t;

#endif
