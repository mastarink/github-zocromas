/* #undef MAS_TRACING */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

#include "duf_maintenance.h"
#include "duf_printn_defs.h"

/* ###################################################################### */
#include "duf_utils.h"
/* ###################################################################### */

#define DUF_SINGLE_QUOTES_2_NOQ_ZERO
char *
duf_single_quotes_2( const char *s )
{
  char *sr = NULL;

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
      sr = mas_malloc( siz );
      /* fprintf( stderr, "Q:[%s] (%lu)\n", s, len ); */
      ip = s;
      op = sr;
      while ( ip && *ip && op < sr + siz )
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
      sr = mas_strdup( s );
    }
#endif
  }
  return sr;
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
  int rpr = 0;
  long l = 0;
  char *pe = NULL;

  if ( s )
  {
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
        rpr = -1;
        l = 0;
        break;
      }
    }
    else
    {
      rpr = -1;
      l = 0;
    }
  }
  if ( pr )
    *pr = rpr;
  return l;
}

long long
duf_strtoll_suff( const char *s, int *pr )
{
  int rpr = 0;
  long l = 0;
  char *pe = NULL;

  if ( s )
  {
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
        rpr = -1;
        l = 0;
        break;
      }
    }
    else
    {
      rpr = -1;
      l = 0;
    }
  }
  if ( pr )
    *pr = rpr;
  return l;
}

unsigned long long
duf_strtime2long( const char *s, int *pr )
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

void
duf_percent( unsigned long long curval, unsigned long long curval2, unsigned long long curval3, unsigned long long maxval, const char *msg )
{
  double width = 80.0;
  int swidth;
  static duf_bar_t bar;
  const char *sc = getenv( "COLUMNS" );
  double delta_sec = 0;

  /* assert( curval <= maxval ); */
  if ( sc && *sc )
  {
    swidth = strtol( sc, NULL, 10 );
    width = ( double ) swidth - 90;
  }
  {
    struct timeval tv;
    static double time0;
    double timec;

    if ( curval == 1 )
    {
      memset( &bar, 0, sizeof( bar ) );
      gettimeofday( &tv, NULL );
      time0 = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
      /* fprintf( stderr,"\n@###@"); */
    }
    gettimeofday( &tv, NULL );
    timec = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
    delta_sec = timec - time0;
  }
  bar.percent = ( ( ( double ) curval ) / ( ( double ) maxval ) );
  bar.width = width * bar.percent;
  if ( ( ( int ) ( bar.percent * 100. ) ) != ( ( int ) ( bar.prev_percent * 100. ) ) || bar.percent > .98 )
    /* if ( bar.percent != bar.prev_percent ) */
  {
    char cur_time[128] = "??";

    duf_strflocaltime( cur_time, sizeof( cur_time ), "%Y%m%d.%H:%M:%S", NULL );

    /* if ( !bar.calls )        */
    /*   fputs( "\n", stderr ); */
    bar.calls++;
    fprintf( stderr, "\r [" );
    {
      DUF_PRINT_COLOR_S( stderr, "\x1b[%sm", "1;33" );
      for ( int i = 0; i < bar.width; i++ )
        fputc( '=', stderr );
      DUF_PRINT_COLOR_S( stderr, "\x1b[%sm", "0" );
    }
    if ( bar.width < width )
    {
      fputc( '>', stderr );
      for ( int i = bar.width + 1; i < width; i++ )
        fputc( ' ', stderr );
    }
#if 1
    fprintf( stderr, "] %d%%; %llu of %llu; %llu to do; %2gs avg:%2gms (%9.7g ~ %llu ~ %llu ~ %9.7g) %s  \r", ( int ) ( bar.percent * 100. ), curval, maxval,
             maxval - curval, delta_sec, delta_sec * 1000. / ( ( double ) curval ), curval2 ? (( double ) delta_sec * 1E9 / ( ( double ) curval2 ) ) : 0.,
             curval2,curval3,  ((double)curval2)/((double)curval3), msg );
#else
    fprintf( stderr, "] %d%%; %llu of %llu; %llu to do; %s %2g %s  ", ( int ) ( bar.percent * 100. ), curval, maxval, maxval - curval, cur_time,
             delta_sec, msg );
#endif
    bar.prev_percent = bar.percent;
    if ( bar.width == width )
      fputs( "\n", stderr );
  }
}
