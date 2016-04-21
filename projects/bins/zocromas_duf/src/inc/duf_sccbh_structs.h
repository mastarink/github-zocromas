#ifndef MAS_DUF_SCCBH_STRUCTS_H
# define MAS_DUF_SCCBH_STRUCTS_H

# include "duf_sccb_structs.h"                                       /* duf_scan_callbacks_s; duf_sccb_data_row_s; duf_scanner_fun_s; ✗ */
# include "duf_sccbh_types.h"                                        /* duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t ✗ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */

/* 20160421.141336 */
struct duf_sccb_handle_s
{
  unsigned total_counted:1;
  unsigned long long total_items;
# if 0
  int targc;
  char *const *targv;
# else
  mas_cargvc_t parg;
# endif
# if 0
  const duf_ufilter_t *pu_x;
# endif
  int pdi_cloned;
  duf_depthinfo_t *pdi;
  unsigned long long changes;
  int sccb_index;
  const duf_scan_callbacks_t *const *sccb_array;
/* const duf_scan_callbacks_t *sccb; */
  duf_sccbh_fun_t progress_leaf_cb;
  duf_sccbh_fun_t progress_node_cb;
  /* duf_node_type_t assert__current_node_type; */
  duf_rsccbh_fun_t atom_cb;
  duf_scanner_set_flags_set_t scanner_set_flags_mask_on;
  duf_sccb_data_list_t rowlist;
  const duf_sccb_data_row_t *current_row;
};

#endif
