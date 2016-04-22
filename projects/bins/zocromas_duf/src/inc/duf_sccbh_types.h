#ifndef MAS_DUF_SCCBH_TYPES_H
# define MAS_DUF_SCCBH_TYPES_H


# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */


typedef struct duf_sccb_handle_s duf_sccb_handle_t;
typedef int ( *duf_scanner_fun_t ) (  /* duf_stmnt_t * pstmt_arg, */ duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh );
typedef int ( *duf_sccbh_fun_t ) ( duf_sccb_handle_t * );
typedef int ( *duf_rsccbh_fun_t ) ( const duf_sccb_handle_t * sccbh, duf_scanstage_t scanstage, duf_scanner_fun_t scanner, duf_node_type_t node_type,
                                    int r );


typedef int ( *duf_sccb_print_cb_t ) ( char *pbuffer, size_t bfsz, const duf_depthinfo_t * pdi, int use_row, const struct duf_sccb_data_row_s * row,
                                       const duf_sccb_handle_t * sccbh, size_t * pwidth );

#endif
