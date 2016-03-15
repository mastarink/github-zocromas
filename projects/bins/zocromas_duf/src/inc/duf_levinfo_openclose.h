#ifndef MAS_DUF_LEVINFO_OPENCLOSE_H
# define MAS_DUF_LEVINFO_OPENCLOSE_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( PDI, levinfo_if_openat_dh_d, duf_depthinfo_t * pdi, int d );

DR( PDI, levinfo_if_openat_dh, duf_depthinfo_t * pdi );
DR( PDI, levinfo_if_openat_dh_up, duf_depthinfo_t * pdi );

/* int duf_levinfo_open_dh( duf_depthinfo_t * pdi, const char *path ); */

/* DR( PDI, levinfo_openat_dh_d, duf_depthinfo_t * pdi, int d ); */

DR( PDI, levinfo_openat_dh, duf_depthinfo_t * pdi );
DR( PDI, levinfo_openat_dh_up, duf_depthinfo_t * pdi );

DR( PDI, levinfo_opened_dh_d, duf_depthinfo_t * pdi, int d );

DR( PDI, levinfo_opened_dh, duf_depthinfo_t * pdi );
DR( PDI, levinfo_opened_dh_up, duf_depthinfo_t * pdi );

DR( PDI, levinfo_opened_here_dh_d, duf_depthinfo_t * pdi, int d );

DR( PDI, levinfo_opened_here_dh, duf_depthinfo_t * pdi );
DR( PDI, levinfo_opened_here_dh_up, duf_depthinfo_t * pdi );

DR( PDI, levinfo_closeat_dh_d, duf_depthinfo_t * pdi, int d );

DR( PDI, levinfo_closeat_dh, duf_depthinfo_t * pdi );
DR( PDI, levinfo_closeat_dh_up, duf_depthinfo_t * pdi );

#endif
