#ifndef MAS_DUF_PDI_SCCB_EVAL_H
# define MAS_DUF_PDI_SCCB_EVAL_H

/* * see duf_sccb_eval.h for names explanation * */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */


DR( PDI, ev_pdi_evnamed_list, duf_depthinfo_t * pdi, const char *names, duf_scan_callbacks_t * first, const mas_cargvc_t * ptarg, bool f_summary )
        __attribute__ ( ( warn_unused_result ) );

/* int duf_ev_pdi_evnamen( duf_depthinfo_t * pdi, const char *name, size_t namelen, duf_scan_callbacks_t * first, const mas_cargvc_t * ptarg, */
/*                         bool f_summary ) __attribute__ ( ( warn_unused_result ) );                                                        */

/* int duf_ev_pdi_evname( duf_depthinfo_t * pdi, const char *name, duf_scan_callbacks_t * first, const mas_cargvc_t * ptarg, bool f_summary ) */
/*       __attribute__ ( ( warn_unused_result ) );                                                                                           */

DR( PDI, ev_pdi_evname_at, duf_depthinfo_t * pdi, const char *name, duf_scan_callbacks_t * first, const char *arg, bool f_summary )
        __attribute__ ( ( warn_unused_result ) );

#endif
