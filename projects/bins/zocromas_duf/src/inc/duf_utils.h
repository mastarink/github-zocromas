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


void duf_percent( unsigned long long curval, unsigned long long curval2, unsigned long long curval3, unsigned long long maxval, const char *msg );
size_t duf_strflocaltime( char *s, size_t max, const char *format, const time_t * ptim );
size_t duf_strfgmtime( char *s, size_t max, const char *format, const time_t * ptim );

long duf_strtol_suff( const char *s, int *pr );
long long duf_strtoll_suff( const char *s, int *pr );

unsigned long long duf_strtime2long( const char *s, int *pr );

#endif
