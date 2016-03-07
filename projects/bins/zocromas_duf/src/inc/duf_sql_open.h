#ifndef MAS_DUF_SQL_OPEN_H
# define MAS_DUF_SQL_OPEN_H

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

int duf_sql_open( const char *dbpath );
int duf_sql_close( void );

#endif
