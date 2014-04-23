#ifndef MAS_DUF_DIR_SCAN_H
#  define MAS_DUF_DIR_SCAN_H

/* duf_scan_dirs_by_parentid
 * 1. for <current> dir call sccb->directory_scan
 * 2. for each dir in <current> dir call str_cb + str_cb_udata
 * 3. for each file in <current> dir call sccb->file_scan
 * see duf_scan_dir_by_pi
 * */
int duf_scan_dirs_by_parentid( unsigned long long pathid, const duf_dirhandle_t * pdh, duf_scan_callback_file_t str_cb,
                               duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, duf_record_t * precord );



int duf_print_dirs( const char *path, duf_ufilter_t u, int tree );


/* will be static! */
int
duf_scan_file( void *str_cb_udata, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s *sccb, duf_record_t * precord,
               const duf_dirhandle_t * pdhu );


#endif
