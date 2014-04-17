#ifndef MAS_DUF_UTILS_H
#  define MAS_DUF_UTILS_H


char *duf_single_quotes_2( const char *s );

int duf_vtrace( const char *name, int level, int minlevel, const char *funcid, int linid, FILE * out, const char *fmt, va_list args );
int duf_trace( const char *name, int level, int minlevel, const char *funcid, int linid, FILE * out, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 7, 8 ) ) );

#endif
