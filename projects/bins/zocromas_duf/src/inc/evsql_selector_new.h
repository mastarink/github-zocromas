#ifndef MAS_EVSQL_SELECTOR_NEW_H
#  define MAS_EVSQL_SELECTOR_NEW_H

#include "duf_hook_types.h" /* duf_sql_set_t */

char *duf_selector2sql_new( const duf_sql_set_t * sql_set, const char *selected_db, unsigned total, int *pr );
char *duf_selector2sql_2new( const duf_sql_set_t * sql_set, const duf_sql_set_t * sql_set2, const char *selected_db, unsigned total, int *pr );
char *duf_expand_sql( const char *sql, const char *dbname );


#endif
