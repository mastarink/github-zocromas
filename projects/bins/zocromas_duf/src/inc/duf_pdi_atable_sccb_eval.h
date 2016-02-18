#ifndef MAS_DUF_PDI_ATABLE_SCCB_EVAL_H
#  define MAS_DUF_PDI_ATABLE_SCCB_EVAL_H

/* * see duf_sccb_eval.h for names explanation * */
#  include "duf_hook_types.h" /* duf_action_table_t */
#include "duf_levinfo_types.h" /* duf_depthinfo_t */


int duf_ev_pdi_atable_evnamed_list( duf_depthinfo_t * pdi, const char *names, const duf_action_table_t * table, const mas_argvc_t * ptarg,
                                    bool f_summary ) __attribute__ ( ( warn_unused_result ) );

int duf_ev_pdi_atable_evnamen( duf_depthinfo_t * pdi, const char *name, size_t namelen, const duf_action_table_t * table, const mas_argvc_t * ptarg,
                               bool f_summary ) __attribute__ ( ( warn_unused_result ) );

int duf_ev_pdi_atable_evname( duf_depthinfo_t * pdi, const char *name, const duf_action_table_t * table, const mas_argvc_t * ptarg, bool f_summary )
      __attribute__ ( ( warn_unused_result ) );

int duf_ev_pdi_atable_evname_at( duf_depthinfo_t * pdi, const char *name, const duf_action_table_t * table, const char *arg, bool f_summary )
      __attribute__ ( ( warn_unused_result ) );

#endif

