#ifndef MAS_DUF_START_END_H
#  define MAS_DUF_START_END_H

/* DUF_TEST_R : takes duf_error_index_t! */
#  define DUF_START() {
#  define DUF_STARTT(_typ, _rt) _rt=(_typ)0; DUF_START()
#  define DUF_STARTTYP(_typ, _rt) _typ DUF_STARTT(_typ, _rt)
#  define DUF_STARTR(_rt)         DUF_STARTTYP(int, _rt)
#  define DUF_STARTULL(_rt)       DUF_STARTTYP(unsigned long long, _rt)
#  define DUF_END() } /* END */
#if 0
#  define DUF_ENDRETT(_rt, _typ)							DUF_END();	{ _typ rrrt=_rt;	return rrrt; }
#  define DUF_ENDR(_rt)           DUF_TEST_R(_rt);					DUF_ENDRETT(_rt, int)
#  define DUF_ENDR_CLEAR(_rt,...) DUF_TEST_R(_rt); DUF_CLEAR_ERROR( _rt, __VA_ARGS__ );	DUF_ENDRETT(_rt, int)
#  define DUF_ENDR_LOWERE(_rt, ...)  DUF_TEST_R_LOWERE(_rt, __VA_ARGS__);		DUF_ENDRETT(_rt, int)
#  define DUF_ENDR_UPPER(_rt,...)	  DUF_TEST_R(_rt); DUF_E_UPPER(__VA_ARGS__);	DUF_ENDRETT(_rt, int)
#  define DUF_ENDULL(_rt)								DUF_ENDRETT(_rt, unsigned long long)
#  define DUF_ENDS(_s)									DUF_ENDRETT(_s, char *)
#  define DUF_ENDCS(_s)									DUF_ENDRETT(_s, const char *)
#else
#  define DUF_ENDRET(_rt)								DUF_END();				return _rt
#  define DUF_ENDR(_rt)           DUF_TEST_R(_rt);					DUF_ENDRET(_rt)
#  define DUF_ENDR_CLEAR(_rt,...) DUF_TEST_R(_rt); DUF_CLEAR_ERROR( _rt, __VA_ARGS__ );	DUF_ENDRET(_rt)
#  define DUF_ENDR_LOWERE(_rt, ...)  DUF_TEST_R_LOWERE(_rt, __VA_ARGS__);		DUF_ENDRET(_rt)
#  define DUF_ENDR_UPPER(_rt,...)	  DUF_TEST_R(_rt); DUF_E_UPPER(__VA_ARGS__);	DUF_ENDRET(_rt)
#  define DUF_ENDULL(_rt)								DUF_ENDRET(_rt)
#  define DUF_ENDS(_s)									DUF_ENDRET(_s)
#  define DUF_ENDCS(_s)									DUF_ENDRET(_s)
#endif
/* #  define DUF_ENDR_YES_CLEAR(_rt,...) DUF_TEST_R(_rt); DUF_E_UPPER(__VA_ARGS__);  DUF_CLEAR_ERROR( _rt, __VA_ARGS__ ); DUF_ENDRETT(_rt) */
/* #  define DUF_ENDRQ(_rt,_cond)    DUF_TEST_RQ(_rt,_cond);                                       DUF_ENDRETT(_rt) */
/* #  define DUF_ENDRN(_rt)          DUF_TEST_RN(_rt);                                             DUF_ENDRETT(_rt, int) */



#endif

/*
vi: ft=c
*/
