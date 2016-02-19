#ifndef MAS_DUF_TRACEN_CONFIG_H
# define MAS_DUF_TRACEN_CONFIG_H

# include "duf_config_types.h"

duf_config_opt_trace_t *duf_get_trace_config( void );

FILE *duf_trace_file( void );
int duf_output_force_color( void );
int duf_output_nocolor( void );
int duf_output_progress( void );
int duf_output_use_binformat( void );
FILE *duf_trace_file_c( const duf_config_opt_trace_t * cfg );

#endif
