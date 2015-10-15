#ifndef MAS_DUF_TRACE_DEFS_SYS_H
#  define MAS_DUF_TRACE_DEFS_SYS_H

#  define DUF_TRACESYS( ...)					DUF_TRACE_WHATSYS(			DUF_TRACE_AT_CFG, 	 __VA_ARGS__ )
#  define DUF_TRACESYSE(_ern, ...)				DUF_TRACE_WHATSYSE( _ern,		DUF_TRACE_AT_CFG,	 __VA_ARGS__ )

#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
