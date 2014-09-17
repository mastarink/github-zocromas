#ifndef MAS_DUF_SQL_H
#  define MAS_DUF_SQL_H

#  include "duf_sql_const.h"


int duf_sql_open( const char *dbpath );
int duf_sql_close( void );

/* char *duf_sql_vmprintf( const char *fmt, va_list args ); */
char *duf_sql_mprintf( const char *fmt, ... );

/* void duf_sql_free( char *s ); */
void duf_sql_clear_bindings( duf_sqlite_stmt_t * stmt );

unsigned long long duf_sql_last_insert_rowid( void );

int duf_sql_changes( void );

#endif
