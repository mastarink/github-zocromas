#include <stdlib.h>
#include <string.h>

#include <search.h>

#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>

#include "mas_varvec_object.h"
#include "mas_varvec_element.h"
#include "mas_varvec_namevalue.h"

#include "mas_varvec_search.h"

#if 1
static const char *final_str = "\r\n";
static size_t final_len = 2;

#  define MAS_FINAL_STR (char*)final_str
#  define MAS_FINAL_LEN final_len
#else
#  define MAS_FINAL_STR NULL
#  define MAS_FINAL_LEN 0
#endif


int
_mas_varvec_compare( const void *a, const void *b )
{
  mas_varvec_t *aclass, *bclass;

  aclass = ( mas_varvec_t * ) a;
  bclass = ( mas_varvec_t * ) b;
  if ( a == b )
    return 0;
  if ( !aclass->name )
    return bclass->name ? -1 : 0;
  if ( !bclass->name )
    return aclass->name ? 1 : 0;
  return strcmp( aclass->name, bclass->name );
}

mas_varvec_element_t *
mas_varvec_find_variable( mas_varvec_t * vclass, const char *name )
{
  mas_varvec_element_t *found = NULL;

  if ( vclass && vclass->vec )
  {
    for ( int j = vclass->has_head; j < vclass->el_cnt - vclass->has_head - vclass->has_tail; j += 2 )
    {
      int len;

      len = strlen( name );
      if ( vclass->vec[j].iov_len != len + 2 )
        continue;
      if ( 0 == memcmp( vclass->vec[j].iov_base, name, len ) )
      {
        found = &vclass->vec[j];
        break;
      }
    }
  }
  return found;
}

mas_varvec_element_t *
__mas_varvec_search_variable( mas_varvec_t * vclass, const char *name )
{
  mas_varvec_element_t *found = NULL;

  if ( vclass )
  {
    found = mas_varvec_find_variable( vclass, name );
    if ( !found )
    {
      found = _mas_varvec_add_elements( vclass, 2 );
      mas_varvec_variable_set_name( found, name );
      if ( 0 )
      {
        found[2].iov_base = MAS_FINAL_STR;
        found[2].iov_len = MAS_FINAL_LEN;
      }
    }
  }
  return found;
}

void
_mas_varvec_search_variable( mas_varvec_t * vclass, const char *name, const char *value )
{
  if ( vclass )
  {
    mas_varvec_element_t *var = NULL;

    if ( name )
    {
      var = __mas_varvec_search_variable( vclass, name );
      if ( var )
        mas_varvec_variable_set_value( var, value );
    }
    vclass->use_var = var;
  }
}

mas_varvec_t *
mas_varvec_search_variable( mas_varvec_t * vclass, const char *vclass_name, const char *name, const char *value )
{
  if ( !vclass )
    vclass = mas_varvec_create( vclass_name );
  _mas_varvec_search_variable( vclass, name, value );
  return vclass;
}

mas_varvec_t *
mas_varvec_search_variable_va( mas_varvec_t * vclass, const char *vclass_name, const char *name,
                               mas_xvsnprintf_t func, const char *fmt, va_list args )
{
  char *text = NULL;
  size_t txsize = 1024 * 10;

  text = mas_malloc( txsize );
  if ( text )
  {
    if ( !func )
      func = mas_xvsnprintf;
    ( *func ) ( text, txsize, fmt, args );
    vclass = mas_varvec_search_variable( vclass, vclass_name, name, text );
    mas_free( text );
  }
  return vclass;
}

mas_varvec_t *
mas_varvec_search_variablef( mas_varvec_t * vclass, const char *vclass_name, const char *name, mas_xvsnprintf_t func, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  vclass = mas_varvec_search_variable_va( vclass, vclass_name, name, func, fmt, args );
  va_end( args );
  return vclass;
}
