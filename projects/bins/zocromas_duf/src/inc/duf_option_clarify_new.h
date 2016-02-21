#ifndef MAS_DUF_OPTION_CLARIFY_NEW_H
# define MAS_DUF_OPTION_CLARIFY_NEW_H


# include "duf_option_types.h"                                       /* duf_xclarifier_t ; duf_option_data_t ; duf_option_adata_t */
# include "duf_option_stage_types.h"                                 /* duf_option_stage_t */
# include "duf_option_source_types.h"                                /* duf_option_source_t */


DR( OPTIONS, soption_xclarify_new_at_stdx_default_with_pod, duf_config_cli_t * cli, const char *string, const char *name, const char *arg, duf_xclarifier_t clarifier,
    char value_separator, duf_option_stage_t istage, duf_option_source_t source, duf_option_data_t * pod, duf_option_adata_t * paod );
DR( OPTIONS, soption_xclarify_snac_new_at_stdx_default, duf_config_cli_t * cli, const char *string, const char *name, const char *arg, duf_xclarifier_t clarifier,
    char value_separator, duf_option_stage_t istage, duf_option_source_t source, duf_option_adata_t * paod );
DR( OPTIONS, soption_xclarify_sna_new_at_stdx_default, duf_config_cli_t * cli, const char *string, const char *name, const char *arg, char value_separator,
    duf_option_stage_t istage, duf_option_source_t source, duf_option_adata_t * paod );
DR( OPTIONS, soption_xclarify_na_new_at_stdx_default, duf_config_cli_t * cli, const char *name, const char *arg, char value_separator, duf_option_stage_t istage,
    duf_option_source_t source, duf_option_adata_t * paod );
DR( OPTIONS, soption_xclarify_s_new_at_stdx_default, duf_config_cli_t * cli, const char *string, char value_separator, duf_option_stage_t istage,
    duf_option_source_t source, duf_option_adata_t * paod );

DR( OPTIONS, soption_xclarify_new_booted_source, duf_config_cli_t * cli, duf_option_stage_t istage, duf_option_source_t source, duf_option_adata_t * paod );


size_t duf_pod_source_count( const duf_option_adata_t * paod, duf_option_stage_t istage, duf_option_source_t source );
size_t duf_pod_stage_count( const duf_option_adata_t * paod, duf_option_stage_t istage );

#endif
