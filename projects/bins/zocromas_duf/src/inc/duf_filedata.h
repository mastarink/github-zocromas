#ifndef MAS_DUF_FILEDATA_H
# define MAS_DUF_FILEDATA_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* unsigned long long duf_pdistat2file_dataid_existed( duf_depthinfo_t * pdi, int *pr ); */
DRP( PDI, unsigned long long, dataid, 0, pdistat2file_dataid_existed, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh );

/* unsigned long long duf_pdistat2file_dataid( duf_depthinfo_t * pdi, int need_id, int *pr ); */
DRP( PDI, unsigned long long, dataid, 0, pdistat2file_dataid, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh, int need_id );

DR( PDI, pdistat2file, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh );

#endif
