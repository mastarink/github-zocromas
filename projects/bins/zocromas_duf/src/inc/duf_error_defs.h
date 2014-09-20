#ifndef DUF_ERROR_DEFS_H
#  define DUF_ERROR_DEFS_H


#  define DUF_E1_NO(...) duf_set_mereport(1, -1, 0, __VA_ARGS__, 0)

#  define DUF_E_NO(...)  duf_set_mereport(0, -1, 0, __VA_ARGS__, 0)
#  define DUF_E_YES(...) duf_set_mereport(0,  1, 0, __VA_ARGS__, 0)

#  define DUF_E_SET(_n, ...) duf_set_mereport(0, _n, 1, __VA_ARGS__, 0)

#  define DUF_E_MAX(cnt, ...) duf_set_memax_count(cnt, __VA_ARGS__, 0)

#  define DUF_EREPORT(r) (duf_get_ereport(r)>0)
#  define DUF_SHOW_ERROR( ... )				DUF_TRACE( error, 0, __VA_ARGS__ )
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
/* #  define DUF_TEST_RX(_rval)         if (_rval) DUF_SHOW_ERROR( " - - - - - -> [%s] (#%d)", _rval<0?duf_error_name(_rval):"+", _rval ) */
#  ifdef DUF_T_NOIF
#    define DUF_TEST_RX(_rval)	   (( (DUF_EREPORT(_rval)) ? \
    					DUF_SHOW_ERROR( " - - - - - -> [%s] (#%d) {+%d} #%u", \
					  (_rval)<0?duf_error_name(_rval):"+", _rval, duf_get_ereport_n(_rval), duf_ecount(_rval) ) : \
    					0 ), _rval)
#  else
#    define DUF_TEST_RX(_rval)	if (DUF_EREPORT(_rval)) \
				{ \
				  DUF_SHOW_ERROR( " - - - - - -> [%s] (#%d) {+%d} #%u", \
				      (_rval)<0?duf_error_namduf_set_memax_counte(_rval):"+", _rval, duf_get_ereport_n(_rval), duf_ecount(_rval) ); \
				}
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

# define DUF_TEST_R_NOE(_rval, ...)	DUF_E_NO(  __VA_ARGS__ );DUF_TEST_R(_rval);DUF_E_YES( __VA_ARGS__ )




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
					DUF_SHOW_ERROR( " - - - - - -> sqlite3 [%s] (#%d)", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval) \
					    ) : 0 )
#    define DUF_TEST_R3S(_rval, _xstr) 	( (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					) ?		\
					DUF_SHOW_ERROR( " - - - - - -> sqlite3 [%s] (#%d) {%s}", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval), _xstr \
					    ) : 0 )
#  else
#    define DUF_TEST_R3(_rval)	if (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					)		\
					DUF_SHOW_ERROR( " - - - - - -> sqlite3 [%s] (#%d)", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval) \
					    )
#    define DUF_TEST_R3S(_rval, _xstr) 	if (_rval \
    			&& (_rval)!=SQLITE_ROW \
    			&& (_rval)!=SQLITE_DONE \
    					)		\
					DUF_SHOW_ERROR( " - - - - - -> sqlite3 [%s] (#%d) {%s}", \
					    DUF_SQLITE_ERROR_CODE(_rval) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(_rval)) : "-", \
					    DUF_SQLITE_ERROR_CODE(_rval), _xstr \
					    )
#  endif




/* ###################################################################### */

/* ###################################################################### */
#  define DUF_CLEAR_ERROR(_rt, ...)  _rt=duf_clear_error( _rt, __VA_ARGS__, 0 )


#endif
