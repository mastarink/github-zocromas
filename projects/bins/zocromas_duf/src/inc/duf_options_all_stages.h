#ifndef MAS_DUF_OPTIONS_ALL_STAGES_H
# define MAS_DUF_OPTIONS_ALL_STAGES_H

# include "duf_option_stage_types.h"                                 /* duf_option_stage_t */
# include "duf_fun_types.h"                                          /* duf_int_void_func_t; duf_cpchar_void_func_t; duf_errc_cs_func_t */

DR( OPTIONS, treat_all_optstages, duf_errc_cs_func_t cb_init_all_optstages, duf_errc_cs_func_t cb_init_loop_optstage,
    duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
DR( TOP, treat_option_stage_ne, duf_option_stage_t istage, duf_errc_cs_func_t cb_init_loop_optstage, duf_int_void_func_t cb_do_interactive,
    duf_cpchar_void_func_t cb_prompt_interactive );
DR( TOP, treat_option_stage, duf_option_stage_t istage, duf_errc_cs_func_t cb_init_loop_optstage, duf_int_void_func_t cb_do_interactive,
    duf_cpchar_void_func_t cb_prompt_interactive );

#endif
