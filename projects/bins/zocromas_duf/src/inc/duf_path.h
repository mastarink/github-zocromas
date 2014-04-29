#ifndef MAS_DUF_PATH_H
#  define MAS_DUF_PATH_H

char *duf_join_path( const char *path, const char *fname );

/* char *duf_pathid_to_path_dh( unsigned long long pathid, duf_dirhandle_t * pdh, const duf_depthinfo_t * pdi, int *pr ); */
#ifdef DUF_COMPILE_EXPIRED
char *duf_pathid_to_path_s( unsigned long long pathid, const duf_depthinfo_t * pdi, int *pr );
#endif

/* unsigned long long duf_path_to_pathid_x( const char *path, unsigned long long *pprevpathid, char **notfound, duf_depthinfo_t * pdi, */
/*                                          int *pr );                                                                                 */


unsigned long long duf_path_to_pathid( const char *path, duf_depthinfo_t * pdi, int *pr );






/* will be static! */
int duf_sel_cb_name_parid( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                           duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb/*, const duf_dirhandle_t * pdhu_unused_off */ );

/* will be static! */
int duf_sel_cb_dirid( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                      duf_depthinfo_t * xpdi, duf_scan_callbacks_t * sccb/*, const duf_dirhandle_t * pdhu_unused_off */ );







#endif
