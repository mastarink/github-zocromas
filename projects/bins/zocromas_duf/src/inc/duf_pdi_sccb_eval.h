#ifndef DUF_PDI_SCCB_EVAL_H
#  define DUF_PDI_SCCB_EVAL_H

const duf_action_table_t *duf_find_sccb_by_namen( const char *name, size_t namelen, const duf_action_table_t * table );


int duf_evaluate_pdi_sccb( duf_depthinfo_t * pdi, duf_argvc_t * ptarg, duf_ufilter_t * pu, duf_scan_callbacks_t * sccb );
int duf_evaluate_pdi_sccb_named_list( const char *names, const duf_action_table_t * table, duf_depthinfo_t * pdi, duf_argvc_t * ptarg,
                                      duf_ufilter_t * pu );
int duf_evaluate_pdi_sccb_namen( const char *name, size_t len, const duf_action_table_t * table, duf_depthinfo_t * pdi, duf_argvc_t * ptarg,
                                 duf_ufilter_t * pu );
int duf_evaluate_pdi_sccb_name( const char *name, const duf_action_table_t * table, duf_depthinfo_t * pdi, duf_argvc_t * ptarg, duf_ufilter_t * pu );


#endif

