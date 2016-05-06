#ifndef MAS_MUC_OPTION_SOURCE_H
# define MAS_MUC_OPTION_SOURCE_H

# include "muc_option_source_types.h"                                /* muc_option_source_t; muc_option_source_code_t ♠ */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t ♠ */

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>[_<entity-details>]_<action-result>
 * 
 * ** subject is entity "incarnation"
 *
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type
 *   <nothing>					=>
 * 3. entity: optsource
 *   3.1. entity-details:
 * 4. result: name
 * */

const char *muc_optsourcecode_name( const muc_config_cli_t * cli, muc_option_source_code_t sourcecode );
const char *muc_optsource_name( const muc_config_cli_t * cli, muc_option_source_t source );
const char *muc_optsource_label( const muc_config_cli_t * cli, muc_option_source_t source );
int muc_optsource_labelled( const muc_config_cli_t * cli, muc_option_source_t source );

#endif
