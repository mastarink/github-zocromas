#ifndef MAS_DUF_DEBUG_DEFS_H
#  define MAS_DUF_DEBUG_DEFS_H
/* #  include "duf_defs.h" */

#  include "duf_start_end.h"


#  ifdef MAS_WRAP_FUNC
#    define DUF_WRAPSTATIC static
#    define DUF_WRAPPED(_f) _f ## _wrap
#  else
#    define DUF_WRAPSTATIC
#    define DUF_WRAPPED(_f) _f
#  endif

#  define DUF_DBGF(_what)       duf_dbgfunc( DBG_ ## _what, DUF_FL )
#  define DUF_DBGFX(_what, ...) duf_dbgfunc( DBG_ ## _what, DUF_FL, __VA_ARGS__ )
#  define  DEBUG_E_LOWER(...)		       DUF_DBGF(STEP)             ; DUF_E_LOWER(__VA_ARGS__)
#  define  DEBUG_E_UPPER(...)		       DUF_DBGF(STEP)             ; DUF_E_UPPER(__VA_ARGS__)
/*XXX  DUF_DBGF* after DUF_START* ! XXX*/
#  define  DEBUG_START()		                                    DUF_START()                                   ;  DUF_DBGF(START)
#  define  DEBUG_STARTTYP(_typ, _rt)					    DUF_STARTTYP(_typ, _rt)			  ;  DUF_DBGF(START)
#  define  DEBUG_STARTR(_rt)		                                    DUF_STARTR(_rt)                               ;  DUF_DBGF(START)
#  define  DEBUG_STARTULL(_rt)		                                    DUF_STARTULL(_rt)                             ;  DUF_DBGF(START)
/*XXX  DUF_DBGF* before DUF_END* ! XXX*/
#  define  DEBUG_END()			       DUF_DBGF(END)              ; DUF_END()
#  define  DEBUG_ENDRET(_rt)		       DUF_DBGF(END)              ; DUF_ENDRET(_rt)
#  define  DEBUG_ENDR(_rt)		       DUF_DBGFX(ENDR, _rt)       ; DUF_ENDR(_rt)
#  define  DEBUG_ENDR_LOWERE(_rt, ...)	       DUF_DBGFX(ENDR, _rt)       ; DUF_ENDR_LOWERE(_rt, __VA_ARGS__)
#  define  DEBUG_ENDR_UPPER(_rt, ...)	       DUF_DBGFX(ENDR, _rt)       ; DUF_ENDR_UPPER(_rt, __VA_ARGS__)
#  define  DEBUG_ENDR_CLEAR(_rt, ...)	       DUF_DBGFX(ENDR, _rt)       ; DUF_ENDR_CLEAR(_rt, __VA_ARGS__)
/* #  define  DEBUG_ENDR_YES_CLEAR(_rt, ...)      DUF_DBGFX(ENDR, _rt)       ; DUF_ENDR_YES_CLEAR(_rt, __VA_ARGS__) */
#  define  DEBUG_ENDRQ(_rt,_cond)	       DUF_DBGFX(ENDR, _rt)       ; DUF_ENDRQ(_rt,_cond)
#  define  DEBUG_ENDRN(_rt)		       DUF_DBGFX(ENDR, _rt)       ; DUF_ENDRN(_rt)
#  define  DEBUG_ENDR3(_rt)		       DUF_DBGFX(ENDR, _rt)       ; DUF_ENDR3(_rt)

#  define  DEBUG_ENDULL(_l)		       DUF_DBGFX(ENDULL, _l)      ; DUF_ENDULL(_l)
#  define  DEBUG_ENDS(_l)		       DUF_DBGFX(ENDS, _l)        ; DUF_ENDS(_l)
#  define  DEBUG_ENDCS(_l)		       DUF_DBGFX(ENDCS, _l)       ; DUF_ENDCS(_l)

#  define  DEBUG_STEP()                        DUF_DBGF(STEP)
#  define  DEBUG_STEPS(_l)                     DUF_DBGFX(STEPS, _l)
#  define  DEBUG_STEPIS(_l, _s)                DUF_DBGFX(STEPIS, _l, _s)
#  define  DEBUG_STEPULL(_l)                   DUF_DBGFX(STEPULL, _l)

#endif

/*
vi: ft=c
*/
