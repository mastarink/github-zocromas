#ifndef MAS_DUF_SQL_H
#  define MAS_DUF_SQ_ERROR_H


int duf_r2sqlite_error_code( int rt );

duf_error_code_t duf_sqlite2r_error_code( int r3 );
duf_error_index_t duf_sqlite2duf( int r3, const char *func, int line );


#endif
