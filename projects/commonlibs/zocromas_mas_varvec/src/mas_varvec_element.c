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
/* #include <mastar/tools/mas_tools.h> */
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_varvec_types.h>
/* #include "mas_varvec_object.h" */


#include "mas_varvec_element.h"



mas_varvec_element_t *
_mas_varvec_element( mas_varvec_t * vclass, size_t num )
{
  mas_varvec_element_t *el = NULL;

  if ( vclass && ( num < vclass->el_cnt ) )
    el = &vclass->vec[num];
  return el;
}

mas_varvec_element_t *
_mas_varvec_add_elements( mas_varvec_t * vclass, size_t num )
{
  mas_varvec_element_t *el = NULL;
  size_t new_size = 0;

  if ( vclass )
  {
    size_t old_cnt = 0;

    old_cnt = vclass->el_cnt;
    if ( vclass->vec )
    {
      new_size = sizeof( mas_varvec_element_t ) * ( ( vclass->el_cnt + num ) + num );
      vclass->vec = mas_realloc( vclass->vec, new_size );

      memset( &vclass->vec[old_cnt], 0, sizeof( mas_varvec_element_t ) * ( num ) );
      el = &vclass->vec[old_cnt];
      vclass->el_cnt += num;
      /* fprintf( stderr, "+ new size: %lu; num:%lu; %lu\n", new_size, num, el - vclass->vec ); */
    }
    else if ( sizeof( mas_varvec_element_t ) )
    {
      new_size = sizeof( mas_varvec_element_t ) * ( num );
      vclass->vec = mas_malloc( new_size );

      memset( &vclass->vec[old_cnt], 0, sizeof( mas_varvec_element_t ) * ( num ) );
      el = &vclass->vec[old_cnt];
      vclass->el_cnt = num;
      /* fprintf( stderr, "0 new size: %lu; num:%lu\n", new_size, num ); */
    }
  }
  /* fprintf( stderr, "z new size: %lu; num:%lu; %lu\n", new_size, num, el - vclass->vec ); */
  return el;
}

void
mas_varvec_variable_set_element( mas_varvec_element_t * var, size_t offset, const char *suffix, size_t suffix_len, const char *str )
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
