#ifndef MAS_DUF_UTILS_H
#  define MAS_DUF_UTILS_H


char *duf_single_quotes_2( const char *s );

int duf_vtrace( const char *name, int level, int minlevel, const char *funcid, int linid, unsigned flags, int nerr, FILE * out,
                const char *fmt, va_list args );
int duf_trace( const char *name, int level, int minlevel, const char *funcid, int linid, unsigned flags, int nerr, FILE * out,
               const char *fmt, ... ) __attribute__ ( ( format( __printf__, 9, 10 ) ) );

typedef enum
{
  DUF_TRACE_FLAGID_SYSTEM,
  DUF_TRACE_FLAGID_MAX,
} duf_trace_flagid_t;

#  define  DUF_TRACE_FLAG_SYSTEM  1 << DUF_TRACE_FLAGID_SYSTEM
#  define  DUF_TRACE_FLAG_MAX     1 << DUF_TRACE_FLAGID_MAX


#endif
