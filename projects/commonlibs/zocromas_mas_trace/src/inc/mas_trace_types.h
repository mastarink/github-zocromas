#ifndef MAS_MAST_TRACE_TYPES_H
# define MAS_MAST_TRACE_TYPES_H

# include <mastar/types/mas_base_output_types.h>                     /* mas_base_output_t */

struct mas_config_trace_s
{
  mas_base_output_t stream;
  double loadtime;
  unsigned fun_width;
  size_t num_classes;
  int *class_levels;
};
typedef struct mas_config_trace_s mas_config_trace_t;

typedef enum
{
# define ENUM_WRAPP(_n, _rf, _rf2)    MAS_TRACE_LEVEL_ ## _rf2,
# include "mas_trace_predef_enum.def"
# undef ENUM_WRAPP
} duf_trace_predef_enum_t;

#endif
