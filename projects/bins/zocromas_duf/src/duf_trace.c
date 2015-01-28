#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fnmatch.h>
/* #include <unistd.h> */
#include <sys/time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_print_defs.h"
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
    const char *s = NULL;

    s = duf_error_name( ern );
    if ( ern < 0 && s )
      fprintf( out, "\n  [%s] (#%d; i.e.%d)\n", duf_error_name( ern ), ern, duf_errindex( ern ) );
    else
      fprintf( out, "Error rv=%d\n", ern );
  }
  return r;
}

int
duf_vtrace( duf_trace_mode_t trace_mode, const char *name, int level, int minlevel, const char *funcid, int linid, double time0, char signum,
            unsigned flags, int nerr, FILE * out, const char *fmt, va_list args )
{
  int r = -1;

  assert( signum != '-' );
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
    int noeol = 0;
    int noinfo = 0;
    int highlight = 0;

#ifndef DUF_NOTIMING
    int rt;
    struct timeval tv;
#endif
    {
      int valid;

      /*
       * . - w/o eol and w/o info-prefix
       * : - w/o eol
       * ; - w/o info-prefix
       * @ - highlight
       * */
      do
      {
        valid = 0;
        if ( strchr( ":.", *fmt ) )
        {
          noeol = valid = 1;
        }
        if ( strchr( ";.", *fmt ) )
        {
          noinfo = valid = 1;
        }
        if ( *fmt == '@' )
        {
          valid = 1;
          highlight++;
        }
      }
      while ( fmt += valid, valid );
    }
    if ( !noinfo )
    {
      const char *pfuncid;

      puname = uname;
      pfuncid = funcid;
      /* if ( 0 == strncmp( pfuncid, "duf_", 4 ) ) */
      /*   pfuncid += 4;                           */
      for ( int i = 0; i < sizeof( uname - 1 ) && name[i]; i++ )
        *puname++ = toupper( name[i] );
      *puname = 0;
      fprintf( out, "%c%2d:%2d [%-7s] %3u:%-" FN_FMT ":", signum, level, minlevel, uname, linid, pfuncid );
    }
#ifndef DUF_NOTIMING
    rt = gettimeofday( &tv, NULL );
    timec = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
    if ( rt >= 0 )
    {
      fprintf( out, "%-7.4f:", timec - time0 );
    }
#endif
    static char *hls[] = { "1;33;41", "1;33;44", "1;37;46", "1;7;33;41", "30;47" };
    if ( highlight > 0 && highlight < sizeof( hls ) / sizeof( hls[0] ) )
      fprintf( out, "\x1b[%sm ", hls[highlight] );
    else if ( highlight )
      fprintf( out, "\x1b[%sm ", hls[0] );
    else
      fprintf( out, " " );
    {
      r = vfprintf( out, fmt, args );
    }
    if ( highlight )
      fprintf( out, "\x1b[m" );
    if ( flags & DUF_TRACE_FLAG_SYSTEM )
    {
      char serr[1024] = "Why?";
      char *s;

      s = strerror_r( nerr, serr, sizeof( serr ) - 1 );
      fprintf( out, "; errno:(%d) [%s]", nerr, s );
    }
    if ( !noeol )
    {
      fprintf( out, "\n" );
    }
  }
  return r;
}

int
duf_trace( duf_trace_mode_t trace_mode, const char *name, int level, int minlevel, const char *funcid, int linid, double time0, char signum,
           unsigned flags, int nerr, FILE * out, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = duf_vtrace( trace_mode, name, level, minlevel, funcid, linid, time0, signum, flags, nerr, out, fmt, args );
  va_end( args );
  return r;
}
