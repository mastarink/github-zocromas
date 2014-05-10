#ifndef MAS_DUF_SQL_FIELD_H
#  define MAS_DUF_SQL_FIELD_H




int __duf_sql_pos_by_name2( duf_sqlite_stmt_t * pstmt, const char *name );
const char *__duf_sql_str_by_name2( duf_sqlite_stmt_t * pstmt, const char *name );
unsigned long long __duf_sql_ull_by_name2(  duf_sqlite_stmt_t * pstmt,const char *name );




#endif
