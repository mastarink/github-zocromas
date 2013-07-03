#define MAS_USE_VARVEC
#ifndef MAS_USE_VARVEC

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/* #include <search.h> */

#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
/* #include <mastar/wrap/mas_lib0.h> */
/* #include <mastar/wrap/mas_lib.h> */
#include <mastar/tools/mas_tools.h>
/* #include <mastar/tools/mas_arg_tools.h> */

#include <mastar/types/mas_varset_types.h>
#include "mas_varset_vclass_object.h"
#include "mas_varset_vclass_element.h"


/* #include "mas_varset_vclass.h" */

void
_mas_varset_vclass_set_head( mas_varset_class_t * vclass, const char *head )
{
  if ( vclass && !vclass->el_cnt )
  {
    mas_vclass_element_t *el = NULL;

    el = _mas_varset_vclass_add_elements( vclass, 1 );
    mas_varset_vclass_variable_set_element( el, 0, "\r\n", 2, head );
    vclass->has_head = 1;
  }
  else
  {
    /* ERROR head ::: vclass->el_cnt ; head ); */
  }
}

mas_varset_class_t *
mas_varset_vclass_set_head( mas_varset_class_t * vclass, const char *vclass_name, const char *head )
{
  if ( !vclass )
    vclass = mas_varset_vclass_create( vclass_name );
  _mas_varset_vclass_set_head( vclass, head );
  return vclass;
}

mas_varset_class_t *
mas_varset_vclass_set_head_va( mas_varset_class_t * vclass, const char *vclass_name, mas_xvsnprintf_t func, const char *fmt, va_list args )
{
  char *text = NULL;
  size_t txsize = 1024 * 10;

  text = mas_malloc( txsize );
  if ( text )
  {
    if ( !func )
      func = mas_xvsnprintf;
    ( *func ) ( text, txsize, fmt, args );
    vclass = mas_varset_vclass_set_head( vclass, vclass_name, text );
    mas_free( text );
  }
  return vclass;
}

mas_varset_class_t *
mas_varset_vclass_set_headf( mas_varset_class_t * vclass, const char *vclass_name, mas_xvsnprintf_t func, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  vclass = mas_varset_vclass_set_head_va( vclass, vclass_name, func, fmt, args );
  va_end( args );
  return vclass;
}

void
_mas_varset_vclass_add_tail( mas_varset_class_t * vclass, const char *tail )
{
  mas_vclass_element_t *el = NULL;

  el = _mas_varset_vclass_add_elements( vclass, 1 );
  if ( el )
  {
    /* mas_varset_vclass_variable_set_element( el, 0, NULL, 0, tail ); */
    mas_varset_vclass_variable_set_element( el, 0, "\r\n", 2, tail );
    vclass->has_tail = 1;
  }
}

mas_varset_class_t *
mas_varset_vclass_add_tail( mas_varset_class_t * vclass, const char *vclass_name, const char *tail )
{
  if ( !vclass )
    vclass = mas_varset_vclass_create( vclass_name );
  _mas_varset_vclass_add_tail( vclass, tail );
  return vclass;
}

#endif
