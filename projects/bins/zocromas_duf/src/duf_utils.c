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
    if ( bar.width == 0 )
      fputs( "\n", stderr );
    fprintf( stderr, "\r [" );
    for ( int i = 0; i < bar.width - 1; i++ )
      fputc( '=', stderr );
    fputc( '>', stderr );
    for ( int i = bar.width; i < width; i++ )
      fputc( ' ', stderr );
    fprintf( stderr, "] %d%%; %llu of %llu; %llu to do; %s  ", ( int ) ( bar.percent * 100. ), curval, maxval, maxval - curval, msg );
    bar.prev_width = bar.width + 1;
    if ( bar.width == width )
      fputs( "\n", stderr );
  }
}
