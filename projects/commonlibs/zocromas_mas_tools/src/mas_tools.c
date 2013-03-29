/* tools */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>




#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_memory.h>
#include "mas_arg_tools.h"

#include "mas_tools.h"


/*
this:
  mas_tools.c
related:
  mas_tools_def.h
  mas_msg_tools.c
  mas_thread_tools.c
  mas_memory.c
  mas_cs.c
  mas_cs.h
  mas_control.c
  mas_opts.c

*/

size_t
mas_tstrftime( char *s, size_t max, const char *format, time_t t )
{
  struct tm *tmp;

  tmp = localtime( &t );
  return strftime( s, max, format, tmp );
}

struct tm *
mas_xlocaltime( void )
{
  time_t t;
  struct tm *tmp;

  t = time( NULL );
  tmp = localtime( &t );
  /* if ( tmp != NULL && strftime( outstr, sizeof( outstr ), "%a, %d %b %Y %T %z", tmp ) == 0 ) */
  return tmp;
}

struct tm *
mas_xgmtime( void )
{
  time_t t;
  struct tm *tmp;

  t = time( NULL );
  tmp = gmtime( &t );
  /* if ( tmp != NULL && strftime( outstr, sizeof( outstr ), "%a, %d %b %Y %T %z", tmp ) == 0 ) */
  return tmp;
}

double
mas_double_time( void )
{
  struct timeval tv;
  double t;

  gettimeofday( &tv, NULL );
  t = ( ( double ) tv.tv_sec ) + ( ( ( double ) tv.tv_usec ) * 1E-6 );
  return t;
}

size_t
mas_xvsnprintf( char *str, size_t size, const char *format, va_list args )
{
  size_t r = 0;

  r = vsnprintf( str, size, format, args );
  return r;
}

size_t
mas_xvstrftime( char *str, size_t size, const char *format, va_list args )
{
  size_t r = 0;
  const struct tm *times;

  times = va_arg( args, const struct tm * );

  r = strftime( str, size, format, times );
  return r;
}

size_t
mas_xvstrftime_time( char *str, size_t size, const char *format, va_list args )
{
  size_t r = 0;
  const struct tm *times;
  time_t time;

  time = va_arg( args, time_t );

  times = gmtime( &time );
  r = strftime( str, size, format, times );
  return r;
}

char *
mas_ip_string( void *sin_addr )
{
  char ip[128];
  const char *pip;

  pip = mas_inet_ntop( AF_INET, sin_addr, ip, sizeof( ip ) );
  return pip ? mas_strdup( pip ) : NULL;
}

size_t
mas_parse_host_port( const char *host, unsigned *phport, unsigned default_port )
{
  const char *semi = NULL;
  size_t len = 0;
  unsigned hport = 0;

  semi = strchr( host, ':' );
  if ( semi )
  {
    len = semi - host;
    sscanf( semi + 1, "%u", &hport );
  }
  else
  {
    len = strlen( host );
  }
  if ( !hport )
    hport = default_port;
  /* if ( !hport )                */
  /*   hport = opts.default_port; */
  if ( phport )
    *phport = hport;
  /* ? hport : ( port ? port : opts.default_port ) */
  return len;
}

char *
mas_load_file( FILE * file, size_t size, size_t * ptruesize, const void *arg )
{
  char *filedata = NULL;

  /* rMSG( "TO load %s : %s", filepath, file ? "OPENED" : "NOT opened" ); */
  if ( file )
  {
    ssize_t r;

    if ( !size )
    {
      struct stat st;

      r = fstat( fileno( file ), &st );
      if ( r >= 0 && S_ISREG( st.st_mode ) )
        size = st.st_size;
    }
    if ( size > 0 )
    {
      filedata = mas_malloc( size );
      r = fread( filedata, 1, size, file );
      if ( r >= 0 && ptruesize )
        *ptruesize = r;
    }
  }
  else
  {
    /* MAS_LOGERR( errno, "file %s not loaded", filepath ); */
    /* errno = 0; */
  }
  return filedata;
}

char *
mas_load_filename( const char *filepath, size_t size, size_t * ptruesize, const void *arg )
{
  char *filedata = NULL;
  FILE *file = NULL;

  file = fopen( filepath, "r" );
  /* rMSG( "TO load %s : %s", filepath, file ? "OPENED" : "NOT opened" ); */
  if ( file )
  {
    filedata = mas_load_file( file, size, ptruesize, arg );
    fclose( file );
  }
  else
  {
    /* MAS_LOGERR( errno, "file %s not loaded", filepath ); */
    /* errno = 0; */
  }
  return filedata;
}

char *
mas_load_filename_at( const char *root, const char *tail, size_t size, size_t * ptruesize, const void *arg )
{
  char *filedata = NULL;
  char *filepath;

  filepath = mas_strdup( root );
  filepath = mas_strcat_x( filepath, tail );
  filedata = mas_load_filename( filepath, size, ptruesize, arg );
  if ( filepath )
    mas_free( filepath );
  return filedata;
}
