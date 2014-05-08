#ifndef MAS_DUF_DIRENT_SCAN_H
#  define MAS_DUF_DIRENT_SCAN_H

int duf_scan_dirents_by_pathid_and_record( duf_depthinfo_t * pdi, duf_record_t * precord, duf_scan_hook_dirent_reg_t scan_dirent_reg,
                                           duf_scan_hook_dirent_dir_t scan_dirent_dir );
int
duf_scan_dirents_by_pathid_and_record2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi,
                                        duf_scan_hook2_dirent_reg_t scan_dirent_reg, duf_scan_hook2_dirent_dir_t scan_dirent_dir );
#endif
