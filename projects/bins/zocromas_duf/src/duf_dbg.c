#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_config_ref.h"


/* ###################################################################### */
#include "duf_dbg.h"
/* ###################################################################### */

int duf_dbgfunlevel = 0;
int dbgfunc_enabled = 0;
int
duf_dbgfuncv( duf_dbgcode_t code, const char *func, int line, va_list args )
{
  int minverbose = 4;

  if ( dbgfunc_enabled && duf_config && ( ( duf_config->cli.dbg.verbose >= minverbose ) || duf_config->cli.trace.calls ) )
  {
    static int inited = 0;
    static char pref[1024];

    if ( !inited )
    {
      memset( pref, 0, sizeof( pref ) );
      inited = 1;
    }
    if ( duf_dbgfunlevel > sizeof( pref ) - 1 )
      duf_dbgfunlevel = sizeof( pref ) - 1;

    pref[duf_dbgfunlevel] = 0;
    if ( ( !duf_config->cli.dbg.max_line || duf_config->cli.dbg.lines < duf_config->cli.dbg.max_line )
         && ( duf_config->cli.dbg.lines >= duf_config->cli.dbg.min_line ) )
    {
      FILE *out = duf_config->cli.trace.out ? duf_config->cli.trace.out : stderr;

      switch ( code )
      {
      case DBG_START:
        /* if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 ) */
        /*   fprintf( out, "#%4lu. ", duf_config->cli.dbg.lines );                                  */
        pref[duf_dbgfunlevel++] = ' ';
        pref[duf_dbgfunlevel] = 0;
        break;
      case DBG_END:
      case DBG_ENDR:
      case DBG_ENDRS:
      case DBG_ENDULL:
      case DBG_ENDS:
        /* if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 ) */
        /*   fprintf( out, "#%4lu. ", duf_config->cli.dbg.lines );                                  */
        /* pref[duf_dbgfunlevel--] = 0; */
        break;
      default:
        break;
      }

      /* fprintf( out, "%3d:%-23s .........\n", duf_config->cli.dbg.lines, duf_dbgfunlevel, line, func ); */
      /* fprintf( out, "%4lu of %4lu <<<<<<<<<<\n", duf_config->cli.dbg.lines, duf_config->cli.dbg.max_line ); */


      /* fprintf( out, "L%2u", duf_dbgfunlevel ); */
      switch ( code )
      {
      case DBG_START:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 )
          fprintf( out, "#%4lu. %s┌─%3d:%-23s .........\n", duf_config->cli.dbg.lines , pref, line, func );
        break;
      case DBG_STEP:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 2 )
          fprintf( out, "#%4lu. %s│ %3d:%-23s .........\n", duf_config->cli.dbg.lines , pref, line, func );
        break;
      case DBG_STEPIS:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 2 )
        {
          int i = 0;
          const char *s;
          i = va_arg( args, int );
          s = va_arg( args, const char * );

          fprintf( out, "#%4lu. %s│ %3d:%-23s ......... int(%d) (%s)\n", duf_config->cli.dbg.lines , pref, line, func, i, s );
        }
        break;
      case DBG_STEPI2S:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 2 )
        {
          int i1 = 0;
          int i2 = 0;
          const char *s;
          i1 = va_arg( args, int );
          i2 = va_arg( args, int );
          s = va_arg( args, const char * );

          fprintf( out, "#%4lu. %s│ %3d:%-23s ......... int(%d;%d) (%s)\n", duf_config->cli.dbg.lines , pref, line, func, i1, i2, s );
        }
        break;
      case DBG_STEPULL:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 2 )
        {
          unsigned long long ull = 0;
          ull = va_arg( args, int );

          fprintf( out, "#%4lu. %s│ %3d:%-23s ......... ull(%llu)\n", duf_config->cli.dbg.lines , pref, line, func, ull );
        }
        break;
      case DBG_STEPS:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 2 )
        {
          const char *s;
          s = va_arg( args, const char * );

          if ( s )
            fprintf( out, "#%4lu. %s│ %3d:%-23s ......... '%s'\n", duf_config->cli.dbg.lines , pref, line, func, s );
          else
            fprintf( out, "#%4lu. %s│ %3d:%-23s ......... Nil\n", duf_config->cli.dbg.lines , pref, line, func );
        }
        break;
      case DBG_END:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 )
          fprintf( out, "#%4lu. %s└─%3d:%-23s .........\n", duf_config->cli.dbg.lines , pref, line, func );
        break;
      case DBG_ENDR:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 )
        {
          int r = 0;
          r = va_arg( args, int );

          fprintf( out, "#%4lu. %s└─%3d:%-23s ......... r=int(%d)\n", duf_config->cli.dbg.lines , pref, line, func, r );
        }
        break;
      case DBG_ENDRS:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 )
        {
          int r = 0;
          const char *s;
          r = va_arg( args, int );
          s = va_arg( args, const char * );

          fprintf( out, "#%4lu. %s└─%3d:%-23s ......... r=int(%d) (%s)\n", duf_config->cli.dbg.lines , pref, line, func, r, s );
        }
        break;
      case DBG_ENDULL:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 )
        {
          unsigned long long r = 0;
          r = va_arg( args, unsigned long long );

          fprintf( out, "#%4lu. %s└─%3d:%-23s ......... llu(%llu)\n", duf_config->cli.dbg.lines , pref, line, func, r );
        }
        break;
      case DBG_ENDS:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 )
        {
          const char *s;
          s = va_arg( args, const char * );

          if ( s )
            fprintf( out, "#%4lu. %s└─%3d:%-23s ......... '%s'\n", duf_config->cli.dbg.lines , pref, line, func, s );
          else
            fprintf( out, "#%4lu. %s└─%3d:%-23s ......... Nil\n", duf_config->cli.dbg.lines , pref, line, func );
        }
        break;
      default:
        if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 )
          fprintf( out, "#%4lu. %s::%3d:%-23s r=%d .........\n", duf_config->cli.dbg.lines , pref, line, func, code );
        break;
      }
            switch ( code )
      {
      case DBG_START:
        /* if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 ) */
        /*   fprintf( out, "#%4lu. ", duf_config->cli.dbg.lines );                                  */
        /* pref[duf_dbgfunlevel++] = ' '; */
        /* pref[duf_dbgfunlevel] = 0;     */
        break;
      case DBG_END:
      case DBG_ENDR:
      case DBG_ENDRS:
      case DBG_ENDULL:
      case DBG_ENDS:
        /* if ( !duf_config->cli.trace.calls || duf_config->cli.trace.calls > duf_dbgfunlevel + 1 ) */
        /*   fprintf( out, "#%4lu. ", duf_config->cli.dbg.lines );                                  */
        pref[duf_dbgfunlevel--] = 0;
        break;
      default:
        break;
      }
    }
    duf_config->cli.dbg.lines++;
  }
  return 0;
}

int
duf_dbgfunc( duf_dbgcode_t code, const char *func, int line, ... )
{
  int r = 0;
  va_list args;

  {
    va_start( args, line );
    r = duf_dbgfuncv( code, func, line, args );
    va_end( args );
  }
  return r;
}
