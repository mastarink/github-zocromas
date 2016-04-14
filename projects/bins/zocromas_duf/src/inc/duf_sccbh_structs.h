#ifndef MAS_DUF_SCCBH_STRUCTS_H
# define MAS_DUF_SCCBH_STRUCTS_H

# include "duf_sccb_types.h"                                         /* duf_scan_callbacks_t; duf_sccb_handle_t; duf_sccb_data_row_t; duf_scanner_fun_t; ✗ */
# include "duf_sccbh_types.h"                                        /* duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t ✗ */

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
/* const duf_sql_set_t *active_leaf_set; */
/* const duf_sql_set_t *second_leaf_set; */
/* const duf_sql_set_t *active_node_set; */
/* const duf_sql_set_t *second_node_set; */
  duf_sccbh_fun_t progress_leaf_cb;
  duf_sccbh_fun_t progress_node_cb;
/* duf_scanstage_t current_scanstage; */
/* duf_stmnt_t *current_statement; */
/* duf_scanner_fun_t current_scanner; */
  duf_node_type_t assert__current_node_type;
  duf_rsccbh_fun_t atom_cb;
/* duf_sccb_data_row_t previous_row; */
  duf_sccb_data_row_t *rows;
  const duf_sccb_data_row_t *current_row;
/* duf_sccb_data_row_t *new_row; */
};

#endif
