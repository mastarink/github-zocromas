#ifndef MAS_DUF_FILE_PATHID_H
#  define MAS_DUF_FILE_PATHID_H

/* unsigned long long duf_file_to_filenameid( const char *path, const char *name, unsigned long long *pmd5id ); */
/* unsigned long long duf_md5id_of_file( const char *path, const char *name ); */
int duf_print_files_by_pathid( unsigned long long pathid, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, unsigned long long items );
int duf_scan_files_by_pathid( unsigned long long pathid, duf_scan_callback_file_t str_cb, duf_dirinfo_t * pdi,
                              duf_scan_callbacks_t * sccb );

#endif
