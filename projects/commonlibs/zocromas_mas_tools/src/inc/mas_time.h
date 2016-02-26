#ifndef MAS_TOOLS_TIME_H
# define MAS_TOOLS_TIME_H

# include <time.h>

struct tm *mas_xlocaltime( void );
double mas_double_time( void );

struct tm *mas_xgmtime( void );

size_t mas_xvstrftime( char *str, size_t size, const char *format, va_list args );
size_t mas_xvstrftime_time( char *str, size_t size, const char *format, va_list args );
size_t mas_tstrflocaltime( char *s, size_t max, const char *format, time_t t );

unsigned long long mas_strtime2long( const char *s, int *pr );
size_t mas_strflocaltime( char *s, size_t max, const char *format, const time_t * ptim );
size_t mas_strfgmtime( char *s, size_t max, const char *format, const time_t * ptim );


#endif
