#ifndef MAS_DUF_SQL_H
#  define MAS_DUF_SQL_H

#  include "duf_sql_def.h"


int duf_sql_open( const char *dbpath );
int duf_sql_close( void );



unsigned long long duf_sql_last_insert_rowid( void );

int duf_sql_changes( void );

#endif
