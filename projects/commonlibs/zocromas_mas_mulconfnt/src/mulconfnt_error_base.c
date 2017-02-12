#include <stdio.h>
#include <stdarg.h>
#include <libgen.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>

#include "mulconfnt_defs.h"

#include "mulconfnt_structs.h"

#include "mulconfnt_error_base.h"

void
mulconfnt_error_close( config_error_t * error )
{
  mas_free( error->msg );
  memset( error, 0, sizeof( config_error_t ) );
}

void
mulconfnt_error_delete( config_error_t * error )
{
  mulconfnt_error_close( error );
  mas_free( error );
}

int
mulconfnt_error_vset( config_error_t * error, int line, const char *func, const char *file, const char *fmt _uUu_, va_list args _uUu_ )
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

      fprintf( stderr, "SETTING ERROR := %d:%s @ %s - %s\n", line, func, basename( filename ), msg );
      mas_free( filename );
    }
    mas_free( error->msg );
    error->msg = mas_strdup( msg );
    r++;
  }
  return r;
}

int
mulconfnt_error_set( config_error_t * error, int line, const char *func, const char *file _uUu_, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mulconfnt_error_vset( error, line, func, file, fmt, args );
  va_end( args );
  return r;
}

int
mulconfnt_error_set_from_error( config_error_t * error, config_error_t * srcerror )
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
