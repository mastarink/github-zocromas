#ifndef MAS_DUF_UTILS_H
#  define MAS_DUF_UTILS_H


char *duf_single_quotes_2( const char *s );

int duf_vtrace( duf_trace_mode_t trace_mode, const char *name, int level, int minlevel, const char *funcid, int linid, unsigned flags,
                int nerr, FILE * out, const char *fmt, va_list args );
int duf_trace( duf_trace_mode_t trace_mode, const char *name, int level, int minlevel, const char *funcid, int linid, unsigned flags,
               int nerr, FILE * out, const char *fmt, ... ) __attribute__ ( ( format( __printf__, 10, 11 ) ) );

int duf_vprintf( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *fmt, va_list args );
int duf_printf( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 6,7 ) ) );

int duf_puts( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str );



const char *duf_error_name( duf_error_code_t c );



typedef enum
{
  DUF_TRACE_FLAGID_SYSTEM,
  DUF_TRACE_FLAGID_MAX,
} duf_trace_flagid_t;

#  define  DUF_TRACE_FLAG_SYSTEM  1 << DUF_TRACE_FLAGID_SYSTEM
#  define  DUF_TRACE_FLAG_MAX     1 << DUF_TRACE_FLAGID_MAX


#endif
