#ifndef MAS_DUF_SQL_PREPARED_H
# define MAS_DUF_SQL_PREPARED_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

DR( SQL, sql_prepare, const char *sql, duf_stmnt_t ** psqlstmt );
int duf_sql_step( duf_stmnt_t * stmt );
int duf_sql_finalize( duf_stmnt_t * stmt );
int duf_sql_reset( duf_stmnt_t * stmt );

int duf_sql_changes( void );

const char *duf_sql_column_name( duf_stmnt_t * stmt, int index );
const char *duf_sql_column_decltype( mas_sqlite_stmt_t * stmt, int index );
duf_sqltype_t duf_sql_column_type( mas_sqlite_stmt_t * stmt, int index );

int duf_sql_column_count( duf_stmnt_t * stmt );

unsigned long long duf_sql_last_insert_rowid( void );

#endif
