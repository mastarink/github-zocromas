#ifndef MAS_DUF_OPTION_NAMES_H
# define MAS_DUF_OPTION_NAMES_H

# include "duf_option_types.h"                                       /* duf_longval_extended_t; duf_longval_extended_vtable_t ♠ */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t ♠ */

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>[_<entity-details>]_<action-result>
 * 
 * ** subject is entity "incarnation"
 *
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type
 *   <nothing>					=>
 * 3. entity: optstage
 *   3.1. entity-details:
 * 4. result/action: 
 * */

const char *duf_coption_cnames_tmp( duf_config_cli_t * cli, int index, duf_option_gen_code_t codeval, const char *delim );

char *duf_coption_names(duf_config_cli_t * cli, duf_option_gen_code_t codeval );
char *duf_coption_names_d( duf_config_cli_t * cli, duf_option_gen_code_t codeval, const char *delim );


#endif
