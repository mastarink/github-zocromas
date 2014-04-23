#ifndef MAS_DUF_PATH_H
#  define MAS_DUF_PATH_H

char *duf_join_path( const char *path, const char *fname );

char *duf_pathid_to_path_dh( unsigned long long pathid, duf_dirhandle_t * pdh, const duf_depthinfo_t * pdi, int *pr );
char *duf_pathid_to_path_s( unsigned long long pathid, const duf_depthinfo_t * pdi, int *pr );
unsigned long long duf_path_to_pathid_x( const char *path, unsigned long long *pprevpathid, char **notfound, duf_depthinfo_t * pdi , int *pr);
unsigned long long duf_path_to_pathid( const char *path, duf_depthinfo_t * pdi, int *pr );
/* int duf_print_paths( const char *groupname ); */

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *             int fun( int nrow, int nrows, duf_record_t *precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata )
 * */
/* int duf_sql_scan_paths( duf_record_t * precord, va_list args, void *sel_cb_udata,                                                */
/*                         duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb ); */


void duf_pathid_group( const char *group, unsigned long long pathid, int add_remove );
void duf_paths_group( const char *group, const char *path, int add_remove );


unsigned long long duf_insert_path( const char *base_name, const struct stat *pst_dir, unsigned long long rs_up );


/* int                                                                                                                            */
/* duf_sql_path_to_pathid( duf_record_t * precord, va_list args, void *sel_cb_udata,                                              */
/*                         duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi, duf_scan_callbacks_t * cb ); */




/* will be static! */
int
duf_sel_cb_name_parid( duf_record_t * precord, va_list args, void *sel_cb_udata,
                       duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb,
                       const duf_dirhandle_t * pdhu );

/* will be static! */
int
duf_sel_cb_dirid( duf_record_t * precord, va_list args, void *sel_cb_udata,
                  duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * xpdi, duf_scan_callbacks_t * sccb,
                  const duf_dirhandle_t * pdhu );








#endif
