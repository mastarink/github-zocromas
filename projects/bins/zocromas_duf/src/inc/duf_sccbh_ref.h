#ifndef MAS_DUF_SCCBH_REF_H
# define MAS_DUF_SCCBH_REF_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_data_row_t; duf_scanner_fun_t; ✗ */
# include "duf_sccbh_types.h"                                        /* duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

void duf_sccbh_set_pdi( duf_sccb_handle_t * sccbh, duf_depthinfo_t * pdi );
duf_depthinfo_t *duf_sccbh_pdi( const duf_sccb_handle_t * sccbh );
duf_depthinfo_t *duf_sccbh_pdi_p( duf_sccb_handle_t * sccbh );

const duf_scan_callbacks_t *const *duf_sccbh_sccbarray( const duf_sccb_handle_t * sccbh );
int duf_sccbh_sccbindex( const duf_sccb_handle_t * sccbh );
int *duf_sccbh_psccbindex( duf_sccb_handle_t * sccbh );

unsigned duf_sccbh_total_counted( const duf_sccb_handle_t * sccbh );
void duf_sccbh_set_total_counted( duf_sccb_handle_t * sccbh, unsigned val );
unsigned long long duf_sccbh_total_items( const duf_sccb_handle_t * sccbh );
unsigned long long *duf_sccbh_ptotal_items( duf_sccb_handle_t * sccbh );

void duf_sccbh_set_changes( duf_sccb_handle_t * sccbh, unsigned long long changes );
unsigned long long duf_sccbh_changes( const duf_sccb_handle_t * sccbh );

const duf_sccb_data_row_t *duf_sccbh_rows( const duf_sccb_handle_t * sccbh );
void duf_sccbh_set_current_row( duf_sccb_handle_t * sccbh, const duf_sccb_data_row_t * row );
const duf_sccb_data_row_t *duf_sccbh_current_row( const duf_sccb_handle_t * sccbh );

const duf_scan_callbacks_t *duf_sccbh_sccb( const duf_sccb_handle_t * sccbh );

duf_sccbh_fun_t duf_sccbh_progress_leaf_cb( const duf_sccb_handle_t * sccbh );
duf_sccbh_fun_t duf_sccbh_progress_node_cb( const duf_sccb_handle_t * sccbh );

#endif
