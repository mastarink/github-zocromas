#ifndef MAS_DUF_OPTIONS_FILE_H
#  define MAS_DUF_OPTIONS_FILE_H

#  include <stdio.h>

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

int duf_source_stdin_parse( duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
int duf_source_indirect_parse( duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
int duf_source_incfg_last_parse( duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
int duf_source_incfg_stg_parse( duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );
int duf_source_incfg_parse( duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive, duf_cpchar_void_func_t cb_prompt_interactive );


const char *duf_options_infilepath( void );
int duf_infile_options_at_filepath( duf_option_stage_t istage, const char *filepath );


#endif
