#ifndef MAS_DUF_SQL_FIELD_H
#  define MAS_DUF_SQL_FIELD_H



/* for DUF_GET_?FIELD2 */
int __duf_sql_pos_by_name2( duf_stmnt_t * pstmt, const char *name );
const char *__duf_sql_str_by_name2( duf_stmnt_t * pstmt, const char *name, int opt );
unsigned long long __duf_sql_ull_by_name2( duf_stmnt_t * pstmt, const char *name, int opt );




#endif
