#ifndef MAS_DUF_SCCB_TYPES_H
# define MAS_DUF_SCCB_TYPES_H

# include "duf_sql_set_types.h"                                      /* duf_sql_set_t */
# include "sql_beginning_types.h"                                    /* duf_sql_sequence_t */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */

typedef unsigned long long duf_scanner_set_flags_set_t;
struct duf_sccb_handle_s;

typedef int ( *duf_scanner_fun_t ) (  /* duf_stmnt_t * pstmt_arg, */ duf_depthinfo_t * pdi, struct duf_sccb_handle_s * sccbh );
typedef struct duf_sccb_data_value_s duf_sccb_data_value_t;

typedef struct duf_sccb_data_row_s duf_sccb_data_row_t;

typedef struct duf_scan_callbacks_s duf_scan_callbacks_t;

typedef struct duf_scanner_set_s duf_scanner_set_t;

typedef enum duf_scanner_set_flags_n_e duf_scanner_set_flags_n_t;
typedef enum duf_scanner_set_flags_e duf_scanner_set_flags_t;

#endif
