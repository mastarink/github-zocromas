#ifndef DUF_DODEFS_H
#  define DUF_DODEFS_H

#  ifdef DUF_T_NOIF
/* DUF_TEST_R : takes duf_error_index_t! */
#    define DUF_DO_TEST_CALL(_x)  ( /* fprintf(stderr, #_x "\n"), */ (_x))

#    define DUF_DO_TEST_R_INTERNAL(_rval, _x)      ( (_rval>=0) ? ( (_rval=DUF_DO_TEST_CALL(_x)), DUF_TEST_R(_rval)  ) : 0 )
#    define DUF_DO_INTERNAL(_rval, _x)   	   (              ( (_rval=DUF_DO_TEST_CALL(_x))  )     )
#    define DUF_DO_TEST_RN_INTERNAL(_rval, _x)     ( (_rval>=0) ? ( (_rval=DUF_DO_TEST_CALL(_x)), DUF_TEST_RN(_rval) ) : 0 )
#    define DUF_DO_TEST_PR(_rval, _x)              ( (_rval>=0) ? ( (      DUF_DO_TEST_CALL(_x)),  DUF_TEST_R(_rval)  ) : 0 )

/* # define DUF_DO_TEST_RQ_INTERNAL(_rval, _x, _cond)     (_rval>=0) ? ( (_rval=(_x)), DUF_TEST_RQ(_rval, _cond) ) : 0 */
#    define DUF_DO_TEST_R(_rval, _x)               ( DUF_DO_TEST_R_INTERNAL(_rval, _x) )
#    define DUF_DO(_rval, _x)               	   ( DUF_DO_INTERNAL(_rval, _x) )
#    define DUF_DO_TEST_R_LOWERE(_rval, _x, ...)      ( DUF_E_LOWER(  __VA_ARGS__ ),DUF_DO_TEST_R(_rval, _x),DUF_E_UPPER( __VA_ARGS__ ) )
#    define DUF_DO_TEST_R_LOWERE_N(_n, _rval, _x, ...)      ( DUF_E_LOWER_N(_n,   __VA_ARGS__ ),DUF_DO_TEST_R(_rval, _x),DUF_E_UPPER_N(_n, __VA_ARGS__ ) )
/* # define DUF_DO_TEST_RQ(_rval, _x, _cond)    ( DUF_DO_TEST_RQ_INTERNAL(_rval, _x, _cond) ) */

#    define DUF_DO_TEST_RN(_rval, _x)              ( DUF_DO_TEST_RN_INTERNAL(_rval, _x) )
#    define DUF_DO_TESTZ_R(_rval, _x)              ( _rval=0, DUF_DO_TEST_R_INTERNAL(_rval, _x)  )

#  else
/* DUF_TEST_R : takes duf_error_index_t! */
#    define DUF_DO_TEST_R(_rval, _x)    if (_rval>=0) { (_rval=(_x)); DUF_TEST_R(_rval); DUF_MAKE_ERROR(_rval,_rval); }
#    define DUF_DO_TEST_R_LOWERE(_rval, _x, ...) DUF_E_LOWER(  __VA_ARGS__ );DUF_DO_TEST_R(_rval, _x);DUF_E_UPPER( __VA_ARGS__ )
#    define DUF_DO_TEST_R_LOWERE_N(_n, _rval, _x, ...) DUF_E_LOWER_N(_n,   __VA_ARGS__ );DUF_DO_TEST_R(_rval, _x);DUF_E_UPPER_N(_n, __VA_ARGS__ )
/* # define DUF_DO_TEST_RQ(_rval, _x, _cond)    if (_rval>=0) { (_rval=(_x)); DUF_TEST_RQ(_rval, _cond); } */
#    define DUF_DO_TEST_RN(_rval, _x)   if (_rval>=0) { (_rval=(_x)); DUF_TEST_RN(_rval); }

#    define DUF_DO_TESTZ_R(_rval, _x)   { _rval=0;DUF_DO_TEST_R(_rval, _x) }

#    define DUF_DO_TEST_PR(_rval, _x)   if (_rval>=0) { _x; DUF_TEST_R(_rval); }
#  endif


/* DOR... : takes duf_error_index_t! */
#  define DOR(_rval, _x)                 DUF_DO_TEST_R(_rval, _x)
#  define DOC(_rval, _x)                 DUF_DO(_rval, _x)
#  define DORF(_rval, _fun, ...)         DOR(_rval, _fun(__VA_ARGS__))
#  define DORPF(_rval, _fun, ...)	 if (_fun) DOR(_rval, _fun(__VA_ARGS__)

#  define IF_DORVF(_rval, _fun, ...)     if ( DUF_NOERROR( r ) ) { if (_fun) DOR(r,(_fun)(__VA_ARGS__)); else DUF_MAKE_ERROR(_rval,DUF_ERROR_OPTION_NO_FUNC); }

#  define DOCF(_rval, _fun, ...)         DOC(_rval, _fun(__VA_ARGS__))
#  define DORFp(_rval, _fun, ...)        DORF(_rval, duf_ ## _fun, __VA_ARGS__)
#  define DOR_LOWERE(_rval, _x, ...)       DUF_DO_TEST_R_LOWERE(_rval, _x, __VA_ARGS__)
#  define DOR_LOWERE_N(_n, _rval, _x, ...) DUF_DO_TEST_R_LOWERE_N(_n, _rval, _x, __VA_ARGS__)
#  define DORF_LOWERE(_rval, _fun, ...)    DOR_LOWERE(_rval, _fun(__VA_ARGS__))
#  define DORF_LOWERE_N(_n, _rval, _fun, ...)     DOR_LOWERE_N(_n, _rval, _fun(__VA_ARGS__))
/* #  define DORQ(_rval, _x, _cond)      DUF_DO_TEST_RQ(_rval, _x, _cond) */
#  define DORN(_rval, _x)                DUF_DO_TEST_RN(_rval, _x)
#  define DOZR(_rval, _x)                DUF_DO_TESTZ_R(_rval, _x)
#  define DOPR(_rval, _x)                DUF_DO_TEST_PR(_rval, _x)


#endif

/*
vi: ft=c
*/
