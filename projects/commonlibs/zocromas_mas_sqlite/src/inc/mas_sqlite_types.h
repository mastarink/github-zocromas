#ifndef MAS_SQLITE_TYPES_H
#  define MAS_SQLITE_TYPES_H

#include <sqlite3.h>
typedef sqlite3_stmt duf_sqlite_stmt_t;

typedef int ( *duf_sqexe_cb_t ) ( void *sqexe_data, int ncolumns, char **presult, char **pnames );


#endif
