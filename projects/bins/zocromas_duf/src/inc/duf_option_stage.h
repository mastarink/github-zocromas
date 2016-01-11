#ifndef MAS_DUF_OPTION_STAGE_H
#  define MAS_DUF_OPTION_STAGE_H

#  include "duf_option_types.h"

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
 * 4. result: name
 * */

const char *duf_optstage_name( duf_option_stage_t istage );
int duf_optstage_check( duf_option_stage_t istage, const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
      __attribute__ ( ( warn_unused_result ) );
char *duf_stages_list( const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable );

#endif
