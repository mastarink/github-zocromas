#ifndef MAS_DUF_PATH2DB_H
#  define MAS_DUF_PATH2DB_H

#  include "duf_levinfo_types.h"


/* char *duf_pathid_to_path_dh( unsigned long long pathid, duf_dirhandle_t * pdh, const duf_depthinfo_t * pdi, int *pr ); */
#  ifdef DUF_COMPILE_EXPIRED
char *duf_pathid_to_path_s( unsigned long long pathid, const duf_depthinfo_t * pdi, int *pr );
#  endif
char *duf_pathid_to_path2( unsigned long long dirid, const duf_depthinfo_t * pdi, int *pr );
const char *duf_pathid_to_path2_tmp( int index, unsigned long long dirid, const duf_depthinfo_t * pdi );


/* unsigned long long duf_path_to_pathid_x( const char *path, unsigned long long *pprevpathid, char **notfound, duf_depthinfo_t * pdi, */
/*                                          int *pr );                                                                                 */


unsigned long long duf_path_to_pathid( const char *path, duf_depthinfo_t * pdi, int *pr );



unsigned long long duf_insert_path_uni2( duf_depthinfo_t * pdi, const char *dename, int ifadd, dev_t dev_id,
                                         ino_t dir_ino /*, unsigned long long parentid : unused */ , int need_id, int *pchanges, int *pr );
int duf_real_path2db( duf_depthinfo_t * pdi, const char *rpath, int ifadd );





#endif

