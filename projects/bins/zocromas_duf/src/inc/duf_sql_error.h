#ifndef MAS_DUF_SQL_ERROR_H
# define MAS_DUF_SQL_ERROR_H

# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

int duf_r2sqlite_error_code( mas_error_code_t rt );

mas_error_code_t duf_sqlite2r_error_code( int r3 );
DR( SQL, sqlite2duf, int r3, const char *func, int line, const char *sql, mas_sqlite_stmt_t * stmt );

#endif
