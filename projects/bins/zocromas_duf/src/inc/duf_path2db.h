#ifndef MAS_DUF_PATH2DB_H
#  define MAS_DUF_PATH2DB_H


unsigned long long duf_dirname_pdistat2dirid( duf_depthinfo_t * pdi, int caninsert, /* const char *dirname, const struct stat *pstat, */
                                              const duf_sql_set_t * sql_set /* const char *node_selector2 */ ,
                                              int need_id, int *pchanges, int *pr );

int duf_real_path2db( duf_depthinfo_t * pdi, int caninsert, const char *rpath, const duf_sql_set_t * sql_set /* const char *node_selector2 */  );

#endif
