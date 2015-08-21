#ifndef MAS_DUF_UTILS_H
#  define MAS_DUF_UTILS_H

#  include <time.h>
/* #  include "duf_trace_types.h"   */
/* #  include "duf_error_types.h"   */
/* #  include "duf_ufilter_types.h" */

typedef struct
{
  float percent;
  unsigned calls;
  int width;
  float prev_percent;
} duf_bar_t;


char *duf_single_quotes_2( const char *s );


int duf_vprintf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, va_list args );
int duf_printf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, ... )
      __attribute__ ( ( format( __printf__, 7, 8 ) ) );

int duf_puts( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str );


void duf_percent( unsigned long long curval, unsigned long long maxval, const char *msg );
size_t duf_strflocaltime( char *s, size_t max, const char *format, const time_t * ptim );
size_t duf_strfgmtime( char *s, size_t max, const char *format, const time_t * ptim );

long duf_strtol_suff( const char *s, int *pr );
long long duf_strtoll_suff( const char *s, int *pr );

unsigned long long duf_strtime2long( const char *s, int *pr );

#endif
