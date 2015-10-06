#ifndef MAS_DUF_SQL_STMT_DEFS_H
#  define MAS_DUF_SQL_STMT_DEFS_H


#  include "duf_sql_types.h"


#  define DUF_SQL_START_STMT_NOPDI( _sql, _rt, _pstmt_m ) \
	{ \
  	  duf_stmnt_t *_pstmt_m = NULL; \
	  DUF_TRACE(sql, 0, "START/NOPDI:%s", _sql); \
          if ( DUF_NOERROR(_rt) ) \
            _rt = duf_sql_prepare( _sql, &_pstmt_m ); \
          DUF_TEST_R( _rt )

#  define DUF_SQL_END_STMT_NOPDI(_rt, _pstmt_m) \
	  { \
	    int __rf = duf_sql_finalize( _pstmt_m ); \
	    DUF_CLEAR_ERROR(_rt, DUF_SQL_ROW, DUF_SQL_DONE ); \
	    if ( DUF_NOERROR(_rt) ) \
	      _rt = __rf; \
	    _pstmt_m = NULL; \
	    DUF_TEST_R( _rt ); \
	  } \
	}


#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_START_STMT( _pdi, _name, _sql, _rt, _pstmt_m ) \
	{ \
  	  duf_stmnt_t *_pstmt_m = NULL; \
	  DUF_TRACE(sql, 0, "START:%s", _sql); \
          /* TODO : static not always or never? */ \
          if ( DUF_NOERROR(_rt) ) \
            _pstmt_m = duf_pdi_find_statement_by_id( _pdi, DUF_SQL_STMD_ID_ ## _name ); \
          if ( DUF_NOERROR(_rt) && !_pstmt_m ) \
            _pstmt_m = duf_pdi_prepare_statement_by_id( _pdi, _sql,  DUF_SQL_STMD_ID_ ## _name, &_rt ); \
          if ( DUF_NOERROR(_rt) && !_pstmt_m ) \
            DUF_MAKE_ERROR(_rt, DUF_ERROR_PDI_SQL); \
          DUF_TEST_R( _rt );
#  elif defined(DUF_SQL_NOPDI_STMT)
#    define DUF_SQL_START_STMT( _pdi, _name, _sql, _rt, _pstmt_m ) \
	DUF_SQL_START_STMT_NOPDI( _sql, _rt, _pstmt_m );
#  else
#    undef DUF_SQL_START_STMT
#  endif


#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_END_STMT(_pdi, _name, _rt, _pstmt_m) \
	  DUF_CLEAR_ERROR(_rt, DUF_SQL_ROW, DUF_SQL_DONE ); \
          if ( DUF_NOERROR(_rt) && !_pstmt_m ) \
            DUF_MAKE_ERROR(_rt, DUF_ERROR_PDI_SQL); \
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
          if ( DUF_NOERROR(_rt) && !_pstmt_m ) \
            _pstmt_m = duf_pdi_prepare_statement_by_id( _pdi, _sql, ( duf_stmt_ident_t ) 0, &_rt ); \
          if ( DUF_NOERROR(_rt) && !_pstmt_m ) \
            DUF_MAKE_ERROR(_rt, DUF_ERROR_PDI_SQL); \
          DUF_TEST_R( _rt );
#  else
#    undef DUF_SQL_START_STMT_LOCAL
#  endif


#  ifdef DUF_SQL_PDI_STMT
#    define DUF_SQL_END_STMT_LOCAL(_pdi, _rt, _pstmt_m) \
	  DUF_CLEAR_ERROR(_rt, DUF_SQL_ROW, DUF_SQL_DONE ); \
          if ( DUF_NOERROR(_rt) && !_pstmt_m ) \
            DUF_MAKE_ERROR(_rt, DUF_ERROR_PDI_SQL); \
	  if (_pstmt_m) \
	  { duf_sql_reset( _pstmt_m ); duf_sql_clear_bindings( _pstmt_m ); } \
	  duf_pdi_finalize_statement_by_stmt(_pdi, _pstmt_m ); \
	}
#  else
#    undef DUF_SQL_END_STMT_LOCAL
#  endif






#  define DUF_SQL_BIND_LL( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
		  {  \
		    DUF_TRACE( sql, 0, "@@bind ll " # _name ": %lld", (long long)_value); \
		    DOR(_rt, duf_sql_bindn_long_long( _pstmt_m, ":" #_name, _value, 0 )); \
		  }
#  define DUF_SQL_BIND_LL_NZ( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
		  {  \
		    DUF_TRACE( sql, 0, "@@bind ll nz " # _name ": %lld", (long long)_value); \
  		    DOR(_rt, duf_sql_bindn_long_long_nz( _pstmt_m, ":" #_name, _value, 0 )); \
		  }
#  define DUF_SQL_BIND_LL_NZ_OPT( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
		  {  \
		    DUF_TRACE( sql, 0, "@@bind ll nz opt " # _name ": %lld", (long long)_value); \
		    DUF_E_LOWER( DUF_ERROR_BIND_NAME ); \
                    DOR(_rt, duf_sql_bindn_long_long_nz( _pstmt_m, ":" #_name, _value, 1 )); \
		    DUF_CLEAR_ERROR(_rt, DUF_ERROR_BIND_NAME); \
		    DUF_E_UPPER( DUF_ERROR_BIND_NAME ); \
		  }

#  define DUF_SQL_BIND_I( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
		  {  \
		    DUF_TRACE( sql, 0, "@@bind i " # _name ": %d", _value); \
		    DOR(_rt, duf_sql_bindn_int( _pstmt_m, ":" #_name, _value, 0 )); \
		  }
#  define DUF_SQL_BIND_I_NZ( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
		  {  \
		    DUF_TRACE( sql, 0, "@@bind i nz " # _name ": %d", _value); \
  		    DOR(_rt, duf_sql_bindn_int_nz( _pstmt_m, ":" #_name, _value, 0 )); \
		  }
#  define DUF_SQL_BIND_I_NZ_OPT( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
		  {  \
		    DUF_TRACE( sql, 0, "@@bind i nz opt " # _name ": %d", _value); \
		    DUF_E_LOWER( DUF_ERROR_BIND_NAME ); \
                    DOR(_rt, duf_sql_bindn_int_nz( _pstmt_m, ":" #_name, _value, 1 )); \
		    DUF_CLEAR_ERROR(_rt, DUF_ERROR_BIND_NAME); \
		    DUF_E_UPPER( DUF_ERROR_BIND_NAME ); \
		  }

#  define DUF_SQL_BIND_U( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
		  {  \
		    DUF_TRACE( sql, 0, "@@bind i " # _name ": %u", _value); \
		    DOR(_rt, duf_sql_bindn_int( _pstmt_m, ":" #_name, _value, 0 )); \
		  }
#  define DUF_SQL_BIND_U_NZ( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
		  {  \
		    DUF_TRACE( sql, 0, "@@bind i nz " # _name ": %u", _value); \
  		    DOR(_rt, duf_sql_bindn_int_nz( _pstmt_m, ":" #_name, _value, 0 )); \
		  }
#  define DUF_SQL_BIND_U_NZ_OPT( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
		  {  \
		    DUF_TRACE( sql, 0, "@@bind i nz opt " # _name ": %u", _value); \
		    DUF_E_LOWER( DUF_ERROR_BIND_NAME ); \
                    DOR(_rt, duf_sql_bindn_int_nz( _pstmt_m, ":" #_name, _value, 1 )); \
		    DUF_CLEAR_ERROR(_rt, DUF_ERROR_BIND_NAME); \
		    DUF_E_UPPER( DUF_ERROR_BIND_NAME ); \
		  }



#  define DUF_SQL_BIND_S( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
                  { \
		    DUF_TRACE( sql, 0, "@@bind s " # _name ": %s", _value); \
		    DOR(_rt, duf_sql_bindn_string( _pstmt_m, ":" #_name, _value, 0 )); \
		  }
#  define DUF_SQL_BIND_S_OPT( _name, _value, _rt, _pstmt_m ) \
		  if ( DUF_NOERROR(_rt) ) \
		  { \
		    DUF_TRACE( sql, 0, "@@bind s opt " # _name ": %s", _value); \
		    DUF_E_LOWER( DUF_ERROR_BIND_NAME ); \
		    DOR(_rt, duf_sql_bindn_string( _pstmt_m, ":" #_name, _value, 1 )); \
		    DUF_CLEAR_ERROR(_rt, DUF_ERROR_BIND_NAME); \
		    DUF_E_UPPER( DUF_ERROR_BIND_NAME ); \
		  }
/* DUF_IS_ERROR_N(_rt, DUF_SQL_ROW) || DUF_IS_ERROR_N(_rt, DUF_SQL_DONE ) */

#  define DUF_SQL_STEP( _rt, _pstmt_m ) \
  		  /* if ( DUF_NOERROR(_rt) || DUF_IS_ERROR_N(_rt, DUF_SQL_ROW) ) */ DOR_LOWERE_N(20, _rt, duf_sql_step( _pstmt_m ), DUF_SQL_ROW, DUF_SQL_DONE)
#  define DUF_SQL_STEPC( _rt, _pstmt_m ) DUF_SQL_STEP( _rt, _pstmt_m ); DUF_CLEAR_ERROR(_rt, DUF_SQL_ROW, DUF_SQL_DONE )
#  define DUF_SQL_CHANGES_NOPDI( _changes, _rt, _pstmt_m ) \
  		  if ( DUF_NOERROR(_rt) || DUF_IS_ERROR_N(_rt, DUF_SQL_ROW) || DUF_IS_ERROR_N(_rt, DUF_SQL_DONE ) ) \
                    _changes = duf_sql_changes(  )
#  define DUF_SQL_CHANGES( _changes, _rt, _pstmt_m )  DUF_SQL_CHANGES_NOPDI( _changes, _rt, _pstmt_m );  duf_pdi_reg_changes( pdi, _changes )

#  define DUF_SQL_EACH_ROW(_rt, _pstmt1, _ops) while ( DUF_NOERROR(_rt) && !DUF_IS_ERROR_N(_rt, DUF_SQL_DONE ) ) \
	{ \
	  DUF_TRACE(sql, 0, "EACH ROW:%s", duf_sql_stmt(_pstmt1)); \
	  DUF_SQL_STEP( _rt, _pstmt1 ); \
	  DUF_TRACE(sql, 0, "(%s) EACH ROW STEP:%s", duf_error_name_i(_rt), duf_sql_stmt(_pstmt1)); \
	  if ( DUF_IS_ERROR_N(_rt, DUF_SQL_ROW) ) \
	  { \
	    DUF_CLEAR_ERROR(_rt, DUF_SQL_ROW ); \
	    DUF_TRACE(sql, 0, "DO EACH ROW STEP:%s", duf_sql_stmt(_pstmt1)); \
	    _ops ; \
            DUF_TRACE(sql, 0, "(OPS:%s) EACH ROW STEP:%s", duf_error_name_i(_rt), duf_sql_stmt(_pstmt1)); \
	  } \
          DUF_TRACE(sql, 0, "(NEXT:%s) EACH ROW STEP:%s", duf_error_name_i(_rt), duf_sql_stmt(_pstmt1)); \
	} \
        DUF_TRACE(sql, 0, "(END:%d:%s) EACH ROW STEP:%s", _rt, duf_error_name_i(_rt), duf_sql_stmt(_pstmt1));


#endif
