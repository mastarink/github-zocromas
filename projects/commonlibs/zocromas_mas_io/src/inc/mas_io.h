#ifndef MAS_IO_H
#  define MAS_IO_H

#  include <stdio.h>
#  include <stdarg.h>

int mas_write_string( int fd, const char *cbuf, int z );

int mas_vwritef( int fd, const char *fmt, va_list args );
int mas_vfprintf( FILE * f, const char *fmt, va_list args );

int mas_writef( int fd, const char *fmt, ... );

int mas_read_buf( int fd, char **pbuf );
int mas_read_string( int fd, char **pbuf );

#  ifndef MAS_CHANNEL_STREAM_READ
int mas_io_read_some( int fd, char **pbuf, size_t * psz, ssize_t maxsz );
#  else
int mas_io_fread_some( FILE * f, char **pbuf, size_t * psz, ssize_t maxsz );
#  endif

int mas_fprintf( FILE * f, const char *fmt, ... );
int mas_fwrite_string( FILE * f, char *cbuf, int z );

size_t mas_fread( void *ptr, size_t size, size_t nmemb, FILE * stream );
size_t mas_fwrite( const void *ptr, size_t size, size_t nmemb, FILE * stream );

/* #include <mastar/types/mas_common_cdefs.h> */
#endif
