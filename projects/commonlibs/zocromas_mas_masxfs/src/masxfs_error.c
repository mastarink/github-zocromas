#include <stdio.h>
#include <stdarg.h>
#include <libgen.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_error.h"

void
masxfs_error_vdie( int line, const char *func, const char *file, const char *fmt, va_list args )
{
  char *pf = strrchr( file, '/' );

  if ( pf )
    pf++;
  fprintf( stderr, "\n@@-=DIE=-@@ at %d:%s @ %s -- ", line, func, pf );
  vfprintf( stderr, fmt, args );
  fprintf( stderr, "\n" );
  exit( 34 );
}

void
masxfs_error_die( int line, const char *func, const char *file, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  masxfs_error_vdie( line, func, file, fmt, args );
  va_end( args );
}
