#ifndef MAS_DUF_OPTION_CLARIFY_BATCH_H
# define MAS_DUF_OPTION_CLARIFY_BATCH_H

# include "duf_option_types.h"                                       /* duf_option_adata_t */
# include "duf_option_stage_types.h"                                 /* duf_option_stage_t */
# include "duf_option_source_types.h"                                /* duf_option_source_t */
# include "duf_config_cli_types.h"                                   /* duf_config_cli_t ♠ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

DR( OPTIONS, boption_xclarify_at_stdx, duf_config_cli_t * cli, char value_separator, duf_option_stage_t istage, duf_option_source_t source,
    const char *cmdstr, char delim, duf_option_adata_t * paod );

#endif
