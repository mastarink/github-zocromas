#ifndef MAS_MAST_TRACE_DEFS_WHAT_H
#  define MAS_MAST_TRACE_DEFS_WHAT_H



#  define MAST_TRACE_WHATSYSE_C( _tcfg, _ern, ... ) 		MAST_TRACE_WHAT_BASE( _tcfg, _tcfg?'$':'_', MAST_TRACE_FLAG_SYSTEM, _ern, __VA_ARGS__ )
#  define MAST_OTRACE_WHATSYSE_C( _out, _tcfg, _ern, ... ) 		MAST_OTRACE_WHAT_BASE( _out, _tcfg, _tcfg?'$':'_', MAST_TRACE_FLAG_SYSTEM, _ern, __VA_ARGS__ )

#  define MAST_TRACE_WHATSYSE( _ern, ... )			MAST_TRACE_WHATSYSE_C( MAST_TRACE_CONFIG, _ern,		__VA_ARGS__ )
#  define MAST_OTRACE_WHATSYSE( _out,_ern, ... )		MAST_OTRACE_WHATSYSE_C( _out,MAST_TRACE_CONFIG, _ern,		__VA_ARGS__ )

#  define MAST_TRACE_WHATSYS( ... )				MAST_TRACE_WHATSYSE( errno,				__VA_ARGS__ )
#  define MAST_OTRACE_WHATSYS(_out, ... )			MAST_OTRACE_WHATSYSE( _out,errno,				__VA_ARGS__ )

#  define MAST_TRACE_WHAT_C( _tcfg, ...)			MAST_TRACE_WHAT_BASE( _tcfg, _tcfg?'*':'-', 0, 0,		__VA_ARGS__ )
#  define MAST_OTRACE_WHAT_C( _out, _tcfg, ...)			MAST_OTRACE_WHAT_BASE( _out, _tcfg, _tcfg?'*':'-', 0, 0,		__VA_ARGS__ )
#  define MAST_TRACE_WHAT( ...)					MAST_TRACE_WHAT_C( MAST_TRACE_CONFIG,			__VA_ARGS__ )
#  define MAST_OTRACE_WHAT( _out, ...)				MAST_OTRACE_WHAT_C( _out, MAST_TRACE_CONFIG,			__VA_ARGS__ )

#  define MAST_TRACE_WHAT_WP_C( _tcfg, _prefix, ...)		MAST_TRACE_WHAT_WP_BASE( _tcfg, _prefix, _tcfg?'*':'-', 0, 0,	__VA_ARGS__ )
#  define MAST_OTRACE_WHAT_WP_C( _out, _tcfg, _prefix, ...)	MAST_OTRACE_WHAT_WP_BASE( _out, _tcfg, _prefix, \
    							_tcfg?'*':'-' /*signum*/, 0/* flags*/, 0/*_ern*/,	__VA_ARGS__ )

#  define MAST_TRACE_WHAT_WP( ...)				MAST_TRACE_WHAT_WP_C( MAST_TRACE_CONFIG,			__VA_ARGS__ )
#  define MAST_OTRACE_WHAT_WP( _out, ...)			MAST_OTRACE_WHAT_WP_C( _out, MAST_TRACE_CONFIG,			__VA_ARGS__ )





#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
