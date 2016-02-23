#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fnmatch.h>
/* #include <unistd.h> */
#include <sys/time.h>

#include "mas_print_defs.h"
/* #include "mas_errors.h" */

/* ###################################################################### */
#include "mas_trace_utils_print.h"
#include "mas_trace.h"
/* ###################################################################### */

/* #define MAST_NOTIMING */

#if 0
static int
mas_vtrace_error( mas_trace_mode_t trace_mode MAST_UNUSED, const char *name MAST_UNUSED, int level MAST_UNUSED, int ern,
                  const char *funcid MAST_UNUSED, int linid MAST_UNUSED, unsigned flags MAST_UNUSED, int nerr MAST_UNUSED, FILE * out,
                  const char *prefix MAST_UNUSED, const char *fmt MAST_UNUSED, va_list args MAST_UNUSED )
{
  int r_ = 0;

  if ( ern < 0 )
  {
    const char *s = NULL;

    s = mas_error_name_i( ern );
    if ( ern < 0 && s )
      MAST_FPRINTFONE( 0, out, force_color, nocolor, "\n  [%s] (#%d; i.e.%d)\n", s, ern, mas_errnumber_i( ern ) );
    else
      MAST_FPRINTFONE( 0, out, force_color, nocolor, "Error rv=%d\n", ern );
  }
  return r_;
}
#endif
int
mas_vtrace( const char *name, int level, int minlevel, const char *funcid, int linid, double time0, char signum, unsigned flags, int nerr, FILE * out,
            const char *prefix, int fun_width, int force_color, int nocolor, const char *fmt, va_list args )
{
  int r_ = -1;
  static int ftimez = 0;
  static double timez = 0;

  if ( !ftimez )
  {
    int ry;
    struct timeval tv;

    ry = gettimeofday( &tv, NULL );
    if ( ry >= 0 )
      timez = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
    ftimez = 1;
  }
/* 
 * needless??
 *
 assert( signum != '-' ); (* '-' means there is no config pointer *)
 *
 * see definition of MAST_TRACE_WHAT_C at mas_trace_defs_what.h
 *
 * */
#if 0
  if ( trace_mode == MAST_TRACE_MODE_errorr )
  {
    r_ = mas_vtrace_error( trace_mode, name, level, minlevel, funcid, linid, flags, nerr, out, prefix, fmt, args );
  }
  else
#endif
  if ( level > minlevel )
  {
    char uname[10], *puname;
    int noeol = 0;
    int noinfo = 0;

  /* int highlight = 0; */

#ifndef MAST_NOTIMING
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
    /* if ( 0 == strncmp( pfuncid, "mas_", 4 ) ) */
    /*   pfuncid += 4;                           */
      for ( unsigned i = 0; i < sizeof( uname - 1 ) && name[i]; i++ )
        *puname++ = toupper( name[i] );
      *puname = 0;
    /* MAST_FPRINTFONE( 0, out, force_color, nocolor, "\r%cL%-2d:%2d ", signum, level, minlevel ); */
      MAST_FPRINTFONE( 0, out, force_color, nocolor, "%cL%-2d:%2d ", signum, level, minlevel );
      MAST_FPRINTFONE( 0, out, force_color, nocolor, "[%s%-7s%s] ", mas_fcoloro_s( out, force_color, nocolor, "\x1b[1;34m" ), uname,
                       mas_fcoloro_s( out, force_color, nocolor, "\x1b[m" ) );
      MAST_FPRINTFONE( 0, out, force_color, nocolor, "%s%3u%s:", mas_fcoloro_s( out, force_color, nocolor, "\x1b[1;33m" ), linid,
                       mas_fcoloro_s( out, force_color, nocolor, "\x1b[m" ) );
#if 0
      MAST_FPRINTFONE( 0, out, force_color, nocolor, "%-" T_FN_FMT "s", pfuncid );
#else
      {
        char xfmt[128];

/* \e[1;33m101\e[1;37m:mas_all_options\e[m */
        snprintf( xfmt, sizeof( xfmt ), "%s%%-%ds%s", mas_fcoloro_s( out, force_color, nocolor, "\x1b[1;32m" ),
                  fun_width ? fun_width /* mas_config && mas_config->opt.output.fun_width ? mas_config->opt.output.fun_width */ : T_FN_FMTN,
                  mas_fcoloro_s( out, force_color, nocolor, "\x1b[m" ) );
        MAST_FPRINTFONE( 0, out, force_color, nocolor, xfmt, pfuncid );
      }
#endif
    }
#ifndef MAST_NOTIMING
    {
      int ry;

      ry = gettimeofday( &tv, NULL );
      if ( ry >= 0 )
      {
# ifndef MAST_NOTIMING
        double timec = 0.;
# endif
        timec = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
        MAST_FPRINTFONE( 0, out, force_color, nocolor, " :%-6.4f:", timec - ( time0 ? time0 : timez ) );
      }
    }
#endif
#if 0
    {
      static char *hls[] = { "1;33;41", "1;7;32;44", "1;7;108;33", "1;7;108;32", "1;33;44", "1;37;46", "1;7;33;41", "7;101;35", "30;47" };
      if ( highlight > 0 && highlight < sizeof( hls ) / sizeof( hls[0] ) )
        MAST_FPRINTFONE( 0, out, force_color, nocolor, "\x1b[%sm ", hls[highlight] );
      else if ( highlight )
        MAST_FPRINTFONE( 0, out, force_color, nocolor, "\x1b[%sm ", hls[0] );
      else
        MAST_FPRINTFONE( 0, out, force_color, nocolor, " " );
    }
#endif
    r_ = MAST_FPRINTFONE( 0, out, force_color, nocolor, prefix ? "%-16s " : "%s", prefix ? prefix : " " );
    r_ = MAST_VFPRINTFONE( 0, out, force_color, nocolor, fmt, args );
#if 0
    if ( highlight )
      MAST_FPRINTFONE( 0, out, force_color, nocolor, "\x1b[m" );
#endif
    if ( flags & MAST_TRACE_FLAG_SYSTEM )
    {
      char serr[1024] = "Why?";
      char *s;

      s = strerror_r( nerr, serr, sizeof( serr ) - 1 );
      MAST_FPRINTFONE( 0, out, force_color, nocolor, "; errno:(%d) [%s]", nerr, s );
    }
    if ( !noeol )
    {
      MAST_FPRINTFONE( 0, out, force_color, nocolor, "\n" );
    }
  }
  return r_;
}

int
mas_trace( const char *name, int level, int minlevel, const char *funcid, int linid, double time0, char signum, unsigned flags, int nerr, FILE * out,
           const char *prefix, int fun_width, int force_color, int nocolor, const char *fmt, ... )
{
  int r_ = 0;
  va_list args;

  va_start( args, fmt );
/* takes ern - error index */
  r_ = mas_vtrace( name, level, minlevel, funcid, linid, time0, signum, flags, nerr, out, prefix, fun_width, force_color, nocolor, fmt, args );
  va_end( args );
  return r_;
}
