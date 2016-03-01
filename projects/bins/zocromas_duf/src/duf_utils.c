/* #undef MAS_TRACING */
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_time.h>                                   /* mas_(|double_|xlocal|xgm|xvstrf|xvstrftime_|(|t)strflocal|strfgm)time ; strtime2long; etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ♠ */

/* ###################################################################### */
#include "duf_utils.h"                                               /* duf_percent;  etc. ♠ */
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

void
duf_percent( unsigned long long curval, unsigned long long curval2, unsigned long long curval3, unsigned long long maxval, const char *msg )
{
  typedef struct
  {
    float percent;
    unsigned calls;
    int width;
    float prev_percent;
  } duf_bar_t;
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

    mas_strflocaltime( cur_time, sizeof( cur_time ), "%Y%m%d.%H:%M:%S", NULL );

  /* if ( !bar.calls )        */
  /*   fputs( "\n", stderr ); */
    bar.calls++;
    fprintf( stderr, "\r [" );
    {
      mas_print_color_s( stderr, "\x1b[%sm", "1;33" );
      for ( int i = 0; i < bar.width; i++ )
        fputc( '=', stderr );
      mas_print_color_s( stderr, "\x1b[%sm", "0" );
    }
    if ( bar.width < width )
    {
      fputc( '>', stderr );
      for ( int i = bar.width + 1; i < width; i++ )
        fputc( ' ', stderr );
    }
#if 1
    fprintf( stderr, "] %d%%; %llu of %llu; %llu to do; %2gs avg:%2gms (%9.7g ~ %llu ~ %llu ~ %9.7g) %s  \r", ( int ) ( bar.percent * 100. ), curval,
             maxval, maxval - curval, delta_sec, delta_sec * 1000. / ( ( double ) curval ),
             curval2 ? ( ( double ) delta_sec * 1E9 / ( ( double ) curval2 ) ) : 0., curval2, curval3,
             ( ( double ) curval2 ) / ( ( double ) curval3 ), msg );
#else
    fprintf( stderr, "] %d%%; %llu of %llu; %llu to do; %s %2g %s  ", ( int ) ( bar.percent * 100. ), curval, maxval, maxval - curval, cur_time,
             delta_sec, msg );
#endif
    bar.prev_percent = bar.percent;
    if ( bar.width == width )
      fputs( "\n", stderr );
  }
}
