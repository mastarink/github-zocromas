#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <search.h>

#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_varset_types.h>
#include "mas_varset_vclass.h"



#if 1
static const char *final_str = "\r\n";
static size_t final_len = 2;

#  define MAS_FINAL_STR (char*)final_str
#  define MAS_FINAL_LEN final_len
#else
#  define MAS_FINAL_STR NULL
#  define MAS_FINAL_LEN 0
#endif

static const char *tail_str = "\r\n";
static size_t tail_len = 2;

#define MAS_TAIL_STR tail_str
#define MAS_TAIL_LEN tail_len



mas_varset_class_t *
mas_varset_vclass_create( const char *vclass_name )
{
  mas_varset_class_t *vclass = NULL;

  vclass = mas_malloc( sizeof( mas_varset_class_t ) );
  if ( vclass )
  {
    memset( vclass, 0, sizeof( mas_varset_class_t ) );
    if ( vclass_name )
      vclass->name = mas_strdup( vclass_name );
  }
  return vclass;
}

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
mas_varset_vclass_delete( mas_varset_class_t * vclass )
{
  if ( vclass )
  {
    if ( vclass->el_cnt && vclass->vec )
    {
      mas_vclass_element_t *vec;

      vec = vclass->vec;
      for ( int j = 0; j < vclass->el_cnt; j++ )
      {
        char *v;

        v = vec[j].iov_base;
        vec[j].iov_base = NULL;
        if ( v )
          mas_free( v );
      }
      if ( vec )
        mas_free( vclass->vec );
    }
    if ( vclass->name )
    {
      mas_free( vclass->name );
      vclass->name = NULL;
    }
    mas_free( vclass );
    vclass = NULL;
  }
}

const char *
mas_varset_vclass_name( mas_varset_class_t * vclass )
{
  return vclass ? vclass->name : NULL;
}

unsigned long
mas_varset_vclass_id( mas_varset_class_t * vclass )
{
  return vclass ? vclass->id : 0;
}

static int
mas_varset_compare_vclass( const void *a, const void *b )
{
  mas_varset_class_t *aclass, *bclass;

  aclass = ( mas_varset_class_t * ) a;
  bclass = ( mas_varset_class_t * ) b;
  if ( a == b )
    return 0;
  if ( !aclass->name )
    return bclass->name ? -1 : 0;
  if ( !bclass->name )
    return aclass->name ? 1 : 0;
  return strcmp( aclass->name, bclass->name );
}

mas_varset_class_t *
mas_varset_search_vclass( mas_varset_t * varset, const char *vclass_name )
{
  mas_varset_class_t *vclass = NULL;
  mas_varset_class_t *found = NULL;

  if ( varset )
  {
    mas_varset_class_t **pfound = NULL;

    vclass = mas_varset_vclass_create( vclass_name );
    if ( vclass )
    {
      pfound = tsearch( vclass, &varset->classes, mas_varset_compare_vclass );
      if ( pfound )
        found = *pfound;
      if ( found == vclass )
      {
        found->id = ++varset->nclasses;
      }
      else
      {
        mas_varset_vclass_delete( vclass );
        vclass = NULL;
      }
    }
  }
  return found;
}

mas_varset_class_t *
mas_varset_find_vclass( mas_varset_t * varset, const char *vclass_name )
{
  mas_varset_class_t *vclass = NULL;
  mas_varset_class_t *found = NULL;

  if ( varset )
  {
    mas_varset_class_t **pfound = NULL;

    vclass = mas_varset_vclass_create( vclass_name );
    if ( vclass )
    {
      pfound = tfind( vclass, &varset->classes, mas_varset_compare_vclass );
      if ( pfound )
        found = *pfound;
      mas_varset_vclass_delete( vclass );
    }
  }
  return found;
}

int
mas_varset_delete_vclass( mas_varset_t * varset, const char *vclass_name )
{
  mas_varset_class_t *vclass = NULL;
  mas_varset_class_t *found = NULL;

  if ( varset )
  {
    mas_varset_class_t **pfound = NULL;

    vclass = mas_varset_vclass_create( vclass_name );
    if ( vclass )
    {
      pfound = tfind( vclass, &varset->classes, mas_varset_compare_vclass );
      if ( pfound )
      {
        found = *pfound;
        tdelete( vclass, &varset->classes, mas_varset_compare_vclass );
        if ( found != vclass )
        {
          mas_varset_vclass_delete( found );
        }
      }
      mas_varset_vclass_delete( vclass );
      vclass = NULL;
    }
  }
  return found ? 1 : 0;
}

