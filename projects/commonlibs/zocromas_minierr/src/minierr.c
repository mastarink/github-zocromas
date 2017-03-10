#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <libgen.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "minierr.h"

void
minierr_vdie( int line, const char *func, const char *file, int fexit, unsigned flags, const char *sid, const char *fmt, va_list args )
{
  char *pf = strrchr( file, '/' );
  char buffer[1024] = "";
  size_t prefwidth = 10;

  if ( pf )
    pf++;
#define MAS_MIER_PRN(...) { size_t l=strlen( buffer ); snprintf( buffer + l, sizeof( buffer ) - l, __VA_ARGS__); }

  /* MAS_MIER_PRN( "[%ld] ", prefwidth ); */
  MAS_MIER_PRN( "-=%s=-", sid ? sid : ( fexit ? "DIE" : "WARN" ) );
  if ( flags & MAS_MIER_FLAG_LINE )
  {
    prefwidth += 5;
    MAS_MIER_PRN( " %d:", line );
  }
  if ( flags & MAS_MIER_FLAG_FUNC )
  {
    prefwidth += 35;
    MAS_MIER_PRN( "%s( )", func );
  }
  if ( flags & MAS_MIER_FLAG_FILE )
  {
    prefwidth += 25;
    MAS_MIER_PRN( " @ %s -- ", pf );
  }
  {
    size_t len = strlen( buffer );
    char *p = buffer + len;

    /* *p++ = '{'; */
    /* justify: */
    while ( p && len < prefwidth && len < sizeof( buffer ) - 4 )
    {
      *p++ = ' ';
      len++;
    }
    /* *p++ = '}'; */
    *p = 0;
  }
  fputs(buffer, stderr);
  /* fprintf( stderr, "%-50s", buffer ); */
  vfprintf( stderr, fmt, args );
  fprintf( stderr, "\n" );

  if ( flags & MAS_MIER_FLAG_SLEEP )
    sleep( 5 );
  if ( flags & MAS_MIER_FLAG_EXIT )
    exit( fexit );
}

void
minierr_die( int line, const char *func, const char *file, int fexit, unsigned flags, const char *sid, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  minierr_vdie( line, func, file, fexit, flags, sid, fmt, args );
  va_end( args );
}
