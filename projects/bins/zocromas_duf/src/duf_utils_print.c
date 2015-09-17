#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_utils_print.h"
/* ###################################################################### */


int
duf_vprintf( int level, int noeol, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, va_list args )
{
  int ry = -1;
  int colorize = 1;

  if ( level >= minlevel )
  {
    char rf = 0;
    const char *pfuncid;
    int highlight = 0;

    pfuncid = funcid;
    /* if ( 0 == strncmp( pfuncid, "duf_", 4 ) ) */
    /*   pfuncid += 4;                           */

    rf = *fmt;
    /* ; - no prefix, cr   */
    /* . - no prefix, no cr */
    /* : - prefix, no cr    */
    if ( rf == '.' || rf == ':' || rf == ';' || rf == '+' )
      fmt++;



    if ( ifexit )
    {
      fprintf( out, "[DIE] @ %d:%d %3u:%-" P_FN_FMT "s: ", level, minlevel, linid, pfuncid );
    }
    else if ( rf == '+' )
    {
      fprintf( out, "%d:%d %3u:%-" P_FN_FMT "s: ", level, minlevel, linid, funcid );
    }
    if ( colorize )
    {
      int valid;

#if 0
      do
      {
        valid = 0;
        if ( *fmt == '@' )
        {
          valid = 1;
          highlight++;
        }
      }
      while ( fmt += valid, valid );
      {
        static char *hls[] = { "1;33;41", "1;7;32;44", "1;7;108;33", "1;7;108;32", "1;33;44", "1;37;46", "1;7;33;41", "7;101;35", "30;47" };
        if ( highlight > 0 && highlight < sizeof( hls ) / sizeof( hls[0] ) )
          fprintf( out, "\x1b[%sm ", hls[highlight] );
        else if ( highlight )
          fprintf( out, "\x1b[%sm ", hls[0] );
        else
          fprintf( out, " " );
      }
      ry = vfprintf( out, fmt, args );
      if ( highlight )
        fprintf( out, "\x1b[m" );
#else
      /* TODO make new fmt with preset colors */
      char buf[1024 * 64];
      const char *pbuf = buf;

      ry = vsnprintf( buf, sizeof( buf ), fmt, args );
      do
      {
        valid = 0;
        if ( *pbuf == '@' )
        {
          valid = 1;
          highlight++;
        }
      }
      while ( pbuf += valid, valid );
      {
        static char *hls[] = {
          "1;39", "1;7;37;40", "1;33;41", "1;7;32;44", "1;7;108;33", "1;7;108;32", "1;33;44", "1;37;46", "1;7;33;41", "7;101;35", "30;47"
        };
        if ( highlight > 0 && highlight < sizeof( hls ) / sizeof( hls[0] ) )
          fprintf( out, "\x1b[%sm", hls[highlight] );
        else if ( highlight )
          fprintf( out, "\x1b[%sm", hls[0] );
        /* fprintf( out, "%s", pbuf ); */
        fwrite( pbuf, 1, strlen( pbuf ), out );
        if ( highlight )
          fprintf( out, "\x1b[m" );
      }
#endif
    }
    if ( rf != '.' && rf != ':' && !noeol )
    {
      fprintf( out, "\n" );
    }
  }
  else
  {
    /* fprintf( out, "SKP %d %d\n", level, minlevel ); */
  }
  if ( ifexit )
    exit( ifexit );
  return ry;
}

int
duf_printf( int level, int noeol, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, ... )
{
  int ry = 0;
  va_list args;

  va_start( args, fmt );
  ry = duf_vprintf( level, noeol, minlevel, ifexit, funcid, linid, out, fmt, args );
  va_end( args );
  return ry;
}

int
duf_puts( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str )
{
  int ry = 0;

  if ( level >= minlevel )
  {
    if ( str && *str )
      ry = fputs( str, out );
    if ( ry >= 0 )
      fputs( "\n", out );
  }
  return ry;
}

int
duf_writes( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str )
{
  int ry = 0;

  if ( level >= minlevel )
  {
    if ( str && *str )
    {
      ry = fwrite( str, 1, strlen( str ), out );
    }
  }
  return ry;
}
