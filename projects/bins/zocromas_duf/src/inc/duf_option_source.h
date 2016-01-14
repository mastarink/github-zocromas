#ifndef MAS_DUF_OPTION_SOURCE_H
#  define MAS_DUF_OPTION_SOURCE_H

#  include "duf_option_types.h"

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

const char *duf_optsource_name( duf_option_source_t source );
const char *duf_optsource_label( duf_option_source_t source );
int duf_optsource_labelled( duf_option_source_t source );

#endif
