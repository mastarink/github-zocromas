#ifndef MAS_DUF_HOOK_TYPES_H
#  define MAS_DUF_HOOK_TYPES_H

#  include <mastar/tools/mas_argvc_types.h>

#  include "duf_record_types.h"
#  include "duf_scan_types.h"   /* duf_node_type_t */
#  include "sql_beginning_types.h"


/* typedef int ( *duf_scan_hook_init_t ) ( duf_depthinfo_t * pdi ); */

typedef int ( *duf_scanner_t ) ( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

/* this is callback of type: duf_scan_hook_dir_t : */
/* typedef int ( *duf_scan_hook2_dir_t ) ( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi ); */

/* this is callback of type: duf_scan_hook_file_t : */
typedef int ( *duf_scan_hook2_file_func_t ) ( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );
typedef int ( *duf_scan_hook2_item_func_t ) ( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

/* typedef int ( *duf_scan_hook2_file_fd_t ) ( duf_stmnt_t * pstmt, int fd, duf_depthinfo_t * pdi ); */

/* typedef int ( *duf_scan_hook2_dirent_t ) ( duf_depthinfo_t * pdi ); */

typedef int ( *duf_anyhook_t ) ( void );

/* this is callback of type: duf_str_cb_t (first range; str_cb) */
typedef int ( *duf_str_cb2_t ) ( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, struct duf_sccb_handle_s /* duf_sccb_handle_t */  * sccbh );



/* KNOWN duf_sel_cb_t callbacks:
 * duf_sel_cb_field_by_sccb	: str_cb_unused	, str_cb_udata_unused, pdi_unused
 * duf_sel_cb_levinfo		: str_cb_unused	, str_cb_udata_unused, xpdi_unused,	sccb_unused
 * duf_sel_cb_name_parid	: str_cb_unused	, str_cb_udata_unused, 			sccb_unused
 * duf_sel_cb_leaf		:		, sel_cb_udata_unused
 * duf_sel_cb_node		:		, sel_cb_udata_unused
*/

typedef int ( *duf_sel_cb2_t ) ( duf_scanstage_t scanstage, duf_stmnt_t * pstmt, duf_str_cb2_t str_cb,
                                 struct duf_sccb_handle_s /* duf_sccb_handle_t */  * sccbh );

#  if 0

typedef int ( *duf_sel_cb2_match_t ) ( duf_stmnt_t * pstmt );
#  endif

typedef struct
{
  unsigned use_std_fieldset:1;
  unsigned use_std_selector2:1;
  unsigned expand_sql:1;
  duf_node_type_t type;
  const char *name;
  const char *fieldset;
  const char *fieldsets[10];
  /* const char *selector; */
  const char *selector2;

  const char *cte;
  const char *selector2_cte;
  const char *matcher;
  const char *filter;
  const char *group;
  const char *order;
  const char *count_aggregate;
  /* const char *selector_total2; */
} duf_sql_set_t;

struct duf_scan_callbacks_s
{
  unsigned def_opendir:1;
  /* unsigned scan_mode_2:1; */
  unsigned disabled:1;
  const char *title;
  const char *name;
  struct duf_scan_callbacks_s *next;

  unsigned use_std_node;
  duf_sql_set_t node;

  unsigned use_std_leaf;
  duf_sql_set_t leaf;

  unsigned count_nodes:1;
  unsigned no_progress:1;
  /* const char *leaf_selector_total2; */
  void *dlhan;
#  if 1
  duf_scanner_t init_scan;

  duf_scanner_t node_scan_before2;
  duf_scanner_t node_scan_before2_deleted;

  duf_scanner_t node_scan_middle2;
  duf_scanner_t node_scan_middle2_deleted;

  duf_scanner_t node_scan_after2;
  duf_scanner_t node_scan_after2_deleted;

  duf_scanner_t leaf_scan2;
  duf_scanner_t leaf_scan2_deleted;
  /* duf_scanner_t leaf_scan_fd2; */
  duf_scanner_t leaf_scan_fd2;
  duf_scanner_t leaf_scan_fd2_deleted;

  duf_scanner_t dirent_file_scan_before2;
  duf_scanner_t dirent_dir_scan_before2;
#  else
  duf_scan_hook_init_t init_scan;

  duf_scan_hook2_dir_t node_scan_before2;
  duf_scan_hook2_dir_t node_scan_before2_deleted;

  duf_scan_hook2_dir_t node_scan_middle2;
  duf_scan_hook2_dir_t node_scan_middle2_deleted;

  duf_scan_hook2_dir_t node_scan_after2;
  duf_scan_hook2_dir_t node_scan_after2_deleted;

  duf_scan_hook2_file_t leaf_scan2;
  duf_scan_hook2_file_t leaf_scan2_deleted;
  /* duf_scan_hook2_file_fd_t leaf_scan_fd2; */
  duf_scan_hook2_file_t leaf_scan_fd2;
  duf_scan_hook2_file_t leaf_scan_fd2_deleted;

  duf_scan_hook2_dirent_t dirent_file_scan_before2;
  duf_scan_hook2_dirent_t dirent_dir_scan_before2;
#  endif
  duf_sql_sequence_t *beginning_sql_seq;
  duf_sql_sequence_t *final_sql_seq;
};

typedef struct duf_scan_callbacks_s duf_scan_callbacks_t;

typedef void ( *duf_sccbh_fun_t ) ( struct duf_sccb_handle_s * );

struct duf_sccb_handle_s
{
  unsigned total_counted:1;
  unsigned long long total_items;
#  if 0
  int targc;
  char *const *targv;
#  else
  mas_cargvc_t parg;
#  endif
#  if 0
  const duf_ufilter_t *pu_x;
#  endif
  int pdi_cloned;
  duf_depthinfo_t *pdi;
  unsigned long long changes;
  const duf_scan_callbacks_t *sccb;
  duf_sccbh_fun_t progress_leaf_cb;
  duf_sccbh_fun_t progress_node_cb;
};

typedef struct duf_sccb_handle_s duf_sccb_handle_t;


typedef struct
{
  unsigned end_of_table:1;
  unsigned in_use:1;
  unsigned tovector:1;
  duf_config_act_flags_combo_t on;
  duf_config_act_flags_combo_t off;
  duf_scan_callbacks_t *sccb;
} duf_action_table_t;


typedef struct
{
  duf_node_type_t type;
  const char *name;
  const char *set;
} duf_fieldset_t;

typedef struct
{
  duf_node_type_t type;
  const char *name;
  const char *selector2;
} duf_selector_t;



#endif

/*
vi: ft=c
*/
