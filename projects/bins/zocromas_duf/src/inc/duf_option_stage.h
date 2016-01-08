#ifndef MAS_DUF_OPTION_STAGE_H
#  define MAS_DUF_OPTION_STAGE_H

#  include "duf_option_types.h"

const char *duf_optstage_name( duf_option_stage_t istage );
int duf_check_stage( duf_option_stage_t istage, const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable )
      __attribute__ ( ( warn_unused_result ) );
char *duf_stages_list( const duf_longval_extended_t * extended, const duf_longval_extended_table_t * xtable );

#endif
