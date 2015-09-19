#ifndef DUF_PDI_SCCB_EVAL_H
#  define DUF_PDI_SCCB_EVAL_H

/* * see duf_sccb_eval.h for names explanation * */

const duf_action_table_t *duf_find_sccb_by_evname( const char *name, const duf_action_table_t * table );
const duf_action_table_t *duf_find_sccb_by_evnamen( const char *name, size_t namelen, const duf_action_table_t * table );


int duf_ev_pdi_sccb( duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_argvc_t * ptarg /*, const duf_ufilter_t * pu */  )
      __attribute__ ( ( warn_unused_result ) );
int duf_ev_pdi_evnamed_list( duf_depthinfo_t * pdi, const char *names, const duf_action_table_t * table,
                             duf_argvc_t * ptarg /*, const duf_ufilter_t * pu */  ) __attribute__ ( ( warn_unused_result ) );
int duf_ev_pdi_evnamen( duf_depthinfo_t * pdi, const char *name, size_t namelen, const duf_action_table_t * table,
                        duf_argvc_t * ptarg /*, const duf_ufilter_t * pu */  ) __attribute__ ( ( warn_unused_result ) );
int duf_ev_pdi_evname( duf_depthinfo_t * pdi, const char *name, const duf_action_table_t * table,
                       duf_argvc_t * ptarg /*, const duf_ufilter_t * pu */  )
      __attribute__ ( ( warn_unused_result ) );

int duf_ev_pdi_evname_at( duf_depthinfo_t * pdi, const char *name, const duf_action_table_t * table,
                          const char *arg /*, const duf_ufilter_t * pu */  ) __attribute__ ( ( warn_unused_result ) );

#endif
