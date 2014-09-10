#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* ###################################################################### */
#include "duf_utils_print.h"
/* ###################################################################### */


int
duf_vprintf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, va_list args )
{
  int r = -1;

  if ( level > minlevel )
  {
    char rf = 0;
    const char *pfuncid;

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
      fprintf( out, "[DIE] @ %d:%d %3u:%-" FN_FMT ": ", level, minlevel, linid, pfuncid );
    }
    else if ( rf == '+' )
    {
      fprintf( out, "%d:%d %3u:%-" FN_FMT ": ", level, minlevel, linid, funcid );
    }
    {
      r = vfprintf( out, fmt, args );
    }
    if ( rf != '.' && rf != ':' )
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
  return r;
}

int
duf_printf( int level, int minlevel, int ifexit, const char *funcid, int linid, FILE * out, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = duf_vprintf( level, minlevel, ifexit, funcid, linid, out, fmt, args );
  va_end( args );
  return r;
}

int
duf_puts( int level, int minlevel, const char *funcid, int linid, FILE * out, const char *str )
{
  int r = 0;

  if ( level >= minlevel )
  {
    if ( str && *str )
      r = fputs( str, out );
    if ( r >= 0 )
      fputs( "\n", out );
  }
  return r;
}
