#ifndef MAS_DUF_UTILS_H
#  define MAS_DUF_UTILS_H

/* duf_trace_mode_t */
#include "duf_trace_types.h"
#include "duf_error_types.h"
#include "duf_ufilter_types.h"

char *duf_single_quotes_2( const char *s );
int duf_pathdepth( const char *path );
int duf_filename_match( duf_filter_glob_t * glob, const char *filename );


int duf_lim_match( duf_limits_t lim, int filesame );


int duf_md5id_match( unsigned long long md5id_filter, unsigned long long md5id );

int duf_vtrace( duf_trace_mode_t trace_mode, const char *name, int level, int minlevel, const char *funcid, int linid, double time0,
                unsigned flags, int nerr, FILE * out, const char *fmt, va_list args );
int duf_trace( duf_trace_mode_t trace_mode, const char *name, int level, int minlevel, const char *funcid, int linid, double time0,
               unsigned flags, int nerr, FILE * out, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 11, 12 ) ) );

int duf_vprintf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, va_list args );
int duf_printf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 7, 8 ) ) );

int duf_puts( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str );

char *duf_realpath( const char *path );


const char *duf_error_name( duf_error_code_t c );



typedef enum
{
  DUF_TRACE_FLAGID_SYSTEM,
  DUF_TRACE_FLAGID_MAX,
} duf_trace_flagid_t;

#  define  DUF_TRACE_FLAG_SYSTEM  1 << DUF_TRACE_FLAGID_SYSTEM
#  define  DUF_TRACE_FLAG_MAX     1 << DUF_TRACE_FLAGID_MAX


#endif
