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

#  define DUF_SQL_START_STMT_NOPDI( _sql, _rt, _pstmt_m ) \
	{ \
  	  duf_sqlite_stmt_t *_pstmt_m = NULL; \
          if ( _rt >= 0 ) \
            _rt = duf_sql_prepare( _sql, &_pstmt_m ); \
          DUF_TEST_R( _rt )
#  define DUF_SQL_END_STMT_NOPDI(_rt, _pstmt_m) \
	  { \
	    int __rf = duf_sql_finalize( _pstmt_m ); \
	    _pstmt_m = NULL; \
	    if ( _rt >= 0 || _rt == DUF_SQL_ROW || _rt == DUF_SQL_DONE ) \
	      _rt = __rf; \
	    DUF_TEST_R( _rt ); \
	  } \
	}


/* DUF_SHOW_ERROR("%s_index: %d, %p s:%p", #name, name ## _index, (void*)pdi, (void*)(pdi?pdi->statements:NULL)); \ */
/* DUF_SHOW_ERROR("%s_index: %d", #name, name ## _index); \                                                         */

#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_START_STMT( _pdi, _name, _sql, _rt, _pstmt_m ) \
	{ \
  	  duf_sqlite_stmt_t *_pstmt_m = NULL; \
          static int _name ## _index = -1; \
          if ( _rt >= 0 ) \
            _pstmt_m = duf_pdi_find_statement( _pdi, &_name ## _index ); \
          if ( _rt>=0 && !_pstmt_m ) \
            _pstmt_m = duf_pdi_prepare_statement( _pdi, _sql, &_name ## _index, &_rt ); \
          if ( _rt >= 0 && !_pstmt_m ) \
            _rt = DUF_ERROR_PDI_SQL; \
          DUF_TEST_R( _rt );
#  else
#    define DUF_SQL_START_STMT( _pdi, _name, _sql, _rt, _pstmt_m ) \
	DUF_SQL_START_STMT_NOPDI( _sql, _rt, _pstmt_m );
#  endif


#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_END_STMT(_name, _rt, _pstmt_m) \
	  if ( _rt == DUF_SQL_ROW || _rt == DUF_SQL_DONE ) \
	      _rt = 0; \
          if ( _rt >= 0 && !_pstmt_m ) \
            _rt = DUF_ERROR_PDI_SQL; \
	  if (_pstmt_m) \
	  { duf_sql_reset( _pstmt_m ); duf_sql_clear_bindings( _pstmt_m ); } \
	  duf_pdi_finalize_statement(pdi, &_name ## _index ); \
	}
#  else
#    define DUF_SQL_END_STMT(_rt, _pstmt_m) \
	DUF_SQL_END_STMT_NOPDI(_rt, _pstmt_m)
#  endif



#  define DUF_SQL_BIND_LL( _name, _value, _rt, _pstmt_m ) \
		    DOR(_rt, duf_sql_bindn_long_long( _pstmt_m, ":" #_name, _value ));
#  define DUF_SQL_BIND_LL_NZ( _name, _value, _rt, _pstmt_m ) \
  		    DOR(_rt, duf_sql_bindn_long_long_nz( _pstmt_m, ":" #_name, _value ));
#  define DUF_SQL_BIND_LL_NZ_OPT( _name, _value, _rt, _pstmt_m ) \
		  if ( _rt >= 0 ) \
		  {  \
		    DUF_E_NO( DUF_ERROR_BIND_NAME ); \
                    DOR(_rt, duf_sql_bindn_long_long_nz( _pstmt_m, ":" #_name, _value )); \
		    DUF_CLEAR_ERROR(_rt, DUF_ERROR_BIND_NAME); \
		    DUF_E_YES( DUF_ERROR_BIND_NAME ); \
		  }

#  define DUF_SQL_BIND_S( _name, _value, _rt, _pstmt_m ) \
		    DOR(_rt, duf_sql_bindn_string( _pstmt_m, ":" #_name, _value ));
#  define DUF_SQL_BIND_S_OPT( _name, _value, _rt, _pstmt_m ) \
		  if ( _rt >= 0 ) \
		  { \
		    DUF_E_NO( DUF_ERROR_BIND_NAME ); \
		    DOR(_rt, duf_sql_bindn_string( _pstmt_m, ":" #_name, _value )); \
		    DUF_CLEAR_ERROR(_rt, DUF_ERROR_BIND_NAME); \
		    DUF_E_YES( DUF_ERROR_BIND_NAME ); \
		  }


#  define DUF_SQL_STEP( _rt, _pstmt_m )		if ( _rt >= 0 || _rt==DUF_SQL_ROW ) DOR_NOE(_rt, duf_sql_step( _pstmt_m ), DUF_SQL_ROW, DUF_SQL_DONE)
#  define DUF_SQL_CHANGES_NOPDI( _changes, _rt, _pstmt_m ) if ( _rt >= 0 || _rt==DUF_SQL_ROW || _rt==DUF_SQL_DONE ) _changes = duf_sql_changes(  )
#  define DUF_SQL_CHANGES( _changes, _rt, _pstmt_m )  DUF_SQL_CHANGES_NOPDI( _changes, _rt, _pstmt_m );  duf_pdi_reg_changes( pdi, _changes )

#  define DUF_SQL_EACH_ROW(_rt, _pstmt1, _ops) while ( _rt >= 0 && _rt != DUF_SQL_DONE ) \
	{ \
	  DUF_SQL_STEP( _rt, _pstmt1 ); \
	  if ( _rt == DUF_SQL_ROW ) \
	  { \
	    _ops ; \
	  } \
	}


#endif
