#ifndef MAS_MAST_TRACE_DEFS_WHAT_H
#  define MAS_MAST_TRACE_DEFS_WHAT_H



#  define MAST_TRACE_WHATSYSE_C( _cfg, _ern, ... ) 		MAST_TRACE_WHAT_BASE( _cfg, _cfg?'$':'_', MAST_TRACE_FLAG_SYSTEM, _ern, \
															__VA_ARGS__ )
#  define MAST_TRACE_WHATSYSE( _ern, ... )			MAST_TRACE_WHATSYSE_C( MAST_TRACE_CONFIG, _ern,		__VA_ARGS__ )
#  define MAST_TRACE_WHATSYS( ... )				MAST_TRACE_WHATSYSE( errno,				__VA_ARGS__ )

#  define MAST_TRACE_WHAT_C( _cfg, ...)				MAST_TRACE_WHAT_BASE( _cfg, _cfg?'*':'-', 0, 0,		__VA_ARGS__ )
#  define MAST_TRACE_WHAT( ...)					MAST_TRACE_WHAT_C( MAST_TRACE_CONFIG,			__VA_ARGS__ )

#  define MAST_TRACE_WHAT_WP_C( _cfg, _prefix, ...)		MAST_TRACE_WHAT_WP_BASE( _cfg, _prefix, _cfg?'*':'-', 0, 0,	__VA_ARGS__ )
#  define MAST_TRACE_WHAT_WP( ...)				MAST_TRACE_WHAT_WP_C( MAST_TRACE_CONFIG,			__VA_ARGS__ )





#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
