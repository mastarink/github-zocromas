#ifndef MAS_DUF_SCCB_HANDLE_H
# define MAS_DUF_SCCB_HANDLE_H

# include <mastar/tools/mas_argvc_types.h>                           /* mas_argvc_t; mas_cargvc_t; ▤ */

/* duf_sccb_handle_t *duf_sccb_handle_create( void ); */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DRP( SCCBH, duf_sccb_handle_t *, sccbh, NULL, sccb_handle_open, duf_depthinfo_t * pdi, const duf_scan_callbacks_t * const *psccb,
     const mas_cargvc_t * ptarg );

DR( SCCBH, sccb_handle_close, duf_sccb_handle_t * sccbh ) __attribute__ ( ( warn_unused_result ) );

duf_sql_set_pair_t duf_sccbh_get_sql_set_f( duf_sccb_handle_t * sccbh, duf_node_type_t node_type );

#endif

/*
vi: ft=c
*/
