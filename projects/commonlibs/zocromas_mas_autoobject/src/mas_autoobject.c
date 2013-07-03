#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/sendfile.h>



#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>

#include "mas_autoobject_types.h"
#include "mas_autoobject.h"

mas_autoobject_t *
mas_autoobject_create( void )
{
  mas_autoobject_t *obj = NULL;

  obj = mas_malloc( sizeof( mas_autoobject_t ) );
  memset( obj, 0, sizeof( mas_autoobject_t ) );
  return obj;
}

void
mas_autoobject_delete( mas_autoobject_t * obj )
{
  if ( obj )
  {
    mas_autoobject_close( obj );
    mas_autoobject_delete_data( obj );
    _mas_autoobject_set_name( obj, NULL );
    mas_free( obj );
  }
}

int
mas_autoobject_fd( mas_autoobject_t * obj )
{
  int fd = 0;

  if ( obj )
  {
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_SENDFILE:
      fd = obj->handler.i;
      break;
    case MAS_IACCESS_FILE:
      fd = fileno( obj->handler.f );
      break;
    case MAS_IACCESS_CHAR:
    case MAS_IACCESS_BAD:
      break;
    }
  }
  return fd;
}

void
mas_autoobject_set_iaccess_type( mas_autoobject_t * obj, mas_iaccess_type_t t )
{
  if ( obj )
  {
    int o;

    o = mas_autoobject_opened( obj );
    if ( o )
    {
      mas_autoobject_close( obj );
      mas_autoobject_delete_data( obj );
    }
    _mas_autoobject_set_iaccess_type( obj, t );
    if ( o )
      mas_autoobject_reopen( obj );
  }
}

void
_mas_autoobject_set_iaccess_type( mas_autoobject_t * obj, mas_iaccess_type_t t )
{
  obj->iaccess_type = t;
}

int
mas_autoobject_delete_data( mas_autoobject_t * obj )
{
  if ( obj && obj->data )
  {
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_SENDFILE:
    case MAS_IACCESS_FILE:
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_CHAR:
      mas_free( obj->data );
      break;
    case MAS_IACCESS_BAD:
      break;
    }
    obj->data = NULL;
  }
  return 0;
}

int
mas_autoobject_qopen( mas_autoobject_t * obj, const char *name )
{
  _mas_autoobject_set_name( obj, name );
  return mas_autoobject_reopen( obj );
}

int
mas_autoobject_reopen( mas_autoobject_t * obj )
{
  if ( obj && !obj->handler.v )
  {
    int rst = -1;
    struct stat st;

    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_SENDFILE:
      obj->handler.i = mas_open( ( char * ) obj->name, O_RDONLY );
      rst = fstat( mas_autoobject_fd( obj ), &st );
      break;
    case MAS_IACCESS_FILE:
      obj->handler.f = fopen( ( char * ) obj->name, "r" );
      rst = fstat( mas_autoobject_fd( obj ), &st );
      break;
    case MAS_IACCESS_CHAR:
    case MAS_IACCESS_BAD:
      break;
    }
    if ( rst >= 0 )
    {
      obj->size = st.st_size;
    }
  }
  else
  {
  }
  return obj->handler.i ? 0 : -1;
}

void
mas_autoobject_set_name( mas_autoobject_t * obj, const char *name )
{
  if ( obj )
  {
    int o;

    o = mas_autoobject_opened( obj );
    mas_autoobject_close( obj );
    _mas_autoobject_set_name( obj, name );
    if ( o )
      mas_autoobject_reopen( obj );
  }
}

void
_mas_autoobject_set_name( mas_autoobject_t * obj, const char *name )
{
  if ( obj )
  {
    if ( obj->name )
      mas_free( obj->name );
    obj->name = NULL;
    if ( name )
      obj->name = mas_strdup( name );
  }
}

int
mas_autoobject_open( mas_autoobject_t * obj, const char *name )
{
  if ( obj )
    mas_autoobject_close( obj );
  return mas_autoobject_qopen( obj, name );
}

int
mas_autoobject_qopen_data( mas_autoobject_t * obj )
{
  return obj->data ? mas_autoobject_qopen( obj, ( char * ) obj->data ) : -1;
}

int
mas_autoobject_opened( mas_autoobject_t * obj )
{
  int opened = 0;

  if ( obj && obj->handler.v )
  {
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_SENDFILE:
      opened = ( mas_autoobject_fd( obj ) ) ? 1 : 0;
      break;
    case MAS_IACCESS_FILE:
      opened = ( ( FILE * ) obj->handler.f ) ? 1 : 0;
      break;
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_CHAR:
    case MAS_IACCESS_BAD:
      break;
    }
  }
  return opened;
}

int
mas_autoobject_close( mas_autoobject_t * obj )
{
  if ( obj && obj->handler.v )
  {
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_SENDFILE:
      close( mas_autoobject_fd( obj ) );
      break;
    case MAS_IACCESS_FILE:
      fclose( ( FILE * ) obj->handler.f );
      break;
    case MAS_IACCESS_FCHAR:
    case MAS_IACCESS_CHAR:
    case MAS_IACCESS_BAD:
      break;
    }
    obj->size = 0;
    obj->handler.v = NULL;
  }
  return 0;
}

int
mas_autoobject_set_data( mas_autoobject_t * obj, void *ptr )
{
  int r = 0;

  if ( obj )
  {
    mas_autoobject_close( obj );
    mas_autoobject_delete_data( obj );
    if ( ptr )
      switch ( obj->iaccess_type )
      {
      case MAS_IACCESS_SENDFILE:
      case MAS_IACCESS_FILE:
      case MAS_IACCESS_CHAR:
        obj->data = mas_strdup( ( char * ) ptr );
        obj->size = strlen( obj->data );
        break;
      case MAS_IACCESS_FCHAR:
        {
          mas_autoobject_qopen( obj, ( const char * ) ptr );
          if ( obj->handler.v )
          {
            if ( obj->size > 0 )
            {
              char *buf;

              buf = mas_malloc( obj->size + 2 );
              if ( buf )
              {
                r = read( mas_autoobject_fd( obj ), buf, obj->size );
                if ( r > 0 )
                {
                  obj->data = buf;
                  buf[obj->size] = 0;
                }
                else
                  mas_free( buf );
              }
            }
            mas_autoobject_close( obj );
          }
        }
        break;
      case MAS_IACCESS_BAD:
        break;
      }
  }
  return 0;
}

int
mas_autoobject_cat( int han, mas_autoobject_t * obj )
{
  if ( obj )
  {
    mas_autoobject_qopen_data( obj );
    switch ( obj->iaccess_type )
    {
    case MAS_IACCESS_CHAR:
    case MAS_IACCESS_FCHAR:
      write( han, obj->data, obj->size );
      break;
    case MAS_IACCESS_SENDFILE:
    case MAS_IACCESS_FILE:
      if ( han && obj->handler.v && obj->size )
      {
        sendfile( han, mas_autoobject_fd( obj ), 0, obj->size );
        lseek( mas_autoobject_fd( obj ), 0, SEEK_SET );
      }
    case MAS_IACCESS_BAD:
      break;
    }
  }
  return 0;
}
