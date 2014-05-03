#ifndef MAS_DUF_ENTRY_SCAN_H
#  define MAS_DUF_ENTRY_SCAN_H

int duf_scan_entries_by_pathid_and_record( duf_depthinfo_t * pdi, duf_record_t * precord, duf_scan_hook_entry_reg_t scan_entry_reg,
                                           duf_scan_hook_entry_dir_t scan_entry_dir );
int
duf_scan_entries_by_pathid_and_record2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi,
                                        duf_scan_hook2_entry_reg_t scan_entry_reg, duf_scan_hook2_entry_dir_t scan_entry_dir );
#endif
