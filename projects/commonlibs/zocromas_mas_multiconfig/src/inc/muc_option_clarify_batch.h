#ifndef MAS_MUC_OPTION_CLARIFY_BATCH_H
# define MAS_MUC_OPTION_CLARIFY_BATCH_H

# include "muc_option_types.h"                                       /* muc_option_adata_t */
# include "muc_option_stage_types.h"                                 /* muc_option_stage_t */
# include "muc_option_source_types.h"                                /* muc_option_source_t */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t ♠ */
# include "muc_se.h"                                                 /* muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

muc_DR( OPTIONS, boption_xclarify_at_stdx, muc_config_cli_t * cli, char value_separator, muc_option_stage_t istage, muc_option_source_t source,
    const char *cmdstr, char delim, muc_option_adata_t * paod );

#endif
