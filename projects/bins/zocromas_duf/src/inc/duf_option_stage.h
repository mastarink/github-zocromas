#ifndef MAS_DUF_OPTION_STAGE_H
# define MAS_DUF_OPTION_STAGE_H

# include "duf_option_types.h"                                       /* duf_longval_extended_t ; duf_longval_extended_vtable_t */
# include "duf_option_stage_types.h"                                 /* duf_option_stage_t ; duf_limits_stage_t */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t */

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

# if 0
#  define DUF_OPTION_CHECK_STAGE(_istage, _extended, _xtable) \
	  ( _istage == -1 || ( !_extended->use_stage || ( _extended->stage.min <= _istage && _extended->stage.max >= _istage ) ) \
       || ( _xtable && ( !_xtable->use_stage || ( _xtable->stage.min <= _istage && _xtable->stage.max >= _istage ) ) ) || _extended->stage.flag )
# else
#  define DUF_OPTION_CHECK_STAGE(_cli, _istage, _extended, _xtable) (duf_optstage_check( _cli, _istage, _extended, _xtable))
# endif

const char *duf_optstage_name( const duf_config_cli_t * cli, duf_option_stage_t istage );
int duf_optstage_check(  const duf_config_cli_t * cli, duf_option_stage_t istage, const duf_longval_extended_t * extended,
                        const duf_longval_extended_vtable_t * xvtable ) __attribute__ ( ( warn_unused_result ) );
char *duf_optstages_list( const duf_config_cli_t * cli, const duf_longval_extended_t * extended, const duf_longval_extended_vtable_t * xvtable );

void duf_optstage_print( const duf_config_cli_t * cli, int use_stage, int use_stage_mask, duf_limits_stage_t stage, unsigned long stage_mask,
                         unsigned eol );

#endif
