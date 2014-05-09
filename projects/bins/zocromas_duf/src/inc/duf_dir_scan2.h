#ifndef MAS_DUF_DIR_SCAN2_H
#  define MAS_DUF_DIR_SCAN2_H

/* duf_scan_dirs_by_parentid
 * 1. for <current> dir call sccb->directory_scan
 * 2. for each dir in <current> dir call str_cb + str_cb_udata
 * 3. for each file in <current> dir call sccb->file_scan
 * see duf_scan_dir_by_pi
 * */
int duf_scan_dirs_by_parentid2( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb );



/* 
 * this is callback of type: duf_str_cb_t (first range; str_cb) 
 * */
/* will be static! */
int duf_str_cb_leaf_scan2( duf_sqlite_stmt_t * pstmt, void *str_cb2_udata, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb );


#endif

