#ifndef MAS_TOOLS_H
#  define MAS_TOOLS_H

#  include <stdio.h>
#  include <stdarg.h>


size_t mas_xvsnprintf( char *str, size_t size, const char *format, va_list args );

struct tm *mas_xlocaltime( void );
double mas_double_time( void );

struct tm *mas_xgmtime( void );

size_t mas_xvstrftime( char *str, size_t size, const char *format, va_list args );
size_t mas_xvstrftime_time( char *str, size_t size, const char *format, va_list args );
size_t mas_tstrftime( char *s, size_t max, const char *format, time_t t );

char *mas_ip_string( void *sin_addr );

size_t mas_parse_host_port( const char *host, unsigned *phport, unsigned default_port );

char *mas_load_filename_at( const char *root, const char *tail, size_t size, size_t * ptruesize, const void *arg );
char *mas_load_filename( const char *tail, size_t size, size_t * ptruesize, const void *arg );
char *mas_load_file( FILE * file, size_t size, size_t * ptruesize, const void *arg );

#endif
