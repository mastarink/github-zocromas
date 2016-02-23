#ifndef MAS_DUF_CONFIG_TRACE_TYPES_H
# define MAS_DUF_CONFIG_TRACE_TYPES_H

# include "duf_config_output_types.h"
typedef enum
{ 
#  define ENUM_WRAPP(_n, _rf, _rf2)    DUF_TRACE_LEVEL_ ## _rf2,
#  include "duf_options_enum_trace.def"
#  undef ENUM_WRAPP
} duf_config_trace_enum_t;
typedef struct
{
  duf_config_output_t output;
  double loadtime;
  unsigned fun_width;
  /* int temp; */
  /* int collect; */
  /* int any; */
    /* (**) int dirs; */
  /* int filedata; */
  /* int filenames; */
/* int fill; */
  /* int nonew; */
  /* int statistics; */
    /**/
#if 0
#  define ENUM_WRAPP(_n, _rf, _rf2)      int _rf2;
#  include "duf_options_enum_trace.def"
#  undef ENUM_WRAPP
#endif
  size_t num_classes;
  int *class_levels;
} duf_config_trace_t;

#endif

/*
vi: ft=c
*/
