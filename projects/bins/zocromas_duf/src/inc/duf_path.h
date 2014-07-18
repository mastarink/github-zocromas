#ifndef MAS_DUF_PATH_H
#  define MAS_DUF_PATH_H

#  include "duf_levinfo_types.h"

char *duf_join_path( const char *path, const char *fname );

/* char *duf_pathid_to_path_dh( unsigned long long pathid, duf_dirhandle_t * pdh, const duf_depthinfo_t * pdi, int *pr ); */
#  ifdef DUF_COMPILE_EXPIRED
char *duf_pathid_to_path_s( unsigned long long pathid, const duf_depthinfo_t * pdi, int *pr );
#  endif
char *duf_pathid_to_path2( unsigned long long dirid, const duf_depthinfo_t * pdi, int *pr );

/* unsigned long long duf_path_to_pathid_x( const char *path, unsigned long long *pprevpathid, char **notfound, duf_depthinfo_t * pdi, */
/*                                          int *pr );                                                                                 */


unsigned long long duf_path_to_pathid( const char *path, duf_depthinfo_t * pdi, int *pr );


int duf_pathdepth( const char *path );





#  ifdef DUF_COMPILE_EXPIRED
/* will be static! */
int duf_sel_cb_name_parid( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                           duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb );
#  endif

/* will be static! */
/* int duf_sel_cb_levinfo( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata, */
/*                         duf_depthinfo_t * xpdi, duf_scan_callbacks_t * sccb );                               */


unsigned long long duf_insert_path_uni( duf_depthinfo_t * pdi, const char *dename, dev_t dev_id, ino_t dir_ino, unsigned long long parentid,
                                        int need_id, int *pchanges, int *pr );
unsigned long long duf_insert_path_uni2( duf_depthinfo_t * pdi, const char *dename, int ifadd, duf_levinfo_t * pli, dev_t dev_id,
                                         ino_t dir_ino, unsigned long long parentid, int need_id, int *pchanges, int *pr );
int duf_real_path2db( duf_depthinfo_t * pdi, const char *rpath, int ifadd );





#endif
