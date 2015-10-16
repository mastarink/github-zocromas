#ifndef MAS_EVSQL_SELECTOR_H
#  define MAS_EVSQL_SELECTOR_H

char *duf_selector2sql( const duf_sql_set_t * sql_set, const char *selected_db, int *pr );

char *duf_selector_total2sql( const duf_sql_set_t * sql_set, const char *selected_db, int *pr );

char *duf_expand_sql( const char *sql, const char *dbname );

#endif
