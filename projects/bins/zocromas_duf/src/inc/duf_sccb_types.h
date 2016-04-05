#ifndef MAS_DUF_SCCB_TYPES_H
# define MAS_DUF_SCCB_TYPES_H

# include "duf_sql_set_types.h"                                      /* duf_sql_set_t */
# include "sql_beginning_types.h"                                    /* duf_sql_sequence_t */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */

typedef struct duf_sccb_handle_s duf_sccb_handle_t;
typedef int ( *duf_scanner_fun_t ) ( duf_stmnt_t * pstmt_arg, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh );

typedef struct duf_sccb_data_value_s duf_sccb_data_value_t;

typedef struct duf_sccb_data_row_s duf_sccb_data_row_t;

typedef struct duf_scan_callbacks_s duf_scan_callbacks_t;

typedef void ( *duf_sccbh_fun_t ) ( duf_sccb_handle_t * );
typedef void ( *duf_rsccbh_fun_t ) ( const duf_sccb_handle_t * sccbh, duf_stmnt_t * pstmt_arg, duf_scanstage_t scanstage, duf_scanner_fun_t scanner,
                                     duf_node_type_t node_type, int r );
typedef struct duf_scanner_set_s duf_scanner_set_t;

#endif
