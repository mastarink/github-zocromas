#ifndef MAS_DUF_PRINT_H
# define MAS_DUF_PRINT_H

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */
# include "duf_fileinfo_types.h"
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_hook_types.h"                                         /* duf_str_cb2_t; duf_sel_cb2_t; duf_anyhook_t; duf_action_table_t; ✗ */
/* # include "duf_format_types.h" */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* char *duf_sformat_file_info( const duf_depthinfo_t * pdi, int use_row, duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi, int is_atty,   */
/*                              const char *format, duf_sccb_print_cb_t prefix_scb, duf_sccb_print_cb_t suffix_scb, size_t max_width, size_t * pslen, */
/*                              size_t * pwidth, int *pover );                                                                            */
DRX( OTHER, char *, buffer, NULL, sformat_file_info, const duf_depthinfo_t * pdi, int use_row, const duf_sccb_data_row_t * row,
     duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi, int fcolor, const char *format, duf_sccb_print_cb_t prefix_scb, duf_sccb_print_cb_t suffix_scb,
     size_t max_width MAS_UNUSED, size_t * pslen, size_t * pwidth, int *pover );

/* size_t duf_print_sformat_file_info( const duf_depthinfo_t * pdi, int use_row, duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi, const char *format, */
/*                                     duf_sccb_print_cb_t prefix_scb, duf_sccb_print_cb_t suffix_scb, size_t max_width, int force_color, int nocolor,            */
/*                                     size_t * pswidth, int *pover );                                                                                */
DRX( OTHER, size_t, slen, 0, print_sformat_file_info, const duf_depthinfo_t * pdi, int use_row, const duf_sccb_data_row_t * row,
     duf_sccb_handle_t * sccbh, duf_fileinfo_t * pfi, const char *format, duf_sccb_print_cb_t prefix_scb, duf_sccb_print_cb_t suffix_scb, size_t max_width,
     int force_color, int nocolor, size_t * pswidth, int *pover );

#endif
