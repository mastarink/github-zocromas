#ifndef MAS_DUF_MAINDB_OPTIONS_H
#  define MAS_DUF_MAINDB_OPTIONS_H

DR( OPTIONS, treat_all_optstages, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
DR( OPTIONS, treat_option_stage, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );


#endif
