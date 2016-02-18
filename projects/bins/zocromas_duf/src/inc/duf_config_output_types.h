#ifndef MAS_DUF_CONFIG_OUTPUT_TYPES_H
# define MAS_DUF_CONFIG_OUTPUT_TYPES_H

# include "duf_format_types.h"
typedef struct
{
  unsigned overwrite:1; /* => output options */
  unsigned append:1; /* => output options */
  unsigned nocolor:1; /* => output options */
  unsigned force_color:1; /* => output options */
  unsigned progress:1; /* => output options */
  unsigned use_binformat:1; /* => output options */
} duf_config_output_flags_t;

typedef union
{
  duf_config_output_flags_t flag;
  unsigned bit;
} duf_config_output_flags_combo_t;

typedef struct
{
  duf_config_output_flags_combo_t v;
  unsigned level;
  unsigned handleid;
  unsigned max_width;
  duf_asformats_t as_formats;
  duf_sformats_t sformat;
  char *file;
  FILE *out;
/* char *history_filename; */
  char *header_tty;
/* duf_bformat_combo_t bformat; */
} duf_config_output_t;

#endif

/*
vi: ft=c
*/
