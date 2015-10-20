#ifndef MAS_DUF_ERRORN_DEFS_H
#  define MAS_DUF_ERRORN_DEFS_H


#  define DUF_ERRSYS( ... )				DUF_TRACESYS( errors /* opt.trace.errors */, 0, __VA_ARGS__ )
#  define DUF_ERRSYSE( _ern, ... )			DUF_TRACESYSE( _ern, errors /* opt.trace.errors */, 0, __VA_ARGS__ )

#  define DUF_T_NOIF

#  ifdef DUF_T_NOIF
/* DUF_TEST_RX : takes duf_error_index_t! */
#    ifdef DUF_STD_VERBOSE_LEVEL
#      define DUF_TEST_RX(_rval)	   (( (DUF_EREPORT(_rval)) ? (DUF_SHOW_ERRORN_TEST_STD(_rval, DUF_STD_VERBOSE_LEVEL)) : \
    					0 ), _rval)
#    else
#      define DUF_TEST_RX(_rval)	   (( (DUF_EREPORT(_rval)) ? (DUF_SHOW_ERRORN_TEST_STD(_rval, 0)) : \
    					0 ), _rval)
#    endif
#    if 0
#      define DUF_TEST_RX_WP(_rval, _prefix)  (( (DUF_EREPORT(_rval)) ? DUF_SHOW_ERRORO_TEST_WP_STD(_rval,_prefix) : \
    					0 ), _rval)
#      define DUF_TEST_RX_SHOW(_rval, _prefix)		DUF_TEST_RX_WP(_rval,  _prefix)
#      define DUF_TEST_RX_START(_rval) if (DUF_TEST_RX(_rval)<0) {
#      define DUF_TEST_RX_END(_rval) }
#    endif
#  endif

/* #  ifdef DUF_T_NOIF                                                               */
/* #    define DUF_TEST_RQX(_rval, _cond)  (( !(_cond) ) ? DUF_TEST_RX( _rval ) : 0) */
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
#endif

/*
vi: ft=c
*/
