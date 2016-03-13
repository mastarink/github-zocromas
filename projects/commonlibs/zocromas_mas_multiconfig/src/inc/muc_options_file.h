#ifndef MAS_MUC_OPTIONS_FILE_H
# define MAS_MUC_OPTIONS_FILE_H

# include "muc_option_types.h"                                       /* muc_longval_extended_t; muc_longval_extended_vtable_t ♠ */
# include "muc_option_stage_types.h"                                 /* muc_option_stage_t ♠ */
# include "muc_option_source_types.h"                                /* muc_option_source_t; muc_option_source_code_t ♠ */
# include "muc_fun_types.h"                                          /* muc_void_voidp_func_t etc. etc. ... ♠ */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t ♠ */
# include "muc_se.h"                                                 /* muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>[_<entity-details>]_<action-result>
 * 
 * ** subject is entity "incarnation"
 *
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type
 *   <nothing>					=>
 * 3. entity: source
 *   3.1. entity-details: stdin, indirect, incfg_last, incfg_stg, incfg
 * 4. action: parse
 * */

muc_DR( OPTIONS, source_stdin_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
    muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod, muc_option_source_code_t sourcecode );
muc_DR( OPTIONS, source_indirect_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
    muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod, muc_option_source_code_t sourcecode );
muc_DR( OPTIONS, source_incfg_last_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
    muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod, muc_option_source_code_t sourcecode );
muc_DR( OPTIONS, source_incfg_stg_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
    muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod, muc_option_source_code_t sourcecode );
muc_DR( OPTIONS, source_incfg_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
    muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod, muc_option_source_code_t sourcecode );

const char *muc_options_infilepath( muc_config_cli_t * cli );
muc_DR( OPTIONS, infile_options_at_filepath, muc_config_cli_t * cli, muc_option_stage_t istage, const char *filepath, muc_option_adata_t * paod );

#endif