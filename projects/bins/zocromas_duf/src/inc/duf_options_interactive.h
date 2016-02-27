#ifndef MAS_DUF_OPTIONS_INTERACTIVE_H
# define MAS_DUF_OPTIONS_INTERACTIVE_H

# include "duf_option_types.h"                                       /* duf_longval_extended_t; duf_longval_extended_vtable_t ♠ */
# include "duf_option_stage_types.h"                                 /* duf_option_stage_t ♠ */
# include "duf_option_source_types.h"                                /* duf_option_source_t; duf_option_source_code_t ♠ */
# include "duf_fun_types.h"                                          /* duf_int_void_func_t; duf_cpchar_void_func_t */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t ♠ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>[_<entity-details>]_<action-result>
 * 
 * ** subject is entity "incarnation"
 *
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type
 *   <nothing>					=>
 * 3. entity: source
 *   3.1. entity-details: interactive
 * 4. action: parse
 * */

DR( OPTIONS, source_interactive_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive,
    duf_cpchar_void_func_t cb_prompt_interactive, duf_option_adata_t * paod, duf_option_source_code_t sourcecode )
__attribute__ ( ( warn_unused_result ) );

#endif
