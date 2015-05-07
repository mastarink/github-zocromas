#ifndef MAS_DUF_TRACE_DEFS_WHAT_H
#  define MAS_DUF_TRACE_DEFS_WHAT_H



#  define DUF_TRACE_WHATSYSE_C( _cfg, _ern, ... ) 		DUF_TRACE_WHAT_BASE( _cfg, _cfg?'$':'_', DUF_TRACE_FLAG_SYSTEM, _ern, \
															__VA_ARGS__ )
#  define DUF_TRACE_WHATSYSE( _ern, ... )			DUF_TRACE_WHATSYSE_C( DUF_TRACE_CONFIG, _ern,		__VA_ARGS__ )
#  define DUF_TRACE_WHATSYS( ... )				DUF_TRACE_WHATSYSE( errno,				__VA_ARGS__ )

#  define DUF_TRACE_WHAT_C( _cfg, ...)				DUF_TRACE_WHAT_BASE( _cfg, _cfg?'*':'-', 0, 0,		__VA_ARGS__ )
#  define DUF_TRACE_WHAT( ...)					DUF_TRACE_WHAT_C( DUF_TRACE_CONFIG,			__VA_ARGS__ )

#  define DUF_TRACE_WHAT_WP_C( _cfg, _prefix, ...)		DUF_TRACE_WHAT_WP_BASE( _cfg, _prefix, _cfg?'*':'-', 0, 0,	__VA_ARGS__ )
#  define DUF_TRACE_WHAT_WP( ...)				DUF_TRACE_WHAT_WP_C( DUF_TRACE_CONFIG,			__VA_ARGS__ )





#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
