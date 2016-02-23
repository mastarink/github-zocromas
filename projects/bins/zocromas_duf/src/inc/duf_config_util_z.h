#ifndef MAS_DUF_CONFIG_UTIL_Z_H
# define MAS_DUF_CONFIG_UTIL_Z_H

# include "duf_config_types.h"/* duf_config_t */

FILE *duf_output_file_c( const duf_config_t * cfg );
int duf_output_force_color_c( const duf_config_t * cfg );
int duf_output_nocolor_c( const duf_config_t * cfg );
int duf_output_progress_c( const duf_config_t * cfg );
/* int duf_output_use_binformat_c( const duf_config_t * cfg ); */


#endif
