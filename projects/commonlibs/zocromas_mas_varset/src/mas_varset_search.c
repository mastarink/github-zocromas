#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_varset_types.h>

#include <mastar/varvec/mas_varvec_object.h>
#include <mastar/varvec/mas_varvec_search.h>
#include <mastar/varvec/mas_varvec_namevalue.h>

#include "mas_varset_object.h"

#include "mas_varset_search.h"

void
mas_varset_walk_classes( mas_varset_t * varset, void ( *action ) ( const void *nodep, const VISIT which, const int depth ) )
{
  if ( varset )
  {
    fprintf( stderr, "Wiiiiiiiiiiiiiiiiz %d\n", varset->classes ? 1 : 0 );
    twalk( varset->classes, action );
  }
}

mas_varvec_element_t *
mas_varset_find_variable( mas_varset_t * varset, const char *vclass_name, const char *name )
{
  mas_varvec_t *vclass;
  mas_varvec_element_t *found = NULL;

  vclass = mas_varset_find_vclass( varset, vclass_name );
  if ( vclass )
    found = mas_varvec_find_variable( vclass, name );
  return found;
}

mas_varset_t *
mas_varset_search_variable( mas_varset_t * varset, const char *vclass_name, const char *name, const char *value )
{
  if ( !varset )
    varset = mas_varset_create(  );
  if ( varset )
  {
    mas_varvec_t *vclass = NULL;

    vclass = mas_varset_search_vclass( varset, vclass_name );
    if ( vclass )
    {
      mas_varvec_element_t *v;

      v = __mas_varvec_search_variable( vclass, name );
      if ( v )
        mas_varvec_variable_set_value( v, value );
    }
  }
  return varset;
}

mas_varset_t *
mas_varset_search_variablef( mas_varset_t * varset, const char *vclass_name, const char *name, mas_xvsnprintf_t func, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  varset = mas_varset_search_variable_va( varset, vclass_name, name, func, fmt, args );
  va_end( args );
  return varset;
}

mas_varset_t *
mas_varset_search_variable_va( mas_varset_t * varset, const char *vclass_name, const char *name,
                               mas_xvsnprintf_t func, const char *fmt, va_list args )
{
  char *text = NULL;
  size_t txsize = 1024 * 10;

  text = mas_malloc( txsize );
  if ( text )
  {
    if ( !func )
      func = mas_xvsnprintf;
    if ( func )
      ( *func ) ( text, txsize, fmt, args );
    if ( name && text )
      varset = mas_varset_search_variable( varset, vclass_name, name, text );
    mas_free( text );
  }
  return varset;
}

mas_varvec_t *
mas_varset_search_vclass( mas_varset_t * varset, const char *vclass_name )
{
  mas_varvec_t *vclass = NULL;
  mas_varvec_t *found = NULL;

  if ( varset )
  {
    mas_varvec_t **pfound = NULL;

    vclass = mas_varvec_create( vclass_name );
    if ( vclass )
    {
      pfound = tsearch( vclass, &varset->classes, _mas_varvec_compare );
      if ( pfound )
        found = *pfound;
      if ( found == vclass )
      {
        found->id = ++varset->nclasses;
      }
      else
      {
        mas_varvec_delete( vclass );
        vclass = NULL;
      }
    }
  }
  return found;
}

mas_varvec_t *
mas_varset_find_vclass( mas_varset_t * varset, const char *vclass_name )
{
  mas_varvec_t *vclass = NULL;
  mas_varvec_t *found = NULL;

  if ( varset )
  {
    mas_varvec_t **pfound = NULL;

    vclass = mas_varvec_create( vclass_name );
    if ( vclass )
    {
      pfound = tfind( vclass, &varset->classes, _mas_varvec_compare );
      if ( pfound )
        found = *pfound;
      mas_varvec_delete( vclass );
    }
  }
  return found;
}
