#ifndef DUF_ERROR_DEFS_H
#  define DUF_ERROR_DEFS_H


#  define DUF_E1_NO(...) duf_set_mereport(1, -1, 0, __VA_ARGS__, 0)

#  define DUF_E_NO(...)  duf_set_mereport(0, -1, 0, __VA_ARGS__, 0)
#  define DUF_E_YES(...) duf_set_mereport(0,  1, 0, __VA_ARGS__, 0)

#  define DUF_E_SET(_n, ...) duf_set_mereport(0, _n, 1, __VA_ARGS__, 0)

#  define DUF_E_MAX(cnt, ...) duf_set_memax_count(cnt, __VA_ARGS__, 0)

#  define DUF_EREPORT(r) (duf_get_ereport(r)>0)

#  define DUF_MAKE_ERROR(_rval, _code) (_rval = duf_make_error(_code, FL))

#  define DUF_SHOW_ERROR_WP( _prefix, ... )		DUF_TRACE_WP( _prefix, error, 0, __VA_ARGS__ )
#  define DUF_SHOW_ERROR( ... )				DUF_SHOW_ERROR_WP(NULL, __VA_ARGS__)
#  define DUF_SHOW_ERROR_TEST( _fmt, ... ) 		DUF_SHOW_ERROR_WP( "> > > > > > > ", _fmt,  __VA_ARGS__ )

/* #  ifdef DUF_T_NOIF                                                                            */
/* #    define DUF_ERRORQ( _cond, ... )            ( ( _cond ) ? DUF_SHOW_ERROR(__VA_ARGS__) : 0) */
/* #  else                                                                                        */
/* #    define DUF_ERRORQ( _cond, ... )            if (_cond)  DUF_SHOW_ERROR(__VA_ARGS__)        */
/* #  endif                                                                                       */
#  define DUF_SHOW_ERRORiV( v )				DUF_SHOW_ERROR( #v ":%d" , v )
#  define DUF_ERRSYS( ... )				DUF_TRACESYS( error, 0, __VA_ARGS__ )
#  define DUF_ERRSYSE( _ern, ... )			DUF_TRACESYSE( _ern, error, 0, __VA_ARGS__ )

/* ###################################################################### */

/* error message if arg is not 0 */
/* #  define DUF_TEST_RX(_rval)         if (_rval) DUF_SHOW_ERROR_TEST( "[%s] (#%d)", _rval<0?duf_error_name(_rval):"+", _rval ) */

#  define DUF_TEST_RX_END(_rval) }

#  define DUF_SHOW_ERROR_TEST_WP_STD(_rval, _prefix) DUF_SHOW_ERROR_WP( _prefix, "[%s] (#%d) {+%d} #%ld #%u", \
					  (_rval)<0?duf_error_name(_rval):"+", _rval, duf_get_ereport_n(_rval), duf_made_errors(_rval), duf_ecount(_rval) )
#  define DUF_SHOW_ERROR_TEST_STD(_rval) DUF_SHOW_ERROR_TEST( "[%s] (#%d) {+%d} #%ld #%u", \
					  (_rval)<0?duf_error_name(_rval):"+", _rval, duf_get_ereport_n(_rval), duf_made_errors(_rval), duf_ecount(_rval) )



#  ifdef DUF_T_NOIF
#    define DUF_TEST_RX(_rval)	   (( (DUF_EREPORT(_rval)) ? (DUF_SHOW_ERROR_TEST_STD(_rval)) : \
    					0 ), _rval)
#    define DUF_TEST_RX_WP(_rval, _prefix)  (( (DUF_EREPORT(_rval)) ? DUF_SHOW_ERROR_TEST_WP_STD(_rval,_prefix) : \
    					0 ), _rval)
#    define DUF_TEST_RX_SHOW(_rval, _prefix)		DUF_TEST_RX_WP(_rval,  _prefix)
#    define DUF_TEST_RX_START(_rval) if (DUF_TEST_RX(_rval)<0) {
#  else
#    define DUF_TEST_RX_START(_rval)	if (DUF_EREPORT(_rval)) { DUF_SHOW_ERROR_TEST_STD(_rval); \
#    define DUF_TEST_RX(_rval) DUF_TEST_RX_START(_rval); DUF_TEST_RX_END(_rval)
#  endif

