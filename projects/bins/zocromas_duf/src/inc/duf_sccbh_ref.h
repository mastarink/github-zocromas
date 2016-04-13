#ifndef MAS_DUF_SCCBH_REF_H
# define MAS_DUF_SCCBH_REF_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t; duf_scanner_fun_t; ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

void duf_sccbh_set_pdi( duf_sccb_handle_t * sccbh, duf_depthinfo_t * pdi );
duf_depthinfo_t *duf_sccbh_pdi( const duf_sccb_handle_t * sccbh );
duf_depthinfo_t *duf_sccbh_pdi_p( duf_sccb_handle_t * sccbh );

#endif
