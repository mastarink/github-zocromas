#ifndef MAS_DUF_OPTION_CLARIFY_BATCH_H
# define MAS_DUF_OPTION_CLARIFY_BATCH_H

# include "duf_option_types.h"                                       /* duf_option_adata_t */
# include "duf_option_stage_types.h"                                 /* duf_option_stage_t */
# include "duf_option_source_types.h"                                /* duf_option_source_t */

DR( OPTIONS, boption_xclarify_at_stdx, char value_separator, duf_option_stage_t istage, duf_option_source_t source, const char *cmdstr, char delim,
    duf_option_adata_t * paod );

#endif
