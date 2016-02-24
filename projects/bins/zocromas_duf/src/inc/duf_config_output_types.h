#ifndef MAS_DUF_CONFIG_OUTPUT_TYPES_H
# define MAS_DUF_CONFIG_OUTPUT_TYPES_H

# include <stdio.h>                                                  /* FILE */
# include "duf_format_types.h"                                       /* duf_asformats_t; duf_sformats_t */
# include "duf_base_output_types.h"                                  /* mas_base_output_t */

typedef struct
{
  unsigned handleid;
  unsigned max_width;
  duf_asformats_t as_formats;
  duf_sformats_t sformat;
  mas_base_output_t stream;
/* char *history_filename; */
/* duf_bformat_combo_t bformat; */
} duf_config_output_t;

#endif

/*
vi: ft=c
*/
