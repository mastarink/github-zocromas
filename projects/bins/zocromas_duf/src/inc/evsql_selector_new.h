#ifndef MAS_EVSQL_SELECTOR_NEW_H
#  define MAS_EVSQL_SELECTOR_NEW_H

char *duf_selector2sql_new( const duf_sql_set_t * sql_set, const char *selected_db, unsigned total, int *pr );
char *duf_expand_sql_new( const char *sql, const char *dbname );


#endif
