#ifndef MAS_ERROR_DEFS_H
# define MAS_ERROR_DEFS_H

# include "mas_error_defs_show.h"                                    /* MASE_SHOW_ERRORN_TEST_STD */

# define MASE_ERRSYS( ... )				MAST_TRACESYS( errors /* opt.trace.errors */, 0, __VA_ARGS__ )
# define MASE_ERRSYSE( _ern, ... )			MAST_TRACESYSE( _ern, errors /* opt.trace.errors */, 0, __VA_ARGS__ )

# define MASE_T_NOIF

# ifdef MASE_T_NOIF
/* MASE_TEST_RX : takes mas_error_index_t! */
#  ifdef MASE_STD_VERBOSE_LEVEL
#   define MASE_TEST_RX(_rval)	   (( (MASE_EREPORT(_rval)) ? (MASE_SHOW_ERRORN_TEST_STD(_rval, MASE_STD_VERBOSE_LEVEL)) : \
    					0 ), (_rval))
#  else
#   define MASE_TEST_RX(_rval)	   (( (MASE_EREPORT(_rval)) ? (MASE_SHOW_ERRORN_TEST_STD(_rval, 0)) : \
    					0 ), (_rval))
#  endif
#  if 0
#   define MASE_TEST_RX_WP(_rval, _prefix)  (( (MASE_EREPORT(_rval)) ? MASE_SHOW_ERRORO_TEST_WP_STD(_rval,_prefix) : \
    					0 ), _rval)
#   define MASE_TEST_RX_SHOW(_rval, _prefix)		MASE_TEST_RX_WP(_rval,  _prefix)
#   define MASE_TEST_RX_START(_rval) if (MASE_TEST_RX(_rval)<0) {
#   define MASE_TEST_RX_END(_rval) }
#  endif
# endif

/* #  ifdef MASE_T_NOIF                                                               */
/* #    define MASE_TEST_RQX(_rval, _cond)  (( !(_cond) ) ? MASE_TEST_RX( _rval ) : 0) */
/* #  endif                                                                          */
/* #  define MASE_TEST_R(_rval)       if ( _rval!=MASE_ERROR_MAX_REACHED && _rval!=MASE_ERROR_MAX_SEQ_REACHED ) MASE_TEST_RX( _rval ) */

/* error message if arg is not 0, except some predefines */

/* #  define MASE_TEST_R(_rval)     MASE_TEST_RQX( _rval,  \       */
/*                            _rval==MASE_ERROR_MAX_REACHED \     */
/*                         || _rval==MASE_ERROR_MAX_SEQ_REACHED \ */
/*                         || _rval==MASE_ERROR_MAX_DEPTH  \      */
/*         )                                                     */
/* #  define MASE_TEST_R(_rval)     ( MASE_E_LOWER(MASE_ERROR_MAX_REACHED,MASE_ERROR_MAX_SEQ_REACHED,MASE_ERROR_MAX_DEPTH,0),MASE_TEST_RX( _rval ),MASE_E_UPPER(MASE_ERROR_MAX_REACHED,MASE_ERROR_MAX_SEQ_REACHED,MASE_ERROR_MAX_DEPTH,0),_rval ) */
/* MASE_TEST_R : takes mas_error_index_t! */
# define MASE_TEST_R(_rval)	((void)(MASE_TEST_RX( _rval )))
# define MASE_TEST_R1(_rval)	(mas_force_ereport(1 /*count*/),MASE_TEST_RX( _rval ))
/* #  define MASE_TEST_R2(_rval)    (mas_force_ereport(2 (*count*)),MASE_TEST_RX( _rval )) */

# define MASE_TEST_R_LOWERE(_rval, ...)	MASE_E_LOWER(  __VA_ARGS__ );MASE_TEST_R(_rval);MASE_E_UPPER( __VA_ARGS__ )

/* #  ifdef MASE_T_NOIF                                                          */
/* #    define MASE_TEST_RN(_rval)     ( ( _rval<0 ) ? MASE_TEST_R( _rval ) : 0 ) */
/* #  else                                                                      */
/* #    define MASE_TEST_RN(_rval)       if ( _rval<0 ) MASE_TEST_R( _rval )      */
/* #  endif                                                                     */

/* #  define MASE_TEST_RR(_rval)      if ( !MASE_IS_ERROR_N(_rval, MASE_SQL_ROW) && !MASE_IS_ERROR_N(_rval,MASE_SQL_DONE) ) MASE_TEST_R( _rval ) */
/* #  define MASE_TEST_RR(_rval)    MASE_TEST_RQ(_rval, MASE_IS_ERROR_N(_rval, MASE_SQL_ROW) || MASE_IS_ERROR_N(_rval,MASE_SQL_DONE) ) */

/* ###################################################################### */
#endif

/*
vi: ft=c
*/
