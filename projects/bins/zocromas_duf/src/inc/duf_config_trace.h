#ifndef MAS_DUF_CONFIG_TRACE_H
# define MAS_DUF_CONFIG_TRACE_H

# include "duf_config_types.h"

duf_config_opt_trace_t *duf_get_trace_config( void );

int duf_verbose( void );
FILE *duf_trace_file( void );
int duf_output_force_color( void );
int duf_output_nocolor( void );
int duf_output_progress( void );
int duf_output_use_binformat( void );

#endif
