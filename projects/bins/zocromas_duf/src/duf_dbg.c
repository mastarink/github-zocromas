#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"
#include "duf_config.h"

/* ###################################################################### */
#include "duf_dbg.h"
/* ###################################################################### */

int duf_dbgfunlevel = 0;
int dbgfunc_enabled = 0;
int
duf_dbgfuncv( duf_dbgcode_t code, const char *func, int line, va_list args )
{
  int minverbose = 4;

  if ( dbgfunc_enabled && duf_config && ( duf_config->cli.dbg.verbose >= minverbose ) )
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
    switch ( code )
    {
    case DBG_START:
      pref[duf_dbgfunlevel++] = ' ';
      pref[duf_dbgfunlevel] = 0;
      break;
    case DBG_END:
    case DBG_ENDR:
    case DBG_ENDRS:
    case DBG_ENDULL:
    case DBG_ENDS:
      pref[duf_dbgfunlevel--] = 0;
      break;
    default:
      break;
    }
    if ( ( !duf_config->cli.dbg.max_line || duf_config->cli.dbg.lines < duf_config->cli.dbg.max_line )
         && ( duf_config->cli.dbg.lines >= duf_config->cli.dbg.min_line ) )
    {

      /* fprintf( stderr, "%3d:%-28s .........\n", duf_config->cli.dbg.lines, duf_dbgfunlevel, line, func ); */
      /* fprintf( stderr, "%4lu of %4lu <<<<<<<<<<\n", duf_config->cli.dbg.lines, duf_config->cli.dbg.max_line ); */
      fprintf( stderr, "%4lu. ", duf_config->cli.dbg.lines );
      /* fprintf( stderr, "L%2u", duf_dbgfunlevel ); */
      switch ( code )
      {
      case DBG_START:
        /* pref[duf_dbgfunlevel++] = ' '; */
        /* pref[duf_dbgfunlevel] = 0; */
        fprintf( stderr, "%s┌─%3d:%-28s .........\n", pref, line, func );
        break;
      case DBG_STEP:
        fprintf( stderr, "%s│ %3d:%-28s .........\n", pref, line, func );
        break;
      case DBG_STEPIS:
        {
          int i = 0;
          const char *s;
          i = va_arg( args, int );
          s = va_arg( args, const char * );

          fprintf( stderr, "%s│ %3d:%-28s ......... int(%d) (%s)\n", pref, line, func, i, s );
        }
        break;
      case DBG_STEPI2S:
        {
          int i1 = 0;
          int i2 = 0;
          const char *s;
          i1 = va_arg( args, int );
          i2 = va_arg( args, int );
          s = va_arg( args, const char * );

          fprintf( stderr, "%s│ %3d:%-28s ......... int(%d;%d) (%s)\n", pref, line, func, i1, i2, s );
        }
        break;
      case DBG_STEPULL:
        {
          unsigned long long ull = 0;
          ull = va_arg( args, int );

          fprintf( stderr, "%s│ %3d:%-28s ......... ull(%llu)\n", pref, line, func, ull );
        }
        break;
      case DBG_STEPS:
        {
          const char *s;
          s = va_arg( args, const char * );

          if ( s )
            fprintf( stderr, "%s│ %3d:%-28s ......... '%s'\n", pref, line, func, s );
          else
            fprintf( stderr, "%s│ %3d:%-28s ......... Nil\n", pref, line, func );
        }
        break;
      case DBG_END:
        /* pref[duf_dbgfunlevel--] = 0; */
        fprintf( stderr, "%s└─%3d:%-28s .........\n", pref, line, func );
        break;
      case DBG_ENDR:
        /* pref[duf_dbgfunlevel--] = 0; */
        {
          int r = 0;
          r = va_arg( args, int );

          fprintf( stderr, "%s└─%3d:%-28s ......... r=int(%d)\n", pref, line, func, r );
        }
        break;
      case DBG_ENDRS:
        /* pref[duf_dbgfunlevel--] = 0; */
        {
          int r = 0;
          const char *s;
          r = va_arg( args, int );
          s = va_arg( args, const char * );

          fprintf( stderr, "%s└─%3d:%-28s ......... r=int(%d) (%s)\n", pref, line, func, r, s );
        }
        break;
      case DBG_ENDULL:
        /* pref[duf_dbgfunlevel--] = 0; */
        {
          unsigned long long r = 0;
          r = va_arg( args, unsigned long long );

          fprintf( stderr, "%s└─%3d:%-28s ......... llu(%llu)\n", pref, line, func, r );
        }
        break;
      case DBG_ENDS:
        /* pref[duf_dbgfunlevel--] = 0; */
        {
          const char *s;
          s = va_arg( args, const char * );

          if ( s )
            fprintf( stderr, "%s└─%3d:%-28s ......... '%s'\n", pref, line, func, s );
          else
            fprintf( stderr, "%s└─%3d:%-28s ......... Nil\n", pref, line, func );
        }
        break;
      default:
        fprintf( stderr, "%s::%3d:%-28s r=%d .........\n", pref, line, func, code );
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

  va_start( args, line );
  r = duf_dbgfuncv( code, func, line, args );
  va_end( args );
  return r;
}
