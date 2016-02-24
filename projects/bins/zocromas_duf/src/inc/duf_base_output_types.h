#ifndef MAS_DUF_BASE_OUTPUT_TYPES_H
# define MAS_DUF_BASE_OUTPUT_TYPES_H

#include <stdio.h> /* FILE */

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


#endif
