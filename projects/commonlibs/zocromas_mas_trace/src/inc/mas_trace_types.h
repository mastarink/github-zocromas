#ifndef MAS_MAST_TRACE_TYPES_H
# define MAS_MAST_TRACE_TYPES_H

# include <mastar/types/mas_base_output_types.h>                     /* mas_base_output_t */

typedef struct
{
  mas_base_output_t stream;
  double loadtime;
  unsigned fun_width;
  size_t num_classes;
  int *class_levels;
} mas_config_trace_t;

#endif
