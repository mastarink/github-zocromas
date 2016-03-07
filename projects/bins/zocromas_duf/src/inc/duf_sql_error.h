#ifndef MAS_DUF_SQL_ERROR_H
# define MAS_DUF_SQL_ERROR_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */
int duf_r2sqlite_error_code( mas_error_code_t rt );

mas_error_code_t duf_sqlite2r_error_code( int r3 );
int duf_sqlite2duf( int r3, const char *func, int line );

#endif
