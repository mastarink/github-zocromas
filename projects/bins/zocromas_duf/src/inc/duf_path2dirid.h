#ifndef MAS_DUF_PATH2DIRID_H
#  define MAS_DUF_PATH2DIRID_H

unsigned long long duf_path2dirid( const char *path, const duf_sql_set_t * sql_set /* const char *node_selector2 */ , int *pr );
char *duf_dirid2path( duf_depthinfo_t * pdi, unsigned long long dirid, int *pr );

#endif

