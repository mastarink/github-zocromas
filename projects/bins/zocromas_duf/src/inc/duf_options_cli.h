#ifndef MAS_DUF_OPTIONS_CLI_H
#  define MAS_DUF_OPTIONS_CLI_H

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>[_<entity-details>]_<action-result>
 * 
 * ** subject is entity "incarnation"
 *
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type
 *   <nothing>					=>
 * 3. entity: source
 *   3.1. entity-details: cli
 * 4. action: parse
 * */

DR( OPTIONS, source_cli_parse, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );



#endif
