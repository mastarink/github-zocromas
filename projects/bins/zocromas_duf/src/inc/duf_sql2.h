#ifndef MAS_DUF_SQL2_H
#  define MAS_DUF_SQL2_H

#  include "duf_sql_def.h"



int duf_sql_prepare( const char *sql, duf_sqlite_stmt_t ** pstmt );
int duf_sql_step( duf_sqlite_stmt_t * stmt );
int duf_sql_finalize( duf_sqlite_stmt_t * stmt );

int duf_sql_bind_long_long( duf_sqlite_stmt_t * stmt, const char *fldname, long long value );
int duf_sql_bind_long_long_nz( duf_sqlite_stmt_t * stmt, const char *fldname, long long value );
int duf_sql_bind_int( duf_sqlite_stmt_t * stmt, const char *fldname, int value );
int duf_sql_bind_int_nz( duf_sqlite_stmt_t * stmt, const char *fldname, int value );
int duf_sql_bind_string( duf_sqlite_stmt_t * stmt, const char *fldname, const char *value );

int duf_sql_column_int( duf_sqlite_stmt_t * stmt, int icol );
long long duf_sql_column_long_long( duf_sqlite_stmt_t * stmt, int icol );
const char *duf_sql_column_string( duf_sqlite_stmt_t * stmt, int icol );

const char *duf_sql_column_name( duf_sqlite_stmt_t * stmt, int index );
int duf_sql_column_count( duf_sqlite_stmt_t * stmt );

#endif
