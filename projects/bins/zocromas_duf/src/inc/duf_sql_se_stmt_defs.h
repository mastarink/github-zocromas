#ifndef MAS_DUF_SQL_SE_STMT_DEFS_H
# define MAS_DUF_SQL_SE_STMT_DEFS_H

# define DUF_SQL_SE_PDI_STMT

# include "duf_sql_types.h"                                          /* duf_stmnt_t ♠ */
/* # include "duf_sql_bind.h" (* duf_sql_clear_bindings *) */

# define DUF_SQL_SE_START_STMT_NOPDI( _sql, _pstmt_m ) \
	{ \
  	  duf_stmnt_t *_pstmt_m = NULL; \
          MAST_TRACE(sql, 4, "START/NOPDI:%s", _sql); \
          if ( QNOERR ) \
            CR(sql_prepare, _sql, &_pstmt_m );

# define DUF_SQL_SE_END_STMT_NOPDI( _pstmt_m) \
	  { \
	    MASE_CLEAR_ERROR(QERRIND, ERRCODE1(SQL_ROW), ERRCODE1(SQL_DONE)); \
	    CR( sql_finalize, _pstmt_m ); \
	    _pstmt_m = NULL; \
	  } \
	}

# ifdef DUF_SQL_SE_PDI_STMT
#  define DUF_SQL_SE_START_STMT( _pdi, _name, _sql, _pstmt_m ) \
	{ \
  	  duf_stmnt_t *_pstmt_m = NULL; \
	  MAST_TRACE(sql, 4, "START:%s (%d:%s)", _sql, QERRIND, QERRNAME); \
          /* TODO : static not always or never? */ \
          if ( QNOERR ) \
            _pstmt_m = duf_pdi_find_statement_by_id( _pdi, DUF_SQL_STMD_ID_ ## _name ); \
          if ( QNOERR && !_pstmt_m ) \
            _pstmt_m = duf_pdi_prepare_statement_by_id( _pdi, _sql,  DUF_SQL_STMD_ID_ ## _name, QPERRIND ); \
	  MAST_TRACE(sql, 4, "START:%s (%d:%s)", _sql, QERRIND, QERRNAME); \
          if ( QNOERR && !_pstmt_m ) \
            ERRMAKE( PDI_SQL); \
	  MAST_TRACE(sql, 4, "START:%s (%d:%s)", _sql, QERRIND, QERRNAME);
# elif defined(DUF_SQL_SE_NOPDI_STMT)
#  define DUF_SQL_SE_START_STMT( _pdi, _name, _sql, _pstmt_m ) \
	DUF_SQL_SE_START_STMT_NOPDI( _sql, _pstmt_m ); \
	MAST_TRACE(sql, 4, "START:%s (%d:%s)", _sql, QERRIND, QERRNAME);
# else
#  undef DUF_SQL_SE_START_STMT
# endif

# ifdef DUF_SQL_SE_PDI_STMT
#  define DUF_SQL_SE_END_STMT(_pdi, _name,  _pstmt_m) \
	  MASE_CLEAR_ERROR(QERRIND, ERRCODE1(SQL_ROW), ERRCODE1(SQL_DONE)); \
          if ( QNOERR && !_pstmt_m ) \
            ERRMAKE( PDI_SQL); \
	  if (_pstmt_m) \
	  { duf_sql_reset( _pstmt_m ); duf_sql_clear_bindings( _pstmt_m ); } \
	  duf_pdi_finalize_statement_by_id(_pdi, DUF_SQL_STMD_ID_ ## _name ); \
	}
# elif defined(DUF_SQL_SE_NOPDI_STMT)
#  define DUF_SQL_SE_END_STMT(_pdi, _pstmt_m) \
	DUF_SQL_SE_END_STMT_NOPDI( _pstmt_m)
# else
#  undef DUF_SQL_SE_END_STMT
# endif

# ifdef DUF_SQL_SE_PDI_STMT
#  define DUF_SQL_SE_START_STMT_LOCAL( _pdi,  _sql, _pstmt_m ) \
	{ \
  	  duf_stmnt_t *_pstmt_m = NULL; \
          /* TODO : static not always or never? */ \
          if ( QNOERR && !_pstmt_m ) \
            _pstmt_m = duf_pdi_prepare_statement_by_id( _pdi, _sql, ( duf_stmt_ident_t ) 0, QPERRIND ); \
          if ( QNOERR && !_pstmt_m ) \
            ERRMAKE( PDI_SQL);
# else
#  undef DUF_SQL_SE_START_STMT_LOCAL
# endif

# ifdef DUF_SQL_SE_PDI_STMT
#  define DUF_SQL_SE_END_STMT_LOCAL(_pdi, _pstmt_m) \
	  MASE_CLEAR_ERROR(QERRIND, ERRCODE1(SQL_ROW), ERRCODE1(SQL_DONE)); \
          if ( QNOERR && !_pstmt_m ) \
            ERRMAKE(PDI_SQL); \
	  if (_pstmt_m) \
	  { duf_sql_reset( _pstmt_m ); duf_sql_clear_bindings( _pstmt_m ); } \
	  duf_pdi_finalize_statement_by_stmt(_pdi, _pstmt_m ); \
	}
# else
#  undef DUF_SQL_SE_END_STMT_LOCAL
# endif

# define DUF_SQL_SE_BIND_LL( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
		  {  \
		    MAST_TRACE( sql, 4, "@@bind ll " # _name ": %lld", (long long)_value); \
		    CR(sql_bindn_long_long, _pstmt_m, ":" #_name, _value, 0 ); \
		  }
# define DUF_SQL_SE_BIND_LL_NZ( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
		  {  \
		    MAST_TRACE( sql, 4, "@@bind ll nz " # _name ": %lld", (long long)_value); \
  		    CR(sql_bindn_long_long_nz, _pstmt_m, ":" #_name, _value, 0 ); \
		  }
# define DUF_SQL_SE_BIND_LL_NZ_OPT( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
		  {  \
		    MAST_TRACE( sql, 4, "@@bind ll nz opt " # _name ": %lld", (long long)_value); \
		    ERRLOWER( BIND_NAME ); \
                    CR(sql_bindn_long_long_nz, _pstmt_m, ":" # _name, _value, 1 ); \
		    ERRCLEAR( BIND_NAME); \
		    ERRUPPER( BIND_NAME ); \
		  }

# define DUF_SQL_SE_BIND_I( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
		  {  \
		    MAST_TRACE( sql, 4, "@@bind i " # _name ": %d", _value); \
		    CR(sql_bindn_int, _pstmt_m, ":" # _name, _value, 0 ); \
		  }
# define DUF_SQL_SE_BIND_I_NZ( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
		  {  \
		    MAST_TRACE( sql, 4, "@@bind i nz " # _name ": %d", _value); \
  		    CR(sql_bindn_int_nz, _pstmt_m, ":" # _name, _value, 0 ); \
		  }
# define DUF_SQL_SE_BIND_I_NZ_OPT( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
		  {  \
		    MAST_TRACE( sql, 4, "@@bind i nz opt " # _name ": %d", _value); \
		    ERRLOWER( BIND_NAME ); \
                    CR(sql_bindn_int_nz, _pstmt_m, ":" # _name, _value, 1 ); \
		    ERRCLEAR( BIND_NAME); \
		    ERRUPPER( BIND_NAME ); \
		  }

# define DUF_SQL_SE_BIND_U( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
		  {  \
		    MAST_TRACE( sql, 4, "@@bind i " # _name ": %u", _value); \
		    CR(sql_bindn_int, _pstmt_m, ":" # _name, _value, 0 ); \
		  }
# define DUF_SQL_SE_BIND_U_NZ( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
		  {  \
		    MAST_TRACE( sql, 4, "@@bind i nz " # _name ": %u", _value); \
  		    CR(sql_bindn_int_nz, _pstmt_m, ":" # _name, _value, 0 ); \
		  }
# define DUF_SQL_SE_BIND_U_NZ_OPT( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
		  {  \
		    MAST_TRACE( sql, 4, "@@bind i nz opt " # _name ": %u", _value); \
		    ERRLOWER( BIND_NAME ); \
                    CR(sql_bindn_int_nz, _pstmt_m, ":" # _name, _value, 1 ); \
		    ERRCLEAR( BIND_NAME); \
		    ERRUPPER( BIND_NAME ); \
		  }

# define DUF_SQL_SE_BIND_S( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
                  { \
		    MAST_TRACE( sql, 4, "@@bind s " # _name ": %s", _value); \
		    CR( sql_bindn_string, _pstmt_m, ":" # _name, _value, 0 ); \
		  }
# define DUF_SQL_SE_BIND_S_OPT( _name, _value, _pstmt_m ) \
		  if ( QNOERR ) \
		  { \
		    MAST_TRACE( sql, 4, "@@bind s opt " # _name ": %s", _value); \
		    ERRLOWER( BIND_NAME ); \
		    CR(sql_bindn_string, _pstmt_m, ":" # _name, _value, 1 ); \
		    ERRCLEAR( BIND_NAME); \
		    ERRUPPER( BIND_NAME ); \
		  }
/* QISERR1_N( SQL_ROW) || QISERR1_N( SQL_DONE ) */

# define DUF_SQL_SE_STEP( _pstmt_m ) \
              ( \
		MASE_E_LOWER_N(20,   DUF_SQL_ROW, DUF_SQL_DONE ), \
		  CR( sql_step, _pstmt_m ), \
		MASE_E_UPPER_N(20, DUF_SQL_ROW, DUF_SQL_DONE ) \
	      )

# define DUF_SQL_SE_STEPC( _pstmt_m ) DUF_SQL_SE_STEP( _pstmt_m );  MASE_CLEAR_ERROR(QERRIND, ERRCODE1(SQL_ROW), ERRCODE1(SQL_DONE));
# define DUF_SQL_SE_CHANGES_NOPDI( _changes, _pstmt_m ) \
  		  if ( QNOERR || QISERR1_N( SQL_ROW) || QISERR1_N( SQL_DONE ) ) \
                    _changes = duf_sql_changes(  )
# define DUF_SQL_SE_CHANGES( _changes, _pstmt_m )  DUF_SQL_SE_CHANGES_NOPDI( _changes, _pstmt_m );  duf_pdi_reg_changes( pdi, _changes )

# define DUF_SQL_SE_EACH_ROW( _pstmt1, _ops) \
      { \
	while ( QNOERR && !QISERR1_N( SQL_DONE ) ) \
	{ \
	  MAST_TRACE(sql, 4, "EACH ROW:%s", duf_sql_stmt(_pstmt1)); \
	  DUF_SQL_SE_STEP( _pstmt1 ); \
	  MAST_TRACE(sql, 4, "(%s) EACH ROW STEP:%s", QERRNAME, duf_sql_stmt(_pstmt1)); \
	  if ( QISERR1_N( SQL_ROW) ) \
	  { \
	    ERRCLEAR1( SQL_ROW ); \
	    MAST_TRACE(sql, 4, "DO EACH ROW STEP:%s", duf_sql_stmt(_pstmt1)); \
	    _ops ; \
            MAST_TRACE(sql, 4, "(OPS:%s) EACH ROW STEP:%s", QERRNAME, duf_sql_stmt(_pstmt1)); \
	  } \
          MAST_TRACE(sql, 4, "(NEXT:%s) EACH ROW STEP:%s", QERRNAME, duf_sql_stmt(_pstmt1)); \
	} \
        MAST_TRACE(sql, 4, "(END:%d:%s) EACH ROW STEP:%s", QERRIND, QERRNAME, duf_sql_stmt(_pstmt1)); \
      }

# define DUF_SQL_SE_BIND_S_OPTQ(_name, _value, _pstmt_m) \
 if ( QNOERR ) \
		  { \
		    MAST_TRACE( sql, 4, "@@bind s opt " # _name ": %s", _value); \
		    ERRLOWER( BIND_NAME ); \
		    CR( sql_bindn_string, _pstmt_m, ":" # _name, _value, 1 ); \
		    ERRCLEAR( BIND_NAME ); \
		    ERRUPPER( BIND_NAME ); \
		  }

#endif
