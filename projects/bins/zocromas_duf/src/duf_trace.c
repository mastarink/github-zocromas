#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fnmatch.h>
/* #include <unistd.h> */
#include <sys/time.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_print_defs.h"
#include "duf_errors.h"


/* ###################################################################### */
#include "duf_utils_print.h"
#include "duf_trace.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */

static int
duf_vtrace_error( duf_trace_mode_t trace_mode DUF_UNUSED, const char *name DUF_UNUSED, int level DUF_UNUSED, duf_error_index_t ern,
                  const char *funcid DUF_UNUSED, int linid DUF_UNUSED, unsigned flags DUF_UNUSED, int nerr DUF_UNUSED, FILE * out,
                  const char *prefix DUF_UNUSED, const char *fmt DUF_UNUSED, va_list args DUF_UNUSED )
{
  int r_ = 0;

  if ( ern < 0 )
  {
    const char *s = NULL;

    s = duf_error_name_i( ern );
    if ( ern < 0 && s )
      DUF_FPRINTFNE( 0, out, "\n  [%s] (#%d; i.e.%d)\n", s, ern, duf_errnumber_i( ern ) );
    else
      DUF_FPRINTFNE( 0, out, "Error rv=%d\n", ern );
  }
  return r_;
}

int
duf_vtrace( duf_trace_mode_t trace_mode, duf_trace_submode_t trace_submode DUF_UNUSED, const char *name, int level, int minlevel, const char *funcid, int linid,
            double time0, char signum, unsigned flags, duf_error_index_t nerr, FILE * out, const char *prefix, const char *fmt, va_list args )
{
  int r_ = -1;

  /* 
   * needless??
   *
   assert( signum != '-' ); (* '-' means there is no config pointer *)
   *
   * see definition of DUF_TRACE_WHAT_C at duf_trace_defs_what.h
   *
   * */
  if ( trace_mode == DUF_TRACE_MODE_errorr )
  {
    r_ = duf_vtrace_error( trace_mode, name, level, minlevel, funcid, linid, flags, nerr, out, prefix, fmt, args );
  }
  else if ( level > minlevel )
  {
    char uname[10], *puname;
    int noeol = 0;
    int noinfo = 0;

    /* int highlight = 0; */

#ifndef DUF_NOTIMING
    struct timeval tv;
#endif
#if 0
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
#endif
    if ( !noinfo )
    {
      const char *pfuncid;

      puname = uname;
      pfuncid = funcid;
      /* if ( 0 == strncmp( pfuncid, "duf_", 4 ) ) */
      /*   pfuncid += 4;                           */
      for ( unsigned i = 0; i < sizeof( uname - 1 ) && name[i]; i++ )
        *puname++ = toupper( name[i] );
      *puname = 0;
      DUF_FPRINTFNE( 0, out, "\r%c%2d:%2d ", signum, level, minlevel );
      DUF_FPRINTFNE( 0, out, "[%s%-7s%s] ", duf_fcolor_s( out, "\x1b[1;34m" ), uname, duf_fcolor_s( out, "\x1b[m" ) );
      DUF_FPRINTFNE( 0, out, "%s%3u%s:", duf_fcolor_s( out, "\x1b[1;33m" ), linid, duf_fcolor_s( out, "\x1b[m" ) );
#if 0
      DUF_FPRINTFNE( 0, out, "%-" T_FN_FMT "s", pfuncid );
#else
      {
        char xfmt[128];

/* \e[1;33m101\e[1;37m:duf_all_options\e[m */
        snprintf( xfmt, sizeof( xfmt ), "%s%%-%ds%s", duf_fcolor_s( out, "\x1b[1;32m" ),
                  duf_config && duf_config->cli.output.fun_width ? duf_config->cli.output.fun_width : T_FN_FMTN, duf_fcolor_s( out, "\x1b[m" ) );
        DUF_FPRINTFNE( 0, out, xfmt, pfuncid );
      }
#endif
    }
#ifndef DUF_NOTIMING
    {
      int ry;

      ry = gettimeofday( &tv, NULL );
      if ( ry >= 0 )
      {
#  ifndef DUF_NOTIMING
        double timec = 0.;
#  endif
        timec = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
        DUF_FPRINTFNE( 0, out, " :%-6.4f:", timec - time0 );
      }
    }
#endif
#if 0
    {
      static char *hls[] = { "1;33;41", "1;7;32;44", "1;7;108;33", "1;7;108;32", "1;33;44", "1;37;46", "1;7;33;41", "7;101;35", "30;47" };
      if ( highlight > 0 && highlight < sizeof( hls ) / sizeof( hls[0] ) )
        DUF_FPRINTFNE( 0, out, "\x1b[%sm ", hls[highlight] );
      else if ( highlight )
        DUF_FPRINTFNE( 0, out, "\x1b[%sm ", hls[0] );
      else
        DUF_FPRINTFNE( 0, out, " " );
    }
#endif
    r_ = DUF_FPRINTFNE( 0, out, prefix ? "%-16s " : "%s", prefix ? prefix : " " );
    r_ = DUF_VFPRINTFNE( 0, out, fmt, args );
#if 0
    if ( highlight )
      DUF_FPRINTFNE( 0, out, "\x1b[m" );
#endif
    if ( flags & DUF_TRACE_FLAG_SYSTEM )
    {
      char serr[1024] = "Why?";
      char *s;

      s = strerror_r( nerr, serr, sizeof( serr ) - 1 );
      DUF_FPRINTFNE( 0, out, "; errno:(%d) [%s]", nerr, s );
    }
    if ( !noeol )
    {
      DUF_FPRINTFNE( 0, out, "\n" );
    }
  }
  return r_;
}

int
duf_trace( duf_trace_mode_t trace_mode, duf_trace_submode_t trace_submode, const char *name, int level, int minlevel, const char *funcid, int linid,
           double time0, char signum, unsigned flags, duf_error_index_t nerr, FILE * out, const char *prefix, const char *fmt, ... )
{
  int r_ = 0;
  va_list args;

  va_start( args, fmt );
  /* takes ern - error index */
  r_ = duf_vtrace( trace_mode, trace_submode, name, level, minlevel, funcid, linid, time0, signum, flags, nerr, out, prefix, fmt, args );
  va_end( args );
  return r_;
}
