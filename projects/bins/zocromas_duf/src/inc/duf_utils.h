#ifndef MAS_DUF_UTILS_H
#  define MAS_DUF_UTILS_H

/* #  include "duf_trace_types.h"   */
/* #  include "duf_error_types.h"   */
#  include "duf_ufilter_types.h"

char *duf_single_quotes_2( const char *s );
int duf_filename_match( duf_filter_glob_t * glob, const char *filename );


int duf_lim_match( duf_limits_t lim, int filesame );
int duf_lim_matchll( duf_limitsll_t lim, int filesame );

int duf_md5id_match( unsigned long long md5id_filter, unsigned long long md5id );


int duf_vprintf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, va_list args );
int duf_printf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 7, 8 ) ) );

int duf_puts( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str );



#endif
