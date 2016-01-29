#ifndef MAS_DUF_OPTION_CLARIFY_NEW_H
#  define MAS_DUF_OPTION_CLARIFY_NEW_H

DR( OPTIONS, soption_xclarify_new_at_stdx_default_with_pod, const char *string, const char *name, const char *arg, char vseparator,
    duf_option_stage_t istage, duf_option_source_t source, duf_option_data_t * pod, duf_option_adata_t * paod );
DR( OPTIONS, soption_xclarify_new_at_stdx_default, const char *string, const char *name, const char *arg, char vseparator, duf_option_stage_t istage,
    duf_option_source_t source, duf_option_adata_t * paod );

DR( OPTIONS, soption_xclarify_new_booted_source, duf_option_stage_t istage, duf_option_source_t source, duf_option_adata_t * paod );

duf_option_data_t *duf_pod_from_paod_n( const duf_option_adata_t * paod, duf_option_stage_t basicstage, duf_option_source_t source, size_t index );
duf_option_data_t *duf_pod_from_paod( const duf_option_adata_t * paod, duf_option_stage_t basicstage, duf_option_stage_t istage,
                                      duf_option_source_t source );

size_t duf_pod_source_count( const duf_option_adata_t * paod, duf_option_stage_t istage, duf_option_source_t source );
size_t duf_pod_stage_count( const duf_option_adata_t * paod, duf_option_stage_t istage );

#endif
