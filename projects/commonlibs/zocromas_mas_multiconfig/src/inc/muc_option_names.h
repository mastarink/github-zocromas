#ifndef MAS_MUC_OPTION_NAMES_H
# define MAS_MUC_OPTION_NAMES_H

# include "muc_option_types.h"                                       /* muc_longval_extended_t; muc_longval_extended_vtable_t ♠ */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t ♠ */

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

const char *muc_coption_cnames_tmp( muc_config_cli_t * cli, int index, muc_option_gen_code_t codeval, const char *delim );

char *muc_coption_names(muc_config_cli_t * cli, muc_option_gen_code_t codeval );
char *muc_coption_names_d( muc_config_cli_t * cli, muc_option_gen_code_t codeval, const char *delim );


#endif