mas_vclass_element_t *
_mas_varset_vclass_find_variable( mas_varset_class_t * vclass, const char *name )
{
  mas_vclass_element_t *found = NULL;

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

mas_vclass_element_t *
mas_varset_vclass_find_variable( mas_varset_class_t * vclass, const char *name )
{
  return _mas_varset_vclass_find_variable( vclass, name );
}

mas_vclass_element_t *
_mas_varset_vclass_element( mas_varset_class_t * vclass, size_t num )
{
  mas_vclass_element_t *el = NULL;

  if ( vclass && ( num < vclass->el_cnt ) )
    el = &vclass->vec[num];
  return el;
}

mas_vclass_element_t *
_mas_varset_vclass_add_elements( mas_varset_class_t * vclass, size_t num )
{
  mas_vclass_element_t *el = NULL;
  size_t new_size = 0;

  if ( vclass )
  {
    size_t old_cnt = 0;

    old_cnt = vclass->el_cnt;
    if ( vclass->vec )
    {
      new_size = sizeof( mas_vclass_element_t ) * ( ( vclass->el_cnt + num ) + num );
      vclass->vec = mas_realloc( vclass->vec, new_size );

      memset( &vclass->vec[old_cnt], 0, sizeof( mas_vclass_element_t ) * ( num ) );
      el = &vclass->vec[old_cnt];
      vclass->el_cnt += num;
      /* fprintf( stderr, "+ new size: %lu; num:%lu; %lu\n", new_size, num, el - vclass->vec ); */
    }
    else if ( sizeof( mas_vclass_element_t ) )
    {
      new_size = sizeof( mas_vclass_element_t ) * ( num );
      vclass->vec = mas_malloc( new_size );

      memset( &vclass->vec[old_cnt], 0, sizeof( mas_vclass_element_t ) * ( num ) );
      el = &vclass->vec[old_cnt];
      vclass->el_cnt = num;
      /* fprintf( stderr, "0 new size: %lu; num:%lu\n", new_size, num ); */
    }
  }
  /* fprintf( stderr, "z new size: %lu; num:%lu; %lu\n", new_size, num, el - vclass->vec ); */
  return el;
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

mas_vclass_element_t *
__mas_varset_vclass_search_variable( mas_varset_class_t * vclass, const char *name )
{
  mas_vclass_element_t *found = NULL;

  if ( vclass )
  {
    found = _mas_varset_vclass_find_variable( vclass, name );
    if ( !found )
    {
      found = _mas_varset_vclass_add_elements( vclass, 2 );
      mas_varset_vclass_variable_set_name( found, name );
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
_mas_varset_vclass_search_variable( mas_varset_class_t * vclass, const char *name, const char *value )
{
  if ( vclass )
  {
    mas_vclass_element_t *var = NULL;

    if ( name )
    {
      var = __mas_varset_vclass_search_variable( vclass, name );
      if ( var )
        mas_varset_vclass_variable_set_value( var, value );
    }
    vclass->use_var = var;
  }
}

mas_varset_class_t *
mas_varset_vclass_search_variable( mas_varset_class_t * vclass, const char *vclass_name, const char *name, const char *value )
{
  if ( !vclass )
    vclass = mas_varset_vclass_create( vclass_name );
  _mas_varset_vclass_search_variable( vclass, name, value );
  return vclass;
}

mas_varset_class_t *
mas_varset_vclass_search_variable_va( mas_varset_class_t * vclass, const char *vclass_name, const char *name,
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
    vclass = mas_varset_vclass_search_variable( vclass, vclass_name, name, text );
    mas_free( text );
  }
  return vclass;
}

mas_varset_class_t *
mas_varset_vclass_search_variablef( mas_varset_class_t * vclass, const char *vclass_name, const char *name,
                                    mas_xvsnprintf_t func, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  vclass = mas_varset_vclass_search_variable_va( vclass, vclass_name, name, func, fmt, args );
  va_end( args );
  return vclass;
}

const char *
mas_varset_vclass_variable_get_name_ref( mas_vclass_element_t * var )
{
  char *v = NULL;

  if ( var && var[0].iov_len )
    v = var[0].iov_base;
  return v;
}

char *
mas_varset_vclass_variable_get_name( mas_vclass_element_t * var )
{
  char *v = NULL;

  if ( var && var[0].iov_len )
    v = mas_strndup( var[0].iov_base, var[0].iov_len - 2 );
  return v;
}

const char *
mas_varset_vclass_variable_get_value_ref( mas_vclass_element_t * var )
{
  char *v = NULL;

  if ( var && var[1].iov_len )
    v = var[1].iov_base;
  return v;
}

char *
mas_varset_vclass_variable_get_value( mas_vclass_element_t * var )
{
  char *v = NULL;

  if ( var && var[1].iov_len )
    v = mas_strndup( var[1].iov_base, var[1].iov_len - 2 );
  return v;
}

/* mas_vclass_element_t *                                                                                                                      */
/* mas_varset_vclass_set_element( mas_varset_class_t * vclass, size_t offset, const char *suffix, size_t suffix_len, const char *str )         */
/* {                                                                                                                                           */
/*   mas_vclass_element_t *var = NULL;                                                                                                         */
/*                                                                                                                                             */
/*   if ( vclass )                                                                                                                             */
/*     var = __mas_varset_vclass_search_variable( vclass, NULL );                                                                              */
/*   return var;                                                                                                                               */
/* }                                                                                                                                           */
/*                                                                                                                                             */
/* mas_vclass_element_t *                                                                                                                      */
/* mas_varset_vclass_set_element_va( mas_varset_class_t * vclass, size_t offset, const char *suffix, size_t suffix_len, mas_xvsnprintf_t func, */
/*                                   const char *fmt, va_list args )                                                                           */
/* {                                                                                                                                           */
/*   mas_vclass_element_t *var = NULL;                                                                                                         */
/*   char *text = NULL;                                                                                                                        */
/*   size_t txsize = 1024 * 10;                                                                                                                */
/*                                                                                                                                             */
/*   text = mas_malloc( txsize );                                                                                                              */
/*   if ( text )                                                                                                                               */
/*   {                                                                                                                                         */
/*     if ( !func )                                                                                                                            */
/*       func = mas_xvsnprintf;                                                                                                                */
/*     ( *func ) ( text, txsize, fmt, args );                                                                                                  */
/*     var = mas_varset_vclass_set_element( vclass, offset, suffix, suffix_len, text );                                                        */
/*     mas_free( text );                                                                                                                       */
/*   }                                                                                                                                         */
/*   return var;                                                                                                                               */
/* }                                                                                                                                           */
/*                                                                                                                                             */
/* mas_vclass_element_t *                                                                                                                      */
/* mas_varset_vclass_set_elementf( mas_varset_class_t * vclass, size_t offset, const char *suffix, size_t suffix_len, mas_xvsnprintf_t func,   */
/*                                 const char *fmt, ... )                                                                                      */
/* {                                                                                                                                           */
/*   mas_vclass_element_t *var = NULL;                                                                                                         */
/*   va_list args;                                                                                                                             */
/*                                                                                                                                             */
/*   va_start( args, fmt );                                                                                                                    */
/*   var = mas_varset_vclass_set_element_va( vclass, offset, suffix, suffix_len, func, fmt, args );                                            */
/*   va_end( args );                                                                                                                           */
/*   return var;                                                                                                                               */
/* }                                                                                                                                           */

void
mas_varset_vclass_variable_set_element( mas_vclass_element_t * var, size_t offset, const char *suffix, size_t suffix_len, const char *str )
{
  if ( var && str )
  {
    size_t l;
    char *s = NULL;

    if ( var[offset].iov_base )
      mas_free( var[offset].iov_base );
    var[offset].iov_base = NULL;
    var[offset].iov_len = 0;
    l = strlen( str );
    s = mas_strndup( str, l );
    if ( suffix )
      s = mas_strncat_x( s, suffix, suffix_len );
    var[offset].iov_len = l + suffix_len;
    var[offset].iov_base = s;
  }
}

void
mas_varset_vclass_variable_set_elementf( mas_vclass_element_t * var, const char *str, size_t offset, const char *suffix, size_t suffix_len )
{
}

void
mas_varset_vclass_variable_set_name( mas_vclass_element_t * var, const char *name )
{
  mas_varset_vclass_variable_set_element( var, 0, ": ", 2, name );
}

void
mas_varset_vclass_variable_set_value( mas_vclass_element_t * var, const char *value )
{
  mas_varset_vclass_variable_set_element( var, 1, MAS_TAIL_STR, MAS_TAIL_LEN, value );
}

void
mas_varset_vclass_variable_set_value_va( mas_vclass_element_t * var, mas_xvsnprintf_t func, const char *fmt, va_list args )
{
  char *text = NULL;
  size_t txsize = 1024 * 10;

  text = mas_malloc( txsize );
  if ( text )
  {
    if ( !func )
      func = mas_xvsnprintf;
    ( *func ) ( text, txsize, fmt, args );
    mas_varset_vclass_variable_set_value( var, text );
    mas_free( text );
  }
}

void
mas_varset_vclass_variable_set_valuef( mas_vclass_element_t * var, mas_xvsnprintf_t func, const char *fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  mas_varset_vclass_variable_set_value_va( var, func, fmt, args );
  va_end( args );
}

void
mas_varset_vclass_write( int fd, mas_varset_class_t * vclass )
{
  if ( fd > 0 && vclass && vclass->vec )
  {
    /* fprintf( stderr, "\t %s %d\t- %u\n", __func__, __LINE__, vclass->el_cnt );                             */
    /* fprintf( stderr, "\n@@@ [" );                                                                          */
    /* for ( int j = 0; j < vclass->el_cnt; j++ )                                                             */
    /*   fprintf( stderr, "[%p] '%s'\n", vclass->vec[j].iov_base, ( const char * ) vclass->vec[j].iov_base ); */
    /* fprintf( stderr, "] @@@\n" );                                                                          */

    writev( fd, vclass->vec, vclass->el_cnt );
  }
}
