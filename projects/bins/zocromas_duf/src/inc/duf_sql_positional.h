#ifndef MAS_DUF_SQL_POSITIONAL_H
#  define MAS_DUF_SQL_POSITIONAL_H


#  include "duf_sql_types.h"



int duf_sql_column_int( duf_stmnt_t * stmt, int icol );
long long duf_sql_column_long_long( duf_stmnt_t * stmt, int icol );
const char *duf_sql_column_string( duf_stmnt_t * stmt, int icol );


#endif
