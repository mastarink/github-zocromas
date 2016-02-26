#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

/* ########################################################################################## */
#include "mas_time.h"
/* ########################################################################################## */

size_t
mas_tstrflocaltime( char *s, size_t max, const char *format, time_t t )
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

unsigned long long
mas_strtime2long( const char *s, int *pr )
{
  int rpr = 0;
  time_t t = 0;
  char *p = NULL;
  struct tm tm;

  {
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y-%m-%d %H:%M:%S", &tm );
    }
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y-%m-%d", &tm );
    }
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y%m%d.%H%M%S", &tm );
    }
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y%m%d.%H%M", &tm );
    }
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y%m%d.%H", &tm );
    }
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y%m%d", &tm );
    }
    tm.tm_zone = tzname[daylight];
    tm.tm_isdst = daylight;
    if ( p )
      t = timelocal( &tm );
    else
      rpr = -1;
  }
  if ( pr )
    *pr = rpr;
  return ( unsigned long long ) t;
}

size_t
mas_strflocaltime( char *s, size_t max, const char *format, const time_t * ptim )
{
  struct tm tm;
  size_t sz = 0;
  time_t timet;

  if ( !ptim )
  {
    timet = time( NULL );
    ptim = &timet;
  }

  if ( localtime_r( ptim, &tm ) )
    sz = strftime( s, max, format, &tm );
  return sz;
}

size_t
mas_strfgmtime( char *s, size_t max, const char *format, const time_t * ptim )
{
  struct tm tm;
  size_t sz = 0;
  time_t timet;

  if ( !ptim )
  {
    timet = time( NULL );
    ptim = &timet;
  }

  if ( gmtime_r( ptim, &tm ) )
    sz = strftime( s, max, format, &tm );
  return sz;
}
