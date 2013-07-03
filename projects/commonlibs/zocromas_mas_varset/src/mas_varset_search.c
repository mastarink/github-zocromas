#define MAS_USE_VARVEC

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

#ifdef MAS_USE_VARVEC
#  include <mastar/varvec/mas_varvec_object.h>
#  include <mastar/varvec/mas_varvec_search.h>
#  include <mastar/varvec/mas_varvec_namevalue.h>
#else
#  include "mas_varset_vclass_object.h"
/* #include "mas_varset_vclass.h" */
#  include "mas_varset_vclass_search.h"
#  include "mas_varset_vclass_namevalue.h"
/* #include "mas_varset_vclass_headtail.h" */
#endif

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

#ifdef MAS_USE_VARVEC
mas_vclass_element_t *
#else
mas_varvec_element_t *
#endif
mas_varset_find_variable( mas_varset_t * varset, const char *vclass_name, const char *name )
{
#ifdef MAS_USE_VARVEC
  mas_varvec_t *vclass;
  mas_varvec_element_t *found = NULL;
#else
  mas_varset_class_t *vclass;
  mas_vclass_element_t *found = NULL;
#endif
  vclass = mas_varset_find_vclass( varset, vclass_name );
  if ( vclass )
#ifdef MAS_USE_VARVEC
    found = _mas_varvec_find_variable( vclass, name );
#else
    found = _mas_varset_vclass_find_variable( vclass, name );
#endif
  return found;
}

mas_varset_t *
mas_varset_search_variable( mas_varset_t * varset, const char *vclass_name, const char *name, const char *value )
{
  if ( !varset )
    varset = mas_varset_create(  );
  if ( varset )
  {
#ifdef MAS_USE_VARVEC
    mas_varvec_t *vclass = NULL;
#else
    mas_varset_class_t *vclass = NULL;
#endif

    vclass = mas_varset_search_vclass( varset, vclass_name );
    if ( vclass )
    {
#ifdef MAS_USE_VARVEC
      mas_varvec_element_t *v;
#else
      mas_vclass_element_t *v;
#endif

#ifdef MAS_USE_VARVEC
      v = __mas_varvec_search_variable( vclass, name );
      if ( v )
        mas_varvec_variable_set_value( v, value );
#else
      v = __mas_varset_vclass_search_variable( vclass, name );
      if ( v )
        mas_varset_vclass_variable_set_value( v, value );
#endif
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

#ifdef MAS_USE_VARVEC
mas_varvec_t *
#else
mas_varset_class_t *
#endif
mas_varset_search_vclass( mas_varset_t * varset, const char *vclass_name )
{
#ifdef MAS_USE_VARVEC
  mas_varvec_t *vclass = NULL;
  mas_varvec_t *found = NULL;
#else
  mas_varset_class_t *vclass = NULL;
  mas_varset_class_t *found = NULL;
#endif

  if ( varset )
  {
#ifdef MAS_USE_VARVEC
    mas_varvec_t **pfound = NULL;
#else
    mas_varset_class_t **pfound = NULL;
#endif

#ifdef MAS_USE_VARVEC
    vclass = mas_varvec_create( vclass_name );
#else
    vclass = mas_varset_vclass_create( vclass_name );
#endif
    if ( vclass )
    {
#ifdef MAS_USE_VARVEC
      pfound = tsearch( vclass, &varset->classes, _mas_varvec_compare );
#else
      pfound = tsearch( vclass, &varset->classes, _mas_varset_compare_vclass );
#endif
      if ( pfound )
        found = *pfound;
      if ( found == vclass )
      {
        found->id = ++varset->nclasses;
      }
      else
      {
#ifdef MAS_USE_VARVEC
        mas_varvec_delete( vclass );
#else
        mas_varset_vclass_delete( vclass );
#endif
        vclass = NULL;
      }
    }
  }
  return found;
}

#ifdef MAS_USE_VARVEC
mas_varvec_t *
mas_varset_find_vclass( mas_varset_t * varset, const char *vclass_name )
#else
mas_varset_class_t *
mas_varset_find_vclass( mas_varset_t * varset, const char *vclass_name )
#endif
{
#ifdef MAS_USE_VARVEC
  mas_varvec_t *vclass = NULL;
  mas_varvec_t *found = NULL;
#else
  mas_varset_class_t *vclass = NULL;
  mas_varset_class_t *found = NULL;
#endif

  if ( varset )
  {
#ifdef MAS_USE_VARVEC
    mas_varvec_t **pfound = NULL;
#else
    mas_varset_class_t **pfound = NULL;
#endif

#ifdef MAS_USE_VARVEC
    vclass = mas_varvec_create( vclass_name );
#else
    vclass = mas_varset_vclass_create( vclass_name );
#endif
    if ( vclass )
    {
#ifdef MAS_USE_VARVEC
      pfound = tfind( vclass, &varset->classes, _mas_varvec_compare );
#else
      pfound = tfind( vclass, &varset->classes, _mas_varset_compare_vclass );
#endif
      if ( pfound )
        found = *pfound;
#ifdef MAS_USE_VARVEC
      mas_varvec_delete( vclass );
#else
      mas_varset_vclass_delete( vclass );
#endif
    }
  }
  return found;
}
