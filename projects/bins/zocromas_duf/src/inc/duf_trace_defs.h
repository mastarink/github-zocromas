#ifndef MAS_DUF_TRACE_DEFS_H
#  define MAS_DUF_TRACE_DEFS_H


#  include "duf_trace_defs_base.h"
#  include "duf_trace_defs_what.h"
#  include "duf_trace_defs_sys.h"


#  ifdef MAS_TRACING
#    define DUF_TRACE_C( _cfg, ... )				DUF_TRACE_WHAT_C( _cfg,			cli.trace,	__VA_ARGS__ )
#    define DUF_TRACE( ... )					DUF_TRACE_C(      DUF_TRACE_CONFIG, 			__VA_ARGS__ )
#  else
#    define DUF_TRACE( ... )
#  endif

#  define DUF_TRACE_WP_C( _cfg, _prefix, ... )			DUF_TRACE_WHAT_WP_C( _cfg, _prefix,	cli.trace,	__VA_ARGS__ )
#  define DUF_TRACE_WP( ... )					DUF_TRACE_WP_C(      DUF_TRACE_CONFIG, 			__VA_ARGS__ )

#  define DUF_DEBUG(_lev, ...)  if ( DUF_IF_DEBUGN( _lev ) ) {  __VA_ARGS__ ; }

#  define T(_fmt, ...) DUF_TRACE(temp,0,"@"_fmt,__VA_ARGS__)
#  define TR(_r) T("@@@@@@r:%s", duf_error_name_i(_r))


#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
