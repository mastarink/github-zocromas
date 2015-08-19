#ifndef MAS_DUF_DIR_SCAN2_STAGES_H
#  define MAS_DUF_DIR_SCAN2_STAGES_H

#  define DUF_HOOK_AT(_sccb, _typ) ( ( duf_scan_hook2_dir_t * ) ( ( ( char * ) _sccb ) + ( offsetof( duf_scan_callbacks_t, _typ ) ) ) )
#  define DUF_SCANNER_AT(_sccb, _typ) ( ( duf_scanner_t  ) ( ( ( char * ) _sccb ) + ( offsetof( duf_scan_callbacks_t, _typ ) ) ) )

int duf_sccbh_eval_fs_items( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh );
int duf_sccbh_eval_db_node_before( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh );
int duf_sccbh_eval_db_leaves( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh );
int duf_sccbh_eval_db_node_middle( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh );
int duf_sccbh_eval_db_subnodes( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh /*, duf_str_cb2_t str_cb2 */  );
int duf_sccbh_eval_db_node_after( duf_sqlite_stmt_t * pstmt, duf_sccb_handle_t * sccbh );

#endif
