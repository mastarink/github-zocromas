#include <stdio.h>
#include <stdarg.h>
#include <libgen.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>

#include "mulconfnt_defs.h"

#include "mulconfnt_structs.h"

#include "mulconfnt_error_base.h"
#include "mulconfnt_error.h"

int
mulconfnt_error_vset_source( config_source_desc_t * osrc, int line, const char *func, const char *file, const char *fmt, va_list args )
{
  int r = 0;

  if ( osrc )
    r = mulconfnt_error_vset( &osrc->error, line, func, file, fmt, args );
  return r;
}

int
mulconfnt_error_set_at_source( config_source_desc_t * osrc, int line, const char *func, const char *file, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mulconfnt_error_vset_source( osrc, line, func, file, fmt, args );
  va_end( args );
  return r;
}

int
mulconfnt_error_vset_option( config_option_t * opt, int line, const char *func, const char *file, const char *fmt, va_list args )
{
  int r = 0;

  if ( opt )
  {
    r = mulconfnt_error_vset( &opt->error, line, func, file, fmt, args );
    if ( opt->source )
      mulconfnt_error_set_at_source_from_option( opt->source, opt );
  }
  return r;
}

int
mulconfnt_error_set_at_option( config_option_t * opt, int line, const char *func, const char *file, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mulconfnt_error_vset_option( opt, line, func, file, fmt, args );
  va_end( args );
  return r;
}

int
mulconfnt_error_set_at_source_from_option( config_source_desc_t * osrc, config_option_t * opt )
{
  int r = 0;

  if ( osrc && opt )
    r = mulconfnt_error_set_from_error( &osrc->error, &opt->error );
  return r;
}

int
mulconfnt_error_source( const config_source_desc_t * osrc )
{
  int nerror = 0;

  if ( osrc )
    nerror = osrc->error.nerror;

  return nerror;
}

const char *
mulconfnt_error_source_msg( const config_source_desc_t * osrc )
{
  return osrc->error.msg;
}

int
mulconfnt_error_option( const config_option_t * opt )
{
  int nerror = 0;

  if ( opt )
  {
    nerror = mulconfnt_error_source( opt->source );
    if ( !nerror )
      nerror = opt->error.nerror;
  }
  return nerror;
}

const char *
mulconfnt_error_option_msg( const config_option_t * opt )
{
  return opt->error.msg;
}
