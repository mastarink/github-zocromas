#ifndef MAS_DUF_HOOK_TYPES_H
#  define MAS_DUF_HOOK_TYPES_H

#  include "duf_record_types.h"
#  include "sql_beginning_types.h"


typedef int ( *duf_scan_hook_init_t ) ( duf_depthinfo_t * pdi );


/* this is callback of type: duf_scan_hook_dir_t : */
typedef int ( *duf_scan_hook2_dir_t ) ( duf_sqlite_stmt_t * pstmt, /* unsigned long long pathid, */ duf_depthinfo_t * pdi );

/* this is callback of type: duf_scan_hook_file_t : */
typedef int ( *duf_scan_hook2_file_t ) ( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );
typedef int ( *duf_scan_hook2_item_t ) ( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );

typedef int ( *duf_scan_hook2_file_fd_t ) ( duf_sqlite_stmt_t * pstmt, int fd, /* const struct stat * pst_file, */ duf_depthinfo_t * pdi );

typedef int ( *duf_scan_hook2_dirent_t ) ( const char *fname, const struct stat * pstat, /* unsigned long long dirid, */ duf_depthinfo_t * pdi );

typedef int ( *duf_anyhook_t ) ( void );

/* this is callback of type: duf_str_cb_t (first range; str_cb) */
typedef int ( *duf_str_cb2_t ) ( duf_sqlite_stmt_t * pstmt, struct duf_sccb_handle_s /* duf_sccb_handle_t */  * sccbh );



/* KNOWN duf_sel_cb_t callbacks:
 * duf_sel_cb_field_by_sccb	: str_cb_unused	, str_cb_udata_unused, pdi_unused
 * duf_sel_cb_levinfo		: str_cb_unused	, str_cb_udata_unused, xpdi_unused,	sccb_unused
 * duf_sel_cb_name_parid	: str_cb_unused	, str_cb_udata_unused, 			sccb_unused
 * duf_sel_cb_leaf		:		, sel_cb_udata_unused
 * duf_sel_cb_node		:		, sel_cb_udata_unused
*/
typedef int ( *duf_sel_cb2_t ) ( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb, struct duf_sccb_handle_s /* duf_sccb_handle_t */ * sccbh );

typedef int ( *duf_sel_cb2_match_t ) ( duf_sqlite_stmt_t * pstmt );

typedef struct
{
  unsigned use_std_fieldset:1;
  unsigned use_std_selector2:1;
  const char *fieldset;
  const char *selector;
  const char *selector2;
  const char *selector_total2;
} duf_sql_set_t;

struct duf_scan_callbacks_s
{
  unsigned def_opendir:1;
  /* unsigned scan_mode_2:1; */
  unsigned disabled:1;
  const char *title;
  const char *name;

  unsigned use_std_node;
  duf_sql_set_t node;
  
  unsigned use_std_leaf;
  duf_sql_set_t leaf;
  
  /* const char *leaf_selector_total2; */

  duf_scan_hook_init_t init_scan;

  duf_scan_hook2_dir_t node_scan_before2;
  duf_scan_hook2_dir_t node_scan_before2_deleted;

  duf_scan_hook2_dir_t node_scan_middle2;
  duf_scan_hook2_dir_t node_scan_middle2_deleted;

  duf_scan_hook2_dir_t node_scan_after2;
  duf_scan_hook2_dir_t node_scan_after2_deleted;

  duf_scan_hook2_file_t leaf_scan2;
  duf_scan_hook2_file_t leaf_scan2_deleted;
  duf_scan_hook2_file_fd_t leaf_scan_fd2;

  duf_scan_hook2_dirent_t dirent_file_scan_before2;
  duf_scan_hook2_dirent_t dirent_dir_scan_before2;

  duf_sql_sequence_t *beginning_sql_argv;
  duf_sql_sequence_t *final_sql_argv;
};

typedef struct duf_scan_callbacks_s duf_scan_callbacks_t;

struct duf_sccb_handle_s
{
  unsigned long long total_files;
  int targc;
  char *const *targv;
  const duf_ufilter_t *pu;
  duf_depthinfo_t *pdi;
  unsigned long long changes;
  const duf_scan_callbacks_t *sccb;
};

typedef struct duf_sccb_handle_s duf_sccb_handle_t;


typedef struct
{
  unsigned tovector:1;
  duf_config_act_flags_combo_t on;
  duf_config_act_flags_combo_t off;
  duf_scan_callbacks_t *sccb;
} duf_action_table_t;





#endif

/*
vi: ft=c
*/
