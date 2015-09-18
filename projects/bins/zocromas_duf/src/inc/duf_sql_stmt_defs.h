#ifndef MAS_DUF_SQL_STMT_DEFS_H
#  define MAS_DUF_SQL_STMT_DEFS_H

#  include <mastar/sqlite/mas_sqlite_const.h>


#  define DUF_SQL_START_STMT_NOPDI( _sql, _rt, _pstmt_m ) \
	{ \
  	  duf_stmnt_t *_pstmt_m = NULL; \
          if ( _rt >= 0 ) \
            _rt = duf_sql_prepare( _sql, &_pstmt_m ); \
          DUF_TEST_R( _rt )

#  define DUF_SQL_END_STMT_NOPDI(_rt, _pstmt_m) \
	  { \
	    int __rf = duf_sql_finalize( _pstmt_m ); \
	    _pstmt_m = NULL; \
	    if ( _rt >= 0 || _rt == MAS_SQL_ROW || _rt == MAS_SQL_DONE ) \
	      _rt = __rf; \
	    DUF_TEST_R( _rt ); \
	  } \
	}


#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_START_STMT( _pdi, _name, _sql, _rt, _pstmt_m ) \
	{ \
  	  duf_stmnt_t *_pstmt_m = NULL; \
          /* TODO : static not always or never? */ \
          if ( _rt >= 0 ) \
            _pstmt_m = duf_pdi_find_statement_by_id( _pdi, DUF_SQL_STMD_ID_ ## _name ); \
          if ( _rt>=0 && !_pstmt_m ) \
            _pstmt_m = duf_pdi_prepare_statement_by_id( _pdi, _sql,  DUF_SQL_STMD_ID_ ## _name, &_rt ); \
          if ( _rt >= 0 && !_pstmt_m ) \
            _rt = DUF_ERROR_PDI_SQL; \
          DUF_TEST_R( _rt );
#  elif defined(DUF_SQL_NOPDI_STMT)
#    define DUF_SQL_START_STMT( _pdi, _name, _sql, _rt, _pstmt_m ) \
	DUF_SQL_START_STMT_NOPDI( _sql, _rt, _pstmt_m );
#  else
#    undef DUF_SQL_START_STMT
#  endif


#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_END_STMT(_pdi, _name, _rt, _pstmt_m) \
	  if ( _rt == MAS_SQL_ROW || _rt == MAS_SQL_DONE ) \
	      _rt = 0; \
          if ( _rt >= 0 && !_pstmt_m ) \
            _rt = DUF_ERROR_PDI_SQL; \
	  if (_pstmt_m) \
	  { duf_sql_reset( _pstmt_m ); duf_sql_clear_bindings( _pstmt_m ); } \
	  duf_pdi_finalize_statement_by_id(_pdi, DUF_SQL_STMD_ID_ ## _name ); \
	}
#  elif defined(DUF_SQL_NOPDI_STMT)
#    define DUF_SQL_END_STMT(_pdi, _rt, _pstmt_m) \
	DUF_SQL_END_STMT_NOPDI(_rt, _pstmt_m)
#  else
#    undef DUF_SQL_END_STMT
#  endif



#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_START_STMT_LOCAL( _pdi,  _sql, _rt, _pstmt_m ) \
	{ \
  	  duf_stmnt_t *_pstmt_m = NULL; \
          /* TODO : static not always or never? */ \
          if ( _rt>=0 && !_pstmt_m ) \
            _pstmt_m = duf_pdi_prepare_statement_by_id( _pdi, _sql, ( duf_stmt_ident_t ) 0, &_rt ); \
          if ( _rt >= 0 && !_pstmt_m ) \
            _rt = DUF_ERROR_PDI_SQL; \
          DUF_TEST_R( _rt );
#  else
#    undef DUF_SQL_START_STMT_LOCAL
#  endif


#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_END_STMT_LOCAL(_pdi, _rt, _pstmt_m) \
	  if ( _rt == MAS_SQL_ROW || _rt == MAS_SQL_DONE ) \
	      _rt = 0; \
          if ( _rt >= 0 && !_pstmt_m ) \
            _rt = DUF_ERROR_PDI_SQL; \
	  if (_pstmt_m) \
	  { duf_sql_reset( _pstmt_m ); duf_sql_clear_bindings( _pstmt_m ); } \
	  duf_pdi_finalize_statement_by_stmt(_pdi, _pstmt_m ); \
	}
#  else
#    undef DUF_SQL_END_STMT_LOCAL
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


#  define DUF_SQL_STEP( _rt, _pstmt_m )		if ( _rt >= 0 || _rt==MAS_SQL_ROW ) DOR_NOE(_rt, duf_sql_step( _pstmt_m ), MAS_SQL_ROW, MAS_SQL_DONE)
#  define DUF_SQL_CHANGES_NOPDI( _changes, _rt, _pstmt_m ) if ( _rt >= 0 || _rt==MAS_SQL_ROW || _rt==MAS_SQL_DONE ) _changes = duf_sql_changes(  )
#  define DUF_SQL_CHANGES( _changes, _rt, _pstmt_m )  DUF_SQL_CHANGES_NOPDI( _changes, _rt, _pstmt_m );  duf_pdi_reg_changes( pdi, _changes )

#  define DUF_SQL_EACH_ROW(_rt, _pstmt1, _ops) while ( _rt >= 0 && _rt != MAS_SQL_DONE ) \
	{ \
	  DUF_SQL_STEP( _rt, _pstmt1 ); \
	  if ( _rt == MAS_SQL_ROW ) \
	  { \
	    _rt = 0; \
	    _ops ; \
	  } \
	}


#endif
