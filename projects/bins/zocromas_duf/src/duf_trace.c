#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fnmatch.h>
/* #include <unistd.h> */
#include <sys/time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_errors.h"


/* ###################################################################### */
#include "duf_trace.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */

static int
duf_vtrace_error( duf_trace_mode_t trace_mode, const char *name, int level, duf_error_code_t ern, const char *funcid, int linid,
                  unsigned flags, int nerr, FILE * out, const char *fmt, va_list args )
{
  int r = 0;

  if ( ern < 0 )
  {
    const char *s = duf_error_name( ern );

    if ( ern < 0 && s )
      fprintf( out, "\n  [%s] (#%d; i.e.%d)\n", duf_error_name( ern ), ern, +ern - DUF_ERROR_ERROR_BASE );
    else
      fprintf( out, "Error rv=%d\n", ern );
  }
  return r;
}

int
duf_vtrace( duf_trace_mode_t trace_mode, const char *name, int level, int minlevel, const char *funcid, int linid, double time0,
            unsigned flags, int nerr, FILE * out, const char *fmt, va_list args )
{
  int r = -1;

#ifndef DUF_NOTIMING
  double timec;
#endif
  if ( trace_mode == DUF_TRACE_MODE_errorr )
  {
    r = duf_vtrace_error( trace_mode, name, level, minlevel, funcid, linid, flags, nerr, out, fmt, args );
  }
  else if ( level > minlevel )
  {
    char uname[10], *puname;
    char rf = 0;

#ifndef DUF_NOTIMING
    int rt;
    struct timeval tv;
#endif
    rf = *fmt;
    /* ; - no prefix, cr   */
    /* . - no prefix, no cr */
    /* : - prefix, no cr    */
    if ( rf == '.' || rf == ':' || rf == ';' )
      fmt++;

    if ( rf != '.' && rf != ';' )
    {
      const char *pfuncid;

      puname = uname;
      pfuncid = funcid;
      /* if ( 0 == strncmp( pfuncid, "duf_", 4 ) ) */
      /*   pfuncid += 4;                           */
      for ( int i = 0; i < sizeof( uname - 1 ) && name[i]; i++ )
        *puname++ = toupper( name[i] );
      *puname = 0;

      fprintf( out, "%d:%d [%-7s] %3u:%-" FN_FMT ": ", level, minlevel, uname, linid, pfuncid );
    }
#ifndef DUF_NOTIMING
    rt = gettimeofday( &tv, NULL );
    timec = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
    if ( rt >= 0 )
    {
      fprintf( out, "%-7.4f: ", timec - time0 );
    }
#endif
    {
      r = vfprintf( out, fmt, args );
    }
    if ( flags & DUF_TRACE_FLAG_SYSTEM )
    {
      char serr[1024] = "Why?";
      char *s;

      s = strerror_r( nerr, serr, sizeof( serr ) - 1 );
      fprintf( out, "; errno:(%d) [%s]", nerr, s );
    }
    if ( rf != '.' && rf != ':' )
    {
      fprintf( out, "\n" );
    }
  }
  return r;
}

int
duf_trace( duf_trace_mode_t trace_mode, const char *name, int level, int minlevel, const char *funcid, int linid, double time0,
           unsigned flags, int nerr, FILE * out, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = duf_vtrace( trace_mode, name, level, minlevel, funcid, linid, time0, flags, nerr, out, fmt, args );
  va_end( args );
  return r;
}
