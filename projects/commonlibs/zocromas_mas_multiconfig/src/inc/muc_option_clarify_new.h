#ifndef MAS_MUC_OPTION_CLARIFY_NEW_H
# define MAS_MUC_OPTION_CLARIFY_NEW_H

# include "muc_option_types.h"                                       /* muc_longval_extended_t; muc_longval_extended_vtable_t ♠ */
# include "muc_option_stage_types.h"                                 /* muc_option_stage_t */
# include "muc_option_source_types.h"                                /* muc_option_source_t */
# include "muc_config_cli_types.h"                                   /* muc_config_cli_t ♠ */
# include "muc_option_data_types.h"                                  /* muc_xclarifier_t etc... ♠ */

# include "muc_se.h"                                                 /* muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

muc_DR( OPTIONS, soption_xclarify_new_at_stdx_default_with_pod, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
    muc_xclarifier_t clarifier, char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_data_t * pod,
    muc_option_adata_t * paod );
muc_DR( OPTIONS, soption_xclarify_snac_new_at_stdx_default, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
    muc_xclarifier_t clarifier, char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_adata_t * paod );
muc_DR( OPTIONS, soption_xclarify_sna_new_at_stdx_default, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
    char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_adata_t * paod );
muc_DR( OPTIONS, soption_xclarify_na_new_at_stdx_default, muc_config_cli_t * cli, const char *name, const char *arg, char value_separator,
    muc_option_stage_t istage, muc_option_source_t source, muc_option_adata_t * paod );
muc_DR( OPTIONS, soption_xclarify_s_new_at_stdx_default, muc_config_cli_t * cli, const char *string, char value_separator, muc_option_stage_t istage,
    muc_option_source_t source, muc_option_adata_t * paod );

muc_DR( OPTIONS, soption_xclarify_new_booted_source, muc_config_cli_t * cli, muc_option_stage_t istage, muc_option_source_t source,
    muc_option_adata_t * paod );

size_t muc_pod_source_count( const muc_option_adata_t * paod, muc_option_stage_t istage, muc_option_source_t source );
size_t muc_pod_stage_count( const muc_option_adata_t * paod, muc_option_stage_t istage );

#endif
