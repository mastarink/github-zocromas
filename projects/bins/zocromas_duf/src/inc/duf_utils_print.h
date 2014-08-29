#ifndef MAS_DUF_UTILS_PRINT_H
#  define MAS_DUF_UTILS_PRINT_H

#  include "duf_print_defs.h"

int duf_vprintf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, va_list args );
int duf_printf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 7, 8 ) ) );

int duf_puts( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str );



#endif