/* #  ifdef DUF_T_NOIF                                                               */
/* #    define DUF_TEST_RQX(_rval, _cond)  (( !(_cond) ) ? DUF_TEST_RX( _rval ) : 0) */
/* #  else                                                                           */
/* #    define DUF_TEST_RQX(_rval, _cond)  if ( !(_cond) ) DUF_TEST_RX( _rval )      */
/* #  endif                                                                          */
/* #  define DUF_TEST_R(_rval)       if ( _rval!=DUF_ERROR_MAX_REACHED && _rval!=DUF_ERROR_MAX_SEQ_REACHED ) DUF_TEST_RX( _rval ) */

/* error message if arg is not 0, except some predefines */

/* #  define DUF_TEST_R(_rval)     DUF_TEST_RQX( _rval,  \       */
/*                            _rval==DUF_ERROR_MAX_REACHED \     */
/*                         || _rval==DUF_ERROR_MAX_SEQ_REACHED \ */
/*                         || _rval==DUF_ERROR_MAX_DEPTH  \      */
/*         )                                                     */
/* #  define DUF_TEST_R(_rval)     ( DUF_E_NO(DUF_ERROR_MAX_REACHED,DUF_ERROR_MAX_SEQ_REACHED,DUF_ERROR_MAX_DEPTH,0),DUF_TEST_RX( _rval ),DUF_E_YES(DUF_ERROR_MAX_REACHED,DUF_ERROR_MAX_SEQ_REACHED,DUF_ERROR_MAX_DEPTH,0),_rval ) */
#  define DUF_TEST_R(_rval)	DUF_TEST_RX( _rval )

#  define DUF_TEST_R_NOE(_rval, ...)	DUF_E_NO(  __VA_ARGS__ );DUF_TEST_R(_rval);DUF_E_YES( __VA_ARGS__ )




#  ifdef DUF_T_NOIF
/* #    define DUF_TEST_RQ(_rval, _cond)   ( ( !(_cond) ) ? DUF_TEST_R( _rval ) : 0 ) */
/* error message if arg is < 0, except some predefines */
#    define DUF_TEST_RN(_rval)	   ( ( _rval<0 ) ? DUF_TEST_R( _rval ) : 0 )
/* #    define DUF_TEST_RQN(_rval, _cond)  ( ( !(_cond) ) ? DUF_TEST_RQ( _rval ) : 0 ) */
#  else
/* #    define DUF_TEST_RQ(_rval, _cond)   if ( !(_cond) ) DUF_TEST_R( _rval ) */
/* error message if arg is < 0, except some predefines */
#    define DUF_TEST_RN(_rval)	     if ( _rval<0 ) DUF_TEST_R( _rval )
/* #    define DUF_TEST_RQN(_rval, _cond)  if ( !(_cond) ) DUF_TEST_RQ( _rval ) */
#  endif


/* #  define DUF_TEST_RR(_rval)      if ( _rval!=DUF_SQL_ROW && _rval!=DUF_SQL_DONE ) DUF_TEST_R( _rval ) */
/* #  define DUF_TEST_RR(_rval)    DUF_TEST_RQ(_rval, _rval==DUF_SQL_ROW || _rval==DUF_SQL_DONE ) */

/* ###################################################################### */

#  ifdef DUF_T_NOIF
#    define DUF_TEST_R3(_rval)	( (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					) ?		\
					DUF_SHOW_ERROR_TEST( "sqlite3 [%s] (#%d)", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval) \
					    ) : 0 )
#    define DUF_TEST_R3S(_rval, _xstr) 	( (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					) ?		\
					DUF_SHOW_ERROR_TEST( "sqlite3 [%s] (#%d) {%s}", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval), _xstr \
					    ) : 0 )
#  else
#    define DUF_TEST_R3(_rval)	if (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					)		\
					DUF_SHOW_ERROR_TEST( "sqlite3 [%s] (#%d)", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval) \
					    )
#    define DUF_TEST_R3S(_rval, _xstr) 	if (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					)		\
					DUF_SHOW_ERROR_TEST( "sqlite3 [%s] (#%d) {%s}", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval), _xstr \
					    )
#  endif




/* ###################################################################### */

/* ###################################################################### */
#  define DUF_CLEAR_ERROR(_rt, ...)  _rt=duf_clear_error( _rt, __VA_ARGS__, 0 )
#  define DUF_CLEARED_ERROR(_rt, ...)  (0==(_rt=duf_clear_error( _rt, __VA_ARGS__, 0 )))
#  define DUF_IS_ERROR(_rt, _er) ( _rt == _er )
#  define DUF_NOERROR(_rt) ( _rt >= 0 )

#endif

/*
vi: ft=c
*/
