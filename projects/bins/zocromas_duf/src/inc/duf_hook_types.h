#ifndef MAS_DUF_HOOK_TYPES_H
#  define MAS_DUF_HOOK_TYPES_H

#include "duf_record_types.h"

typedef int ( *duf_scan_hook_init_t ) ( void );


/* this is callback of type: duf_scan_hook_dir_t : */
typedef int ( *duf_scan_hook_dir_t ) ( unsigned long long pathid, duf_depthinfo_t * pdi, duf_record_t * precord );
typedef int ( *duf_scan_hook2_dir_t ) ( duf_sqlite_stmt_t * pstmt, unsigned long long pathid, duf_depthinfo_t * pdi );

/* this is callback of type: duf_scan_hook_file_t : */
typedef int ( *duf_scan_hook_file_t ) ( duf_depthinfo_t * pdi, duf_record_t * precord );
typedef int ( *duf_scan_hook2_file_t ) ( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );

typedef int ( *duf_scan_hook_file_fd_t ) ( int fd, const struct stat * pst_file, duf_depthinfo_t * pdi, duf_record_t * precord );
typedef int ( *duf_scan_hook2_file_fd_t ) ( duf_sqlite_stmt_t * pstmt, int fd, const struct stat * pst_file, duf_depthinfo_t * pdi );


typedef int ( *duf_scan_hook_dirent_reg_t ) ( const char *fname, const struct stat * pstat, unsigned long long dirid, duf_depthinfo_t * pdi,
                                              duf_record_t * precord );
typedef int ( *duf_scan_hook2_dirent_reg_t ) ( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat * pstat,
                                               unsigned long long dirid, duf_depthinfo_t * pdi );


typedef int ( *duf_scan_hook_dirent_dir_t ) ( const char *fname, const struct stat * pstat, unsigned long long dirid,
                                              duf_depthinfo_t * pdi, duf_record_t * precord );
typedef int ( *duf_scan_hook2_dirent_dir_t ) ( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat * pstat,
                                               unsigned long long dirid, duf_depthinfo_t * pdi );


typedef int ( *duf_scan_hook_dirent_parent_t ) ( const struct stat * pstat, unsigned long long dirid, duf_depthinfo_t * pdi,
                                                 duf_record_t * precord );
typedef int ( *duf_scan_hook2_dirent_parent_t ) ( duf_sqlite_stmt_t * pstmt, const struct stat * pstat, unsigned long long dirid,
                                                  duf_depthinfo_t * pdi );



typedef int ( *duf_anyhook_t ) ( void );




/* this is callback of type: duf_str_cb_t (first range; str_cb) */
typedef int ( *duf_str_cb2_t ) ( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s * sccb );



/* KNOWN duf_sel_cb_t callbacks:
 * duf_sel_cb_field_by_sccb	: str_cb_unused	, str_cb_udata_unused, pdi_unused
 * duf_sel_cb_levinfo		: str_cb_unused	, str_cb_udata_unused, xpdi_unused,	sccb_unused
 * duf_sel_cb_name_parid	: str_cb_unused	, str_cb_udata_unused, 			sccb_unused
 * duf_sel_cb_leaf		:		, sel_cb_udata_unused
 * duf_sel_cb_node		:		, sel_cb_udata_unused
*/
typedef int ( *duf_sel_cb2_t ) ( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb, duf_depthinfo_t * pdi,
                                 struct duf_scan_callbacks_s * sccb );

typedef int ( *duf_sel_cb2_match_t ) ( duf_sqlite_stmt_t * pstmt );



struct duf_scan_callbacks_s
{
  unsigned opendir:1;
  unsigned scan_mode_2:1;
  const char *title;

  const char *node_fieldset;
  const char *node_selector;
  const char *node_selector2;

  const char *leaf_fieldset;
  const char *leaf_selector;
  const char *leaf_selector2;
  const char *leaf_selector_total2;

  duf_scan_hook_init_t init_scan;

  duf_scan_hook_dir_t node_scan_before;
  duf_scan_hook2_dir_t node_scan_before2;
  duf_scan_hook2_dir_t node_scan_before2_deleted;

  duf_scan_hook_dir_t node_scan_middle;
  duf_scan_hook2_dir_t node_scan_middle2;
  duf_scan_hook2_dir_t node_scan_middle2_deleted;

  duf_scan_hook_dir_t node_scan_after;
  duf_scan_hook2_dir_t node_scan_after2;
  duf_scan_hook2_dir_t node_scan_after2_deleted;

  duf_scan_hook_file_t leaf_scan;
  duf_scan_hook2_file_t leaf_scan2;
  duf_scan_hook2_file_t leaf_scan2_deleted;

  duf_scan_hook_file_fd_t leaf_scan_fd;
  duf_scan_hook2_file_fd_t leaf_scan_fd2;

  duf_scan_hook_dirent_reg_t dirent_file_scan_before;
  duf_scan_hook2_dirent_reg_t dirent_file_scan_before2;

  duf_scan_hook_dirent_dir_t dirent_dir_scan_before;
  duf_scan_hook2_dirent_dir_t dirent_dir_scan_before2;

  const char **final_sql_argv;
};

typedef struct duf_scan_callbacks_s duf_scan_callbacks_t;


#endif
