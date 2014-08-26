#ifndef MAS_DUF_FILE_PATHID2_H
#  define MAS_DUF_FILE_PATHID2_H

/* duf_scan_files_by_dirid(2?):
 * call str_cb + pdi (also) as str_cb_udata for each <file> record by pathid with corresponding args
 * */

int duf_scan_files_by_dirid2( unsigned long long dirid, duf_str_cb2_t str_cb2, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb );

#endif
