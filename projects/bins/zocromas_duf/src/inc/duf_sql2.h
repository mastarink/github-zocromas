#ifndef MAS_DUF_SQL2_H
#  define MAS_DUF_SQL2_H

#  include "duf_sql_def.h"



int duf_sql_prepare( const char *sql, duf_sqlite_stmt_t ** pstmt );
int duf_sql_step( duf_sqlite_stmt_t * stmt );
int duf_sql_finalize( duf_sqlite_stmt_t * stmt );
int duf_sql_reset( duf_sqlite_stmt_t * stmt );

int duf_sql_bindu_long_long( duf_sqlite_stmt_t * stmt, const char *fldname, int pi, long long value );
int duf_sql_bindu_long_long_nz( duf_sqlite_stmt_t * stmt, const char *fldname, int pi, long long value );
int duf_sql_bindu_int( duf_sqlite_stmt_t * stmt, const char *fldname, int pi, int value );
int duf_sql_bindu_int_nz( duf_sqlite_stmt_t * stmt, const char *fldname, int pi, int value );
int duf_sql_bindu_string( duf_sqlite_stmt_t * stmt, const char *fldname, int pi, const char *value );

int duf_sql_bindn_long_long( duf_sqlite_stmt_t * stmt, const char *fldname, long long value );
int duf_sql_bindn_long_long_nz( duf_sqlite_stmt_t * stmt, const char *fldname, long long value );
int duf_sql_bindn_int( duf_sqlite_stmt_t * stmt, const char *fldname, int value );
int duf_sql_bindn_int_nz( duf_sqlite_stmt_t * stmt, const char *fldname, int value );
int duf_sql_bindn_string( duf_sqlite_stmt_t * stmt, const char *fldname, const char *value );

int duf_sql_bind_long_long( duf_sqlite_stmt_t * stmt, const char *fldname, long long value );
int duf_sql_bind_long_long_nz( duf_sqlite_stmt_t * stmt, const char *fldname, long long value );
int duf_sql_bind_int( duf_sqlite_stmt_t * stmt, const char *fldname, int value );
int duf_sql_bind_int_nz( duf_sqlite_stmt_t * stmt, const char *fldname, int value );
int duf_sql_bind_string( duf_sqlite_stmt_t * stmt, const char *fldname, const char *value );

int duf_sql_column_int( duf_sqlite_stmt_t * stmt, int icol );
long long duf_sql_column_long_long( duf_sqlite_stmt_t * stmt, int icol );
const char *duf_sql_column_string( duf_sqlite_stmt_t * stmt, int icol );

const char *duf_sql_column_name( duf_sqlite_stmt_t * stmt, int index );
int duf_sql_column_count( duf_sqlite_stmt_t * stmt );

#define DUF_SQL_START_STMT_NOPDI( sql, r, pstmt ) \
	{ \
  	  duf_sqlite_stmt_t *pstmt = NULL; \
          if ( r >= 0 ) \
            r = duf_sql_prepare( sql, &pstmt ); \
          DUF_TEST_R( r )
#define DUF_SQL_END_STMT_NOPDI(r, pstmt) \
	  { \
	    int rf = duf_sql_finalize( pstmt ); \
	    pstmt = NULL; \
	    if ( r >= 0 || r == DUF_SQL_DONE ) \
	      r = rf; \
	    DUF_TEST_R( r ); \
	  } \
	}


/* DUF_ERROR("%s_index: %d, %p s:%p", #name, name ## _index, (void*)pdi, (void*)(pdi?pdi->statements:NULL)); \ */
/* DUF_ERROR("%s_index: %d", #name, name ## _index); \                                                         */

#ifdef DUF_SQL_PDI_STMT
#define DUF_SQL_START_STMT( pdi, name, sql, r, pstmt ) \
	{ \
  	  duf_sqlite_stmt_t *pstmt = NULL; \
          static int name ## _index = -1; \
          if ( name ## _index < 0 ) \
            r = duf_pdi_prepare_statement( pdi, sql, &name ## _index ); \
          if ( r >= 0 && name ## _index >= 0 ) \
            pstmt = duf_pdi_statement( pdi, name ## _index ); \
          if ( r >= 0 && ( !pstmt || name ## _index < 0 ) ) \
            r = DUF_ERROR_PDI_SQL; \
          DUF_TEST_R( r );
#else
#define DUF_SQL_START_STMT( pdi, name, sql, r, pstmt ) \
	DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
#endif


#ifdef DUF_SQL_PDI_STMT
#define DUF_SQL_END_STMT(r, pstmt) \
	  if ( r == DUF_SQL_DONE ) \
	      r = 0; \
          if ( r >= 0 && !pstmt ) \
            r = DUF_ERROR_PDI_SQL; \
	  if (pstmt) \
	  { duf_sql_reset( pstmt ); duf_sql_clear_bindings( pstmt ); } \
	}
#else
#define DUF_SQL_END_STMT(r, pstmt) \
	DUF_SQL_END_STMT_NOPDI(r, pstmt)
#endif

#define DUF_SQL_BIND_LL( name, value, r, pstmt ) { if ( r >= 0 ) r = duf_sql_bindn_long_long( pstmt, ":" #name, value ); DUF_TEST_R( r ); }
#define DUF_SQL_BIND_S( name, value, r, pstmt ) { if ( r >= 0 ) r = duf_sql_bindn_string( pstmt, ":" #name, value ); DUF_TEST_R( r ); }

#define DUF_SQL_STEP( r, pstmt ) if ( r >= 0 ) r = duf_sql_step( pstmt )
#define DUF_SQL_CHANGES( changes, r, pstmt ) { if ( r >= 0 ) changes = duf_sql_changes(  );  duf_pdi_reg_changes( pdi, changes ); }
#define DUF_SQL_CHANGES_NOPDI( changes, r, pstmt ) { if ( r >= 0 ) changes = duf_sql_changes(  ); }



#endif
