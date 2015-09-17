#ifndef MAS_SQLITE_TYPES_H
#  define MAS_SQLITE_TYPES_H

#  include <sqlite3.h>
typedef sqlite3_stmt mas_sqlite_stmt_t;

typedef int ( *mas_sqexe_cb_t ) ( void *sqexe_data, int ncolumns, char **presult, char **pnames );


#endif
