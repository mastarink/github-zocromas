#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>

#include "mas_varvec_element.h"


#include "mas_varvec_namevalue.h"



static const char *tail_str = "\r\n";
static size_t tail_len = 2;

#define MAS_TAIL_STR tail_str
#define MAS_TAIL_LEN tail_len

const char *
mas_varvec_name( mas_varvec_t * vclass )
{
  return vclass ? vclass->name : NULL;
}
const char *
mas_varvec_variable_get_name_ref( mas_varvec_element_t * var )
{
  char *v = NULL;

  if ( var && var[0].iov_len )
    v = var[0].iov_base;
  return v;
}

char *
mas_varvec_variable_get_name( mas_varvec_element_t * var )
{
  char *v = NULL;

  if ( var && var[0].iov_len )
    v = mas_strndup( var[0].iov_base, var[0].iov_len - 2 );
  return v;
}

const char *
mas_varvec_variable_get_value_ref( mas_varvec_element_t * var )
{
  char *v = NULL;

  if ( var && var[1].iov_len )
    v = var[1].iov_base;
  return v;
}

char *
mas_varvec_variable_get_value( mas_varvec_element_t * var )
{
  char *v = NULL;

  if ( var && var[1].iov_len )
    v = mas_strndup( var[1].iov_base, var[1].iov_len - 2 );
  return v;
}

void
mas_varvec_variable_set_name( mas_varvec_element_t * var, const char *name )
{
  mas_varvec_variable_set_element( var, 0, ": ", 2, name );
}

void
mas_varvec_variable_set_value( mas_varvec_element_t * var, const char *value )
{
  mas_varvec_variable_set_element( var, 1, MAS_TAIL_STR, MAS_TAIL_LEN, value );
}

void
mas_varvec_variable_set_value_va( mas_varvec_element_t * var, mas_xvsnprintf_t func, const char *fmt, va_list args )
{
  char *text = NULL;
  size_t txsize = 1024 * 10;

  text = mas_malloc( txsize );
  if ( text )
  {
    if ( !func )
      func = mas_xvsnprintf;
    ( *func ) ( text, txsize, fmt, args );
    mas_varvec_variable_set_value( var, text );
    mas_free( text );
  }
}

void
mas_varvec_variable_set_valuef( mas_varvec_element_t * var, mas_xvsnprintf_t func, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  mas_varvec_variable_set_value_va( var, func, fmt, args );
  va_end( args );
}
