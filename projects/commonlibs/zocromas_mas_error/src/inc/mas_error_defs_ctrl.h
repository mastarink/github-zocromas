#ifndef MAS_ERROR_DEFS_CTRL_H
# define MAS_ERROR_DEFS_CTRL_H

# include "mas_error_base.h"                                         /* mas_error_code_i */
# include "mas_error_reporting.h"                                    /* mas_enable_mereport_c; mas_enabled_ereport_i; mas_set_memax_count_c */
# include "mas_error_clear.h"                                        /* mas_clear_error_i */

/* #  define MASE_E1_LOWER(...) mas_enable_mereport_c(1, -1, 0, __VA_ARGS__, 0) */

# define MASE_E_SET(_n, ...)       mas_enable_mereport_c(0, _n,  1, __VA_ARGS__, 0)
# define MASE_E_SHIFT(_n, ...)     mas_enable_mereport_c(0, _n,  0, __VA_ARGS__, 0)

# if 0
#  define MASE_E_LOWER(...)       { mas_enable_mereport_c(0, -1,  0, __VA_ARGS__, 0)
#  define MASE_E_LOWER_N(_n, ...) { mas_enable_mereport_c(0, -_n, 0, __VA_ARGS__, 0)
#  define MASE_E_UPPER(...)         mas_enable_mereport_c(0,  1,  0, __VA_ARGS__, 0); }
#  define MASE_E_UPPER_N(_n, ...)   mas_enable_mereport_c(0, _n,  0, __VA_ARGS__, 0); }
# else
#  define MASE_E_LOWER_N(_n, ...) { MASE_E_SHIFT(-(_n), __VA_ARGS__)
#  define MASE_E_LOWER(...)         MASE_E_LOWER_N(1, __VA_ARGS__)
#  define MASE_E_UPPER_N(_n, ...)   MASE_E_SHIFT( _n, __VA_ARGS__); }
#  define MASE_E_UPPER(...)         MASE_E_UPPER_N(1, __VA_ARGS__)
# endif

# define MASE_E_MAX(cnt, ...) mas_set_memax_count_c(cnt, __VA_ARGS__, 0)

# define MASE_EREPORT(rc) (mas_enabled_ereport_i(rc)>0)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * -*/
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# define MASE_CLEAR_ERROR(_rt, ...)        (_rt=mas_clear_error_i( (_rt), __VA_ARGS__, 0 ))
# define MASE_CLEARED_ERROR(_rt, ...)  (0==(_rt=mas_clear_error_i( (_rt), __VA_ARGS__, 0 )))
/* #  define MASE_CLEARED_ERROR(_rt, ...)  (0==MASE_CLEAR_ERROR(_rt, __VA_ARGS__)) */
# define MASE_IS_ERROR_N(_rt, _er) ( mas_error_code_i( _rt ) == _er )
# define MASE_IS_ERROR(_rt) ( _rt < 0 )
# define MASE_NOERROR(_rt) ( _rt >= 0 )

# define TER MASE_TEST_R( QERRIND )

# define SERRV(_errv)			MASE_MAKE_ERROR( QERRIND, _errv )
# define SERRMV(_errv, ...)		MASE_MAKE_ERRORM( QERRIND, _errv, __VA_ARGS__ )
# define ERRCODE(_err)		DUF_ERROR_ ## _err
# define ERRCODE1(_err)		DUF_ ## _err

# define SERR(_err)			SERRV( ERRCODE( _err ) )
# define SERRM(_err, ...)		SERRMV( ERRCODE( _err ), __VA_ARGS__ )
# define SERR1(_err)			SERRV( ERRCODE1( _err ) )
# define SERR1M(_err, ...)		SERRMV( ERRCODE1( _err ), __VA_ARGS__ )

# define QNOERR				MASE_NOERROR( QERRIND )
# define QNOERRV(_errv)			MASE_NOERROR( _errv )

# define QISERR_N(_err)			MASE_IS_ERROR_N( QERRIND, ERRCODE( _err ) )
# define QISERR				MASE_IS_ERROR( QERRIND )
# define QISERR1_N(_err)		MASE_IS_ERROR_N( QERRIND, ERRCODE1( _err ) )
# define QISERR1			MASE_IS_ERROR( QERRIND )

# define QERRIND fundecl_.r.ei
# define QERRCODE mas_error_code_i(QERRIND)
# define QPERRIND &(fundecl_.r.ei)
# define QERRNAME			mas_error_name_i( QERRIND )

# define ERRUPPER(_err)			MASE_CLEAR_UPPER( QERRIND, ERRCODE( _err ) )
# define ERRRUPPER(_rt, _err)		MASE_CLEAR_UPPER( _rt, ERRCODE( _err ) )
# define ERRLOWER(_err)			MASE_CLEAR_LOWER( QERRIND, ERRCODE( _err ) )
# define ERRRLOWER(_rt, _err)		MASE_CLEAR_LOWER( _rt, ERRCODE( _err ) )

# define ERRCLEAR_X(...)		MASE_CLEAR_ERROR( QERRIND, __VA_ARGS__ )
# define ERRCLEAR(_err)			MASE_CLEAR_ERROR( QERRIND, ERRCODE( _err ) )
# define ERRRCLEAR(_rt, _err)		MASE_CLEAR_ERROR( _rt, ERRCODE( _err ) )
# define ERRCLEAR1(_err)		MASE_CLEAR_ERROR( QERRIND, ERRCODE1( _err ) )
# define ERRRCLEAR1(_rt, _err)		MASE_CLEAR_ERROR( _rt, ERRCODE1( _err ) )

# define ERRMAKE_FL(_err)		MASE_MAKE_ERRORFL(QERRIND, _err, FL )

# define ERRMAKE(_err)			MASE_MAKE_ERROR( QERRIND, ERRCODE( _err ) )
# define ERRMAKE_M(_err, ...)		MASE_MAKE_ERRORM(QERRIND, ERRCODE( _err ), __VA_ARGS__ )
# define ERRMAKE1(_err)			MASE_MAKE_ERROR( QERRIND, ERRCODE1( _err ) )
# define ERRMAKE1_M(_err, ...)		MASE_MAKE_ERRORM(QERRIND, ERRCODE1( _err ), __VA_ARGS__ )

# define ERRMAKE_IF(_cond,_err)			if (_cond) { MASE_MAKE_ERROR( QERRIND, ERRCODE( _err ) ); }
# define ERRMAKE_IF_M(_cond,_err, ...)		if (_cond) { MASE_MAKE_ERRORM(QERRIND, ERRCODE( _err ), __VA_ARGS__ ); }
# define ERRMAKE1_IF(_cond,_err)		if (_cond) { MASE_MAKE_ERROR( QERRIND, ERRCODE1( _err ) ); }
# define ERRMAKE1_IF_M(_cond,_err, ...)		if (_cond) { MASE_MAKE_ERRORM(QERRIND, ERRCODE1( _err ), __VA_ARGS__ ); }

#endif

/*
vi: ft=c
*/
