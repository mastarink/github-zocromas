#ifndef MAS_DUF_DEBUG_DEFS_H
#  define MAS_DUF_DEBUG_DEFS_H
#  include "duf_defs.h"

#  define  DEBUG_START() duf_dbgfunc( DBG_START, DUF_FL )
#  define  DEBUG_END() duf_dbgfunc( DBG_ENDR, DUF_FL )
#  define  DEBUG_ENDR(r)  DUF_TEST_R( r ); duf_dbgfunc( DBG_ENDR, DUF_FL, r )
#  define  DEBUG_ENDR3(r)  DUF_TEST_R( DUF_SQLITE_ERROR_CODE(r) ); duf_dbgfunc( DBG_ENDR, DUF_FL, r )
#  define  DEBUG_ENDULL(l) duf_dbgfunc( DBG_ENDULL, DUF_FL, l )
#  define  DEBUG_ENDS(l) duf_dbgfunc( DBG_ENDS, DUF_FL, l )
#  define  DEBUG_STEP() duf_dbgfunc( DBG_STEP, DUF_FL )
#  define  DEBUG_STEPS(l) duf_dbgfunc( DBG_STEPS, DUF_FL, l )
#  define  DEBUG_STEPIS(l, s) duf_dbgfunc( DBG_STEPIS, DUF_FL, l, s )
#  define  DEBUG_STEPULL(l) duf_dbgfunc( DBG_STEPULL, DUF_FL, l )

#  define DUF_IF_VERBOSE()				DUF_IF_TRACE_WHAT( cli.dbg, verbose )
#  define DUF_IF_VERBOSEN( lev )			DUF_IF_TRACE_WHATN( cli.dbg, verbose, lev )
#  define DUF_VERBOSE( lev, ... )			DUF_TRACE_WHAT( cli.dbg, verbose, lev, __VA_ARGS__ )

#endif
