#include <stdio.h>
#include <stdarg.h>
#include <libgen.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mulconfnt_defs.h"

#include "mulconfnt_structs.h"

#include "option_tablist_base.h"

#include "option.h"
#include "source.h"

#include "mulconfnt_error_base.h"
#include "mulconfnt_error.h"

void
mucs_error_vdie( int line, const char *func, const char *file, int fexit, const char *fmt, va_list args )
{
  char *pf = strrchr( file, '/' );

  if ( pf )
    pf++;
  fprintf( stderr, "\n@@-=DIE=-@@ at %d:%s @ %s -- ", line, func, pf );
  vfprintf( stderr, fmt, args );
  fprintf( stderr, "\n" );
  if (fexit)exit( 34 );
}

//_attribute__ ( ( format( __printf__, 13, 14 ) ) )
void
mucs_error_die( int line, const char *func, const char *file, int fexit, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  mucs_error_vdie( line, func, file, fexit, fmt, args );
  va_end( args );
}

int
mucs_error_vset_at_source( mucs_source_han_t * osrc, int line, const char *func, const char *file, const char *fmt, va_list args )
{
  int r = 0;

  if ( osrc )
    r = mucs_error_vset( &osrc->error, line, func, file, mucs_source_flags( osrc ), fmt, args );
  return r;
}

int
mucs_error_set_at_source( mucs_source_han_t * osrc, int line, const char *func, const char *file, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mucs_error_vset_at_source( osrc, line, func, file, fmt, args );
  va_end( args );
  return r;
}

int
mucs_error_vset_at_option( mucs_option_han_t * opt, int line, const char *func, const char *file, unsigned long flags, const char *fmt,
                                va_list args )
{
  int r = 0;

  if ( opt )
  {
    r = mucs_error_vset( &opt->error, line, func, file, flags, fmt, args );
    if ( opt->source )
      mucs_error_set_at_source_from_option( opt->source, opt );
  }
  return r;
}

int
mucs_error_set_at_option( mucs_option_han_t * opt, int line, const char *func, const char *file, unsigned long flags, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mucs_error_vset_at_option( opt, line, func, file, flags, fmt, args );
  va_end( args );
  return r;
}

int
mucs_error_set_at_source_from_option( mucs_source_han_t * osrc, mucs_option_han_t * opt )
{
  int r = 0;

  if ( osrc && opt )
    r = mucs_error_set_from_error( &osrc->error, &opt->error );
  return r;
}

int
mucs_error_source( const mucs_source_han_t * osrc )
{
  int nerror = 0;

  if ( osrc )
    nerror = osrc->error.nerror;

  return nerror;
}

const char *
mucs_error_source_msg( const mucs_source_han_t * osrc )
{
  return osrc->error.msg;
}

int
mucs_error_option( const mucs_option_han_t * opt )
{
  int nerror = 0;

  if ( opt )
  {
    nerror = mucs_error_source( opt->source );
    if ( !nerror )
      nerror = opt->error.nerror;
  }
  return nerror;
}

const char *
mucs_error_option_msg( const mucs_option_han_t * opt )
{
  return opt->error.msg;
}
