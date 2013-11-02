#ifndef MAS_DUF_PATH_H
#  define MAS_DUF_PATH_H

char *duf_join_path( const char *path, const char *fname );

char *duf_pathid_to_path( unsigned long long pathid );
unsigned long long duf_path_to_pathid_x( const char *path, unsigned long long *pprevpathid, char **notfound );
unsigned long long duf_path_to_pathid( const char *path );
int duf_print_paths( const char *groupname );

/* 
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 *             int fun( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata )
 * */
int duf_sql_scan_paths( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata );


int duf_pathid_group( const char *group, unsigned long long pathid );
int duf_paths_group( const char *group, const char *path );

unsigned long long duf_group_to_groupid( const char *group );

#endif
