#ifndef MAS_DUF_OPTION_SOURCE_H
# define MAS_DUF_OPTION_SOURCE_H

# include "duf_option_source_types.h"                                /* duf_option_source_t; duf_option_source_code_t ♠ */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t ♠ */

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

const char *duf_optsourcecode_name( duf_config_cli_t * cli, duf_option_source_code_t sourcecode );
const char *duf_optsource_name( duf_config_cli_t * cli, duf_option_source_t source );
const char *duf_optsource_label( duf_config_cli_t * cli, duf_option_source_t source );
int duf_optsource_labelled( duf_config_cli_t * cli, duf_option_source_t source );

#endif
