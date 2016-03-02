#ifndef MAS_MUC_OPTION_STAGE_H
# define MAS_MUC_OPTION_STAGE_H

# include "muc_option_types.h"                                       /* muc_longval_extended_t ; muc_longval_extended_vtable_t */
# include "muc_option_stage_types.h"                                 /* muc_option_stage_t ; muc_limits_stage_t */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t */

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
#  define MUC_OPTION_CHECK_STAGE(_istage, _extended, _xtable) \
	  ( _istage == -1 || ( !_extended->use_stage || ( _extended->stage.min <= _istage && _extended->stage.max >= _istage ) ) \
       || ( _xtable && ( !_xtable->use_stage || ( _xtable->stage.min <= _istage && _xtable->stage.max >= _istage ) ) ) || _extended->stage.flag )
# else
#  define MUC_OPTION_CHECK_STAGE(_cli, _istage, _extended, _xtable) (muc_optstage_check( _cli, _istage, _extended, _xtable))
# endif

const char *muc_optstage_name( const muc_config_cli_t * cli, muc_option_stage_t istage );
int muc_optstage_check(  const muc_config_cli_t * cli, muc_option_stage_t istage, const muc_longval_extended_t * extended,
                        const muc_longval_extended_vtable_t * xvtable ) __attribute__ ( ( warn_unused_result ) );
char *muc_optstages_list( const muc_config_cli_t * cli, const muc_longval_extended_t * extended, const muc_longval_extended_vtable_t * xvtable );

void muc_optstage_print( const muc_config_cli_t * cli, int use_stage, int use_stage_mask, muc_limits_stage_t stage, unsigned long stage_mask,
                         unsigned eol );

#endif
