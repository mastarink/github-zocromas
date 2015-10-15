#ifndef MAS_DUF_TRACE_H
#  define MAS_DUF_TRACE_H

#  include <stdio.h>

/* duf_trace_mode_t */
#  include "duf_trace_defs.h"
/* #  include "duf_trace_types.h" */


int duf_vtrace( /* duf_trace_mode_t trace_mode,  duf_trace_submode_t trace_submode, */const char *name, int level, int minlevel, const char *funcid,
                int linid, double time0, char signum, unsigned flags, int nerr, FILE * out, const char *prefix, int fun_width,
                const char *fmt, va_list args );
int duf_trace( /* duf_trace_mode_t trace_mode,  duf_trace_submode_t trace_submode, */const char *name, int level, int minlevel, const char *funcid,
               int linid, double time0, char signum, unsigned flags, int nerr, FILE * out, const char *prefix, int fun_width,
               const char *fmt, ... ) __attribute__ ( ( format( __printf__, 13, 14 ) ) );


typedef enum
{
  DUF_TRACE_FLAGID_SYSTEM,
  DUF_TRACE_FLAGID_MAX,
} duf_trace_flagid_t;

#  define  DUF_TRACE_FLAG_SYSTEM  1 << DUF_TRACE_FLAGID_SYSTEM
#  define  DUF_TRACE_FLAG_MAX     1 << DUF_TRACE_FLAGID_MAX


#endif
