#include <stdio.h>
#include <stdarg.h>
#include <libgen.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mulconfnt_defs.h"

#include "mulconfnt_structs.h"

#include "mulconfnt_error_base.h"

void
mucs_error_reset( mucs_error_t * error )
{
  mas_free( error->msg );
  memset( error, 0, sizeof( mucs_error_t ) );
}

void
mucs_error_delete( mucs_error_t * error )
{
  mucs_error_reset( error );
  mas_free( error );
}

int
mucs_error_vset( mucs_error_t * error, int line, const char *func, const char *file, unsigned long flags, const char *fmt, va_list args )
{
  int r = 0;

  error->nerror++;
  error->line = line;
  error->func = func;
  error->file = file;
  {
    char msg[1024 * 5];

    vsnprintf( msg, sizeof( msg ), fmt, args );
    {
      char *filename = mas_strdup( file );

      if ( !( flags & MUCS_FLAG_SILENT ) )
        fprintf( stderr, "%lx SETTING ERROR := %d:%s @ %s - %s\n", flags, line, func, basename( filename ), msg );
      mas_free( filename );
    }
    if ( error->msg )
      mas_free( error->msg );
    error->msg = mas_strdup( msg );
    r++;
  }
  return r;
}

int
mucs_error_set( mucs_error_t * error, int line, const char *func, const char *file, unsigned long flags, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mucs_error_vset( error, line, func, file, flags, fmt, args );
  va_end( args );
  return r;
}

int
mucs_error_set_from_error( mucs_error_t * error, mucs_error_t * srcerror )
{
  if ( error && srcerror )
  {
    error->nerror += srcerror->nerror;
    if ( !error->line && !error->func && !error->file )
    {
      error->line = srcerror->line;
      error->func = srcerror->func;
      error->file = srcerror->file;
      error->msg = mas_strdup( srcerror->msg );
    }
  }
  return error ? error->nerror : 0;
}
