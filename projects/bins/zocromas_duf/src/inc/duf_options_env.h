#ifndef MAS_DUF_OPTIONS_ENV_H
#  define MAS_DUF_OPTIONS_ENV_H

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

int duf_source_env_parse( duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive,
                          duf_option_adata_t * paod, duf_option_source_code_t sourcecode );


#endif
