#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

/* ###################################################################### */
#include "duf_utils.h"
/* ###################################################################### */

#define DUF_SINGLE_QUOTES_2_NOQ_ZERO
char *
duf_single_quotes_2( const char *s )
{
  char *r = NULL;

#ifdef DUF_SINGLE_QUOTES_2_NOQ_ZERO
  if ( s && strchr( s, '\'' ) )
#endif
  {
    size_t len;
    const char *ip;
    char *op;
    size_t siz;
    unsigned qcnt = 0;

    len = strlen( s );
    ip = s;
    while ( ip && *ip )
    {
      if ( *ip++ == '\'' )
        qcnt++;
    }
    if ( qcnt )
    {
      siz = ( len + 1 + qcnt );
      r = mas_malloc( siz );
      /* fprintf( stderr, "Q:[%s] (%lu)\n", s, len ); */
      ip = s;
      op = r;
      while ( ip && *ip && op < r + siz )
      {
        if ( *ip == '\'' )
          *op++ = '\'';
        *op++ = *ip;
        ip++;
      }
      *op++ = 0;
    }
#ifndef DUF_SINGLE_QUOTES_2_NOQ_ZERO
    else
    {
      r = mas_strdup( s );
    }
#endif
  }
  return r;
}

void
duf_percent( unsigned long long curval, unsigned long long maxval, const char *msg )
{
  float width = 90;
  static duf_bar_t bar;

  bar.percent = ( ( ( float ) curval ) / ( ( float ) maxval ) );
  bar.width = width * bar.percent;
  if ( bar.width != ( bar.prev_width - 1 ) )
  {
    char cur_time[128] = "??";

    duf_strflocaltime( cur_time, sizeof( cur_time ), "%Y%m%d.%H:%M:%S", NULL );

    if ( bar.width == 0 )
      fputs( "\n", stderr );
    fprintf( stderr, "\r [" );
    for ( int i = 0; i < bar.width - 1; i++ )
      fputc( '=', stderr );
    fputc( '>', stderr );
    for ( int i = bar.width; i < width; i++ )
      fputc( ' ', stderr );

    fprintf( stderr, "] %d%%; %llu of %llu; %llu to do; %s %s  ", ( int ) ( bar.percent * 100. ), curval, maxval, maxval - curval, cur_time, msg );
    bar.prev_width = bar.width + 1;
    if ( bar.width == width )
      fputs( "\n", stderr );
  }
}

size_t
duf_strflocaltime( char *s, size_t max, const char *format, const time_t * ptim )
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
duf_strfgmtime( char *s, size_t max, const char *format, const time_t * ptim )
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

long
duf_strtol_suff( const char *s, int *pr )
{
  int r = 0;
  long l = 0;
  char *pe = NULL;

  l = strtol( s, &pe, 10 );
  if ( pe )
  {
    switch ( *pe )
    {
    case 'G':
      l *= 1024 * 1024 * 1024;
      break;
    case 'M':
      l *= 1024 * 1024;
      break;
    case 'k':
      l *= 1024;
      break;
    case 'w':
      l *= 2;
      break;
    case 'c':
    case '\0':
      break;
    case 'b':
      l *= 512;
      break;
    default:
      r=-1;
      l = 0;
      break;
    }
  }
  else
  {
    r=-1;
    l = 0;
  }
  if ( pr )
    *pr = r;
  return l;
}

long long
duf_strtoll_suff( const char *s, int *pr )
{
  int r = 0;
  long l = 0;
  char *pe = NULL;

  l = strtoll( s, &pe, 10 );
  if ( pe )
  {
    switch ( *pe )
    {
    case 'G':
      l *= 1024 * 1024 * 1024;
      break;
    case 'M':
      l *= 1024 * 1024;
      break;
    case 'k':
      l *= 1024;
      break;
    case 'w':
      l *= 2;
      break;
    case 'c':
    case '\0':
      break;
    case 'b':
      l *= 512;
      break;
    default:
      r=-1;
      l = 0;
      break;
    }
  }
  else
  {
    r=-1;
    l = 0;
  }
  if ( pr )
    *pr = r;
  return l;
}

unsigned long long
duf_strtime2long( const char *s, int *pr )
{
  int r = 0;
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
      r=-1;
  }
  if ( pr )
    *pr = r;
  return ( unsigned long long ) t;
}
