#ifndef DUF_PDI_SCCB_EVAL_H
#  define DUF_PDI_SCCB_EVAL_H

const duf_action_table_t *duf_find_sccb_by_namen( const char *name, size_t namelen, const duf_action_table_t * table );


int duf_evaluate_pdi_sccb( duf_depthinfo_t * pdi, duf_argvc_t * ptarg, const duf_ufilter_t * pu, duf_scan_callbacks_t * sccb )
      __attribute__ ( ( warn_unused_result ) );
int duf_ev_pdi_named_sccbs( const char *names, const duf_action_table_t * table, duf_depthinfo_t * pdi, duf_argvc_t * ptarg,
                            const duf_ufilter_t * pu ) __attribute__ ( ( warn_unused_result ) );
int duf_ev_pdi_sccb_namen( const char *name, size_t len, const duf_action_table_t * table, duf_depthinfo_t * pdi, duf_argvc_t * ptarg,
                           const duf_ufilter_t * pu ) __attribute__ ( ( warn_unused_result ) );
int duf_evaluate_pdi_sccb_name( const char *name, const duf_action_table_t * table, duf_depthinfo_t * pdi, duf_argvc_t * ptarg,
                                const duf_ufilter_t * pu ) __attribute__ ( ( warn_unused_result ) );

int duf_evaluate_pdi_sccb_name_at( const char *name, const duf_action_table_t * table, duf_depthinfo_t * pdi, const char *arg,
                                   const duf_ufilter_t * pu ) __attribute__ ( ( warn_unused_result ) );

#endif
