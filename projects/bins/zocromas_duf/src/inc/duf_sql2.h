#ifndef MAS_DUF_SQL2_H
#  define MAS_DUF_SQL2_H


#  include "duf_sql_types.h"

#  include <mastar/sqlite/mas_sqlite_const.h>

int duf_sql_prepare( const char *sql, duf_stmnt_t ** pstmt );
int duf_sql_step( duf_stmnt_t * stmt );
int duf_sql_finalize( duf_stmnt_t * stmt );
int duf_sql_reset( duf_stmnt_t * stmt );


int duf_sql_column_int( duf_stmnt_t * stmt, int icol );
long long duf_sql_column_long_long( duf_stmnt_t * stmt, int icol );
const char *duf_sql_column_string( duf_stmnt_t * stmt, int icol );

const char *duf_sql_column_name( duf_stmnt_t * stmt, int index );
int duf_sql_column_count( duf_stmnt_t * stmt );


#endif
