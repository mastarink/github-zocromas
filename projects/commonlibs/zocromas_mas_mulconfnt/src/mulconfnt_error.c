#include <stdio.h>

#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"

static int _mulconfnt_error = 0;

int
mulconfnt_set_error_list( config_source_list_t * list, int line, const char *func )
{
  int r = 0;

  if ( list )
  {
    list->error++;
    fprintf( stderr, "SETTING ERROR (list): %d:%s\n", line, func );
    r++;
  }
  _mulconfnt_error++;
  return r;
}

int
mulconfnt_set_error_source( config_source_desc_t * osrc, int line, const char *func )
{
  int r = 0;

  if ( osrc )
  {
    osrc->error++;
    r = mulconfnt_set_error_list( osrc->list, line, func );
    if ( !r )
    {
      fprintf( stderr, "SETTING ERROR (source): %d:%s\n", line, func );
      r++;
    }
  }
  _mulconfnt_error++;
  return r;
}

int
mulconfnt_set_error_option( config_option_t * opt, int line, const char *func )
{
  int r = 0;

  if ( opt )
  {
    opt->error++;
    if ( opt->source )
      r = mulconfnt_set_error_source( opt->source, line, func );
    if ( !r )
    {
      fprintf( stderr, "SETTING ERROR (option): %d:%s\n", line, func );
      r++;
    }
  }
  _mulconfnt_error++;
  return r;
}

int
mulconfnt_error_list( config_source_list_t * list )
{
  int error = 0;

  if ( list )
    error = list->error;
  else
    error = _mulconfnt_error;
  return error;
}

int
mulconfnt_error_source( config_source_desc_t * osrc )
{
  int error = 0;

  if ( osrc )
  {
    error = mulconfnt_error_list( osrc->list );
    if ( !error )
      error = osrc->error;
  }
  else
    error = _mulconfnt_error;

  return error;
}
