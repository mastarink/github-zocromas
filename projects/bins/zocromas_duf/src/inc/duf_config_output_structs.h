#ifndef MAS_DUF_CONFIG_OUTPUT_STRUCTS_H
# define MAS_DUF_CONFIG_OUTPUT_STRUCTS_H

# include <stdio.h>                                                  /* FILE */
# include "duf_format_structs.h"
# include "duf_base_output_types.h"                                  /* mas_base_output_t */
# include "duf_config_output_types.h" 

struct duf_config_output_s
{
  unsigned handleid;
  unsigned max_width;
  union
  {
    duf_asformats_t as_formats;
    duf_filedirformat_t formats[sizeof( duf_asformats_t ) / sizeof( duf_filedirformat_t )];
  };
  duf_sformats_t sformat;
  mas_base_output_t stream;
/* char *history_filename; */
/* duf_bformat_combo_t bformat; */
};

#endif

/*
vi: ft=c
*/
