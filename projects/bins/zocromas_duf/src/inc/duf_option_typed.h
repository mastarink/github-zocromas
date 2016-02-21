#ifndef MAS_DUF_OPTION_TYPED_H
# define MAS_DUF_OPTION_TYPED_H


# include "duf_option_types.h"                                       /* duf_longval_extended_t; duf_longval_extended_vtable_t */
# include "duf_option_stage_types.h"                                 /* duf_option_stage_t */
# include "duf_option_source_types.h"                                /* duf_option_source_t */

int duf_xoption_clarify_typed( duf_config_cli_t * cli,const duf_longval_extended_t * extended, const char *optargg, const duf_longval_extended_vtable_t * xtable,
                               unsigned noo, duf_option_stage_t istage, duf_option_source_t source );

#endif
