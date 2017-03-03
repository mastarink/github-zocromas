#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <libgen.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "minierr.h"

void
minierr_vdie( int line, const char *func, const char *file, int fexit, const char *sid, const char *fmt, va_list args )
{
  char *pf = strrchr( file, '/' );
  char buffer[1024];

  if ( pf )
    pf++;

  snprintf( buffer, sizeof( buffer ), "-=%s=- %d:%s( ) @ %s -- ", sid ? sid : ( fexit ? "DIE" : "WARN" ), line, func, pf );
  fprintf( stderr, "%-50s", buffer );
  vfprintf( stderr, fmt, args );
  fprintf( stderr, "\n" );
  if ( fexit )
  {
    sleep( 5 );
    exit( fexit );
  }
}

void
minierr_die( int line, const char *func, const char *file, int fexit, const char *sid, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  minierr_vdie( line, func, file, fexit, sid, fmt, args );
  va_end( args );
}
