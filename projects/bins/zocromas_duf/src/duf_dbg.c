#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


/* ###################################################################### */
#include "duf_dbg.h"
/* ###################################################################### */

int duf_dbgfunlevel = 0;
int dbgfunc_enabled = 0;

/* 20150924.144132 */
int
duf_dbgfuncv( duf_dbgcode_t code, const char *func, int line, va_list args )
{
  int minverbose = 4;

  if ( dbgfunc_enabled && duf_config && ( ( DUF_CONFIGX( cli.dbg.verbose ) >= minverbose ) || DUF_CONFIGX( cli.trace.calls ) ) )
  {
    static int inited = 0;
    static char pref[1024];

#define DUF_PREF_ISIZE ((int)sizeof( pref ))

    if ( !inited )
    {
      memset( pref, 0, DUF_PREF_ISIZE );
      inited = 1;
    }
    if ( duf_dbgfunlevel > DUF_PREF_ISIZE - 1 )
      duf_dbgfunlevel = DUF_PREF_ISIZE - 1;

    assert( duf_dbgfunlevel < DUF_PREF_ISIZE );
    pref[duf_dbgfunlevel] = 0;
    if ( ( !DUF_CONFIGX( cli.dbg.max_line ) || DUF_CONFIGX( cli.dbg.lines ) < DUF_CONFIGX( cli.dbg.max_line ) )
         && ( DUF_CONFIGX( cli.dbg.lines ) >= DUF_CONFIGX( cli.dbg.min_line ) ) )
    {
      FILE *out = DUF_CONFIGX( cli.trace.output.out ) ? DUF_CONFIGX( cli.trace.output.out ) : stderr;

      switch ( code )
      {
      case DBG_START:
        /* if ( !DUF_CONFIGX(cli.trace.calls) || DUF_CONFIGX(cli.trace.calls) > duf_dbgfunlevel + 1 ) */
        /*   DUF_FPRINTFNE( 0,  out, "#%4lu. ", DUF_CONFIGX(cli.dbg.lines );                                  */
        assert( duf_dbgfunlevel < DUF_PREF_ISIZE );
        pref[duf_dbgfunlevel++] = ' ';
        pref[duf_dbgfunlevel] = 0;
        assert( duf_dbgfunlevel < DUF_PREF_ISIZE );
        break;
      case DBG_END:
      case DBG_ENDR:
      case DBG_ENDRS:
      case DBG_ENDULL:
      case DBG_ENDS:
      case DBG_ENDCS:
        /* if ( !DUF_CONFIGX(cli.trace.calls) || DUF_CONFIGX(cli.trace.calls) > duf_dbgfunlevel + 1 ) */
        /*   DUF_FPRINTFNE( 0,  out, "#%4lu. ", DUF_CONFIGX(cli.dbg.lines) );                                  */
        /* pref[duf_dbgfunlevel--] = 0; */
        break;
      case DBG_STEP:
      case DBG_STEPS:
      case DBG_STEPIS:
      case DBG_STEPI2S:
      case DBG_STEPULL:
        break;
        /* default: */
        /*   break; */
      }

      /* DUF_FPRINTFNE( 0,  out, "%3d:%-23s .........\n", DUF_CONFIGX(cli.dbg.lines), duf_dbgfunlevel, line, func ); */
      /* DUF_FPRINTFNE( 0,  out, "%4lu of %4lu <<<<<<<<<<\n", DUF_CONFIGX(cli.dbg.lines), DUF_CONFIGX(cli.dbg.max_line) ); */


      /* DUF_FPRINTFNE( 0,  out, "L%2u", duf_dbgfunlevel ); */
      switch ( code )
      {
      case DBG_START:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 1 )
          DUF_FPRINTFNE( 0, out, "#%4lu. %s┌─%3d:%-23s .........\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func );
        break;
      case DBG_STEP:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 2 )
          DUF_FPRINTFNE( 0, out, "#%4lu. %s│ %3d:%-23s .........\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func );
        break;
      case DBG_STEPIS:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 2 )
        {
          int i = 0;
          const char *s;
          i = va_arg( args, int );
          s = va_arg( args, const char * );

          DUF_FPRINTFNE( 0, out, "#%4lu. %s│ %3d:%-23s ......... int(%d) (%s)\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func, i, s );
        }
        break;
      case DBG_STEPI2S:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 2 )
        {
          int i1 = 0;
          int i2 = 0;
          const char *s;
          i1 = va_arg( args, int );
          i2 = va_arg( args, int );
          s = va_arg( args, const char * );

          DUF_FPRINTFNE( 0, out, "#%4lu. %s│ %3d:%-23s ......... int(%d;%d) (%s)\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func, i1, i2, s );
        }
        break;
      case DBG_STEPULL:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 2 )
        {
          unsigned long long ull = 0;
          ull = va_arg( args, int );

          DUF_FPRINTFNE( 0, out, "#%4lu. %s│ %3d:%-23s ......... ull(%llu)\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func, ull );
        }
        break;
      case DBG_STEPS:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 2 )
        {
          const char *s;
          s = va_arg( args, const char * );

          if ( s )
            DUF_FPRINTFNE( 0, out, "#%4lu. %s│ %3d:%-23s ......... '%s'\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func, s );
          else
            DUF_FPRINTFNE( 0, out, "#%4lu. %s│ %3d:%-23s ......... Nil\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func );
        }
        break;
      case DBG_END:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 1 )
          DUF_FPRINTFNE( 0, out, "#%4lu. %s└─%3d:%-23s .........\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func );
        break;
      case DBG_ENDR:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 1 )
        {
          int rt = 0;
          rt = va_arg( args, int );

          DUF_FPRINTFNE( 0, out, "#%4lu. %s└─%3d:%-23s ......... rt=int(%d)\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func, rt );
        }
        break;
      case DBG_ENDRS:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 1 )
        {
          int rt = 0;
          const char *s;
          rt = va_arg( args, int );
          s = va_arg( args, const char * );

          DUF_FPRINTFNE( 0, out, "#%4lu. %s└─%3d:%-23s ......... rt=int(%d) (%s)\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func, rt, s );
        }
        break;
      case DBG_ENDULL:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 1 )
        {
          unsigned long long rt = 0;
          rt = va_arg( args, unsigned long long );

          DUF_FPRINTFNE( 0, out, "#%4lu. %s└─%3d:%-23s ......... llu(%llu)\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func, rt );
        }
        break;
      case DBG_ENDS:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 1 )
        {
          const char *s;
          s = va_arg( args, const char * );

          if ( s )
            DUF_FPRINTFNE( 0, out, "#%4lu. %s└─%3d:%-23s ......... '%s'\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func, s );
          else
            DUF_FPRINTFNE( 0, out, "#%4lu. %s└─%3d:%-23s ......... Nil\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func );
        }
        break;
      default:
        if ( !DUF_CONFIGX( cli.trace.calls ) || DUF_CONFIGX( cli.trace.calls ) > duf_dbgfunlevel + 1 )
          DUF_FPRINTFNE( 0, out, "#%4lu. %s::%3d:%-23s code=%d .........\n", DUF_CONFIGX( cli.dbg.lines ), pref, line, func, code );
        break;
      }
      switch ( code )
      {
      case DBG_START:
        /* if ( !DUF_CONFIGX(cli.trace.calls) || DUF_CONFIGX(cli.trace.calls) > duf_dbgfunlevel + 1 ) */
        /*   DUF_FPRINTFNE( 0,  out, "#%4lu. ", DUF_CONFIGX(cli.dbg.lines) );                                  */
        /* pref[duf_dbgfunlevel++] = ' '; */
        /* pref[duf_dbgfunlevel] = 0;     */
        break;
      case DBG_END:
      case DBG_ENDR:
      case DBG_ENDRS:
      case DBG_ENDULL:
      case DBG_ENDS:
        /* if ( !DUF_CONFIGX(cli.trace.calls) || DUF_CONFIGX(cli.trace.calls) > duf_dbgfunlevel + 1 ) */
        /*   DUF_FPRINTFNE( 0,  out, "#%4lu. ", DUF_CONFIGX(cli.dbg.lines) );                                  */
        pref[duf_dbgfunlevel--] = 0;
        break;
      default:
        break;
      }
    }
    DUF_CONFIGW( cli.dbg.lines )++;
  }
  return 0;
}

int
duf_dbgfunc( duf_dbgcode_t code, const char *func, int line, ... )
{
  int rt = 0;
  va_list args;

  {
    va_start( args, line );
    rt = duf_dbgfuncv( code, func, line, args );
    va_end( args );
  }
  return rt;
}
