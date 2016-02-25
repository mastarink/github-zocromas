#ifndef MAS_ERROR_DEFS_MAKE_H
# define MAS_ERROR_DEFS_MAKE_H

# include "mas_error_regfind.h"                                      /* mas_register_error_c */

# define MASE_MAKE_ERRORFLM(_rval, ...)  ( _rval = mas_register_error_c( __VA_ARGS__ ) )
# define MASE_MAKE_ERRORM(_rval, _code, ...)  MASE_MAKE_ERRORFLM(_rval, _code, __func__, __LINE__, __VA_ARGS__ )
# define MASE_MAKE_ERROR(_rval, _code )         (_rval = mas_register_error_c( _code, __func__, __LINE__, NULL ))
# define MASE_MAKE_ERRORFL(_rval, _code, _func, _line )         MASE_MAKE_ERRORFLM(_rval, _code, _func, _line, NULL )

#endif

/*
vi: ft=c
*/
