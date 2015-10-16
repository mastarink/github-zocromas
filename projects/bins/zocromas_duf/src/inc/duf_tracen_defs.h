#ifndef MAS_DUF_TRACEN_DEFS_H
#  define MAS_DUF_TRACEN_DEFS_H


/* #  include "duf_tracen_defs_base.h" */
/* #  include "duf_tracen_defs_what.h" */
/* #  include "duf_tracen_defs_sys.h" */

#  define DUF_TRACE( ... )  \
         MAST_TRACE( __VA_ARGS__ )
#  define DUF_TRACE_WP( ... )  \
         MAST_TRACE_WP( __VA_ARGS__ )
#  define DUF_TRACE_WHAT( ... )  \
         MAST_TRACE_WHAT( __VA_ARGS__ )
#  define DUF_IF_TRACE( ... )  \
         MAST_IF_TRACE( __VA_ARGS__ )
#  define DUF_TRACESYS( ... )  \
         MAST_TRACESYS( __VA_ARGS__ )
#  define DUF_TRACESYSE( ... )  \
         MAST_TRACESYSE( __VA_ARGS__ )

#  define DUF_DEBUG(_lev, ... ) \
         MAST_DEBUG(_lev, __VA_ARGS__ )

#  define TR(_r) if (DUF_IS_ERROR(_r)) {T("@@@@@@(i:%d:c:%d) %s", _r, duf_error_code_i(_r), duf_error_name_i(_r));}
#endif

/*
vi: ft=c
vi: colorcolumn=65,105,121
*/
