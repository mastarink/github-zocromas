#ifndef MAS_MAST_TRACE_DEFS_SYS_H
#  define MAS_MAST_TRACE_DEFS_SYS_H

#  define MAST_TRACESYS( ...)					MAST_TRACE_WHATSYS(			MAST_TRACE_AT_CFG_, 	 __VA_ARGS__ )
#  define MAST_TRACESYSE(_ern, ...)				MAST_TRACE_WHATSYSE( _ern,		MAST_TRACE_AT_CFG_,	 __VA_ARGS__ )

#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
