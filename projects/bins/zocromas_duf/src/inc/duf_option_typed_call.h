#ifndef MAS_DUF_OPTION_TYPED_CALL_H
# define MAS_DUF_OPTION_TYPED_CALL_H

# include "duf_option_types.h"                                       /* duf_longval_extended_t  */
# include "duf_option_stage_types.h"                                 /* duf_option_stage_t */
# include "duf_option_source_types.h"                                /* duf_option_source_t */

int duf_xoption_clarify_typed_call( duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg,
                                    unsigned noo, duf_option_stage_t istage, duf_option_source_t source );

#endif
