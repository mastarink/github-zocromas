#ifndef MAS_DUF_PATH2DB_H
#  define MAS_DUF_PATH2DB_H

#  include "duf_levinfo_types.h"


/* unsigned long long duf_path_to_pathid_x( const char *path, unsigned long long *pprevpathid, char **notfound, duf_depthinfo_t * pdi, */
/*                                          int *pr );                                                                                 */


unsigned long long duf_path_to_pathid( const char *path, duf_depthinfo_t * pdi, int *pr );



unsigned long long duf_insert_path_uni2( duf_depthinfo_t * pdi, const char *dename, int tag, int caninsert, dev_t dev_id, ino_t dir_ino,
                                         const char *node_selector2 /*, unsigned long long parentid : unused */ , int need_id, int *pchanges,
                                         int *pr );
int duf_real_path2db( duf_depthinfo_t * pdi, const char *rpath, int tag, int caninsert, const char *node_selector2 );

unsigned long long duf_path2db( const char *path, const char *node_selector2, int *pr );




#endif
