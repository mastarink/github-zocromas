#ifndef MAS_DUF_TRACEN_DEFS_H
# define MAS_DUF_TRACEN_DEFS_H

# include "duf_tracen_defs_preset.h"

# define DUF_TRACE( _id, ... )		MAST_TRACE( _id, __VA_ARGS__ )
/* # define ..._TRACE_WP( _id, ... )       MAST_TRACE_WP( _id, __VA_ARGS__ )   */
/* # define ..._TRACE_WHAT( _id, ... )     MAST_TRACE_WHAT( _id, __VA_ARGS__ ) */
/* # define ..._IF_TRACE( _id, ... )       MAST_IF_TRACE( _id, __VA_ARGS__ ) */
/* # define ..._TRACESYS( _id, ... )       MAST_TRACESYS( _id, __VA_ARGS__ ) */
/* # define ..._TRACESYSE( _id, ... )      MAST_TRACESYSE( _id, __VA_ARGS__ ) */

# define DUF_DEBUG(_lev, ... )	MAST_DEBUG(_lev, __VA_ARGS__ )

# define T(_fmt, ...) DUF_TRACE(temp,0,"@"_fmt,__VA_ARGS__)
# define TT( ...) DUF_TRACE(temp,0,__VA_ARGS__)

# define TR(_r) if (DUF_IS_ERROR(_r)) {T("@@@@@@(i:%d:c:%d) %s", _r, mas_error_code_i(_r), mas_error_name_i(_r));}

# include <mastar/trace/mas_trace.h>

#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
