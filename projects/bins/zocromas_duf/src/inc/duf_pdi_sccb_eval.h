#ifndef MAS_DUF_PDI_SCCB_EVAL_H
# define MAS_DUF_PDI_SCCB_EVAL_H

# include <mastar/tools/mas_argvc_types.h>                           /* mas_argvc_t; mas_cargvc_t; ▤ */

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t ✗ */

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
