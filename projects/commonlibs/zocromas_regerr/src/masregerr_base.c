#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>

#include "masregerr_defs.h"
#include "masregerr_structs.h"

#include "masregerr_base.h"

masregerrs_t *
masregerrs_create( void )
{
  masregerrs_t *regerrs = mas_calloc( 1, sizeof( masregerrs_t ) );

  return regerrs;
}

void
masregerrs_init( masregerrs_t * regerrs, size_t size )
{
  if ( regerrs )
  {
    regerrs->err_array = mas_calloc( size, sizeof( masregerr_t ) );
    regerrs->size = size;
    regerrs->current = regerrs->err_array;
  }
}

masregerrs_t *
masregerrs_create_setup( size_t size )
{
  if ( !size )
    size = MASREGERR_DEFAULT_SIZE;
  masregerrs_t *regerrs = masregerrs_create(  );

  masregerrs_init( regerrs, size );
  return regerrs;
}

void
masregerrs_reset( masregerrs_t * regerrs )
{
  if ( regerrs )
  {
    for ( size_t n = 0; n < regerrs->size; n++ )
      masregerr_reset( regerrs->err_array + n );

  /* memset( regerrs->err_array, 0, regerrs->size * sizeof( masregerr_t ) ); */
    mas_free( regerrs->err_array );
  }
}

void
masregerrs_delete( masregerrs_t * regerrs )
{
  masregerrs_reset( regerrs );
  mas_free( regerrs );
}

void
masregerr_reset( masregerr_t * rge )
{
  if ( rge )
  {
    if ( rge->data )
      mas_free( rge->data );
    if ( rge->msg )
      mas_free( rge->msg );
    memset( rge, 0, sizeof( masregerr_t ) );
  }
}
