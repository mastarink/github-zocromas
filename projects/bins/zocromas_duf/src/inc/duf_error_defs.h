#ifndef DUF_ERROR_DEFS_H
#  define DUF_ERROR_DEFS_H


/* #  define DUF_E1_LOWER(...) duf_enable_mereport_c(1, -1, 0, __VA_ARGS__, 0) */

#  define DUF_E_LOWER(...)  duf_enable_mereport_c(0, -1, 0, __VA_ARGS__, 0)
#  define DUF_E_LOWER_N(_n, ...)  duf_enable_mereport_c(0, -_n, 0, __VA_ARGS__, 0)
#  define DUF_E_UPPER(...) duf_enable_mereport_c(0,  1, 0, __VA_ARGS__, 0)
#  define DUF_E_UPPER_N(_n, ...) duf_enable_mereport_c(0, _n, 0, __VA_ARGS__, 0)

#  define DUF_E_SET(_n, ...) duf_enable_mereport_c(0, _n, 1, __VA_ARGS__, 0)

#  define DUF_E_MAX(cnt, ...) duf_set_memax_count_c(cnt, __VA_ARGS__, 0)

#  define DUF_EREPORT(rc) (duf_enabled_ereport_i(rc)>0)

#  define DUF_MAKE_ERRORMFL(_rval, _code, _msg, _func, _line)  (_rval=(DUF_IS_ERROR(_code) ? duf_register_error_c( _code, _func, _line, _msg) :    0 ))
#  define DUF_MAKE_ERRORM(_rval, _code, _msg)  DUF_MAKE_ERRORMFL(_rval, _code, _msg, __func__, __LINE__)
#  define DUF_MAKE_ERROR(_rval, _code)         DUF_MAKE_ERRORM(_rval, _code, NULL)
#  define DUF_MAKE_ERRORFL(_rval, _code, _func, _line)         DUF_MAKE_ERRORMFL(_rval, _code, NULL, _func, _line)

#  define DUF_SHOW_ERROR_WP( _prefix, ... )		DUF_TRACE_WP( _prefix, error, 0, __VA_ARGS__ )
#  define DUF_SHOW_ERROR( ... )				DUF_SHOW_ERROR_WP( "@@  ERROR", __VA_ARGS__)
#  define DUF_SHOW_ERROR_TEST( _fmt, ... ) 		DUF_SHOW_ERROR_WP( "@@> > > > > > > ", "@@@@@@@@@"_fmt,  __VA_ARGS__ )

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
/* #  define DUF_TEST_RX(_rval)         if (_rval) DUF_SHOW_ERROR_TEST( "[%s] (#%d)", _rval<0?duf_error_name_i(_rval):"+", _rval ) */

#  define DUF_TEST_RX_END(_rval) }

#  define DUF_SHOW_ERROR_TEST_WP_STD(_rval, _prefix) DUF_SHOW_ERROR_WP( _prefix, "[%s] (#%d) {+%d} #%ld #%u", \
					  (_rval)<0?duf_error_name_i(_rval):"+", _rval, duf_enabled_ereport_n_i(_rval), \
    						duf_error_list_size(), duf_ecount_reported_i(_rval) )
/* DUF_SHOW_ERROR_TEST_STD : takes duf_error_index_t! */
#  define DUF_SHOW_ERROR_TEST_STD(_rval) DUF_SHOW_ERROR_TEST( "[%s] (#%d) {+%d} #%ld #%u", \
					  (_rval)<0?duf_error_name_i(_rval):"+", _rval, duf_enabled_ereport_n_i(_rval), \
    						duf_error_list_size(), duf_ecount_reported_i(_rval) )



#  ifdef DUF_T_NOIF
/* DUF_TEST_RX : takes duf_error_index_t! */
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
/* #  define DUF_TEST_R(_rval)     ( DUF_E_LOWER(DUF_ERROR_MAX_REACHED,DUF_ERROR_MAX_SEQ_REACHED,DUF_ERROR_MAX_DEPTH,0),DUF_TEST_RX( _rval ),DUF_E_UPPER(DUF_ERROR_MAX_REACHED,DUF_ERROR_MAX_SEQ_REACHED,DUF_ERROR_MAX_DEPTH,0),_rval ) */
/* DUF_TEST_R : takes duf_error_index_t! */
#  define DUF_TEST_R(_rval)	(DUF_TEST_RX( _rval ))
#  define DUF_TEST_R1(_rval)	(duf_force_ereport(1 /*count*/),DUF_TEST_RX( _rval ))
/* #  define DUF_TEST_R2(_rval)    (duf_force_ereport(2 (*count*)),DUF_TEST_RX( _rval )) */

#  define DUF_TEST_R_LOWERE(_rval, ...)	DUF_E_LOWER(  __VA_ARGS__ );DUF_TEST_R(_rval);DUF_E_UPPER( __VA_ARGS__ )




/* #  ifdef DUF_T_NOIF                                                          */
/* #    define DUF_TEST_RN(_rval)     ( ( _rval<0 ) ? DUF_TEST_R( _rval ) : 0 ) */
/* #  else                                                                      */
/* #    define DUF_TEST_RN(_rval)       if ( _rval<0 ) DUF_TEST_R( _rval )      */
/* #  endif                                                                     */


/* #  define DUF_TEST_RR(_rval)      if ( !DUF_IS_ERROR_N(_rval, DUF_SQL_ROW) && !DUF_IS_ERROR_N(_rval,DUF_SQL_DONE) ) DUF_TEST_R( _rval ) */
/* #  define DUF_TEST_RR(_rval)    DUF_TEST_RQ(_rval, DUF_IS_ERROR_N(_rval, DUF_SQL_ROW) || DUF_IS_ERROR_N(_rval,DUF_SQL_DONE) ) */

/* ###################################################################### */
#  define DUF_CLEAR_ERROR(_rt, ...)    (_rt=duf_clear_error_i( (_rt), __VA_ARGS__, 0 ))
#  define DUF_CLEARED_ERROR(_rt, ...)  (0==(_rt=duf_clear_error_i( (_rt), __VA_ARGS__, 0 )))
#  define DUF_IS_ERROR_N(_rt, _er) ( duf_error_code_i( _rt ) == _er )
#  define DUF_IS_ERROR(_rt) ( _rt < 0 )
#  define DUF_NOERROR(_rt) ( _rt >= 0 )

#endif

/*
vi: ft=c
*/
