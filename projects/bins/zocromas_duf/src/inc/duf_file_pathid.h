#ifndef MAS_DUF_FILE_PATHID_H
#  define MAS_DUF_FILE_PATHID_H

/* int duf_print_files_by_pathid( unsigned long long pathid, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, unsigned long long items ); */

/* duf_scan_files_by_dirid:
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by pathid with corresponding args
 * */
int duf_scan_files_by_dirid( unsigned long long pathid, duf_scan_callback_file_t str_cb, duf_dirinfo_t * pdi,
                              duf_scan_callbacks_t * sccb );

#endif
