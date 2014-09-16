#ifndef MAS_DUF_SQL2_H
#  define MAS_DUF_SQL2_H


#  include "duf_sql_types.h"


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

#  define DUF_SQL_START_STMT_NOPDI( sql, r, _pstmt_m ) \
	{ \
  	  duf_sqlite_stmt_t *_pstmt_m = NULL; \
          if ( r >= 0 ) \
            r = duf_sql_prepare( sql, &_pstmt_m ); \
          DUF_TEST_R( r )
#  define DUF_SQL_END_STMT_NOPDI(r, _pstmt_m) \
	  { \
	    int rf = duf_sql_finalize( _pstmt_m ); \
	    _pstmt_m = NULL; \
	    if ( r >= 0 || r == DUF_SQL_ROW || r == DUF_SQL_DONE ) \
	      r = rf; \
	    DUF_TEST_R( r ); \
	  } \
	}


/* DUF_SHOW_ERROR("%s_index: %d, %p s:%p", #name, name ## _index, (void*)pdi, (void*)(pdi?pdi->statements:NULL)); \ */
/* DUF_SHOW_ERROR("%s_index: %d", #name, name ## _index); \                                                         */

#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_START_STMT( pdi, name, sql, r, _pstmt_m ) \
	{ \
  	  duf_sqlite_stmt_t *_pstmt_m = NULL; \
          static int name ## _index = -1; \
          if ( r >= 0 ) \
            _pstmt_m = duf_pdi_find_statement( pdi, &name ## _index ); \
          if ( r>=0 && !_pstmt_m ) \
            _pstmt_m = duf_pdi_prepare_statement( pdi, sql, &name ## _index, &r ); \
          if ( r >= 0 && !_pstmt_m ) \
            r = DUF_ERROR_PDI_SQL; \
          DUF_TEST_R( r );
#  else
#    define DUF_SQL_START_STMT( pdi, name, sql, r, _pstmt_m ) \
	DUF_SQL_START_STMT_NOPDI( sql, r, _pstmt_m );
#  endif


#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_END_STMT(name, r, _pstmt_m) \
	  if ( r == DUF_SQL_ROW || r == DUF_SQL_DONE ) \
	      r = 0; \
          if ( r >= 0 && !_pstmt_m ) \
            r = DUF_ERROR_PDI_SQL; \
	  if (_pstmt_m) \
	  { duf_sql_reset( _pstmt_m ); duf_sql_clear_bindings( _pstmt_m ); } \
	  duf_pdi_finalize_statement(pdi, &name ## _index ); \
	}
#  else
#    define DUF_SQL_END_STMT(r, _pstmt_m) \
	DUF_SQL_END_STMT_NOPDI(r, _pstmt_m)
#  endif



#  define DUF_SQL_BIND_LL( name, value, r, _pstmt_m ) \
		{ \
		  if ( r >= 0 ) \
		  { \
		    r = duf_sql_bindn_long_long( _pstmt_m, ":" #name, value ); \
		    DUF_TEST_R( r ); \
		  } \
		}
#  define DUF_SQL_BIND_LL_NZ    ( name, value, r, _pstmt_m ) \
		  if ( r >= 0 ) \
		  { \
		    r = duf_sql_bindn_long_long_nz( _pstmt_m, ":" #name, value ); \
		    DUF_TEST_R( r ); \
		  }
#  define DUF_SQL_BIND_LL_NZ_OPT( name, value, r, _pstmt_m ) \
		  if ( r >= 0 ) \
		  {  \
                    r = duf_sql_bindn_long_long_nz( _pstmt_m, ":" #name, value ); \
		    if ( r == DUF_ERROR_BIND_NAME ) \
		      r = 0; \
		    DUF_TEST_R( r ); \
		  }

#  define DUF_SQL_BIND_S( name, value, r, _pstmt_m ) \
		  if ( r >= 0 ) \
		  { \
		    r = duf_sql_bindn_string( _pstmt_m, ":" #name, value ); \
		    DUF_TEST_R( r ); \
		  }
#  define DUF_SQL_BIND_S_OPT( name, value, r, _pstmt_m ) \
		  if ( r >= 0 ) \
		  { \
		    r = duf_sql_bindn_string( _pstmt_m, ":" #name, value ); \
		    if ( r == DUF_ERROR_BIND_NAME ) \
		      r = 0; \
		    DUF_TEST_R( r );\
		  }


#  define DUF_SQL_STEP( r, _pstmt_m )                   if ( r >= 0 || r==DUF_SQL_ROW ) r = duf_sql_step( _pstmt_m )
#  define DUF_SQL_CHANGES_NOPDI( changes, r, _pstmt_m ) if ( r >= 0 || r==DUF_SQL_ROW || r==DUF_SQL_DONE ) changes = duf_sql_changes(  )
#  define DUF_SQL_CHANGES( changes, r, _pstmt_m )  DUF_SQL_CHANGES_NOPDI( changes, r, _pstmt_m );  duf_pdi_reg_changes( pdi, changes )

#  define DUF_SQL_EACH_ROW(r1, pstmt1, ops) while ( r1 >= 0 && r1 != DUF_SQL_DONE ) \
	{ \
	  DUF_SQL_STEP( r1, pstmt1 ); \
	  DUF_TEST_RR( r1 ); \
	  if ( r1 == DUF_SQL_ROW ) \
	  { \
	    ops ; \
	  } \
	}


#endif
