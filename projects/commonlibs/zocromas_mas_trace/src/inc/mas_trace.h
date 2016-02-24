#ifndef MAS_MAST_TRACE_H
# define MAS_MAST_TRACE_H

# include <stdio.h>

/* mas_trace_mode_t */
# include "mas_trace_defs.h"
/* #  include "mas_trace_types.h" */

int mas_vtrace( const mas_config_trace_t * tcfg, const char *name, int trace_index, int minlevel, const char *funcid, int linid, /* double time0, */ char signum,
                unsigned flags, int nerr, /* FILE * out, */ const char *prefix, /* unsigned fun_width, int force_color, int nocolor, */const char *fmt,
                va_list args );

int mas_trace( const mas_config_trace_t * tcfg, const char *name, int trace_index, int minlevel, const char *funcid, int linid, /* double time0, */ char signum,
               unsigned flags, int nerr, /* FILE * out, */ const char *prefix, /* , unsigned fun_width, int force_color, int nocolor */ const char *fmt, ... )
        __attribute__ ( ( format( __printf__, 11, 12 ) ) );

typedef enum
{
  MAST_TRACE_FLAGID_SYSTEM,
  MAST_TRACE_FLAGID_MAX,
} mas_trace_flagid_t;

# define  MAST_TRACE_FLAG_SYSTEM  1 << MAST_TRACE_FLAGID_SYSTEM
# define  MAST_TRACE_FLAG_MAX     1 << MAST_TRACE_FLAGID_MAX

#endif
