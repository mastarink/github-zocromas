#ifndef MAS_DUF_PATH2DIRID_H
#  define MAS_DUF_PATH2DIRID_H

unsigned long long duf_path2dirid( const char *path, const duf_sql_set_t * sql_set, int *pr );
char *duf_dirid2path( unsigned long long dirid, int *pr );

#endif
