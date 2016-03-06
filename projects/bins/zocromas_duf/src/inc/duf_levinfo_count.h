#ifndef MAS_DUF_LEVINFO_COUNT_H
# define MAS_DUF_LEVINFO_COUNT_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DRX( PDI, unsigned long long, x, 0, levinfo_count_childs_d, const duf_depthinfo_t * pdi, int d );
DRX( PDI, unsigned long long, x, 0, levinfo_count_childs, const duf_depthinfo_t * pdi );
DRX( PDI, unsigned long long, x, 0, levinfo_count_childs_up, const duf_depthinfo_t * pdi );

DRX( PDI, unsigned long long, x, 0, levinfo_count_gfiles_d, const duf_depthinfo_t * pdi, int d );
DRX( PDI, unsigned long long, x, 0, levinfo_count_gfiles, const duf_depthinfo_t * pdi );
DRX( PDI, unsigned long long, x, 0, levinfo_count_gfiles_up, const duf_depthinfo_t * pdi );

#endif
