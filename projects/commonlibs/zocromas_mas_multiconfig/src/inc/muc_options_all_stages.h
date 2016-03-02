#ifndef MAS_MUC_OPTIONS_ALL_STAGES_H
# define MAS_MUC_OPTIONS_ALL_STAGES_H

# include "muc_option_stage_types.h"  /* muc_option_stage_t ♠*/
# include "muc_fun_types.h"                                          /* muc_int_void_func_t; muc_cpchar_void_func_t; muc_errc_cs_func_t */
# include "muc_config_cli_types.h"  /* muc_config_cli_t ♠*/
# include "muc_se.h"                                                 /* muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

muc_DR( OPTIONS, treat_all_optstages,muc_config_cli_t *cli, muc_errc_cs_func_t cb_init_all_optstages, muc_errc_cs_func_t cb_init_loop_optstage,
    muc_int_void_func_t cb_do_interactive, muc_cpchar_void_func_t cb_prompt_interactive );
muc_DR( TOP, treat_option_stage_ne, muc_config_cli_t *cli,muc_option_stage_t istage, muc_errc_cs_func_t cb_init_loop_optstage, muc_int_void_func_t cb_do_interactive,
    muc_cpchar_void_func_t cb_prompt_interactive );
muc_DR( TOP, treat_option_stage, muc_config_cli_t *cli, muc_option_stage_t istage, muc_errc_cs_func_t cb_init_loop_optstage, muc_int_void_func_t cb_do_interactive,
    muc_cpchar_void_func_t cb_prompt_interactive );

#endif
