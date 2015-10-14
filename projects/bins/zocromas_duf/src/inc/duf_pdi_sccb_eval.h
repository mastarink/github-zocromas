#ifndef DUF_PDI_SCCB_EVAL_H
#  define DUF_PDI_SCCB_EVAL_H

/* * see duf_sccb_eval.h for names explanation * */

#include <mastar/tools/mas_argvc_types.h>

int duf_ev_pdi_sccb( duf_depthinfo_t * pdi, const duf_scan_callbacks_t * sccb, const mas_argvc_t * ptarg, bool f_summary )
      __attribute__ ( ( warn_unused_result ) );
int duf_ev_pdi_evnamed_list( duf_depthinfo_t * pdi, const char *names, const duf_action_table_t * table, const mas_argvc_t * ptarg, bool f_summary )
      __attribute__ ( ( warn_unused_result ) );
int duf_ev_pdi_evnamen( duf_depthinfo_t * pdi, const char *name, size_t namelen, const duf_action_table_t * table, const mas_argvc_t * ptarg,
                        bool f_summary ) __attribute__ ( ( warn_unused_result ) );
int duf_ev_pdi_evname( duf_depthinfo_t * pdi, const char *name, const duf_action_table_t * table, const mas_argvc_t * ptarg, bool f_summary )
      __attribute__ ( ( warn_unused_result ) );

int duf_ev_pdi_evname_at( duf_depthinfo_t * pdi, const char *name, const duf_action_table_t * table,
                          const char *arg, bool f_summary ) __attribute__ ( ( warn_unused_result ) );

#endif
