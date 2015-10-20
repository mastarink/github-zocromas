#ifndef MAS_DUF_ERROR_DEFS_MAKE_H
#  define MAS_DUF_ERROR_DEFS_MAKE_H




#  define DUF_MAKE_ERRORFLM(_rval, ...)  ( _rval = duf_register_error_c( __VA_ARGS__ ) )
#  define DUF_MAKE_ERRORM(_rval, _code, ...)  DUF_MAKE_ERRORFLM(_rval, _code, __func__, __LINE__, __VA_ARGS__ )
#  define DUF_MAKE_ERROR(_rval, _code )         (_rval = duf_register_error_c( _code, __func__, __LINE__, NULL ))
#  define DUF_MAKE_ERRORFL(_rval, _code, _func, _line )         DUF_MAKE_ERRORFLM(_rval, _code, _func, _line, NULL )




#endif

/*
vi: ft=c
*/

