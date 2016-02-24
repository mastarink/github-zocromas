#ifndef MAS_DUF_CONFIG_TRACE_TYPES_H
# define MAS_DUF_CONFIG_TRACE_TYPES_H
/* TODO 20160223.182804 move to trace-lib; rename accordingly; */

#include <mastar/trace/mas_trace_types.h>

# include "duf_base_output_types.h"                                  /* mas_base_output_t */
# include "duf_config_output_types.h"                                /* duf_config_output_t */

#if 0
typedef struct
{
/* duf_config_output_t output; */
  mas_base_output_t stream;
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
# if 0
#  define ENUM_WRAPP(_n, _rf, _rf2)      int _rf2;
#  include "duf_options_enum_trace.def"
#  undef ENUM_WRAPP
# endif
    size_t num_classes;
  int *class_levels;
} duf_config_trace_t;
#else
/* typedef mas_config_trace_t duf_config_trace_t; */
#endif

#endif

/*
vi: ft=c
*/
