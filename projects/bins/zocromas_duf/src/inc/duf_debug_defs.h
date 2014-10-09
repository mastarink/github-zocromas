#ifndef MAS_DUF_DEBUG_DEFS_H
#  define MAS_DUF_DEBUG_DEFS_H
#  include "duf_defs.h"

#define DUF_DBGF(_what)       duf_dbgfunc( DBG_ ## _what, DUF_FL )
#define DUF_DBGFX(_what, ...) duf_dbgfunc( DBG_ ## _what, DUF_FL, __VA_ARGS__ )
#  define  DEBUG_E_NO(...)			DUF_E_NO(__VA_ARGS__);                DUF_DBGF(STEP)
#  define  DEBUG_E_YES(...)			DUF_E_YES(__VA_ARGS__);               DUF_DBGF(STEP)
#  define  DEBUG_START()			DUF_START();                          DUF_DBGF(START)
#  define  DEBUG_STARTR(_rt)			DUF_STARTR(_rt);                      DUF_DBGF(START)
#  define  DEBUG_STARTULL(_rt)			DUF_STARTULL(_rt);                    DUF_DBGF(START)
#  define  DEBUG_END()				DUF_END();                            DUF_DBGF(END)
#  define  DEBUG_ENDR(_rt)			DUF_ENDR(_rt);                        DUF_DBGFX(ENDR, _rt)
#  define  DEBUG_ENDR_NOE(_rt, ...)		DUF_ENDR_NOE(_rt, __VA_ARGS__);       DUF_DBGFX(ENDR, _rt)
#  define  DEBUG_ENDR_YES(_rt, ...)		DUF_ENDR_YES(_rt, __VA_ARGS__);       DUF_DBGFX(ENDR, _rt)
#  define  DEBUG_ENDR_CLEAR(_rt, ...)		DUF_ENDR_CLEAR(_rt, __VA_ARGS__);     DUF_DBGFX(ENDR, _rt)
#  define  DEBUG_ENDR_YES_CLEAR(_rt, ...)	DUF_ENDR_YES_CLEAR(_rt, __VA_ARGS__); DUF_DBGFX(ENDR, _rt)
#  define  DEBUG_ENDRQ(_rt,_cond)		DUF_ENDRQ(_rt,_cond);                 DUF_DBGFX(ENDR, _rt)
#  define  DEBUG_ENDRN(_rt)			DUF_ENDRN(_rt);                       DUF_DBGFX(ENDR, _rt)
#  define  DEBUG_ENDR3(_rt)			DUF_ENDR3(_rt);                       DUF_DBGFX(ENDR, _rt)
                                                                                  
#  define  DEBUG_ENDULL(_l)			DUF_ENDULL(_l);                       DUF_DBGFX(ENDULL, _l)
#  define  DEBUG_ENDS(_l)			DUF_ENDS(_l);                         DUF_DBGFX(ENDS, _l)
                                                                          
#  define  DEBUG_STEP()                                                           DUF_DBGF(STEP)
#  define  DEBUG_STEPS(_l)                                                        DUF_DBGFX(STEPS, _l)
#  define  DEBUG_STEPIS(_l, _s)                                                   DUF_DBGFX(STEPIS, _l, _s)
#  define  DEBUG_STEPULL(_l)                                                      DUF_DBGFX(STEPULL, _l)
                                                                                  
#  define DUF_IF_VERBOSE()			                          DUF_IF_TRACE_WHAT( cli.dbg, verbose )
#  define DUF_IF_VERBOSEN( _lev )		                          DUF_IF_TRACE_WHATN( cli.dbg, verbose, _lev )
#  define DUF_VERBOSE( _lev, ... )		                          DUF_TRACE_WHAT( cli.dbg, verbose, _lev, __VA_ARGS__ )

#endif

/*
vi: ft=c
*/
