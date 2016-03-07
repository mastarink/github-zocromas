#ifndef MAS_DUF_SQL_BIND_H
# define MAS_DUF_SQL_BIND_H

# include "duf_sql_types.h"                                          /* duf_stmnt_t ✗ */
/* # include "duf_se.h"                                                 (* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ *) */

DR( SQL, sql_bindu_long_long, duf_stmnt_t * stmt, const char *fldname, int pi, long long value, int opt );
DR( SQL, sql_bindu_long_long_nz, duf_stmnt_t * stmt, const char *fldname, int pi, long long value, int opt );
DR( SQL, sql_bindu_int, duf_stmnt_t * stmt, const char *fldname, int pi, int value, int opt );
DR( SQL, sql_bindu_int_nz, duf_stmnt_t * stmt, const char *fldname, int pi, int value, int opt );
DR( SQL, sql_bindu_string, duf_stmnt_t * stmt, const char *fldname, int pi, const char *value, int opt );

DR( SQL, sql_bindn_long_long, duf_stmnt_t * stmt, const char *fldname, long long value, int opt );
DR( SQL, sql_bindn_long_long_nz, duf_stmnt_t * stmt, const char *fldname, long long value, int opt );
DR( SQL, sql_bindn_int, duf_stmnt_t * stmt, const char *fldname, int value, int opt );
DR( SQL, sql_bindn_int_nz, duf_stmnt_t * stmt, const char *fldname, int value, int opt );
DR( SQL, sql_bindn_string, duf_stmnt_t * stmt, const char *fldname, const char *value, int opt );

DR( SQL, sql_bind_long_long, duf_stmnt_t * stmt, const char *fldname, long long value, int opt );
DR( SQL, sql_bind_long_long_nz, duf_stmnt_t * stmt, const char *fldname, long long value, int opt );
DR( SQL, sql_bind_int, duf_stmnt_t * stmt, const char *fldname, int value, int opt );
DR( SQL, sql_bind_int_nz, duf_stmnt_t * stmt, const char *fldname, int value, int opt );
DR( SQL, sql_bind_string, duf_stmnt_t * stmt, const char *fldname, const char *value, int opt );

void duf_sql_clear_bindings( duf_stmnt_t * stmt );

const char *duf_sql_stmt( mas_sqlite_stmt_t * stmt );

#endif
