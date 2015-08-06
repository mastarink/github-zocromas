#ifndef MAS_DUF_PATH2DB_H
#  define MAS_DUF_PATH2DB_H


#  if 0
unsigned long long duf_dirname2dirid( duf_depthinfo_t * pdi, const char *dename, int caninsert, dev_t dev_id, ino_t dir_ino,
                                      const char *node_selector2, int need_id, int *pchanges, int *pr );
#  endif
unsigned long long duf_dirname_pdistat2dirid( duf_depthinfo_t * pdi, int caninsert, /* const char *dirname, const struct stat *pstat, */
                                           const char *node_selector2,
                                           int need_id, int *pchanges, int *pr );

int duf_real_path2db( duf_depthinfo_t * pdi, int caninsert, const char *rpath, const char *node_selector2 );

unsigned long long duf_path2dirid( const char *path, const char *node_selector2, int *pr );



#endif
