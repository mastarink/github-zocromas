#ifndef MAS_MUC_OPTIONS_ENV_H
# define MAS_MUC_OPTIONS_ENV_H

# include "muc_option_types.h"                                       /* muc_longval_extended_t; muc_longval_extended_vtable_t ♠ */
# include "muc_option_stage_types.h"                                 /* muc_option_stage_t ♠ */
# include "muc_option_source_types.h"                                /* muc_option_source_t; muc_option_source_code_t ♠ */
# include "muc_fun_types.h"                                          /* muc_int_void_func_t; muc_cpchar_void_func_t */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t ♠ */

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>[_<entity-details>]_<action-result>
 * 
 * ** subject is entity "incarnation"
 *
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type
 *   <nothing>					=>
 * 3. entity: source
 *   3.1. entity-details: env
 * 4. action: parse
 * */

muc_DR( OPTIONS, source_env_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
    muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod, muc_option_source_code_t sourcecode );

#endif
