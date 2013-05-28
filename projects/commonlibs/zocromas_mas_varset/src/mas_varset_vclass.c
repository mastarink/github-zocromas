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

#include "mas_varset_types.h"
#include "mas_varset_vclass.h"

mas_varset_class_t *
mas_varset_vclass_create( const char *vclass_name )
{
  mas_varset_class_t *vclass = NULL;

  if ( vclass_name )
  {
    vclass = mas_malloc( sizeof( mas_varset_class_t ) );
    if ( vclass )
    {
      memset( vclass, 0, sizeof( mas_varset_class_t ) );
      vclass->name = mas_strdup( vclass_name );
    }
  }
  return vclass;
}

void
mas_varset_vclass_delete( mas_varset_class_t * vclass )
{
  if ( vclass )
  {
    if ( vclass->veccnt && vclass->vec )
    {
      mas_var_t *vec;

      vec = vclass->vec;
      for ( int i = 0; i < vclass->veccnt * 2; i++ )
      {
        char *v;

        v = vec[i].iov_base;
        vec[i].iov_base = NULL;
        if ( v )
        {
          mas_free( v );
        }
      }
      if ( vec )
      {
        mas_free( vclass->vec );
      }
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

mas_var_t *
mas_varset_vclass_find_variable( mas_varset_class_t * vclass, const char *name )
{
  mas_var_t *found = NULL;

  if ( vclass && vclass->vec )
  {
    for ( int i = 0; i < vclass->veccnt; i++ )
    {
      int len;
      int j;

      j = i * 2;
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

mas_var_t *
mas_varset_vclass_search_variable( mas_varset_class_t * vclass, const char *name )
{
  mas_var_t *found = NULL;

  if ( vclass )
  {
    if ( !vclass->vec )
    {
      found = mas_malloc( sizeof( mas_var_t ) * 2 );
      memset( found, 0, sizeof( mas_var_t ) * 2 );
      vclass->vec = found;
      vclass->veccnt = 1;
    }
    else
    {
      found = mas_varset_vclass_find_variable( vclass, name );
      if ( !found )
      {
        int c, j;

        c = vclass->veccnt;
        j = c * 2;
        /* if ( 0 )                                                                 */
        /* {                                                                        */
        /*   mas_var_t *new_vec;                                                    */
        /*                                                                          */
        /*   new_vec = mas_malloc( sizeof( mas_var_t ) * ( j + 2 ) );               */
        /*   fprintf( stderr, "malloc %p - CHANGED VECTOR\n", ( void * ) new_vec ); */
        /*   memset( new_vec, 0, sizeof( mas_var_t ) * ( j + 2 ) );                 */
        /*   memcpy( new_vec, vclass->vec, sizeof( mas_var_t ) * j );               */
        /*   fprintf( stderr, "%d free %p\n", __LINE__, ( void * ) vclass->vec );   */
        /*   mas_free( vclass->vec );                                               */
        /*   vclass->vec = new_vec;                                                 */
        /* }                                                                        */
        /* else                                                                     */
        {
          vclass->vec = mas_realloc( vclass->vec, sizeof( mas_var_t ) * ( j + 2 ) );
          memset( vclass->vec + j, 0, sizeof( mas_var_t ) * 2 );
        }
        found = &vclass->vec[j];
        found->iov_base = NULL;
        found->iov_len = 0;
        c++;
        vclass->veccnt = c;
      }
    }
  }
  if ( found && !mas_varset_vclass_variable_get_name_ref( found ) )
    mas_varset_vclass_variable_set_name( found, name );
  return found;
}

const char *
mas_varset_vclass_variable_get_name_ref( mas_var_t * var )
{
  char *v = NULL;

  if ( var && var[0].iov_len )
    v = var[0].iov_base;
  return v;
}

char *
mas_varset_vclass_variable_get_name( mas_var_t * var )
{
  char *v = NULL;

  if ( var && var[0].iov_len )
  {
    v = mas_strndup( var[0].iov_base, var[0].iov_len - 2 );
  }
  return v;
}

const char *
mas_varset_vclass_variable_get_value_ref( mas_var_t * var )
{
  char *v = NULL;

  if ( var && var[1].iov_len )
  {
    v = var[1].iov_base;
  }
  return v;
}

char *
mas_varset_vclass_variable_get_value( mas_var_t * var )
{
  char *v = NULL;

  if ( var && var[1].iov_len )
    v = mas_strndup( var[1].iov_base, var[1].iov_len );
  return v;
}

void
mas_varset_vclass_variable_set_name( mas_var_t * var, const char *name )
{
  if ( var && name )
  {
    size_t l;
    char *s;

    if ( var[0].iov_base )
      mas_free( var[0].iov_base );
    var[0].iov_base = NULL;
    var[0].iov_len = 0;
    l = strlen( name );
    s = mas_strndup( name, l );
    s = mas_strcat_x( s, ": " );
    var[0].iov_len = l + 2;
    var[0].iov_base = s;
  }
}

void
mas_varset_vclass_variable_set_value( mas_var_t * var, const char *value )
{
  if ( var )
  {
    if ( var[1].iov_base )
      mas_free( var[1].iov_base );
    var[1].iov_base = NULL;
    var[1].iov_len = 0;
    if ( value )
    {
      size_t l;
      char *s;

      l = strlen( value );
      s = mas_strndup( value, l );
      s = mas_strcat_x( s, "\r\n" );
      var[1].iov_len = l + 2;
      var[1].iov_base = s;
    }
  }
}

void
mas_varset_vclass_write( int fd, mas_varset_class_t * vclass )
{
  if ( fd > 0 && vclass )
    writev( fd, vclass->vec, vclass->veccnt * 2 );
}
