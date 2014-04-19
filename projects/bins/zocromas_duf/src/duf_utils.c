#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"


/* ###################################################################### */
#include "duf_utils.h"
/* ###################################################################### */

#define DUF_SINGLE_QUOTES_2_NOQ_ZERO
char *
duf_single_quotes_2( const char *s )
{
  char *r = NULL;

#ifdef DUF_SINGLE_QUOTES_2_NOQ_ZERO
  if ( strchr( s, '\'' ) )
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

int
duf_vtrace( const char *name, int level, int minlevel, const char *funcid, int linid, FILE * out, const char *fmt, va_list args )
{
  int r = -1;

  if ( level > minlevel )
  {
    char uname[10], *puname;
    char rf = 0;

    rf = *fmt;
    /* ; - no prfefix, cr   */
    /* . - no prefix, no cr */
    /* : - prefix, no cr    */
    if ( rf == '.' || rf == ':' || rf == ';' )
      fmt++;

    if ( rf != '.' && rf != ';' )
    {
      puname = uname;
      for ( int i = 0; i < sizeof( uname - 1 ) && name[i]; i++ )
        *puname++ = toupper( name[i] );
      *puname = 0;
      fprintf( out, "%d:%d [%-7s] %3u:%-23s: ", level, minlevel, uname, linid, funcid );
    }
    {
      r = vfprintf( out, fmt, args );
    }
    if ( rf != '.' && rf != ':' )
    {
      fprintf( out, ".\n" );
    }
  }
  return r;
}

int
duf_trace( const char *name, int level, int minlevel, const char *funcid, int linid, FILE * out, const char *fmt, ... )
{
  int r;
  va_list args;

  va_start( args, fmt );
  r = duf_vtrace( name, level, minlevel, funcid, linid, out, fmt, args );
  va_end( args );
  return r;
}
