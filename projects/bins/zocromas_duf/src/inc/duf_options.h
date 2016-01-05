#ifndef MAS_DUF_OPTIONS_H
#  define MAS_DUF_OPTIONS_H

#  include "duf_option_types.h"


int duf_all_options(  /* int argc, char *argv[], */ duf_option_stage_t istage, /* int is_interactive, */ duf_int_void_func_t cb_do_interactive,
                     duf_cpchar_void_func_t cb_prompt_interactive );

#endif
