#ifndef  MAS_DUF_CONFIG_TRACE_H
# define MAS_DUF_CONFIG_TRACE_H

# include <stddef.h>                                                 /* FILE */
# include <mastar/trace/mas_trace_types.h>

/* # include "duf_config_trace_types.h"                                 (* duf_config_trace_t *) */

/* TODO 20160223.182516 move to trace-lib; rename accordingly */
FILE *duf_trace_file_c( const mas_config_trace_t * cfg );
int duf_trace_output_force_color_c( const mas_config_trace_t * tcfg );
int duf_trace_output_no_color_c( const mas_config_trace_t * tcfg );

#endif
