#ifndef MAS_DUF_SCCBH_SCANNER_H
# define MAS_DUF_SCCBH_SCANNER_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t; duf_scanner_fun_t; ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( SCCBH, sccbh_call_scanner, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused, */ duf_scanstage_t scanstage, duf_scanner_fun_t scanner,
    duf_node_type_t node_type );

DR( SCCBH, sccbh_call_leaf_pack_scanner, duf_sccb_handle_t * sccbh, duf_scanstage_t scanstage );

DRX( SCCBH, unsigned, has, 0, sccb_has_new_scanner, duf_sccb_handle_t * sccbh, duf_node_type_t nt, duf_scanstage_t scanstage, int db, int dirent );

#endif
