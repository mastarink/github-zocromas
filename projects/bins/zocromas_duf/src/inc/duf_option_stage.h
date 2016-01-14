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

#  if 0
#    define DUF_OPTION_CHECK_STAGE(_istage, _extended, _xtable) \
	  ( _istage == -1 || ( !_extended->use_stage || ( _extended->stage.min <= _istage && _extended->stage.max >= _istage ) ) \
       || ( _xtable && ( !_xtable->use_stage || ( _xtable->stage.min <= _istage && _xtable->stage.max >= _istage ) ) ) || _extended->stage.flag )
#  else
#    define DUF_OPTION_CHECK_STAGE(_istage, _extended, _xtable) (duf_optstage_check( _istage, _extended, _xtable))
#  endif


const char *duf_optstage_name( duf_option_stage_t istage );
int duf_optstage_check( duf_option_stage_t istage, const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
      __attribute__ ( ( warn_unused_result ) );
char *duf_optstages_list( const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable );

void duf_optstage_print( int use_stage, int use_stage_mask, duf_limits_stage_t stage, unsigned long stage_mask, unsigned eol );

#endif
