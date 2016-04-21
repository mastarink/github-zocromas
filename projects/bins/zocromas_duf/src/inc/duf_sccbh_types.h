#ifndef MAS_DUF_SCCBH_TYPES_H
# define MAS_DUF_SCCBH_TYPES_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_data_row_t; duf_scanner_fun_t; ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */

/* 20160421.141350 */
typedef struct duf_sccb_handle_s duf_sccb_handle_t;
typedef int ( *duf_sccbh_fun_t ) ( duf_sccb_handle_t * );
typedef int ( *duf_rsccbh_fun_t ) ( const duf_sccb_handle_t * sccbh, duf_scanstage_t scanstage, duf_scanner_fun_t scanner, duf_node_type_t node_type,
                                    int r );

#endif
