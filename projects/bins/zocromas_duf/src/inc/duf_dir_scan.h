#ifndef MAS_DUF_DIR_SCAN_H
#  define MAS_DUF_DIR_SCAN_H

int duf_scan_dirs_by_parentid( unsigned long long pathid, duf_scan_callback_file_t str_cb, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb );

int duf_print_dirs( const char *path, duf_ufilter_t u, int tree );

#endif
