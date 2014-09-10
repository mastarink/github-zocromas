#ifndef MAS_DUF_DEBUG_DEFS_H
#  define MAS_DUF_DEBUG_DEFS_H
#  include "duf_defs.h"

#  define  DEBUG_START()            DUF_START();              duf_dbgfunc( DBG_START, DUF_FL )
#  define  DEBUG_STARTR(_rt)        DUF_STARTR(_rt);          duf_dbgfunc( DBG_START, DUF_FL )
#  define  DEBUG_STARTULL(_rt)      DUF_STARTULL(_rt);        duf_dbgfunc( DBG_START, DUF_FL )
#  define  DEBUG_END()              DUF_END();                duf_dbgfunc( DBG_END, DUF_FL )
#  define  DEBUG_ENDR(_rt)          DUF_ENDR(_rt);            duf_dbgfunc( DBG_ENDR, DUF_FL, _rt )
#  define  DEBUG_ENDRQ(_rt,_cond)   DUF_ENDRQ(_rt,_cond);     duf_dbgfunc( DBG_ENDR, DUF_FL, _rt )
#  define  DEBUG_ENDRN(_rt)         DUF_ENDRN(_rt);           duf_dbgfunc( DBG_ENDR, DUF_FL, _rt )
#  define  DEBUG_ENDR3(_rt)         DUF_ENDR3(_rt);           duf_dbgfunc( DBG_ENDR, DUF_FL, _rt )
                                                            
#  define  DEBUG_ENDULL(_l)         DUF_ENDULL(_l);           duf_dbgfunc( DBG_ENDULL, DUF_FL, _l )
#  define  DEBUG_ENDS(_l)           DUF_ENDS(_l);             duf_dbgfunc( DBG_ENDS, DUF_FL, _l )
                                                            
#  define  DEBUG_STEP()                                       duf_dbgfunc( DBG_STEP, DUF_FL )
#  define  DEBUG_STEPS(l)                                     duf_dbgfunc( DBG_STEPS, DUF_FL, l )
#  define  DEBUG_STEPIS(l, s)                                 duf_dbgfunc( DBG_STEPIS, DUF_FL, l, s )
#  define  DEBUG_STEPULL(l)                                   duf_dbgfunc( DBG_STEPULL, DUF_FL, l )
                                                            
#  define DUF_IF_VERBOSE()			              DUF_IF_TRACE_WHAT( cli.dbg, verbose )
#  define DUF_IF_VERBOSEN( lev )		              DUF_IF_TRACE_WHATN( cli.dbg, verbose, lev )
#  define DUF_VERBOSE( lev, ... )		              DUF_TRACE_WHAT( cli.dbg, verbose, lev, __VA_ARGS__ )

#endif
