#ifndef MAS_MAST_TRACE_CREDEL_H
# define MAS_MAST_TRACE_CREDEL_H

# include "mas_trace_types.h"

mas_config_trace_t *mas_config_trace_create( size_t level_max );
void mas_config_trace_delete( mas_config_trace_t * tcfg );

#endif
