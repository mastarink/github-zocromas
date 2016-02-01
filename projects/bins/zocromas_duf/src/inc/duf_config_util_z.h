#ifndef MAS_DUF_CONFIG_UTIL_Z_H
#  define MAS_DUF_CONFIG_UTIL_Z_H

#  include "duf_config_types.h" /* duf_config_t */

duf_config_t *duf_get_config( void );


FILE *duf_output_file_c( const duf_config_t * cfg );

FILE *duf_trace_file_c( const duf_config_t * cfg );

int duf_trace_force_color_c( const duf_config_t * cfg );
int duf_trace_nocolor_c( const duf_config_t * cfg );



#endif

