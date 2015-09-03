#ifndef DUF_SCCB_EVAL_STD_H
#  define DUF_SCCB_EVAL_STD_H



int duf_evaluate_sccb_named_list_std( const char *names );
int duf_evaluate_sccb_namen_std( const char *name, size_t len );
int duf_evaluate_sccb_name_std( const char *name );

int duf_evaluate_pdi_sccb_namen_std( const char *name, size_t len, duf_depthinfo_t * pdi, duf_argvc_t * ptarg, duf_ufilter_t * pu );
int duf_evaluate_pdi_sccb_name_std( const char *name, duf_depthinfo_t * pdi, duf_argvc_t * ptarg, duf_ufilter_t * pu );

int duf_evaluate_pdi_sccb_name_std_at( const char *name,  duf_depthinfo_t * pdi, const char *arg, duf_ufilter_t * pu );
int duf_evaluate_pdi_sccb_std        ( const char *name,  duf_depthinfo_t * pdi,  duf_ufilter_t * pu );

#endif